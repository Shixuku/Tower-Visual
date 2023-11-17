#include "Instance.h"
#include <iostream>
#include<fstream>
#include"InterFace.h"
#include <vtkConeSource.h>
#include<vtkLineSource.h>
#include <vtkPolyData.h>
#include <vtkIdFilter.h>
#include<vtkDataSetSurfaceFilter.h>

Instance::Instance()
{
	//默认添加重力
	m_Gravitys.push_back(ParameterGravity(1, 1, 2, -9.8));
}

void Instance::Show_VTKtruss(vtkRenderer* renderer)
{
	if (!m_lines) m_lines = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkLine>line = vtkSmartPointer<vtkLine>::New();
	//点
	int nTruss = m_Elements_Trusses.size();
	for (size_t i = 0; i < nTruss; i++)
	{
		int trussOneId = m_Elements_Trusses[i].m_idNode[0] - 1;
		int trussTowId = m_Elements_Trusses[i].m_idNode[1] - 1;
		line->GetPointIds()->SetId(0, trussOneId);
		line->GetPointIds()->SetId(1, trussTowId);
		m_lines->InsertNextCell(line);
	}


	linesPolyData = vtkSmartPointer<vtkPolyData>::New();
	linesPolyData->SetPoints(m_pts);
	linesPolyData->SetLines(m_lines);

	//做框选
	vtkNew<vtkIdFilter> idFilter;//保存数据
	idFilter->SetInputData(linesPolyData);
	idFilter->SetCellIdsArrayName("OriginalIds");//保存单元
	idFilter->SetPointIdsArrayName("OriginalIds");//保存点
	idFilter->Update();
	//简化成表面
	vtkNew<vtkDataSetSurfaceFilter> surfaceFilter;
	surfaceFilter->SetInputConnection(idFilter->GetOutputPort());
	surfaceFilter->Update();

	vtkPolyData* input = surfaceFilter->GetOutput();
	//std::cout << "points" << input->GetNumberOfPoints() << "cells:" << input->GetNumberOfCells() << std::endl;

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper->SetInputData(linesPolyData);
	//kuangxuang
	mapper->SetInputData(input);
	mapper->ScalarVisibilityOff();

	m_TrussActor = vtkSmartPointer<vtkActor>::New();
	m_TrussActor->SetMapper(mapper);
	m_TrussActor->GetProperty()->SetColor(0, 1, 0);
	renderer->AddActor(m_TrussActor);
}

void Instance::Show_VTKbeam(vtkRenderer* renderer)
{
	if (!m_lines) m_lines = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkLine>line = vtkSmartPointer<vtkLine>::New();
	int nTruss = m_Elements_beams.size();
	for (size_t i = 0; i < m_Elements_beams.size(); i++)
	{
		int trussOneId = m_Elements_beams[i].m_idNode[0] - 1;
		int trussTowId = m_Elements_beams[i].m_idNode[1] - 1;
		line->GetPointIds()->SetId(0, trussOneId);
		line->GetPointIds()->SetId(1, trussTowId);
		m_lines->InsertNextCell(line);
	}

	linesPolyData = vtkSmartPointer<vtkPolyData>::New();
	linesPolyData->SetPoints(m_pts);
	linesPolyData->SetLines(m_lines);

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(linesPolyData);
	m_BeamActor = vtkSmartPointer<vtkActor>::New();
	m_BeamActor->SetMapper(mapper);
	m_BeamActor->GetProperty()->SetColor(0, 1, 0);
	renderer->AddActor(m_BeamActor);
}

void Instance::Show_VTKnode(vtkRenderer* renderer)
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
	Node_actor->GetProperty()->SetPointSize(4);
	renderer->AddActor(Node_actor);
}

int Instance::Creat_Node(double x, double y, double z)
{
	int SIZE = susPoint.size();
	int judg = 0;
	for (int i = 0; i < SIZE; ++i)
	{
		if (((abs(NodeData[susPoint[i]].x - x) < 1e-8)) && 
			((abs(NodeData[susPoint[i]].y - y)) < 1e-8) && 
			((abs(NodeData[susPoint[i]].z - z)) < 1e-8))
		{
		
			return NodeData[susPoint[i]].m_idNode; //重节点
		}
		else
		{
			judg++;

		}

	}

	if (judg == SIZE)
	{
		int id = m_Nodes.size() + 1;
		m_Nodes.push_back(Node(id, x, y, z, 0));
		NodeData[id] = m_Nodes[id - 1];
		return m_Nodes[id - 1].m_idNode;
	}
	return 0;
}

