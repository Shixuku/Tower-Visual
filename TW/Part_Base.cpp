#include "Part_Base.h"
#include <vtkIdTypeArray.h>
#include <QOpenGLWidget>//UINT_PTR
#include <vtkPointData.h>//->AddArray(Ptr)
#include"InterFace.h"
#include"beamActor.h"
#include"circle.h"
#include "vtkLineSource.h"
#include<vtkTubeFilter.h>
#include"Element_Beam.h"
#pragma execution_character_set("utf-8")
int Part_Base::Get_id() const
{
	return m_id;
}

int Part_Base::Find_tower_idNode(int idNode) const
{
	return part_to_tower[idNode - 1];
}

void Part_Base::Show_VTKtruss(vtkRenderer* renderer)
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
//	vtkSmartPointer<vtkPoints> pts = vtkSmartPointer<vtkPoints>::New();
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
	m_TrussActor->GetProperty()->SetColor(0, 0, 0);//设置颜色(0,0,0)表示黑色
	renderer->AddActor(m_TrussActor);
}

void Part_Base::Show_VTKbeam(vtkRenderer* renderer)
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
	m_BeamActor->GetProperty()->SetColor(0, 0, 0);//设置颜色(0,0,0)表示黑色
	renderer->AddActor(m_BeamActor);
}

void Part_Base::ShowMessage()
{
	for (int i = 0; i < m_Elements_beams.size(); i++)
	{
		cout << m_Elements_beams[i].m_idElement << "  " << m_Elements_beams[i].direction[0] << "  "
			<< m_Elements_beams[i].direction[1] << "  " << m_Elements_beams[i].direction[2] << "\n";
	}
	
}

void Part_Base::Show_VTKnode(vtkRenderer* renderer)
{
	vtkNew<vtkIdTypeArray> Ptr;//zhan
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
		//m_Nodes[i].show();//显示节点坐标
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
	//auto dataArray = Node_mapper->GetInput()->GetPointData()->GetArray("Address");//取得地址数组
	//auto ptrArray = dynamic_cast<vtkIdTypeArray*> (dataArray);//转换为整数数组

	Node_actor = vtkSmartPointer<vtkActor>::New();
	Node_actor->SetMapper(Node_mapper);
	Node_actor->GetProperty()->SetColor(255, 255, 0);
	Node_actor->GetProperty()->SetPointSize(5);
	renderer->AddActor(Node_actor);

}


void Part_Base::Clear_Part()
{
	m_Elements_beams.resize(0);
	m_Elements_Trusses.resize(0);
}

void Part_Base::interpolation(const Node& n1, const Node& n2, double num, Node& n)
{
	n.x = n1.x + (n2.x - n1.x) * num;
	n.y = n1.y + (n2.y - n1.y) * num;
	n.z = n1.z + (n2.z - n1.z) * num;
}

void Part_Base::Node4_Creat1(const Node& n1, const Node& n2, const Node& n3, const Node& n4, Node& n)
{
	double a1 = n2.x - n1.x; double b1 = n2.y - n1.y; double c1 = n2.z - n1.z;
	double a2 = n3.x - n1.x; double b2 = n3.y - n1.y; double c2 = n3.z - n1.z;
	double a3 = n4.x - n1.x; double b3 = n4.y - n1.y; double c3 = n4.z - n1.z;

	//首先判断两直线是否重合，即向量AB，AC，AD是否成比例
	if ((a1 * b2 == b1 * a2 && a1 * b3 == b1 * a3) /*&& (a1 * b3 == b1 * a3 && a1 * c3 == c1 * a3)*/)
		std::cout << "两直线重合!" << "\n";
	else
	{
		//根据四点共面定理,若三个向量的混合积(AB,AC,AD)!=0,则四点不共面.
		if ((a1 * b2 * c3 + a2 * b3 * c1 + a3 * b1 * c2 - a3 * b2 * c1 - a1 * b3 * c2 - a2 * b1 * c3) != 0)
		{
			std::cout << "两直线异面，没有交点!" << "\n";
		}
		else
		{   //共面以后判断是否平行，向量AB=(a1,b1,c1),向量CD=(a3-a2,b3-b2,c3-c2)
			//若向量CD是AB成比例，则表示平行.
			if ((a3 - a2) * b1 == (b3 - b2) * a1 && (a3 - a2) * c1 == (c3 - c2) * a1)
			{
				std::cout << "两直线平行，没有交点!" << "\n";
			}
			else
			{
				//通过计算，得到交点的坐标
				n.x = (a1 * a3 * b2 - a1 * a2 * b3) / (b1 * a3 + b2 * a1 - b3 * a1 - a2 * b1) + n1.x;
				n.y = (b1 * a3 * b2 - b1 * a2 * b3) / (b1 * a3 + b2 * a1 - b3 * a1 - a2 * b1) + n1.y;
				n.z = (c1 * a3 * b2 - c1 * a2 * b3) / (b1 * a3 + b2 * a1 - b3 * a1 - a2 * b1) + n1.z;
			}

		}

	}
}

