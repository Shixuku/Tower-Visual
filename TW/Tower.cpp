#include "Tower.h"
#include<vtkTransform.h>
#include <vtkIdTypeArray.h>
#include <QOpenGLWidget>//UINT_PTR
#include <vtkPointData.h>//->AddArray(Ptr)
#include"beamActor.h"
#include<vtkDoubleArray.h>
#include<vtkArrowSource.h>
#include <vtkTubeFilter.h>
#include <iostream>
#include<fstream>
#include <vtkAppendPolyData.h>
#include <vtkConeSource.h>
#include<vtkLineSource.h>
#include <vtkPolyData.h>
#include"InterFace.h"
#include <vtkAssembly.h>
#include<vtkTextProperty.h>
#include<vtkLabelHierarchy.h>
#include<vtkStringArray.h>
#include<vtkPointSetToLabelHierarchy.h>
#include<vtkLabelPlacementMapper.h>
#include<vtkFreeTypeLabelRenderStrategy.h>

#include"HangPoint.h"
#include<vtkActor2D.h>
int Tower::FindGroupIdNode(int idNode) const
{
	return TowerToGroup[idNode - 1];
}



void Tower::addPart(Part_Base* part)
{
	if (!part) return;
	addNodeToTower(part);
	addElementToTower(part);
	//addSectionToTower(part);//��ÿ�ζ��ѽ�����ӽ�ȥ��ֻ�����һ��
	addRestraintNode(part);
	addSuspensionNode(part);
	addHangPoint(part);
	//HangPintList();
	part->part_to_tower.clear();
}

void Tower::Check_Beam()
{
	for (vector<Element_Beam>::iterator it = m_Elements_beams.begin(); it != m_Elements_beams.end(); ++it)
	{
		Node* n1 = FindNode(it->m_idNode[0]);
		Node* n2 = FindNode(it->m_idNode[1]);
		double dx = n2->x - n1->x;
		double dy = n2->y - n1->y;
		double dz = n2->z - n1->z;

		for (auto& j : m_Nodes)
		{
			if ((j.m_idNode == n1->m_idNode) || (j.m_idNode == n2->m_idNode)) continue;
			double lx = j.x - n1->x;
			double ly = j.y - n1->y;
			double lz = j.z - n1->z;

			double rx = j.x - n2->x;
			double ry = j.y - n2->y;
			double rz = j.z - n2->z;

			bool isPara = (sqrt(dx * dx + dy * dy + dz * dz) == sqrt(rx * rx + ry * ry + rz * rz) + sqrt(lx * lx + ly * ly + lz * lz));
			if (isPara)
			{//�����߶���
				it->m_idNode[0] = j.m_idNode;
				it->m_idNode[1] = n1->m_idNode;
				int num = m_Elements_beams.size() + m_Elements_Trusses.size();
				m_Elements_beams.push_back(Element_Beam(num + 1, j.m_idNode, n2->m_idNode, it->ClassSectionID, it->direction, 1));

			}
		}
	}
}

Node* Tower::FindNode(int id)
{
	for (auto& i : m_Nodes)
	{
		if (i.m_idNode == id) return &i;
	}
	cout << "û�ҵ�\n";
	return nullptr;
}

void Tower::move(double dx, double dy, double dz)//ֱ���ƶ�
{
	for (auto& i : this->m_Nodes)
	{
		i.x += dx, i.y += dy, i.z += dz;
	}
}

void Tower::rotation(double angle)//ֱ����ת
{
	double cos_theta = cos(angle);
	double sin_theta = sin(angle);
	Vector3d fai;
	fai = { 0,0,1 };
	for (auto& i : this->m_Nodes)
	{
		Vector3d u = { i.x, i.y, i.z };
		Vector3d up;
		up = u * cos_theta + fai.cross(u) * sin_theta + fai.dot(u) * fai * (1 - cos_theta);
		i.x = up[0];
		i.y = up[1];
		i.z = up[2];
	}
	for (auto& j : this->m_Elements_beams)
	{//ԭʼ����
		double x = j.direction[0];
		double y = j.direction[1];
		double z = j.direction[2];
		//��ת
		double new_x = cos_theta * x - sin_theta * z;
		double new_y = y;
		double new_z = sin_theta * x + cos_theta * z;
		//������ת����
		j.direction[0] = new_x;
		j.direction[1] = new_y;
		j.direction[2] = new_z;
	}
}

