#include "Tower.h"
#include<vtkTransform.h>
#include <vtkIdTypeArray.h>
#include <QOpenGLWidget>//UINT_PTR
#include <vtkPointData.h>//->AddArray(Ptr)
#include"beamActor.h"

void Tower::Show_VTKtruss(vtkRenderer* renderer)
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
	//vtkSmartPointer<vtkPoints> pts = vtkSmartPointer<vtkPoints>::New();
	//for (int i = 0; i < m_Nodes.size(); i++)
	//{
	//	vector<double> p;
	//	p.resize(4);
	//	p[0] = m_Nodes[i].m_idNode;//m_Nodes[0]指 编号为1的点
	//	p[1] = m_Nodes[i].x;
	//	p[2] = m_Nodes[i].y;
	//	p[3] = m_Nodes[i].z;
	//	pts->InsertNextPoint(p[1], p[2], p[3]);
	//}

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

void Tower::Show_VTKbeam(vtkRenderer* renderer)
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

void Tower::Show_VTKnode(vtkRenderer* renderer)
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

void Tower::addPart(Part_Base* part)
{
	if (!part) return;
	addNodeToTower(part);
	addElementToTower(part);
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

Tower Tower::moved(double dx, double dy, double dz) const//复制 然后移动
{
	Tower t(*this);
	for (auto& i : t.m_Nodes)
	{
		i.x += dx, i.y += dy, i.z += dz;
	}
	return t;
}

Tower Tower::rotationed(Vector3d fai, double angle) const//复制 然后旋转
{
	Tower t(*this);

	double cos_theta = cos(angle);
	double sin_theta = sin(angle);
	fai.normalize();

	for (auto& i : t.m_Nodes)
	{
		Vector3d u = { i.x, i.y, i.z };
		Vector3d up;
		up = u * cos_theta + fai.cross(u) * sin_theta + fai.dot(u) * fai * (1 - cos_theta);
		i.x = up[0];
		i.y = up[1];
		i.z = up[2];
	}
	return t;
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
}

void Tower::addNodeToTower(Part_Base* part)
{
	size_t partNode = part->m_Nodes.size();//part中的节点
	cout << typeid(*part).name() << ": " << partNode << "\n";

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

int Tower::Get_id() const
{
	return m_id;
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
		cout << m_Elements_beams[i].m_idNode[0] << "   " << m_Elements_beams[i].m_idNode[1] << "\n";
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
			bA.Create_Actor(0, 1, 0, actor);
			Nactor.push_back(actor);
		}
	}

}

void Tower::addElementToTower(Part_Base* part)
{
	
	size_t tpart = part->m_Elements_Trusses.size();
	size_t bpart = part->m_Elements_beams.size();

	for (size_t i = 0; i < tpart; ++i)//Truss
	{
		Element* pE = &part->m_Elements_Trusses[i];
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

	}
	for (size_t i = 0; i < bpart; ++i)
	{
		Element* pE = &part->m_Elements_beams[i];
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
	}
}