void Part_Base::Creat_Beams(vector<Element_Beam>& m_Elements_beams, vector<int> ids)
{
	size_t size = ids.size();
	int node1 = ids[0];
	id_BeamSection++;
	double iDirection[3];
	for (int i = 1; i < size; i++)
	{
		int node2 = ids[i];
		if (m_Nodes[node1-1].x > 0 && m_Nodes[node1-1].y > 0 && m_Nodes[node1-1].z != m_Nodes[node2-1].z)
		{
			iDirection[0] = 0;iDirection[1] = -1;iDirection[2] = 0;
		}
		else if (m_Nodes[node1 - 1].x < 0 && m_Nodes[node1 - 1].y < 0 && m_Nodes[node1 - 1].z != m_Nodes[node2 - 1].z)
		{
			iDirection[0] = 0;iDirection[1] = 1;iDirection[2] = 0;
		}
		else if (m_Nodes[node1 - 1].x > 0 && m_Nodes[node1 - 1].y < 0 && m_Nodes[node1 - 1].z != m_Nodes[node2 - 1].z)
		{
			iDirection[0] = -1;iDirection[1] = 0;iDirection[2] = 0;
		}
		else if (m_Nodes[node1 - 1].x < 0 && m_Nodes[node1 - 1].y > 0 && m_Nodes[node1 - 1].z != m_Nodes[node2 - 1].z)
		{
			iDirection[0] = 1;iDirection[1] = 0;iDirection[2] = 0;
		}
		else
		{
			iDirection[0] = 0; iDirection[1] = 0; iDirection[2] = -1;
		}
		m_Elements_beams.push_back(Element_Beam(Beam_elementsID + 1, node1, node2, id_BeamSection, iDirection));
		Beam_elementsID++;
		node1 = node2;
	}
}

void Part_Base::Creat_Beams1(vector<Element_Beam>& m_Elements_Beams, vector<int> ids)
{
	size_t size = ids.size();
	id_BeamSection++;
	double iDirection[3];
	for (int i = 0; i < size; i += 2)
	{
		int node1 = ids[i];
		int node2 = ids[i + 1];
		if (m_Nodes[node1 - 1].x > 0 && m_Nodes[node1 - 1].y > 0 && m_Nodes[node1 - 1].z != m_Nodes[node2 - 1].z)
		{
			iDirection[0] = 0;iDirection[1] = -1;iDirection[2] = 0;
		}
		else if (m_Nodes[node1 - 1].x < 0 && m_Nodes[node1 - 1].y < 0 && m_Nodes[node1 - 1].z != m_Nodes[node2 - 1].z)
		{
			iDirection[0] = 0;iDirection[1] = 1;iDirection[2] = 0;
		}
		else if (m_Nodes[node1 - 1].x > 0 && m_Nodes[node1 - 1].y < 0 && m_Nodes[node1 - 1].z != m_Nodes[node2 - 1].z)
		{
			iDirection[0] = -1;iDirection[1] = 0;iDirection[2] = 0;	
		}
		else if (m_Nodes[node1 - 1].x < 0 && m_Nodes[node1 - 1].y > 0 && m_Nodes[node1 - 1].z != m_Nodes[node2 - 1].z)
		{
			iDirection[0] = 1;iDirection[1] = 0;iDirection[2] = 0;
		}
		else
		{
			iDirection[0] = 0;iDirection[1] = 0;iDirection[2] = -1;
		}
		m_Elements_Beams.push_back(Element_Beam(Beam_elementsID + 1, node1, node2, id_BeamSection, iDirection));
		Beam_elementsID++;
	}
}

