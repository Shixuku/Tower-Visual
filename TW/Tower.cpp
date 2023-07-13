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


int Tower::FindGroupIdNode(int idNode) const
{
	return TowerToGroup[idNode - 1];
}

//void Tower::Show_VTKtruss(vtkRenderer* renderer)
//{
//	if (!m_lines) m_lines = vtkSmartPointer<vtkCellArray>::New();
//		
//	vtkSmartPointer<vtkLine>line = vtkSmartPointer<vtkLine>::New();
//	//点
//	for (size_t i = 0; i < m_Elements_Trusses.size(); i++)
//	{
//		line->GetPointIds()->SetId(0, m_Elements_Trusses[i].m_idNode[0] - 1);
//		line->GetPointIds()->SetId(1, m_Elements_Trusses[i].m_idNode[1] - 1);
//		m_lines->InsertNextCell(line);
//	}
//	//vtkSmartPointer<vtkPoints> pts = vtkSmartPointer<vtkPoints>::New();
//	//for (int i = 0; i < m_Nodes.size(); i++)
//	//{
//	//	vector<double> p;
//	//	p.resize(4);
//	//	p[0] = m_Nodes[i].m_idNode;//m_Nodes[0]指 编号为1的点
//	//	p[1] = m_Nodes[i].x;
//	//	p[2] = m_Nodes[i].y;
//	//	p[3] = m_Nodes[i].z;
//	//	pts->InsertNextPoint(p[1], p[2], p[3]);
//	//}
//
//	vtkSmartPointer<vtkPolyData> linesPolyData = vtkSmartPointer<vtkPolyData>::New();
//	linesPolyData->SetPoints(m_pts);
//	linesPolyData->SetLines(m_lines);
//
//	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//	mapper->SetInputData(linesPolyData);
//	m_TrussActor = vtkSmartPointer<vtkActor>::New();
//	m_TrussActor->SetMapper(mapper);
//	m_TrussActor->GetProperty()->SetColor(0, 1, 0);
//	renderer->AddActor(m_TrussActor);
//}
//void Tower::Show_VTKbeam(vtkRenderer* renderer)
//{
//	if (!m_lines) m_lines = vtkSmartPointer<vtkCellArray>::New();
//	vtkSmartPointer<vtkLine>line = vtkSmartPointer<vtkLine>::New();
//
//	for (size_t i = 0; i < m_Elements_beams.size(); i++)
//	{
//		line->GetPointIds()->SetId(0, m_Elements_beams[i].m_idNode[0] - 1);
//		line->GetPointIds()->SetId(1, m_Elements_beams[i].m_idNode[1] - 1);
//		m_lines->InsertNextCell(line);
//	}
//
//	vtkSmartPointer<vtkPolyData> linesPolyData = vtkSmartPointer<vtkPolyData>::New();
//	linesPolyData->SetPoints(m_pts);
//	linesPolyData->SetLines(m_lines);
//
//	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//	mapper->SetInputData(linesPolyData);
//	m_BeamActor = vtkSmartPointer<vtkActor>::New();
//	m_BeamActor->SetMapper(mapper);
//	m_BeamActor->GetProperty()->SetColor(0, 1, 0);
//	renderer->AddActor(m_BeamActor);
//}
//void Tower::Show_VTKnode(vtkRenderer* renderer)
//{
//	vtkNew<vtkIdTypeArray> Ptr;
//	Ptr->SetName("Address");
//	m_pts = vtkSmartPointer<vtkPoints>::New();
//	int nNode = m_Nodes.size();
//	m_pts->SetNumberOfPoints(nNode);
//	for (int i = 0; i < m_Nodes.size(); i++)
//	{
//		vector<double> p;
//		p.resize(4);
//		p[0] = m_Nodes[i].m_idNode;//m_Nodes[0]指 编号为1的点
//		p[1] = m_Nodes[i].x;
//		p[2] = m_Nodes[i].y;
//		p[3] = m_Nodes[i].z;
//		m_pts->SetPoint(i, p[1], p[2], p[3]);
//		Node* pNode = &m_Nodes[i];//zhan
//		Ptr->InsertNextValue((UINT_PTR)pNode);//将地址转换为整数		
//	}
//	vtkSmartPointer<vtkPolyData> linesPolyData = vtkSmartPointer<vtkPolyData>::New();
//	linesPolyData->SetPoints(m_pts);
//	linesPolyData->GetPointData()->AddArray(Ptr);
//
//	//点
//	Node_actor = vtkSmartPointer<vtkActor>::New();
//	vtkNew<vtkVertexGlyphFilter> VertexFilter;
//	VertexFilter->SetInputData(linesPolyData);
//	VertexFilter->Update();
//
//	vtkSmartPointer<vtkPolyDataMapper> Node_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//	Node_mapper->SetInputConnection(VertexFilter->GetOutputPort());
//
//	Node_actor = vtkSmartPointer<vtkActor>::New();
//	Node_actor->SetMapper(Node_mapper);
//	Node_actor->GetProperty()->SetColor(255, 255, 0);
//	Node_actor->GetProperty()->SetPointSize(5);
//	renderer->AddActor(Node_actor);
//}

