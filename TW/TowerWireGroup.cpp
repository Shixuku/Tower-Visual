#include "TowerWireGroup.h"
#include"CreatWire.h"
#include"CreateStrainWire.h"
#include <algorithm>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
int TowerWireGroup::Get_id() const
{
	return m_id;
}
void TowerWireGroup::SaveTo(QDataStream& fin) const
{
	fin << groupId;
	int nNode = m_Nodes.size();
	fin << nNode;
	for (int i = 0; i < nNode; ++i)
	{
		m_Nodes[i].SaveTo(fin);
	}
	int nbeam = m_Elements_beams.size();
	fin << nbeam;
	for (int i = 0; i < nbeam; ++i)
	{
		m_Elements_beams[i].SaveTo(fin);
	}
	int nTruss = m_Elements_Trusses.size();
	fin << nTruss;
	for (int i = 0; i < nTruss; ++i)
	{
		m_Elements_Trusses[i].SaveTo(fin);
	}
	int nSuspension = SuspensionNode.size();
	fin << nSuspension;
	for (int i = 0; i < nSuspension; ++i)
	{
		fin << SuspensionNode[i];
	}
		
}

void TowerWireGroup::Input(QDataStream& fin)
{
	fin >> groupId;
	int nNode;
	fin >> nNode;
	m_Nodes.resize(nNode);
	for (int i = 0; i < nNode; ++i)
	{
		m_Nodes[i].Input(fin);
	}
	int nbeam;
	fin >> nbeam;
	m_Elements_beams.resize(nbeam);
	for (int i = 0; i < nbeam; ++i)
	{
		m_Elements_beams[i].Input(fin);
	}
	int nTruss;
	fin >> nTruss;
	m_Elements_Trusses.resize(nTruss);
	for (int i = 0; i < nTruss; ++i)
	{
		m_Elements_Trusses[i].Input(fin);
	}
	int nSuspension;
	fin >> nSuspension;
	SuspensionNode.resize(nSuspension);
	for (int i = 0; i < nSuspension; ++i)
	{
		fin >> SuspensionNode[i];
	}
}

void TowerWireGroup::VectorToMap()
{
	for (const auto& data : m_Nodes)
	{
		NodeData.insert(std::make_pair(data.m_idNode, data));
	}
	// 将 vectorBeam 中的元素插入到 map 中
	for (const auto& data : m_Elements_beams)
	{
		BeamData.insert(std::make_pair(data.m_idElement, data));
	}
	// 将 vectorTruss 中的元素插入到 map 中
	for (const auto& data : m_Elements_Trusses)
	{
		TrussData.insert(std::make_pair(data.m_idElement, data));
	}
}

void TowerWireGroup::AddTowerToGroup(Tower* tower, int towerId, double dx, double dy, double dz, double dAngle)
{
	AddTowerRotation(tower, dAngle);
	AddTowerNode(tower, towerId, dx, dy, dz);
	AddTowerElement(tower, towerId);
	AddSuspensionNode(tower);
	addRestraintNode(tower);
	addHangPoint(tower);
	tower->TowerToGroup.clear();
}

void TowerWireGroup::AddTowerRotation(Tower* tower, double dAngle)
{
	double radianAngle = dAngle * M_PI / 180.0; // Convert degrees to radians
	double cosTheta = cos(radianAngle);
	double sinTheta = sin(radianAngle);
	// 旋转矩阵
	double rotationMatrix[3][3] = {
		{cosTheta, -sinTheta, 0},
		{sinTheta, cosTheta, 0},
		{0, 0, 1} };
	for (auto& i : tower->m_Nodes)
	{
		double newX = i.x * cos(radianAngle) - i.y * sin(radianAngle);
		double newY = i.x * sin(radianAngle) + i.y * cos(radianAngle);
		i.x = newX;
		i.y = newY;
	}
	for (auto& i : tower->m_Elements_beams)
	{
		double newX = i.direction[0] * rotationMatrix[0][0] + i.direction[1] * rotationMatrix[0][1] + i.direction[2] * rotationMatrix[0][2];
		double newY = i.direction[0] * rotationMatrix[1][0] + i.direction[1] * rotationMatrix[1][1] + i.direction[2] * rotationMatrix[1][2];
		double newZ = i.direction[0] * rotationMatrix[2][0] + i.direction[1] * rotationMatrix[2][1] + i.direction[2] * rotationMatrix[2][2];
		i.direction[0] = newX;
		i.direction[1] = newY;
		i.direction[2] = newZ;
	}
}