void Instance::SaveSus(vector<int> ids)
{
	realSuspoint.push_back(ids[0]);
}

void Instance::CreatWireEle(vector<Element_Truss>& m_Elements, vector<int> ids)
{
	size_t size = ids.size();
	int node1 = ids[0];
	for (int i = 1; i < size; i++)
	{
		int node2 = ids[i];
		if (node1 == node2)
		{
			node1 = node2;
		}
		else
		{
			m_Elements_Trusses.push_back(Element_Truss(Truss_elementsID + 1, node1, node2, 0, 0));
			Truss_elementsID++;
			node1 = node2;
		}
	}
}

void Instance::SaveTo(QDataStream& fin) const
{
	fin << m_id;//实例的id,可能会有bug
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
	////集中力
	//int nLoadForce = Load.size();
	//fin << nLoadForce;
	//for (int i = 0; i < nLoadForce; i++)
	//{
	//	Load[i].SaveTo(fin);
	//}
}

void Instance::Input(QDataStream& fin)
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
	int nSuspension;
	fin >> nSuspension;
	SuspensionNode.resize(nSuspension);
	for (int i = 0; i < nSuspension; ++i)
	{
		fin >> SuspensionNode[i];
	}
	////集中力
	//int nLoadForce; 
	//fin << nLoadForce;
	//Load.resize(nLoadForce);
	//for (int i = 0; i < nLoadForce; i++)
	//{
	//	Load[i].Input(fin);
	//}
}

void Instance::VectorToMap()
{
	// 将 vectorNode 中的元素插入到 map 中
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
	// 将 vectorSection 中的元素插入到 map 中

}

void Instance::CreateOutPut()
{
	InterFace* pInterFace = Get_InterFace();
	QString filename = QFileDialog::getSaveFileName(pInterFace, "保存", "/", "datafile(*.txt);;All file(*.*)");
	if (filename == nullptr)
	{
		return;
	}
	else
	{
		qDebug() << filename;
		Qf.setFileName(filename);
		Qf.open(QIODevice::WriteOnly);

		Stream.setDevice(&Qf);
		if (!Qf.isOpen())
		{
			cout << "文件打开失败！\n";
			return;
		}
		this->m_filename = filename;
		NodeTxT();//节点
		TrussTxT();//桁架单元
		BeamTxT();//梁单元
		//Stream << 0 << " \n";//没有索单元暂时为0
		//分析步
		AnalysisStepTxT();
		//荷载
		ConcentrationTxT();//集中力
		GravityTxT();//重力
		IceLoadTxT();//冰单元

		WindTxT();//风载荷
		//边界条件
		RestraintTxT();
		MaterialTxT();//材料
		TrussSectionTxT();//桁架截面数据
		BeamSectionTxT();//梁截面数据
		Section_Assign();//单元指派
		Axial_force();//初始轴力
		if (TypeWind == 1)
		{
			WindEleTxT();//风区单元集
		}
		// 关闭文件
		Qf.close();
	}

}

void Instance::NodeTxT()
{
	int NodeSize = m_Nodes.size();
	Stream <<"*Node," << NodeSize << " \n";
	for (int i = 0; i < m_Nodes.size(); i++)
	{
		Stream << "  " << m_Nodes[i].m_idNode << "  " << m_Nodes[i].x << "  " << m_Nodes[i].y <<
			"  " << m_Nodes[i].z << "  " << "\n";
	}
}

void Instance::BeamTxT()
{
	int BeamSize = m_Elements_beams.size();
	Stream << "*Element_Beam3D," << BeamSize << " \n";
	for (int i = 0; i < m_Elements_beams.size(); i++)
	{
		if (m_IceTypeElement.size() > 0)
		{
			//覆冰没有type
			Stream << "  " << m_Elements_beams[i].m_idElement << "  " << "  " << m_Elements_beams[i].m_idNode[0] << "  " << m_Elements_beams[i].m_idNode[1] << "  " << "\n";
		}
		else
		{
			//算风有type
			Stream << "  " << m_Elements_beams[i].m_idElement << "  " << m_Elements_beams[i].Type << "  " << m_Elements_beams[i].m_idNode[0] << "  " << m_Elements_beams[i].m_idNode[1] << "  " << "\n";
		}
	}
}