void Tower::showHangPoint(vtkRenderer* renderer)
{
	vtkSmartPointer<vtkPoints> m_pts = vtkSmartPointer<vtkPoints>::New();
	m_pts->SetNumberOfPoints(TP_HangPoint.size());
	vtkSmartPointer<vtkPoints> PYm_pts = vtkSmartPointer<vtkPoints>::New();//����ƫ��
	PYm_pts->SetNumberOfPoints(TP_HangPoint.size());
	vtkNew<vtkLabelHierarchy> polyData;
	vtkSmartPointer<vtkStringArray>label = vtkSmartPointer<vtkStringArray>::New();
	vtkNew<vtkTextProperty> TextProperty;
	vtkNew<vtkPointSetToLabelHierarchy> Hie;
	vtkNew<vtkLabelPlacementMapper> MapperLabel;
	vtkNew<vtkFreeTypeLabelRenderStrategy> strategy_tmp;
	// ����һ��vtkActor2D����vtkTextMapper����Ϊ��ӳ����
	vtkSmartPointer<vtkActor2D> actor = vtkSmartPointer<vtkActor2D>::New();
	for (int i = 0; i < TP_HangPoint.size(); i++)
	{
		for (const auto& m : m_Nodes)
		{
			if (TP_HangPoint.Find_Entity(i + 1)->NodeId == m.m_idNode)//����ڵ��id==������
			{
				//�������ҵ����
				m_pts->SetPoint(i, m.x, m.y, m.z);
				vtkSmartPointer<vtkPolyData> linesPolyData = vtkSmartPointer<vtkPolyData>::New();
				linesPolyData->SetPoints(m_pts);
				vtkNew<vtkVertexGlyphFilter> VertexFilter;
				VertexFilter->SetInputData(linesPolyData);
				VertexFilter->Update();
				vtkSmartPointer<vtkPolyDataMapper> Node_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
				Node_mapper->SetInputConnection(VertexFilter->GetOutputPort());
				vtkSmartPointer<vtkActor>Node_actor = vtkSmartPointer<vtkActor>::New();
				Node_actor->SetMapper(Node_mapper);
				Node_actor->GetProperty()->SetColor(0.818, 0.365, 0.132);//ƫ��ɫ
				Node_actor->GetProperty()->SetPointSize(8);
				renderer->AddActor(Node_actor);
				m_HangPointActor.push_back(Node_actor);//���뵽actor��������
				////�������ּ��ж�
				PYm_pts->SetPoint(i, m.x + 0.030, m.y + 0.030, m.z + 0.030);//����ƫ�Ƶ�λ��
				QString WireLoge = TP_HangPoint.Find_Entity(i + 1)->WireLoge + "-" + TP_HangPoint.Find_Entity(i + 1)->StringClass;
				std::string str = WireLoge.toStdString();
				const char* name = str.c_str();
				label->InsertNextValue(name);
				label->SetName("Labels");
				polyData->SetPoints(PYm_pts);
				polyData->GetPointData()->AddArray(label);
				TextProperty->SetFontSize(15);
				TextProperty->SetColor(0.818, 0.365, 0.132);//ƫ��ɫ
				TextProperty->SetBold(true);
				TextProperty->SetFontFamilyToArial();
				Hie->SetInputData(polyData);
				Hie->SetMaximumDepth(1000);
				Hie->SetLabelArrayName("Labels");
				Hie->SetTextProperty(TextProperty);
				MapperLabel->SetInputConnection(Hie->GetOutputPort());	
				MapperLabel->SetRenderStrategy(strategy_tmp);
				MapperLabel->SetShapeToNone();
				MapperLabel->UseDepthBufferOn();
				MapperLabel->SetStyleToOutline();
				actor->SetMapper(MapperLabel);
				renderer->AddActor(actor);
				m_HangPointLabelActor.push_back(actor);
			}
		}
	}
}