void TowerWireGroup::AddTowerNode(Tower* tower, int towerId, double dx, double dy, double dz)
{
	size_t towerNode = tower->m_Nodes.size();//tower中的节点

	//add
	for (size_t i = 0; i < towerNode; ++i)
	{

		this->m_Nodes.push_back(tower->m_Nodes[i]);
		size_t total = this->m_Nodes.size() - 1;
		this->m_Nodes[total].x += dx;
		this->m_Nodes[total].y += dy;
		this->m_Nodes[total].z += dz;
		this->m_Nodes[total].m_idNode = total+1;
		this->m_Nodes[total].groupTowerId = towerId;
		tower->TowerToGroup.push_back(total+1);
	}
}

void TowerWireGroup::AddTowerElement(Tower* tower, int towerId)
{
	size_t tTower = tower->m_Elements_Trusses.size();
	size_t bTower = tower->m_Elements_beams.size();
	size_t SusElementClass = tower->SuspensionElementClass.size();
	size_t SusElement = tower->SuspensionElement.size();
	int size = 0;//添加悬挂绝缘子串的单元时，添加完后后面的单元不用再做循环判断
	for (size_t i = 0; i < tTower; ++i)//Truss
	{
		Element_Truss* pE = &tower->m_Elements_Trusses[i];
		this->m_Elements.push_back(tower->m_Elements_Trusses[i]);
		size_t total = this->m_Elements.size() - 1;
		this->m_Elements[total].m_idElement = total + 1;//放入实例的总单元
		this->m_Elements[total].m_idNode[0] = tower->FindGroupIdNode(pE->m_idNode[0]);
		this->m_Elements[total].m_idNode[1] = tower->FindGroupIdNode(pE->m_idNode[1]);
		this->m_Elements[total].groupTowerId = towerId;
		this->m_Elements_Trusses.push_back(tower->m_Elements_Trusses[i]);
		size_t totalT = this->m_Elements_Trusses.size() - 1;
		this->m_Elements_Trusses[totalT].m_idElement = total + 1;//放入实例的杆单元
		this->m_Elements_Trusses[totalT].m_idNode[0] = tower->FindGroupIdNode(pE->m_idNode[0]);
		this->m_Elements_Trusses[totalT].m_idNode[1] = tower->FindGroupIdNode(pE->m_idNode[1]);
		this->m_Elements_Trusses[totalT].ClassSectionID = pE->ClassSectionID;
		this->m_Elements_Trusses[totalT].MaterialID = pE->MaterialID;
		this->m_Elements_Trusses[totalT].groupTowerId = towerId;
		this->m_Elements_Trusses[totalT].Type = "T-" + QString::number(towerId + 1);
		if (size != SusElementClass)
		{
			for (int j = 0; j < SusElement; j++)
			{
				if (tower->m_Elements_Trusses[i].m_idElement == tower->SuspensionElement[j])
				{
					SuspensionElement.push_back(total + 1);
					size++;
				}
			}
		}
	}
	for (size_t i = 0; i < bTower; ++i)
	{
		Element_Beam* pE = &tower->m_Elements_beams[i];
		this->m_Elements.push_back(tower->m_Elements_beams[i]);
		size_t total = this->m_Elements.size() - 1;
		this->m_Elements[total].m_idElement = total + 1;//放入实例的总单元
		this->m_Elements[total].m_idNode[0] = tower->FindGroupIdNode(pE->m_idNode[0]);
		this->m_Elements[total].m_idNode[1] = tower->FindGroupIdNode(pE->m_idNode[1]);
		this->m_Elements[total].groupTowerId = towerId;
		this->m_Elements_beams.push_back(tower->m_Elements_beams[i]);
		size_t totalT = this->m_Elements_beams.size() - 1;
		this->m_Elements_beams[totalT].m_idElement = total + 1;//放入实例的梁单元
		this->m_Elements_beams[totalT].m_idNode[0] = tower->FindGroupIdNode(pE->m_idNode[0]);
		this->m_Elements_beams[totalT].m_idNode[1] = tower->FindGroupIdNode(pE->m_idNode[1]);
		this->m_Elements_beams[totalT].ClassSectionID = pE->ClassSectionID;
		this->m_Elements_beams[totalT].MaterialID = pE->MaterialID;
		this->m_Elements_beams[totalT].direction[0] = pE->direction[0];
		this->m_Elements_beams[totalT].direction[1] = pE->direction[1];
		this->m_Elements_beams[totalT].direction[2] = pE->direction[2];
		this->m_Elements_beams[totalT].groupTowerId = towerId;
		this->m_Elements_beams[totalT].Type = "T-" + QString::number(towerId + 1);
	}
	for (int i = 0; i < tower->SuspensionElementClass.size(); i++)
	{
		SuspensionElementClass.push_back(tower->SuspensionElementClass[i]);
	}
}