void Instance::TrussTxT()
{
	int TressSize = m_Elements_Trusses.size();
	Stream <<"*Element_Truss3D," << TressSize << " \n";
	for (int i = 0; i < m_Elements_Trusses.size(); i++)
	{
		if (m_IceTypeElement.size() > 0)
		{
			//覆冰没有type
			Stream << "  " << m_Elements_Trusses[i].m_idElement << "  "<< m_Elements_Trusses[i].m_idNode[0] << "  " << m_Elements_Trusses[i].m_idNode[1] << "  " << "\n";
		}
		else
		{
			//算风有type
			Stream << "  " << m_Elements_Trusses[i].m_idElement << "  " << m_Elements_Trusses[i].Type << "  " << m_Elements_Trusses[i].m_idNode[0] << "  " << m_Elements_Trusses[i].m_idNode[1] << "  " << "\n";
		}
	//	Stream << "  " << m_Elements_Trusses[i].m_idElement << "  " << m_Elements_Trusses[i].Type << "  " << m_Elements_Trusses[i].m_idNode[0] << "  " << m_Elements_Trusses[i].m_idNode[1] << "\n";
	}
}

void Instance::AnalysisStepTxT()
{
	InterFace* pInterFace = Base::Get_InterFace();
	int AnalysisStepSize = pInterFace->ME_AnalysisSteps.size();
	Stream << "*Analysis_Step," << AnalysisStepSize << "\n";
	for (auto& i : pInterFace->ME_AnalysisSteps)
	{
		QString m_Type;//分析步类型 静力还是动力
		double m_TimeSlot;//时间段
		double m_TimeIncrement;//时间增量
		double m_MinAllowableValue;//最小容许值
		int m_MaxEpoch;//最大迭代次数
		//编号，材料号 类型 A, Iy,  Iz,Iyz, J  ----Iyz暂时为0
		Stream << "  " << i.second->m_id << "  " << i.second->m_Type << "  "  << i.second->m_TimeSlot <<"  "
			<< i.second->m_TimeIncrement << "  " << i.second->m_MinAllowableValue << "  " << i.second->m_MaxEpoch << "\n";
	}
}

void Instance::ConcentrationTxT()
{
	int LoadSize = Load.size();
	Stream <<"*Force_Node, " << LoadSize << " \n";
	for (int i = 0; i < Load.size(); i++)
	{
		Stream << "  " << Load[i].m_id << "  " <<Load[i].m_AnalysisStep<<"  " << Load[i].id_node << "  " << Load[i].DirectionForce << "  " << Load[i].Force<< "\n";
	}
}

void Instance::GravityTxT()
{
	int GravitySize = m_Gravitys.size();
	Stream << "*Force_Gravity," << GravitySize << "\n";//重力
	for (int i = 0; i < GravitySize; i++)
	{	//1 0 - 9.8(编号 方向012-xyz 大小)
		if (m_IceTypeElement.size() > 0)
		{
			//覆冰
			Stream << "  " << m_Gravitys[i].m_id << "  " << m_Gravitys[i].m_AnalysisStep << "  " << m_Gravitys[i].m_Direction << "  " << m_Gravitys[i].m_g * (-1) << "  " << "\n";
		}
		else
		{
			//算风
			Stream << "  " << m_Gravitys[i].m_id << "  " << m_Gravitys[i].m_AnalysisStep << "  " << m_Gravitys[i].m_Direction << "  " << m_Gravitys[i].m_g << "  " << "\n";
		}
		
	}
}