void Part_Base::Creat_Trusses(vector<Element_Truss>& m_Elements_Trusses, vector<int> ids)
{
	size_t size = ids.size();
	int node1 = ids[0];
	id_TrussSection++;
	for (int i = 1; i < size; i++)
	{
		int node2 = ids[i];
		m_Elements_Trusses.push_back(Element_Truss(Truss_elementsID + 1, node1, node2, id_TrussSection));
		Truss_elementsID++;
		node1 = node2;
	}
}

void Part_Base::Creat_Trusses1(vector<Element_Truss>& m_Elements_Trusses, vector<int> ids)
{
	size_t size = ids.size();
	id_TrussSection++;
	for (int i = 0; i < size; i += 2)
	{
		int node1 = ids[i];
		int node2 = ids[i + 1];
		m_Elements_Trusses.push_back(Element_Truss(Truss_elementsID + 1, node1, node2, id_TrussSection));
		Truss_elementsID++;
	}
}

void Part_Base::InPutRestraintNode(vector<int> ids)
{
	RestraintNode.push_back(ids[0]);
}

int Part_Base::Creat_Node(double x, double y, double z)
{

	int SIZE = m_Nodes.size();
	if (SIZE == 0)
	{
		m_Nodes.push_back(Node(1, x, y, z));
		return m_Nodes[0].m_idNode;
	}
	int Judg = 0;
	for (int i = 0; i < SIZE; i++)
	{
		if (abs(m_Nodes[i].x - x) < 1 && abs(m_Nodes[i].y - y) < 1 && abs(m_Nodes[i].z - z) < 1)
		{
			return m_Nodes[i].m_idNode; //重节点
			i = SIZE;
		}
		else
		{
			Judg++;
		}
	}

	if (Judg == SIZE)
	{
		//m_Nodes[Judg - 1].m_idNode = SIZE;
		//m_Nodes[Judg - 1].x = x;
		//m_Nodes[Judg - 1].y = y;
		//m_Nodes[Judg - 1].z = z;
		m_Nodes.push_back(Node(Judg + 1, x, y, z));

		return m_Nodes[Judg].m_idNode;
	}

}

void Part_Base::SaveTo(QDataStream& fin) const
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
	fin << id_BeamSection;
	fin << id_TrussSection;
}

void Part_Base::Input(QDataStream& fin)
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
	fin >> id_BeamSection;
	fin >> id_TrussSection;

}