void Tower::DrawForceX(Node* n, int a, vtkRenderer* renderer)
{
	// 创建锥形
	vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();
	double x1 = n->x + 2.000 * a;
	cone->SetCenter(x1, n->y, n->z); // 设置中心点坐标
	cone->SetHeight(0.5000);     // 设置高度
	cone->SetRadius(0.3000);     // 设置半径
	cone->SetDirection(1 * a, 0, 0);//设置方向--数值与大小无关
	cone->SetResolution(10);  // 设置分辨率
	vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
	lineSource->SetPoint1(n->x, n->y, n->z);
	lineSource->SetPoint2(x1 + 0.250 * a, n->y, n->z);
	lineSource->Update();

	vtkNew<vtkAppendPolyData> appendFilter;
	appendFilter->AddInputConnection(cone->GetOutputPort());
	appendFilter->AddInputConnection(lineSource->GetOutputPort());
	appendFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(appendFilter->GetOutputPort());

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	// 将渲染属性设置为线框模式
	vtkSmartPointer<vtkProperty> p_property = vtkSmartPointer<vtkProperty>::New();
	p_property->SetRepresentationToWireframe();
	actor->SetProperty(p_property);
	actor->GetProperty()->SetColor(1, 0, 0);
	m_LoadActor.push_back(actor);

	//在主界面显示
	renderer->AddActor(actor);
}

void Tower::DrawForceY(Node* n, int a, vtkRenderer* renderer)
{
	// 创建锥形
	vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();
	double y1 = n->y + 2.000 * a;
	cone->SetCenter(n->x, y1, n->z); // 设置中心点坐标
	cone->SetHeight(0.5000);     // 设置高度
	cone->SetRadius(0.3000);     // 设置半径
	cone->SetDirection(0, 1 * a, 0);//设置方向--数值与大小无关
	cone->SetResolution(10);  // 设置分辨率
	vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
	lineSource->SetPoint1(n->x, n->y, n->z);
	lineSource->SetPoint2(n->x, y1 + 0.250 * a, n->z);
	lineSource->Update();

	vtkNew<vtkAppendPolyData> appendFilter;
	appendFilter->AddInputConnection(cone->GetOutputPort());
	appendFilter->AddInputConnection(lineSource->GetOutputPort());
	appendFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(appendFilter->GetOutputPort());

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	// 将渲染属性设置为线框模式
	vtkSmartPointer<vtkProperty> p_property = vtkSmartPointer<vtkProperty>::New();
	p_property->SetRepresentationToWireframe();
	actor->SetProperty(p_property);
	actor->GetProperty()->SetColor(1, 0, 0);
	m_LoadActor.push_back(actor);
	renderer->AddActor(actor);
	
}