void Instance::WindTxT()
{
	int StableWindSize = m_StableWind.size();
	Stream << "*Force_Wind," << StableWindSize << "\n";//风载荷
	if (TypeWind == 0)
	{
		for (int i = 0; i < StableWindSize; i++)
		{	//1 0 - 9.8(编号 方向012-xyz 大小)
			Stream << "  " << m_StableWind[i].m_id << "  " << m_StableWind[i].m_AnalysisStep << "  "  << m_StableWind[i].m_angle << "  " << m_StableWind[i].m_v << "  " 
				<< m_StableWind[i].m_splitCount << "  " << m_StableWind[i].m_alf_select << "  " << m_StableWind[i].m_coeff << "\n";
		}
	}
	else
	{
		for (int i = 0; i < StableWindSize; i++)
		{	//1 0 - 9.8(编号 方向012-xyz 大小)
			Stream << "  " << m_StableWind[i].m_id << "  " << m_StableWind[i].m_AnalysisStep << "  "  << m_StableWind[i].m_angle << "  " << m_StableWind[i].m_fileName
				<< "  " << m_StableWind[i].m_splitCount << "  " << m_StableWind[i].m_alf_select << "  " << m_StableWind[i].m_coeff << "\n";
		}
	}
}

void Instance::IceLoadTxT()
{
	//新改的
	Stream << "*Element_ice, " << m_IceTypeElement.size() << "\n";
	for (int i = 0; i < m_IceTypeElement.size(); i++)
	{
		Stream << "   " << m_IceTypeElement[i].m_id << "  "<< m_IceTypeElement[i].m_StartAnalysisStep<< "  " << m_IceTypeElement[i].m_EndAnalysisStep << "  " << m_IceTypeElement[i].m_Thickness << "\n";
	}
}

void Instance::MaterialTxT()
{
	Stream <<"*Material," << 6 << "\n";
	for (int i = 0; i < 3; i++)
	{
		//**编号，弹性模量，泊松比，质量密度，热膨胀系数，没有时用0占位
		Stream << "   " << i + 1 << "  " << 2.1e11 << "  " << 0.3 << "  " << 7850 <<"  " << 0 << "\n";
	}
	Stream << "   " << 4 << "  " << 6.3e10<< "  " << 0.3 << "  " <<3080 << "  " << 0 << "\n";
	Stream << "   " << 5 << "  " << 2.00e11 << "  " << 0.3 << "  " << 9.8e3 << "  " << 0 << "\n";
	Stream << "   " << 6 << "  " << 4.79e11 << "  " << 0.3 << "  " << 980 << "  " << 0 << "\n";
}

void Instance::BeamSectionTxT()
{
	InterFace* pInterFace = Base::Get_InterFace();
	int SectionSize = pInterFace->Ms.size();
	Stream << "*Section_Beam3D," << SectionSize << "\n";
	for (auto& i : pInterFace->Ms)
	{
		//编号，材料号 类型 A, Iy,  Iz,Iyz, J  ----Iyz暂时为0
		Stream << "  " << i.second->m_id << "  " << i.second->ClassM << "  " << "Data " << i.second->S << "  " << i.second->B_Iy << "  " << i.second->B_Iz << "  " <<0.0<< "  " << i.second->B_J << "\n";
	}
}

void Instance::TrussSectionTxT()
{
	InterFace* pInterFace = Base::Get_InterFace();
	int SectionSize = pInterFace->Ms.size();
	Stream <<"*Section_Truss," << SectionSize << "\n";
	for (auto& i : pInterFace->Ms)
	{//**截面的编号，材料号，面积
		Stream << "   " << i.second->m_id << "  " << i.second->ClassM << "  " << i.second->S << "\n";
	}
}

void Instance::WindEleTxT()
{
	int TotalWindEle = m_EleWind.size();
	//Stream << "*Elset," << TotalWindEle << " \n";
	for (const auto& windEle : m_EleWind)
	{
		int segmentIndex = windEle.first;//风区
		const std::vector<int>& eleIds = windEle.second;//单元集合
		Stream << "*Elset,"  << segmentIndex << " \n ";
		for (int eleId : eleIds)
		{
			Stream << eleId << "  ";
		}
		Stream << "\n";
	}
}

