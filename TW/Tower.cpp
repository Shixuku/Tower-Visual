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
#include <vtkConeSource.h>
#include<vtkLineSource.h>
#include <vtkPolyData.h>
#include"InterFace.h"
#include <vtkAssembly.h>

int Tower::FindGroupIdNode(int idNode) const
{
	return TowerToGroup[idNode - 1];
}



void Tower::addPart(Part_Base* part)
{
	if (!part) return;
	addNodeToTower(part);
	addElementToTower(part);
	//addSectionToTower(part);//不每次都把截面添加进去，只能添加一次
	addRestraintNode(part);
	addSuspensionNode(part);
	part->part_to_tower.clear();
}

void Tower::Check()
{
	for (vector<Element>::iterator it = m_Elements.begin(); it != m_Elements.end(); ++it)
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
			{//点在线段内
				it->m_idNode[0] = j.m_idNode;
				it->m_idNode[1] = n1->m_idNode;
				//cout << "修改了一个单元\n";

				int num = m_Elements.size();
				m_Elements.push_back(Element(num + 1, j.m_idNode, n2->m_idNode));

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
	cout << "没找到\n";
	return nullptr;
}

void Tower::move(double dx, double dy, double dz)//直接移动
{
	for (auto& i : this->m_Nodes)
	{
		i.x += dx, i.y += dy, i.z += dz;
	}
}

void Tower::rotation(double angle)//直接旋转
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
	{//原始方向
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

void Tower::addNodeToTower(Part_Base* part)
{
	size_t partNode = part->m_Nodes.size();//part中的节点
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
				if (abs(this->m_Nodes[j].x - part->m_Nodes[i].x) < 1e-2 && abs(this->m_Nodes[j].y - part->m_Nodes[i].y) < 1e-2 && abs(this->m_Nodes[j].z - part->m_Nodes[i].z) < 1e-2)
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
				//part-tower 节点对应关系
				part->part_to_tower.push_back(total + 1);
			}

		}
	}

}

Tower::Tower()
{
}

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
	fin << m_name;//实例名称
	//约束
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

void Tower::Show_Beam(int BeamID, int SectionClass, double a, double b)
{
	for (int i = 0; i < m_Elements_beams.size(); i++)
	{
		if (m_Elements_beams[i].m_idElement == BeamID)
		{
			beamActor bA;
			int ipt1 = m_Elements_beams[i].m_idNode[0] - 1; //id
			int ipt2 = m_Elements_beams[i].m_idNode[1] - 1;
			double x1 = m_Nodes[ipt1].x; double y1 = m_Nodes[ipt1].y; double z1 = m_Nodes[ipt1].z;
			double x2 = m_Nodes[ipt2].x; double y2 = m_Nodes[ipt2].y; double z2 = m_Nodes[ipt2].z;
			if (z2 - z1 > 0.01)//
			{
				bA.setNode(x1, y1, z1, x2, y2, z2);
				double x[6];
				double y[6];



				x[0] = 0; x[1] = a; x[2] = a; x[3] = b; x[4] = b; x[5] = 0;
				y[0] = 0; y[1] = 0;   y[2] = b; y[3] = b; y[4] = a; y[5] = a;

				bA.SetSection(x, y);
				int n11 = m_Elements_beams[i].m_idNode[0] - 1;
				int n12 = m_Elements_beams[i].m_idNode[1] - 1;
				if (m_Nodes[n11].x > 0 && m_Nodes[n11].y > 0 && m_Nodes[n11].z != m_Nodes[n12].z)
				{
					bA.Set_zAxis(0, -1, 0);
				}
				else if (m_Nodes[n11].x < 0 && m_Nodes[n11].y < 0 && m_Nodes[n11].z != m_Nodes[n12].z)
				{
					bA.Set_zAxis(0, 1, 0);
				}
				else if (m_Nodes[n11].x > 0 && m_Nodes[n11].y < 0 && m_Nodes[n11].z != m_Nodes[n12].z)
				{
					bA.Set_zAxis(-1, 0, 0);
				}
				else if (m_Nodes[n11].x < 0 && m_Nodes[n11].y > 0 && m_Nodes[n11].z != m_Nodes[n12].z)
				{
					bA.Set_zAxis(1, 0, 0);
				}
			}
			else
			{
				double Horizontalvectors1[3];
				double Horizontalvectors2[3];
				double Horizontalvectors3[3];
				Horizontalvectors1[0] = x2 - x1;
				Horizontalvectors1[1] = y2 - y1;
				Horizontalvectors1[2] = z2 - z1;
				Horizontalvectors2[0] = 0 - x1;
				Horizontalvectors2[1] = 0 - y1;
				Horizontalvectors2[2] = z1 - z1;
				vtkMath::Cross(Horizontalvectors1, Horizontalvectors2, Horizontalvectors3);
				if (Horizontalvectors3[2] < 0)
				{
					bA.setNode(x1, y1, z1, x2, y2, z2);
					double x[6];
					double y[6];
					x[0] = 0; x[1] = a; x[2] = a; x[3] = b; x[4] = b; x[5] = 0;
					y[0] = 0; y[1] = 0;   y[2] = b; y[3] = b; y[4] = a; y[5] = a;
					bA.SetSection(x, y);
					bA.Set_zAxis(0, 0, -1);
				}
				else
				{
					bA.setNode(x2, y2, z2, x1, y1, z1);
					double x[6];
					double y[6];
					x[0] = 0; x[1] = a; x[2] = a; x[3] = b; x[4] = b; x[5] = 0;
					y[0] = 0; y[1] = 0;   y[2] = b; y[3] = b; y[4] = a; y[5] = a;
					bA.SetSection(x, y);
					bA.Set_zAxis(0, 0, -1);
				}
			}
			vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
			//vtkSmartPointer<vtkAssembly>test_assemblyNactor = vtkSmartPointer<vtkAssembly>::New();
			//bA.Create_Actor(0, 1, 0, actor/*, test_assemblyNactor*/);
			//InstanceNactor.push_back(actor);
		}
	}

}

void Tower::AddNewSection(int id)
{
	 pSection.push_back(id);
}

void Tower::addElementToTower(Part_Base* part)
{

	size_t tpart = part->m_Elements_Trusses.size();
	size_t bpart = part->m_Elements_beams.size();
	size_t SusElementClass = part->SuspensionElementClass.size();
	size_t SusElement = part->SuspensionElement.size();
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


void Tower::addRestraintNode(Part_Base* part)
{
	size_t ResNode = part->RestraintNode.size();
	cout << "ResNode= " << ResNode << "\n";
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