void Tower::DrawForceZ(Node* n, int a, vtkRenderer* renderer)
{
	// 创建锥形
	vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();
	double z1 = n->z + 2.000 * a;
	cone->SetCenter(n->x, n->y, z1); // 设置中心点坐标
	cone->SetHeight(0.5000);     // 设置高度
	cone->SetRadius(0.3000);     // 设置半径
	cone->SetDirection(0, 0, 1 * a);//设置方向--数值与大小无关
	cone->SetResolution(10);  // 设置分辨率
	vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
	lineSource->SetPoint1(n->x, n->y, n->z);
	lineSource->SetPoint2(n->x, n->y, z1 + 0.250 * a);
	lineSource->Update();

	vtkNew<vtkAppendPolyData> appendFilter;
	appendFilter->AddInputConnection(cone->GetOutputPort());
	appendFilter->AddInputConnection(lineSource->GetOutputPort());
	appendFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(appendFilter->GetOutputPort());
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	// 将渲染属性设置为线框模式
	vtkSmartPointer<vtkProperty> p_property = vtkSmartPointer<vtkProperty>::New();
	p_property->SetRepresentationToWireframe();
	actor->SetProperty(p_property);
	actor->GetProperty()->SetColor(1, 0, 0);

	m_LoadActor.push_back(actor);

	renderer->AddActor(actor);

}

//尝试 画约束
void Tower::Draw_fixed_Constrained(double x, double y, double z, vtkRenderer* renderer)
{
	struct ConeInfo
	{
		double radius;
		double height;
		double direction[3];
		double center[3];
		double color[3];
	};

	ConeInfo cones[] = {
		{ 0.2, 0.6, { 0, 1, 0 }, { x, y - 0.3, z }, { 0.0, 0.0, 1.0 } },  // y方向
		{ 0.3, 0.12, { 0, 1, 0 }, { x, y - 0.6, z }, { 0.0, 0.0, 1.0 } },
		{ 0.3, 0.5, { 0, 1, 0 }, { x, y - 0.25, z }, { 1.0, 0.5, 0.0 } },
		{ 0.2, 0.6, { 1, 0, 0 }, { x - 0.3, y, z }, { 0.0, 0.0, 1.0 } },  // x方向
		{ 0.3, 0.12, { 1, 0, 0 }, { x - 0.6, y, z }, { 0.0, 0.0, 1.0 } },
		{ 0.3, 0.5, { 1, 0, 0 }, { x - 0.25, y, z }, { 1.0, 0.5, 0.0 } },
		{ 0.2, 0.6, { 0, 0, -1 }, { x, y, z + 0.3 }, { 0.0, 0.0, 1.0 } },  // z方向
		{ 0.3, 0.12, { 0, 0, -1 }, { x, y, z + 0.6 }, { 0.0, 0.0, 1.0 } },
		{ 0.3, 0.5, { 0, 0, -1 }, { x, y, z + 0.25 }, { 1.0, 0.5, 0.0 } }
	};

	int numCones = sizeof(cones) / sizeof(cones[0]);

	for (int i = 0; i < numCones; i++)
	{
		vtkSmartPointer<vtkConeSource> coneSource = vtkSmartPointer<vtkConeSource>::New();
		coneSource->SetRadius(cones[i].radius);
		coneSource->SetHeight(cones[i].height);
		coneSource->SetDirection(cones[i].direction);
		coneSource->SetCenter(cones[i].center);
		coneSource->Update();

		vtkSmartPointer<vtkPolyDataMapper> coneMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		coneMapper->SetInputConnection(coneSource->GetOutputPort());

		vtkSmartPointer<vtkActor> coneActor = vtkSmartPointer<vtkActor>::New();
		coneActor->SetMapper(coneMapper);

		coneActor->GetProperty()->SetColor(cones[i].color);
		coneActor->GetProperty()->SetRepresentationToWireframe();

		m_ConstraintActor.push_back(coneActor);
		//std::cout << "m_ConstraintActor.size() = " << m_ConstraintActor.size() << "\n";
		renderer->AddActor(coneActor);

	}

	//renderer->ResetCamera();
}