void Tower::addNodeToTower(Part_Base* part)
{
	size_t partNode = part->m_Nodes.size();//part�еĽڵ�
	//add
	for (size_t i = 0; i < partNode; ++i)
	{

		int a = this->m_Nodes.size();
		int Judg = 0;
		if (a == 0)
		{
			this->m_Nodes.push_back(part->m_Nodes[i]);
			this->m_Nodes[0].m_idNode = 1;
			part->part_to_tower.push_back(1);
		}
		else
		{
			for (int j = 0; j < a; j++)
			{
				if (abs(this->m_Nodes[j].x - part->m_Nodes[i].x) < 1e-3 && abs(this->m_Nodes[j].y - part->m_Nodes[i].y) < 1e-3 && abs(this->m_Nodes[j].z - part->m_Nodes[i].z) < 1e-3)
				{
					part->part_to_tower.push_back(this->m_Nodes[j].m_idNode);
					j = a;

				}
				else
				{
					Judg++;

				}

			}
			if (Judg == a)
			{
				this->m_Nodes.push_back(part->m_Nodes[i]);
				size_t total = this->m_Nodes.size() - 1;//
				this->m_Nodes[total].m_idNode = total + 1;
				//part-tower �ڵ��Ӧ��ϵ
				part->part_to_tower.push_back(total + 1);
			}

		}
	}

}

Tower::Tower(){}

void Tower::SaveTo(QDataStream& fin) const
{
	fin << m_id;
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
	fin << m_name;//ʵ������
	//Լ��
	int nRestraintNode = RestraintNode.size();
	fin << nRestraintNode;
	for (int i = 0; i < nRestraintNode; i++)
	{
		fin << RestraintNode[i];
		cout << RestraintNode[i] << "\n";
	}
}

void Tower::Input(QDataStream& fin)
{
	fin >> m_id;
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
	int nSuspension ;
	fin >> nSuspension;
	SuspensionNode.resize(nSuspension);
	for (int i = 0; i < nSuspension; ++i)
	{
		fin >> SuspensionNode[i];
	}
	fin >> m_name;
	int nRestraintNode;
	fin >> nRestraintNode;
	RestraintNode.resize(nRestraintNode);
	for (int i = 0; i < nRestraintNode; i++)
	{
		fin >> RestraintNode[i];
	}
}

void Tower::ShowNode()const
{
	for (auto& j : m_Nodes)
	{
		j.show();
	}
}

void Tower::ShowElement() const
{
	for (auto& j : m_Elements)
	{
		j.show();
	}
}

void Tower::ShowTrussElement() const
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

void Tower::ShowBeamElement() const
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