//void Part_Base::show_SectionBeam(double a, double b, int section, int M, int group, bool IsBeam)
//{
//	pMaterial.push_back(Section(a, b, group, section, M));
//	int number = 0;
//	if (IsBeam == true)
//	{
//		switch (section)
//		{
//		case ET_L:
//
//			for (int i = 0; i < m_Elements_beams.size(); i++)
//			{
//				if (group == m_Elements_beams[i].sectionID)
//				{
//					beamActor bA;
//					int ipt1 = m_Elements_beams[i].m_idNode[0] - 1; //id
//					int ipt2 = m_Elements_beams[i].m_idNode[1] - 1;
//					double x1 = m_Nodes[ipt1].x; double y1 = m_Nodes[ipt1].y; double z1 = m_Nodes[ipt1].z;
//					double x2 = m_Nodes[ipt2].x; double y2 = m_Nodes[ipt2].y; double z2 = m_Nodes[ipt2].z;
//					if (z2 - z1 > 0.01)//
//					{
//						bA.setNode(x1, y1, z1, x2, y2, z2);
//						double x[6];
//						double y[6];
//						for (auto& j : pMaterial)
//						{
//							if (m_Elements_beams[i].sectionID == j.m_id)
//							{
//								x[0] = 0; x[1] = j.a; x[2] = j.a; x[3] = j.b; x[4] = j.b; x[5] = 0;
//								y[0] = 0; y[1] = 0;   y[2] = j.b; y[3] = j.b; y[4] = j.a; y[5] = j.a;
//							}
//						}
//						bA.SetSection(x, y);
//						int n11 = m_Elements_beams[i].m_idNode[0] - 1;
//						int n12 = m_Elements_beams[i].m_idNode[1] - 1;
//						if (m_Nodes[n11].x > 0 && m_Nodes[n11].y > 0 && m_Nodes[n11].z != m_Nodes[n12].z)
//						{
//							bA.Set_zAxis(0, -1, 0);
//						}
//						else if (m_Nodes[n11].x < 0 && m_Nodes[n11].y < 0 && m_Nodes[n11].z != m_Nodes[n12].z)
//						{
//							bA.Set_zAxis(0, 1, 0);
//						}
//						else if (m_Nodes[n11].x > 0 && m_Nodes[n11].y < 0 && m_Nodes[n11].z != m_Nodes[n12].z)
//						{
//							bA.Set_zAxis(-1, 0, 0);
//						}
//						else if (m_Nodes[n11].x < 0 && m_Nodes[n11].y > 0 && m_Nodes[n11].z != m_Nodes[n12].z)
//						{
//							bA.Set_zAxis(1, 0, 0);
//						}
//					}
//					else
//					{
//						double Horizontalvectors1[3];
//						double Horizontalvectors2[3];
//						double Horizontalvectors3[3];
//						Horizontalvectors1[0] = x2 - x1;
//						Horizontalvectors1[1] = y2 - y1;
//						Horizontalvectors1[2] = z2 - z1;
//						Horizontalvectors2[0] = 0 - x1;
//						Horizontalvectors2[1] = 0 - y1;
//						Horizontalvectors2[2] = z1 - z1;
//						vtkMath::Cross(Horizontalvectors1, Horizontalvectors2, Horizontalvectors3);
//						if (Horizontalvectors3[2] < 0)
//						{
//							bA.setNode(x1, y1, z1, x2, y2, z2);
//							double x[6];
//							double y[6];
//							for (auto& j : pMaterial)
//							{
//								if (m_Elements_beams[i].sectionID == j.m_id)
//								{
//									x[0] = 0; x[1] = j.a; x[2] = j.a; x[3] = j.b; x[4] = j.b; x[5] = 0;
//									y[0] = 0; y[1] = 0;   y[2] = j.b; y[3] = j.b; y[4] = j.a; y[5] = j.a;
//								}
//							}
//							bA.SetSection(x, y);
//							bA.Set_zAxis(0, 0, -1);
//						}
//						else
//						{
//							bA.setNode(x2, y2, z2, x1, y1, z1);
//							double x[6];
//							double y[6];
//							for (auto& j : pMaterial)
//							{
//								if (m_Elements_beams[i].sectionID == j.m_id)
//								{
//									x[0] = 0; x[1] = j.a; x[2] = j.a; x[3] = j.b; x[4] = j.b; x[5] = 0;
//									y[0] = 0; y[1] = 0;   y[2] = j.b; y[3] = j.b; y[4] = j.a; y[5] = j.a;
//								}
//							}
//							bA.SetSection(x, y);
//							bA.Set_zAxis(0, 0, -1);
//						}
//					}
//					vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
//					bA.Create_Actor(0, 1, 0, actor);
//					Nactor.push_back(actor);
//				}
//			}
//			break;
//		case ET_Circle:
//
//
//			for (int i = 0; i < m_Elements_beams.size(); i++)
//			{
//
//				if (group == m_Elements_beams[i].sectionID)
//				{
//					qDebug() << "number:" << number;
//					circle C;
//					int ipt1 = m_Elements_beams[i].m_idNode[0] - 1; //id
//					int ipt2 = m_Elements_beams[i].m_idNode[1] - 1;
//					double x1 = m_Nodes[ipt1].x; double y1 = m_Nodes[ipt1].y; double z1 = m_Nodes[ipt1].z;
//					double x2 = m_Nodes[ipt2].x; double y2 = m_Nodes[ipt2].y; double z2 = m_Nodes[ipt2].z;
//
//					C.SetNode(x1, y1, z1, x2, y2, z2);//设置节点
//					double x = a;
//					double y = b;
//					C.SetSection(x, y);//设置内圆外圆半径
//					vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
//					C.CreateCircularSection(actor);
//					Nactor.push_back(actor);
//					number++;
//
//				}
//			}
//
//			break;
//		default:
//			break;
//		}
//	}
//	else
//	{
//		int BeamSize = m_Elements_beams.size() - 1;//找到part梁的最后一个分组
//		int BeamSizeGroup = m_Elements_beams[BeamSize].sectionID;
//		for (int i = 0; i < m_Elements_Trusses.size(); i++)
//		{
//			if (group - BeamSizeGroup == m_Elements_Trusses[i].sectionID)
//			{
//				int ipt1 = m_Elements_Trusses[i].m_idNode[0] - 1; //id
//				int ipt2 = m_Elements_Trusses[i].m_idNode[1] - 1;
//				double x1 = m_Nodes[ipt1].x; double y1 = m_Nodes[ipt1].y; double z1 = m_Nodes[ipt1].z;
//				double x2 = m_Nodes[ipt2].x; double y2 = m_Nodes[ipt2].y; double z2 = m_Nodes[ipt2].z;
//				double radius = 0;
//				for (auto& j : pMaterial)
//				{
//					if (m_Elements_Trusses[i].sectionID + BeamSizeGroup == j.m_id)
//					{
//						radius = j.R;
//					}
//				}
//				vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
//				lineSource->SetPoint1(x1, y1, z1);
//				lineSource->SetPoint2(x2, y2, z2);
//
//				vtkSmartPointer<vtkTubeFilter> tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
//				tubeFilter->SetInputConnection(lineSource->GetOutputPort());
//				tubeFilter->SetRadius(radius);
//				tubeFilter->SetNumberOfSides(50);
//				tubeFilter->CappingOn();
//
//				vtkSmartPointer<vtkPolyDataMapper> cylinderMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//				cylinderMapper->SetInputConnection(tubeFilter->GetOutputPort());
//
//				vtkSmartPointer<vtkActor> cylinderActor = vtkSmartPointer<vtkActor>::New();
//				cylinderActor->SetMapper(cylinderMapper);
//				Nactor.push_back(cylinderActor);
//			}
//		}
//
//	}
//
//}