void TowerWireGroup::addRestraintNode(Tower* tower)
{
	size_t ResNode = tower->RestraintNode.size();
	for (int i = 0; i < ResNode; i++)
	{
		this->RestraintNode.push_back(tower->RestraintNode[i]);
		int id = RestraintNode.size() - 1;
		RestraintNode[id] = tower->FindGroupIdNode(tower->RestraintNode[i]);
	}
}


void TowerWireGroup::AddSuspensionNode(Tower* tower)
{
	size_t SusNode = tower->SuspensionNode.size();
	for (int i = 0; i < SusNode; i++)
	{
		this->SuspensionNode.push_back(tower->SuspensionNode[i]);
		this->WireLogo.push_back(tower->WireLogo[i]);
		size_t totalT = this->SuspensionNode.size() - 1;
		SuspensionNode[totalT] = tower->FindGroupIdNode(tower->SuspensionNode[i]);
	}
	size_t realSusNode = tower->realSuspoint.size();
	for (int i = 0; i < realSusNode; i++)
	{
		this->realSuspoint.push_back(tower->realSuspoint[i]);
		size_t totalT = this->realSuspoint.size() - 1;
		realSuspoint[totalT] = tower->FindGroupIdNode(tower->realSuspoint[i]);
	}
}

void TowerWireGroup::addHangPoint(Tower* tower)
{
	size_t m_HangListSize = tower->m_HangList.size();
	qDebug() << "m_HangListSize" << m_HangListSize << "\n";
	for (int i = 0; i < m_HangListSize; i++)
	{
		this->m_HangList.push_back(tower->m_HangList[i]);
		size_t totalT = this->m_HangList.size() - 1;
		m_HangList[totalT].m_id = totalT + 1;
		if (tower->m_HangList[i].StringClass == "V")
		{
			m_HangList[totalT].NodeId[0] = tower->FindGroupIdNode(tower->m_HangList[i].NodeId[0]);
			m_HangList[totalT].NodeId[1] = tower->FindGroupIdNode(tower->m_HangList[i].NodeId[1]);
		}
		else
		{
			m_HangList[totalT].NodeId[0] = tower->FindGroupIdNode(tower->m_HangList[i].NodeId[0]);
		}
		

		/*HangPoint* hangPoint = new HangPoint();
		hangPoint->m_id = this->TP_HangPoint.size() + 1;
		hangPoint->StringClass = tower->TP_HangPoint.Find_Entity(i + 1)->StringClass;
		QString tt = tower->TP_HangPoint.Find_Entity(i + 1)->WireLoge;
		qDebug() << tt << "\n";
		hangPoint->WireLoge = tower->TP_HangPoint.Find_Entity(i + 1)->WireLoge;
		qDebug() << tower->TP_HangPoint.Find_Entity(i + 1)->NodeId << "\n";
		hangPoint->NodeId = tower->FindGroupIdNode(tower->TP_HangPoint.Find_Entity(i + 1)->NodeId);
		this->TP_HangPoint.Add_Entity(hangPoint);*/
	}
}