void Tower::addElementToTower(Part_Base* part)
{

	size_t tpart = part->m_Elements_Trusses.size();
	size_t bpart = part->m_Elements_beams.size();
	size_t SusElementClass = part->SuspensionElementClass.size();
	size_t SusElement = part->SuspensionElement.size();
	int size = 0;//������Ҿ�Ե�Ӵ��ĵ�Ԫʱ�����������ĵ�Ԫ��������ѭ���ж�
	for (size_t i = 0; i < tpart; ++i)//Truss
	{
		Element_Truss* pE = &part->m_Elements_Trusses[i];
		this->m_Elements.push_back(part->m_Elements_Trusses[i]);
		size_t total = this->m_Elements.size() - 1;
		this->m_Elements[total].m_idElement = total + 1;//����ʵ�����ܵ�Ԫ
		this->m_Elements[total].m_idNode[0] = part->Find_tower_idNode(pE->m_idNode[0]);
		this->m_Elements[total].m_idNode[1] = part->Find_tower_idNode(pE->m_idNode[1]);

		this->m_Elements_Trusses.push_back(part->m_Elements_Trusses[i]);
		size_t totalT = this->m_Elements_Trusses.size() - 1;
		this->m_Elements_Trusses[totalT].m_idElement = total + 1;//����ʵ���ĸ˵�Ԫ
		this->m_Elements_Trusses[totalT].m_idNode[0] = part->Find_tower_idNode(pE->m_idNode[0]);
		this->m_Elements_Trusses[totalT].m_idNode[1] = part->Find_tower_idNode(pE->m_idNode[1]);
		this->m_Elements_Trusses[totalT].ClassSectionID = pE->ClassSectionID;
		this->m_Elements_Trusses[totalT].MaterialID = pE->MaterialID;
		if (size != SusElementClass)
		{
			for (int j = 0; j < SusElement; j++)
			{
				if (part->m_Elements_Trusses[i].m_idElement == part->SuspensionElement[j])
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
		this->m_Elements[total].m_idElement = total + 1;//����ʵ�����ܵ�Ԫ
		this->m_Elements[total].m_idNode[0] = part->Find_tower_idNode(pE->m_idNode[0]);
		this->m_Elements[total].m_idNode[1] = part->Find_tower_idNode(pE->m_idNode[1]);

		this->m_Elements_beams.push_back(part->m_Elements_beams[i]);
		size_t totalT = this->m_Elements_beams.size() - 1;
		this->m_Elements_beams[totalT].m_idElement = total + 1;//����ʵ��������Ԫ
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


void Tower::addRestraintNode(Part_Base* part)
{
	size_t ResNode = part->RestraintNode.size();
	for (int i = 0; i < ResNode; i++)
	{
		this->RestraintNode.push_back(part->RestraintNode[i]);
		RestraintNode[i] = part->Find_tower_idNode(part->RestraintNode[i]);
	}
}

void Tower::addSuspensionNode(Part_Base* part)
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

void Tower::addHangPoint(Part_Base* part)
{
	size_t HangPointSize = part->TP_HangPoint.size();
	for (int i = 0; i < HangPointSize; i++)
	{
		HangPoint* hangPoint = new HangPoint();
		hangPoint->m_id = this->TP_HangPoint.size() + 1;
		hangPoint->StringClass = part->TP_HangPoint.Find_Entity(i + 1)->StringClass;
		hangPoint->WireLoge = part->TP_HangPoint.Find_Entity(i + 1)->WireLoge;
		hangPoint->NodeId= part->Find_tower_idNode(part->TP_HangPoint.Find_Entity(i + 1)->NodeId);
		
		this->TP_HangPoint.Add_Entity(hangPoint);
		QString StringClass = hangPoint->StringClass;
		QString WireLoge = hangPoint->WireLoge;
		int nodeid = hangPoint->NodeId;
		int HangSize = this->m_HangList.size() + 1;
		if (StringClass == "V")
		{
			bool isFind = false;
			for (auto& j : m_HangList)
			{
				if (j.StringClass == StringClass)
				{
					if (j.WireLoge == WireLoge)
					{
						j.NodeId.push_back(nodeid);
						for (auto& j : this->m_Nodes)
						{
							if (j.m_idNode == nodeid)
							{
								j.Type = 1;
								break;
							}
						}
						isFind = true;
					}
				}
			}
			if (isFind == false)
			{
				m_HangList.push_back(HangList(HangSize, StringClass, WireLoge));
				m_HangList[HangSize - 1].NodeId.push_back(nodeid);
				for (auto& j : this->m_Nodes)
				{
					if (j.m_idNode == nodeid)
					{
						j.Type = 1;
						break;
					}
				}
			}
		}
		else
		{
			m_HangList.push_back(HangList(HangSize, StringClass, WireLoge));
			m_HangList[HangSize - 1].NodeId.push_back(nodeid);
			for (auto& j : this->m_Nodes)
			{
				if (j.m_idNode == nodeid)
				{
					j.Type = 1;
					break;
				}
			}
		}
	}
}

//void Tower::HangPintList()
//{
//	size_t HangPointSize = this->TP_HangPoint.size();
//	for (int i = 0; i < HangPointSize; i++)
//	{
//		QString StringClass = TP_HangPoint.Find_Entity(i + 1)->StringClass;
//		QString WireLoge = TP_HangPoint.Find_Entity(i + 1)->WireLoge;
//		int nodeid= TP_HangPoint.Find_Entity(i + 1)->NodeId;
//		int HangSize = this->m_HangList.size() + 1;
//		if (StringClass == "V")
//		{
//			bool isFind = false;
//			for (auto& j : m_HangList)
//			{
//				if (j.StringClass == StringClass)
//				{
//					if (j.WireLoge == WireLoge)
//					{
//						j.NodeId.push_back(nodeid);
//						isFind = true;
//					}
//				}
//			}
//			if (isFind == false)
//			{
//				m_HangList.push_back(HangList(HangSize, StringClass, WireLoge));
//				m_HangList[HangSize - 1].NodeId.push_back(nodeid);
//			}
//		}
//		else
//		{
//			m_HangList.push_back(HangList(HangSize, StringClass, WireLoge));
//			m_HangList[HangSize - 1].NodeId.push_back(nodeid);
//		}
//		
//	}
//	size_t m_HangListSize = m_HangList.size();
//	qDebug() << "m_HangListSize" << m_HangListSize << "\n";
//}

