#include "TowerWireGroup.h"
#include"CreatWire.h"
int TowerWireGroup::Get_id() const
{
	return 0;
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
}

void TowerWireGroup::AddTowerToGroup(Tower* tower, int towerId, double dx, double dy, double dz, double dAngle)
{
	AddTowerNode(tower, towerId, dx, dy, dz);
	AddTowerElement(tower, towerId);
	rotation(dAngle, towerId);
	AddSuspensionNode(tower);
	tower->TowerToGroup.clear();
}

void TowerWireGroup::AddTowerNode(Tower* tower, int towerId, double dx, double dy, double dz)
{
	size_t towerNode = tower->m_Nodes.size();//tower中的节点
	cout << typeid(*tower).name() << ": " << towerNode << "\n";

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

void TowerWireGroup::AddWireToGroup(CreatWire* wire)
{
	AddWireNode(wire);
	AddWireElement(wire);
}

void TowerWireGroup::AddWireNode(CreatWire* wire)
{
	size_t wireNode = wire->m_Nodes.size();//tower中的节点
	bool isSuspensionNode = 0;
	//add
	for (size_t i = 0; i < wireNode; ++i)
	{
		for (int j = 0; j < wire->susPoint.size(); ++j)
		{
			if (wire->m_Nodes[i].m_idNode = susPoint[j])
			{
				for (int z = 0; z < this->SuspensionNode.size(); z++)
				{
					if (((abs(this->NodeData[SuspensionNode[i]].x - wire->NodeData[susPoint[j]].x) < 1e-8)) &&
						((abs(this->NodeData[SuspensionNode[i]].y - wire->NodeData[susPoint[j]].y)) < 1e-8) &&
						((abs(this->NodeData[SuspensionNode[i]].z - wire->NodeData[susPoint[j]].z)) < 1e-8))
					{
						isSuspensionNode = true;
						wire->wireToGroup.push_back(SuspensionNode[i]);
					}
				}
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

void TowerWireGroup::Show_VTKtruss(vtkRenderer* renderer)
{
	vtkSmartPointer<vtkCellArray>lines = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkLine>line = vtkSmartPointer<vtkLine>::New();
	//点
	for (size_t i = 0; i < m_Elements_Trusses.size(); i++)
	{
		line->GetPointIds()->SetId(0, m_Elements_Trusses[i].m_idNode[0] - 1);
		line->GetPointIds()->SetId(1, m_Elements_Trusses[i].m_idNode[1] - 1);
		lines->InsertNextCell(line);
	}

	vtkSmartPointer<vtkPolyData> linesPolyData = vtkSmartPointer<vtkPolyData>::New();
	linesPolyData->SetPoints(m_pts);
	linesPolyData->SetLines(lines);

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(linesPolyData);
	m_TrussActor = vtkSmartPointer<vtkActor>::New();
	m_TrussActor->SetMapper(mapper);
	m_TrussActor->GetProperty()->SetColor(0, 1, 0);
	renderer->AddActor(m_TrussActor);
}

void TowerWireGroup::Show_VTKbeam(vtkRenderer* renderer)
{
	vtkSmartPointer<vtkCellArray>lines = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkLine>line = vtkSmartPointer<vtkLine>::New();

	for (size_t i = 0; i < m_Elements_beams.size(); i++)
	{
		line->GetPointIds()->SetId(0, m_Elements_beams[i].m_idNode[0] - 1);
		line->GetPointIds()->SetId(1, m_Elements_beams[i].m_idNode[1] - 1);
		lines->InsertNextCell(line);
	}

	vtkSmartPointer<vtkPolyData> linesPolyData = vtkSmartPointer<vtkPolyData>::New();
	linesPolyData->SetPoints(m_pts);
	linesPolyData->SetLines(lines);

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(linesPolyData);
	m_BeamActor = vtkSmartPointer<vtkActor>::New();
	m_BeamActor->SetMapper(mapper);
	m_BeamActor->GetProperty()->SetColor(0, 1, 0);
	renderer->AddActor(m_BeamActor);
}

void TowerWireGroup::Show_VTKnode(vtkRenderer* renderer)
{
	vtkNew<vtkIdTypeArray> Ptr;
	Ptr->SetName("Address");
	m_pts = vtkSmartPointer<vtkPoints>::New();
	int nNode = m_Nodes.size();
	m_pts->SetNumberOfPoints(nNode);
	for (int i = 0; i < m_Nodes.size(); i++)
	{
		vector<double> p;
		p.resize(4);
		p[0] = m_Nodes[i].m_idNode;//m_Nodes[0]指 编号为1的点
		p[1] = m_Nodes[i].x;
		p[2] = m_Nodes[i].y;
		p[3] = m_Nodes[i].z;
		m_pts->SetPoint(i, p[1], p[2], p[3]);
		Node* pNode = &m_Nodes[i];//zhan
		Ptr->InsertNextValue((UINT_PTR)pNode);//将地址转换为整数		
	}
	vtkSmartPointer<vtkPolyData> linesPolyData = vtkSmartPointer<vtkPolyData>::New();
	linesPolyData->SetPoints(m_pts);
	linesPolyData->GetPointData()->AddArray(Ptr);

	//点
	Node_actor = vtkSmartPointer<vtkActor>::New();
	vtkNew<vtkVertexGlyphFilter> VertexFilter;
	VertexFilter->SetInputData(linesPolyData);
	VertexFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> Node_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	Node_mapper->SetInputConnection(VertexFilter->GetOutputPort());

	Node_actor = vtkSmartPointer<vtkActor>::New();
	Node_actor->SetMapper(Node_mapper);
	Node_actor->GetProperty()->SetColor(255, 255, 0);
	Node_actor->GetProperty()->SetPointSize(5);
	renderer->AddActor(Node_actor);
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