void Part_Base::SubstaceActor(Element_Beam& beam)
{

	for (auto i : pMaterial)
	{
		if (i.m_id == beam.ClassSectionID)
		{
			switch (i.ClassSe)
			{
			case 0:
				SetL(beam);
				break;
			case 1:
				SetCir(beam);
				break;
			}
		}
	}

}

void Part_Base::AssginSectionGroup(int ElementGroup, int SectionGroup)
{
	int BeamSize = m_Elements_beams.size() - 1;
	int BeamMaxGroup = m_Elements_beams[BeamSize].sectionID;
	if (ElementGroup <= BeamMaxGroup)
	{

		for (auto i : m_Elements_beams)
		{
			if (i.sectionID == ElementGroup)
			{
				i.ClassSectionID = SectionGroup;
				SubstaceActor(i);
			}
		}
	}
	else
	{
		for (auto i : m_Elements_Trusses)
		{
			if (i.sectionID == ElementGroup - BeamMaxGroup)
			{
				i.ClassSectionID = SectionGroup;

			}
		}
	}

}

void Part_Base::AddAllSection(vector<Section>Ma)
{
	for (auto i : Ma)
	{
		pMaterial.push_back(i);
	}
}

void Part_Base::AddNewSection(double ia, double ib, int id, int iClassSe, int iClassM)
{
	pMaterial.push_back(Section(ia, ib, id, iClassSe, iClassM));
}