void TowerWireGroup::AddWireToGroup(CreatWire* wire)
{
	AddWireNode(wire);
	AddWireElement(wire);
	AddAxialForceToInsulator(wire);

}

void TowerWireGroup::AddWireNode(CreatWire* wire)
{
	size_t wireNode = wire->m_Nodes.size();//tower中的节点
	//add
	for (size_t i = 0; i < wireNode; ++i)//所有线节点的循环
	{
		bool isSuspensionNode = false;
		
		for (int j = 0; j < this->realSuspoint.size(); j++)
		{
			if ((abs(wire->m_Nodes[i].x - NodeData[realSuspoint[j]].x)<1e-1) &&
				(abs(wire->m_Nodes[i].y - NodeData[realSuspoint[j]].y) < 1e-1) &&
				(abs(wire->m_Nodes[i].z - NodeData[realSuspoint[j]].z) < 1e-1))
			{
				isSuspensionNode = true;
				wire->wireToGroup.push_back(realSuspoint.at(j));
			}
		}
		if (isSuspensionNode == false)
		{
			this->m_Nodes.push_back(wire->m_Nodes[i]);
			size_t total = this->m_Nodes.size() - 1;//
			this->m_Nodes[total].m_idNode = total + 1;
			//part-tower 节点对应关系
			wire->wireToGroup.push_back(total + 1);
		}
	}

}

void TowerWireGroup::AddWireElement(CreatWire* wire)
{
	size_t tTower = wire->m_Elements_Trusses.size();
	size_t bpart = wire->m_Elements_beams.size();
	for (size_t i = 0; i < tTower; ++i)//Truss
	{
		Element_Truss* pE = &wire->m_Elements_Trusses[i];
		this->m_Elements.push_back(wire->m_Elements_Trusses[i]);
		size_t total = this->m_Elements.size() - 1;
		this->m_Elements[total].m_idElement = total + 1;//放入实例的总单元
		this->m_Elements[total].m_idNode[0] = wire->FindGroupIdNode(pE->m_idNode[0]);
		this->m_Elements[total].m_idNode[1] = wire->FindGroupIdNode(pE->m_idNode[1]);
		this->m_Elements_Trusses.push_back(wire->m_Elements_Trusses[i]);
		size_t totalT = this->m_Elements_Trusses.size() - 1;
		this->m_Elements_Trusses[totalT].m_idElement = total + 1;//放入实例的杆单元
		this->m_Elements_Trusses[totalT].m_idNode[0] = wire->FindGroupIdNode(pE->m_idNode[0]);
		this->m_Elements_Trusses[totalT].m_idNode[1] = wire->FindGroupIdNode(pE->m_idNode[1]);
		this->m_Elements_Trusses[totalT].ClassSectionID = pE->ClassSectionID;
		this->m_Elements_Trusses[totalT].MaterialID = pE->MaterialID;
	}
	for (size_t i = 0; i < bpart; ++i)
	{
		Element_Beam* pE = &wire->m_Elements_beams[i];
		this->m_Elements.push_back(wire->m_Elements_beams[i]);
		size_t total = this->m_Elements.size() - 1;
		this->m_Elements[total].m_idElement = total + 1;//放入实例的总单元
		this->m_Elements[total].m_idNode[0] = wire->FindGroupIdNode(pE->m_idNode[0]);
		this->m_Elements[total].m_idNode[1] = wire->FindGroupIdNode(pE->m_idNode[1]);

		this->m_Elements_beams.push_back(wire->m_Elements_beams[i]);
		size_t totalT = this->m_Elements_beams.size() - 1;
		this->m_Elements_beams[totalT].m_idElement = total + 1;//放入实例的梁单元
		this->m_Elements_beams[totalT].m_idNode[0] = wire->FindGroupIdNode(pE->m_idNode[0]);
		this->m_Elements_beams[totalT].m_idNode[1] = wire->FindGroupIdNode(pE->m_idNode[1]);
		this->m_Elements_beams[totalT].ClassSectionID = pE->ClassSectionID;
		this->m_Elements_beams[totalT].MaterialID = pE->MaterialID;
		this->m_Elements_beams[totalT].direction[0] = pE->direction[0];
		this->m_Elements_beams[totalT].direction[1] = pE->direction[1];
		this->m_Elements_beams[totalT].direction[2] = pE->direction[2];
		
	}
}