void Instance::Section_Assign()
{
	int TotalElementSize = m_Elements_Trusses.size() + m_Elements_beams.size();
	//考虑冰单元
	if (m_IceTypeElement.size() > 0)
	{
		TotalElementSize *= 2;
	}
	Stream << "*Section_Assign," << TotalElementSize << " \n";
	
	//先杆
	for (int i = 0; i < m_Elements_Trusses.size(); i++)
	{
		Stream << "  " << m_Elements_Trusses[i].m_idElement << "  " << m_Elements_Trusses[i].ClassSectionID << "  " << "\n";
	}
	//再梁
	for (int i = 0; i < m_Elements_beams.size(); i++)
	{

		Stream << "  "<<m_Elements_beams[i].m_idElement<< "  " << m_Elements_beams[i].ClassSectionID << "  " << m_Elements_beams[i].direction[0] << "  " <<m_Elements_beams[i].direction[1] << "  " 
			<<m_Elements_beams[i].direction[2] << "  " << "\n";
	}
	//冰单元的指派，通过m_IceTypeElement.size()来判断，如果>0；就输出，没有就不输出
	if (m_IceTypeElement.size() > 0)
	{
		for (int i = 1; i < m_Elements_Trusses.size() + m_Elements_beams.size() + 1; i++)
		{//冰单元编号，单元编号，冰单元类型
			Stream << "  " << m_Elements_Trusses.size() + m_Elements_beams.size() + i << "  " << i << "  " << "1" << "\n";
		}
	}
}

void Instance::Axial_force()
{
	int Axial_forcesize = 0;
	for (int i = 0; i < m_Elements_Trusses.size(); i++)
	{
		if (m_Elements_Trusses[i].AxialForce != 0)
		{
			Axial_forcesize++;
		}
	}
	Stream << "*Axial_force," << Axial_forcesize << " \n";
	for (int i = 0; i < m_Elements_Trusses.size(); i++)
	{
		if (m_Elements_Trusses[i].AxialForce != 0)
		{
			Stream<< "  " << m_Elements_Trusses[i].m_idElement<< "  " << m_Elements_Trusses[i].AxialForce << "  " << "\n";
		}
	}
}

void Instance::Suspensioncombined()
{
	for (size_t i = 0; i < SuspensionNode.size(); ++i) {
		combined.push_back(std::make_pair(SuspensionNode[i], WireLogo[i]));
	}
	// 按照序号进行排序
	std::sort(combined.begin(), combined.end(), [](const std::pair<int, int>& pair1, const std::pair<int, int>& pair2) {
		return pair1.second < pair2.second;
		});
}

void Instance::RestraintTxT()
{
	int RestraintNodesize = RestraintNode.size() * 6;//塔脚的4个完全约束
	int m_ConstraintSize = m_Constraint.size();//增加的约束
	int totalRestraint = RestraintNodesize + m_ConstraintSize;

	Stream <<"*Constraint," << totalRestraint << "\n";
	int m_id=1;
	for (int i = 0; i < RestraintNode.size(); i++)
	{
		for (int j = 0; j < 6; j++)
		{
			Stream << "  " << m_id << "  " << RestraintNode[i] << "  " << j << "  " << 0 << "\n";
			m_id++;
		}	
	}
	//默认约束
	for (int i = 0; i < m_ConstraintSize; i++)
	{
		Stream << "  " << m_Constraint[i].m_idConstraint << "  " << m_Constraint[i].m_idNode << "  " << m_Constraint[i].m_Direction << "  " << 0 << "\n";
	}
}

//尝试 画约束
void Instance::Draw_fixed_Constrained(double x, double y, double z, vtkRenderer* renderer)
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
		renderer->AddActor(coneActor);

	}

	//renderer->ResetCamera();
}

void Instance::Draw_hinge_joint(double x, double y, double z, vtkRenderer* renderer)
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

		{ 0.3, 0.5, { 0, 1, 0 }, { x, y - 0.25, z }, { 1.0, 0.5, 0.0 } },
		{ 0.3, 0.5, { 1, 0, 0 }, { x - 0.25, y, z }, { 1.0, 0.5, 0.0 } },
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
		renderer->AddActor(coneActor);
	}
	renderer->ResetCamera();
}
void Instance::DrawForceX(Node* n, int a, vtkRenderer* renderer)
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

void Instance::DrawForceY(Node* n, int a, vtkRenderer* renderer)
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

void Instance::DrawForceZ(Node* n, int a, vtkRenderer* renderer)
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