void Part_Base::SetL(Element_Beam& EB)
{
	beamActor bA;
	int ipt1 = EB.m_idNode[0] - 1; //id
	int ipt2 = EB.m_idNode[1] - 1;
	double n1 = EB.direction[0];
	double n2 = EB.direction[1];
	double n3 = EB.direction[2];
	cout << "点" << EB.m_idNode[0] - 1 << "  " << EB.m_idNode[1] - 1 << "\n";
	double x1 = m_Nodes[ipt1].x; double y1 = m_Nodes[ipt1].y; double z1 = m_Nodes[ipt1].z;
	double x2 = m_Nodes[ipt2].x; double y2 = m_Nodes[ipt2].y; double z2 = m_Nodes[ipt2].z;
	if (abs(z2 - z1 )> 0.1)//
	{
		bA.setNode(x1, y1, z1, x2, y2, z2);
		double x[6];
		double y[6];
		for (auto& j : pMaterial)
		{
			if (EB.ClassSectionID == j.m_id)
			{
				x[0] = 0; x[1] = j.a; x[2] = j.a; x[3] = j.b; x[4] = j.b; x[5] = 0;
				y[0] = 0; y[1] = 0;   y[2] = j.b; y[3] = j.b; y[4] = j.a; y[5] = j.a;
				EB.MaterialID = j.ClassM;
			}
		}
		bA.SetSection(x, y);
		bA.Set_zAxis(n1, n2, n3);
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
			for (auto& j : pMaterial)
			{
				if (EB.ClassSectionID == j.m_id)
				{
					x[0] = 0; x[1] = j.a; x[2] = j.a; x[3] = j.b; x[4] = j.b; x[5] = 0;
					y[0] = 0; y[1] = 0;   y[2] = j.b; y[3] = j.b; y[4] = j.a; y[5] = j.a;
				}
			}
			bA.SetSection(x, y);
			bA.Set_zAxis(n1, n2, n3);
		}
		else
		{
			bA.setNode(x2, y2, z2, x1, y1, z1);
			double x[6];
			double y[6];
			for (auto& j : pMaterial)
			{
				if (EB.ClassSectionID == j.m_id)
				{
					x[0] = 0; x[1] = j.a; x[2] = j.a; x[3] = j.b; x[4] = j.b; x[5] = 0;
					y[0] = 0; y[1] = 0;   y[2] = j.b; y[3] = j.b; y[4] = j.a; y[5] = j.a;
				}
			}
			bA.SetSection(x, y);
			bA.Set_zAxis(0, 0, -1);
		}
	}
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	bA.Create_Actor(0, 1, 0, actor);
	Nactor.push_back(actor);
}

void Part_Base::SetCir(Element_Beam& beam)
{
	circle C;
	int ipt1 = beam.m_idNode[0] - 1; //id
	int ipt2 = beam.m_idNode[1] - 1;
	double x1 = m_Nodes[ipt1].x; double y1 = m_Nodes[ipt1].y; double z1 = m_Nodes[ipt1].z;
	double x2 = m_Nodes[ipt2].x; double y2 = m_Nodes[ipt2].y; double z2 = m_Nodes[ipt2].z;

	C.SetNode(x1, y1, z1, x2, y2, z2);//设置节点
	double x = 0;
	double y = 0;
	for (auto& j : pMaterial)
	{
		if (beam.ClassSectionID == j.m_id)
		{
			x = j.a;
			y = j.b;

		}
	}
	C.SetSection(x, y);//设置内圆外圆半径
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	C.CreateCircularSection(actor);
	Nactor.push_back(actor);


}