void TowerWireGroup::AddAxialForceToInsulator(CreatWire* wire)
{
	Splits = wire->fenlie;
	areaWire = wire->area;
	cout << "erro" << TrussData.size() << "\n";
	//暂时只考虑悬垂型（加入塔线组合需修改）
	for (int i = 0; i < this->SuspensionElementClass.size(); i++)
	{
		int xxx=this->SuspensionElement[i];
		int kkk = wire->WireGravity[i];
		int zzz = wire->WireGravity[i + 1];
		 TrussData[this->SuspensionElement[i]].AxialForce = 
			 (wire->WireGravity[i] + wire->WireGravity[i + 1]) / 2;
		 for (int j = 0; j < m_Elements_Trusses.size(); j++)
		 {
			 if (m_Elements_Trusses[j].m_idElement == this->SuspensionElement[i])
			 {
				 this->m_Elements_Trusses[j].AxialForce = TrussData[this->SuspensionElement[i]].AxialForce;
				 this->m_Elements_Trusses[j].ClassSectionID = wire->InsulatorSectionId;
			 }
			 
		 }
		 
	}
}

void TowerWireGroup::rotation(double angle, int towerId)
{
	double cos_theta = cos(angle);
	double sin_theta = sin(angle);
	Vector3d fai;
	fai = { 0,0,1 };
	for (auto& i : this->m_Nodes)
	{
		if (i.groupTowerId == towerId)
		{
			Vector3d u = { i.x, i.y, i.z };
			Vector3d up;
			up = u * cos_theta + fai.cross(u) * sin_theta + fai.dot(u) * fai * (1 - cos_theta);
			i.x = up[0];
			i.y = up[1];
			i.z = up[2];
		}

	}
	for (auto& j : this->m_Elements_beams)
	{
		if (j.groupTowerId == towerId)
		{
			//原始方向
			double x = j.direction[0];
			double y = j.direction[1];
			double z = j.direction[2];
			//旋转
			double new_x = cos_theta * x - sin_theta * z;
			double new_y = y;
			double new_z = sin_theta * x + cos_theta * z;
			//更新旋转后方向
			j.direction[0] = new_x;
			j.direction[1] = new_y;
			j.direction[2] = new_z;
		}
	}
}

void TowerWireGroup::ShowNode()const
{
	for (auto& j : m_Nodes)
	{
		j.show();
	}
}

void TowerWireGroup::ShowElement() const
{
	for (auto& j : m_Elements)
	{
		j.show();
	}
}

void TowerWireGroup::ShowTrussElement() const
{
	for (auto& j : m_Elements_Trusses)
	{
		j.show();
	}
	for (auto& j : m_Elements_Trusses)
	{
		j.showid();
	}
}