void Tower::Draw_hinge_joint(double x, double y, double z, vtkRenderer* renderer)
{
	struct ConeInfo
	{
		double radius;
		double height;
		double direction[3];
		double center[3];
		double color[3];
	};

	ConeInfo cones[] = {

		{ 300, 500.0, { 0, 1, 0 }, { x, y - 250, z }, { 1.0, 0.5, 0.0 } },
		{ 300, 500.0, { 1, 0, 0 }, { x - 250, y, z }, { 1.0, 0.5, 0.0 } },
		{ 300, 500.0, { 0, 0, -1 }, { x, y, z + 250 }, { 1.0, 0.5, 0.0 } }
	};

	int numCones = sizeof(cones) / sizeof(cones[0]);

	for (int i = 0; i < numCones; i++)
	{
		vtkSmartPointer<vtkConeSource> coneSource = vtkSmartPointer<vtkConeSource>::New();
		coneSource->SetRadius(cones[i].radius);
		coneSource->SetHeight(cones[i].height);
		coneSource->SetDirection(cones[i].direction);
		coneSource->SetCenter(cones[i].center);
		coneSource->Update();

		vtkSmartPointer<vtkPolyDataMapper> coneMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		coneMapper->SetInputConnection(coneSource->GetOutputPort());

		vtkSmartPointer<vtkActor> coneActor = vtkSmartPointer<vtkActor>::New();
		coneActor->SetMapper(coneMapper);
		coneActor->GetProperty()->SetColor(cones[i].color);
		coneActor->GetProperty()->SetRepresentationToWireframe();
		m_ConstraintActor.push_back(coneActor);
		renderer->AddActor(coneActor);

	}

	renderer->ResetCamera();
}