void TowerWireGroup::ShowBeamElement() const
{
	for (auto& j : m_Elements_beams)
	{
		j.show();
	}
	for (auto& j : m_Elements_beams)
	{
		j.showid();
	}
}

void TowerWireGroup::Suspensioncombined()
{
	for (size_t i = 0; i < SuspensionNode.size(); ++i) {
		combined.push_back(std::make_pair(SuspensionNode[i], WireLogo[i]));
	}
	// 按照序号进行排序
	std::sort(combined.begin(), combined.end(), [](const std::pair<int, int>& pair1, const std::pair<int, int>& pair2) {
		return pair1.second < pair2.second;
		});
}

void TowerWireGroup::addSpacer(Part_Base* part)
{
	addSpacerNode(part);
	addSpacerElement(part);
	addSpacerSuspensionNode(part);
}

void TowerWireGroup::addSpacerNode(Part_Base* part)
{
	size_t partNode = part->m_Nodes.size();//part中的节点
	for (size_t i = 0; i < partNode; ++i)
	{
		this->m_Nodes.push_back(part->m_Nodes[i]);
		size_t total = this->m_Nodes.size() - 1;//
		this->m_Nodes[total].m_idNode = total + 1;
		//part-tower 节点对应关系
		part->part_to_tower.push_back(total + 1);
	}	
}

void TowerWireGroup::addSpacerElement(Part_Base* part)
{
	size_t tpart = part->m_Elements_Trusses.size();
	size_t SusElementClass = part->SuspensionElementClass.size();
	size_t SusElement = part->SuspensionElement.size();
	size_t bpart = part->m_Elements_beams.size();
	int size = 0;//添加悬挂绝缘子串的单元时，添加完后后面的单元不用再做循环判断
	for (size_t i = 0; i < tpart; ++i)//Truss
	{
		Element_Truss* pE = &part->m_Elements_Trusses[i];
		this->m_Elements.push_back(part->m_Elements_Trusses[i]);
		size_t total = this->m_Elements.size() - 1;
		this->m_Elements[total].m_idElement = total + 1;//放入实例的总单元
		this->m_Elements[total].m_idNode[0] = part->Find_tower_idNode(pE->m_idNode[0]);
		this->m_Elements[total].m_idNode[1] = part->Find_tower_idNode(pE->m_idNode[1]);

		this->m_Elements_Trusses.push_back(part->m_Elements_Trusses[i]);
		size_t totalT = this->m_Elements_Trusses.size() - 1;
		this->m_Elements_Trusses[totalT].m_idElement = total + 1;//放入实例的杆单元
		this->m_Elements_Trusses[totalT].m_idNode[0] = part->Find_tower_idNode(pE->m_idNode[0]);
		this->m_Elements_Trusses[totalT].m_idNode[1] = part->Find_tower_idNode(pE->m_idNode[1]);
		this->m_Elements_Trusses[totalT].ClassSectionID = pE->ClassSectionID;
		this->m_Elements_Trusses[totalT].MaterialID = pE->MaterialID;
		if (size != SusElementClass)
		{
			for (int i = 0; i < SusElement; i++)
			{
				if (part->m_Elements_Trusses[i].m_idElement == part->SuspensionElement[i])
				{
					SuspensionElement.push_back(total + 1);
					size++;
				}
			}
		}
	}
	for (size_t i = 0; i < bpart; ++i)
	{
		Element_Beam* pE = &part->m_Elements_beams[i];
		this->m_Elements.push_back(part->m_Elements_beams[i]);
		size_t total = this->m_Elements.size() - 1;
		this->m_Elements[total].m_idElement = total + 1;//放入实例的总单元
		this->m_Elements[total].m_idNode[0] = part->Find_tower_idNode(pE->m_idNode[0]);
		this->m_Elements[total].m_idNode[1] = part->Find_tower_idNode(pE->m_idNode[1]);

		this->m_Elements_beams.push_back(part->m_Elements_beams[i]);
		size_t totalT = this->m_Elements_beams.size() - 1;
		this->m_Elements_beams[totalT].m_idElement = total + 1;//放入实例的梁单元
		this->m_Elements_beams[totalT].m_idNode[0] = part->Find_tower_idNode(pE->m_idNode[0]);
		this->m_Elements_beams[totalT].m_idNode[1] = part->Find_tower_idNode(pE->m_idNode[1]);
		this->m_Elements_beams[totalT].ClassSectionID = pE->ClassSectionID;
		this->m_Elements_beams[totalT].MaterialID = pE->MaterialID;
		this->m_Elements_beams[totalT].direction[0] = pE->direction[0];
		this->m_Elements_beams[totalT].direction[1] = pE->direction[1];
		this->m_Elements_beams[totalT].direction[2] = pE->direction[2];
	}
	for (int i = 0; i < part->SuspensionElementClass.size(); i++)
	{
		SuspensionElementClass.push_back(part->SuspensionElementClass[i]);
	}
}

void TowerWireGroup::addSpacerSuspensionNode(Part_Base* part)
{
	size_t SusNode = part->SuspensionNode.size();
	for (int i = 0; i < SusNode; i++)
	{
		this->SuspensionNode.push_back(part->SuspensionNode[i]);
		this->WireLogo.push_back(part->WireLoge[i]);
		size_t totalT = this->SuspensionNode.size() - 1;
		SuspensionNode[totalT] = part->Find_tower_idNode(part->SuspensionNode[i]);
	}
	size_t realSusNode = part->realSuspoint.size();
	for (int i = 0; i < realSusNode; i++)
	{
		this->realSuspoint.push_back(part->realSuspoint[i]);
		size_t totalT = this->realSuspoint.size() - 1;
		realSuspoint[totalT] = part->Find_tower_idNode(part->realSuspoint[i]);
	}
}

void TowerWireGroup::AddStrainWireToGroup(CreateStrainWire* wire)
{
	AddStrainWireNode(wire);
	AddWireElement(wire);
	AddAxialForceToInsulator(wire);
	AddRestraintId(wire);
}

void TowerWireGroup::AddStrainWireNode(CreateStrainWire* wire)
{
	size_t wireNode = wire->m_Nodes.size();//tower中的节点
	//add
	for (size_t i = 0; i < wireNode; ++i)//所有线节点的循环
	{
		bool isSuspensionNode = false;

		for (int j = 0; j < this->InstanceSuspoint.size(); j++)
		{
			if ((abs(wire->m_Nodes[i].x - NodeData[InstanceSuspoint[j]].x) < 1e-1) &&
				(abs(wire->m_Nodes[i].y - NodeData[InstanceSuspoint[j]].y) < 1e-1) &&
				(abs(wire->m_Nodes[i].z - NodeData[InstanceSuspoint[j]].z) < 1e-1))
			{
				isSuspensionNode = true;
				wire->wireToGroup.push_back(InstanceSuspoint.at(j));
			}
		}
		if (isSuspensionNode == false)
		{
			this->m_Nodes.push_back(wire->m_Nodes[i]);
			size_t total = this->m_Nodes.size() - 1;//
			this->m_Nodes[total].m_idNode = total + 1;
			//part-tower 节点对应关系
			wire->wireToGroup.push_back(total + 1);
		}
	}
	
}