//void Tower::CreateOutPut()
//{
//	InterFace* pInterFace = Get_InterFace();
//	QString filename = QFileDialog::getSaveFileName(pInterFace, "保存", "/", "datafile(*.txt);;All file(*.*)");
//	if (filename == nullptr)
//	{
//		return;
//	}
//	else
//	{
//		qDebug() << filename;
//		Qf.setFileName(filename);
//		Qf.open(QIODevice::ReadWrite);
//
//		Stream.setDevice(&Qf);
//		if (!Qf.isOpen())
//		{
//			cout << "文件打开失败！\n";
//			return;
//		}
//		this->m_name = filename;
//		NodeTxT();
//		BeamTxT();
//		TrussTxT();
//		Stream << 0 << " \n";//没有索单元暂时为0
//		ConcentrationTxT();
//		//1（重力数量）
//		//1 0 - 9.8 0(编号 重力加速度Vector3d)
//		Stream << 0 << "\n";//重力暂时为空
//		//1（多项式函数数量）
//		//1 11  2  1 50  0  0  1.01(编号  受力作用的节点号 受力自由度方向   多项式次数（项数 = 次数 + 1）  多项式各项系数  力作用的时间区间)
//		Stream << 0 << "\n";//多项式函数暂时为空
//		RestraintTxT();
//		MaterialTxT();
//		BeamSectionTxT();
//		TrussSectionTxT();
//		// 关闭文件
//		Qf.close();
//	}
//}
//void Tower::NodeTxT()
//{
//	int NodeSize = m_Nodes.size();
//	Stream << NodeSize << " \n";
//	for (int i = 0; i < m_Nodes.size(); i++)
//	{
//		Stream << "   " << m_Nodes[i].m_idNode << "      " << m_Nodes[i].x * 1e-3 << "      " << m_Nodes[i].y * 1e-3 <<
//			"     " << m_Nodes[i].z * 1e-3 << " " << "\n";
//	}
//}
//void Tower::BeamTxT()
//{
//	int BeamSize = m_Elements_beams.size();
//	Stream << BeamSize << " \n";
//	for (int i = 0; i < m_Elements_beams.size(); i++)
//	{
//
//		Stream << m_Elements_beams[i].m_idElement << "      " << m_Elements_beams[i].m_idNode[0] << "    " << m_Elements_beams[i].m_idNode[1] << "  " << m_Elements_beams[i].MaterialID//材料号
//			<< "  " << m_Elements_beams[i].ClassSectionID << "  " << m_Elements_beams[i].AxialForce << "  " << m_Elements_beams[i].direction[0] << "  " <<
//			m_Elements_beams[i].direction[1] << "  " <<
//			m_Elements_beams[i].direction[2] << "  " << "\n";
//	}
//}
//void Tower::TrussTxT()
//{
//	int TressSize = m_Elements_Trusses.size();
//	Stream << TressSize << " \n";
//	for (int i = 0; i < m_Elements_Trusses.size(); i++)
//	{
//		Stream << m_Elements_Trusses[i].m_idElement << "      " << m_Elements_Trusses[i].m_idNode[0] << "    " << m_Elements_Trusses[i].m_idNode[1] << "  " << m_Elements_Trusses[i].MaterialID
//			<< "  " << m_Elements_Trusses[i].ClassSectionID << "  " << m_Elements_beams[i].AxialForce << "\n";
//	}
//}
//void Tower::ConcentrationTxT()
//{
//	int LoadSize = Load.size();
//	Stream << LoadSize << " \n";
//	for (int i = 0; i < Load.size(); i++)
//	{
//		Stream << Load[i].id_force << "      " << Load[i].id_node << "    " << Load[i].DirectionForce << "  " << Load[i].Force
//			<< "  " << Load[i].StartTime << "  " << Load[i].EndTime << "\n";
//	}
//}
//void Tower::MaterialTxT()
//{
//	Stream << 3 << "\n";
//	for (int i = 0; i < 3; i++)
//	{
//		Stream << i + 1 << "  " << 2.1e11 << "  " << 0.0 << "  " << 1e4 << "\n";
//	}
//}
//void Tower::BeamSectionTxT()
//{
//	InterFace* pInterFace = Base::Get_InterFace();
//	int SectionSize = pInterFace->Ms.size();
//	Stream << SectionSize << "\n";
//	for (auto& i : pInterFace->Ms)
//	{
//		Stream << i.second->m_id << "  " << i.second->S*1e-6 << "  " << i.second->B_Iy*1e-12 << "  " << i.second->B_Iz * 1e-12 << "  " << i.second->B_J * 1e-12 << "\n";
//	}
//}
//void Tower::TrussSectionTxT()
//{
//	InterFace* pInterFace = Base::Get_InterFace();
//	int SectionSize = pInterFace->Ms.size();
//	Stream << SectionSize << "\n";
//	for (auto& i : pInterFace->Ms)
//	{
//		Stream << i.second->m_id << "  " << i.second->S * 1e-6 << "\n";
//	}
//}
//void Tower::RestraintTxT()
//{
//	//int a = 24;//只考虑塔脚的4个完全约束
//	//Stream << a << "\n";
//	//
//	//for (int i = 0; i < RestraintNode.size(); i++)
//	//{
//	//	for (int j = 0; j < 6; j++)
//	//	{
//	//		Stream << (j + 1) * (i + 1) << "  " << RestraintNode[i] << "  " << j << "  " << 0 << "\n";
//	//	}
//	//}
//	int m_ConstraintSize = m_Constraint.size();
//	Stream << m_ConstraintSize << " \n";
//	for (int i = 0; i < m_Constraint.size(); i++)
//	{
//		Stream << m_Constraint[i].m_idConstraint << "      " << m_Constraint[i].m_idNode << "    " << m_Constraint[i].m_Direction << "\n";
//	}
//
//}

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
	//cout << typeid(*part).name() << ": " << partNode << "\n";

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
		//cout << m_Elements_beams[i].m_idNode[0] << "   " << m_Elements_beams[i].m_idNode[1] << "\n";
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

void Tower::AddNewSection(int id)
{
	 pSection.push_back(id);
}

void Tower::addElementToTower(Part_Base* part)
{

	size_t tpart = part->m_Elements_Trusses.size();
	size_t bpart = part->m_Elements_beams.size();

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
		//cout << pE->direction[0] << "  " << pE->direction[1] << "  " << pE->direction[2] << "\n";
	}
}

//void Tower::addSectionToTower(Part_Base* part)
//{
//	int SectionSize = part->pSection.size();
//	for (int i = 0; i < SectionSize; i++)
//	{
//		double ia = part->pSection[i].a;
//		double ib = part->pSection[i].b;
//		int id = part->pSection[i].m_id;
//		int iClassSe = part->pSection[i].ClassSe;
//		int iClassM = part->pSection[i].ClassM;
//
//		pSection.push_back(Section(ia, ib, id, iClassSe, iClassM));
//		
//	}
//}

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