void TowerWireGroup::AddWireElement(CreateStrainWire* wire)
{
	size_t tTower = wire->m_Elements_Trusses.size();
	size_t bpart = wire->m_Elements_beams.size();
	for (size_t i = 0; i < tTower; ++i)//Truss
	{
		Element_Truss* pE = &wire->m_Elements_Trusses[i];
		this->m_Elements.push_back(wire->m_Elements_Trusses[i]);
		size_t total = this->m_Elements.size() - 1;
		this->m_Elements[total].m_idElement = total + 1;//放入实例的总单元
		this->m_Elements[total].m_idNode[0] = wire->FindGroupIdNode(pE->m_idNode[0]);
		this->m_Elements[total].m_idNode[1] = wire->FindGroupIdNode(pE->m_idNode[1]);
		this->m_Elements_Trusses.push_back(wire->m_Elements_Trusses[i]);
		size_t totalT = this->m_Elements_Trusses.size() - 1;
		this->m_Elements_Trusses[totalT].m_idElement = total + 1;//放入实例的杆单元
		this->m_Elements_Trusses[totalT].m_idNode[0] = wire->FindGroupIdNode(pE->m_idNode[0]);
		this->m_Elements_Trusses[totalT].m_idNode[1] = wire->FindGroupIdNode(pE->m_idNode[1]);
		this->m_Elements_Trusses[totalT].ClassSectionID = pE->ClassSectionID;
		this->m_Elements_Trusses[totalT].MaterialID = pE->MaterialID;
		this->m_Elements_Trusses[totalT].Type = pE->Type;
	}
	for (size_t i = 0; i < bpart; ++i)
	{
		Element_Beam* pE = &wire->m_Elements_beams[i];
		this->m_Elements.push_back(wire->m_Elements_beams[i]);
		size_t total = this->m_Elements.size() - 1;
		this->m_Elements[total].m_idElement = total + 1;//放入实例的总单元
		this->m_Elements[total].m_idNode[0] = wire->FindGroupIdNode(pE->m_idNode[0]);
		this->m_Elements[total].m_idNode[1] = wire->FindGroupIdNode(pE->m_idNode[1]);

		this->m_Elements_beams.push_back(wire->m_Elements_beams[i]);
		size_t totalT = this->m_Elements_beams.size() - 1;
		this->m_Elements_beams[totalT].m_idElement = total + 1;//放入实例的梁单元
		this->m_Elements_beams[totalT].m_idNode[0] = wire->FindGroupIdNode(pE->m_idNode[0]);
		this->m_Elements_beams[totalT].m_idNode[1] = wire->FindGroupIdNode(pE->m_idNode[1]);
		this->m_Elements_beams[totalT].ClassSectionID = pE->ClassSectionID;
		this->m_Elements_beams[totalT].MaterialID = pE->MaterialID;
		this->m_Elements_beams[totalT].direction[0] = pE->direction[0];
		this->m_Elements_beams[totalT].direction[1] = pE->direction[1];
		this->m_Elements_beams[totalT].direction[2] = pE->direction[2];
		this->m_Elements_beams[totalT].Type = pE->Type;
	}

	
}

void TowerWireGroup::AddAxialForceToInsulator(CreateStrainWire* wire)
{
	Splits = wire->fenlie;
	areaWire = wire->area;
	//暂时只考虑悬垂型（加入塔线组合需修改）
	for (int i = 0; i < this->SuspensionElementClass.size(); i++)
	{
		TrussData[this->SuspensionElement[i]].AxialForce =
			(wire->WireGravity[i] + wire->WireGravity[i + 1]) / 2;
		for (int j = 0; j < m_Elements_Trusses.size(); j++)
		{
			if (m_Elements_Trusses[j].m_idElement == this->SuspensionElement[i])
			{
				this->m_Elements_Trusses[j].AxialForce = TrussData[this->SuspensionElement[i]].AxialForce;
				this->m_Elements_Trusses[j].ClassSectionID = wire->InsulatorSectionId;
			}

		}

	}
}

void TowerWireGroup::AddRestraintId(CreateStrainWire* wire)
{
	this->StrainAllRestraintNode = wire->StrainAllRestraintNode;
	this->StrainJointRestraintNode = wire->StrainJointRestraintNode;
}
