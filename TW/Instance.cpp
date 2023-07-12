#include "Instance.h"
#include <iostream>
#include<fstream>
#include"InterFace.h"
void Instance::Show_VTKtruss(vtkRenderer* renderer)
{
	vtkSmartPointer<vtkCellArray>lines = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkLine>line = vtkSmartPointer<vtkLine>::New();
	//点
	int nTruss = m_Elements_Trusses.size();
	for (size_t i = 0; i < nTruss; i++)
	{
		int trussOneId = m_Elements_Trusses[i].m_idNode[0] - 1;
		int trussTowId = m_Elements_Trusses[i].m_idNode[1] - 1;
		line->GetPointIds()->SetId(0, trussOneId);
		line->GetPointIds()->SetId(1, trussTowId);
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

void Instance::Show_VTKbeam(vtkRenderer* renderer)
{
	vtkSmartPointer<vtkCellArray>lines = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkLine>line = vtkSmartPointer<vtkLine>::New();
	int nTruss = m_Elements_beams.size();
	for (size_t i = 0; i < m_Elements_beams.size(); i++)
	{
		int trussOneId = m_Elements_beams[i].m_idNode[0] - 1;
		int trussTowId = m_Elements_beams[i].m_idNode[1] - 1;
		line->GetPointIds()->SetId(0, trussOneId);
		line->GetPointIds()->SetId(1, trussTowId);
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
	Node_actor->GetProperty()->SetPointSize(5);
	renderer->AddActor(Node_actor);
}

int Instance::Creat_Node(double x, double y, double z)
{
	int SIZE = susPoint.size();
	std::cout << "size:" << SIZE << std::endl;
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
		m_Nodes.push_back(Node(id, x, y, z));
		NodeData[id] = m_Nodes[id - 1];
		return m_Nodes[id - 1].m_idNode;
	}
	return 0;
}

void Instance::SaveSus(vector<int> ids)
{
	susPoint.push_back(ids[0]);
}

void Instance::CreatWireEle(vector<Element_Truss>& m_Elements, vector<int> ids)
{
	size_t size = ids.size();
	int node1 = ids[0];
	for (int i = 1; i < size; i++)
	{
		int node2 = ids[i];
		m_Elements_Trusses.push_back(Element_Truss(Truss_elementsID + 1, node1, node2, 0));
		Truss_elementsID++;
		node1 = node2;
	}
}

void Instance::SaveTo(QDataStream& fin) const
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
	fout.open("../statics_input.txt");   //关联一个文件
	NodeTxT();
	BeamTxT();
	TrussTxT();
	fout << 0 << " \n";//没有索单元暂时为0
	ConcentrationTxT();
	//1（重力数量）
	//1 0 - 9.8 0(编号 重力加速度Vector3d)
	fout << 0 << "\n";//重力暂时为空
	//1（多项式函数数量）
	//1 11  2  1 50  0  0  1.01(编号  受力作用的节点号 受力自由度方向   多项式次数（项数 = 次数 + 1）  多项式各项系数  力作用的时间区间)
	fout << 0 << "\n";//多项式函数暂时为空
	RestraintTxT();
	MaterialTxT();
	BeamSectionTxT();
	TrussSectionTxT();
	fout.close();
}

void Instance::NodeTxT()
{
	int NodeSize = m_Nodes.size();
	fout << NodeSize << " \n";
	for (int i = 0; i < m_Nodes.size(); i++)
	{
		fout << "   " << m_Nodes[i].m_idNode << "      " << m_Nodes[i].x * 1e-3 << "      " << m_Nodes[i].y * 1e-3 <<
			"     " << m_Nodes[i].z * 1e-3 << " " << "\n";
	}
}

void Instance::BeamTxT()
{
	int BeamSize = m_Elements_beams.size();
	fout << BeamSize << " \n";
	for (int i = 0; i < m_Elements_beams.size(); i++)
	{
		//m_Elements_beams[i].ClassSectionID  截面号

		fout << m_Elements_beams[i].m_idElement << "      " << m_Elements_beams[i].m_idNode[0] << "    " << m_Elements_beams[i].m_idNode[1] << "  " << m_Elements_beams[i].MaterialID//材料号
			<< "  " << m_Elements_beams[i].ClassSectionID << "  " << m_Elements_beams[i].AxialForce << "  " << m_Elements_beams[i].direction[0] << "  " <<
			m_Elements_beams[i].direction[1] << "  " <<
			m_Elements_beams[i].direction[2] << "  " << "\n";
	}
}

void Instance::TrussTxT()
{
	int TressSize = m_Elements_Trusses.size();
	fout << TressSize << " \n";
	for (int i = 0; i < m_Elements_Trusses.size(); i++)
	{
		fout << m_Elements_Trusses[i].m_idElement << "      " << m_Elements_Trusses[i].m_idNode[0] << "    " << m_Elements_Trusses[i].m_idNode[1] << "  " << m_Elements_Trusses[i].MaterialID
			<< "  " << m_Elements_Trusses[i].ClassSectionID << "  " << m_Elements_beams[i].AxialForce << "\n";
	}
}

void Instance::ConcentrationTxT()
{
	int LoadSize = Load.size();
	fout << LoadSize << " \n";
	for (int i = 0; i < Load.size(); i++)
	{
		fout << Load[i].id_force << "      " << Load[i].id_node << "    " << Load[i].DirectionForce << "  " << Load[i].Force
			<< "  " << Load[i].StartTime << "  " << Load[i].EndTime << "\n";
	}
}

void Instance::MaterialTxT()
{
	fout << 3 << "\n";
	for (int i = 0; i < 3; i++)
	{
		fout << i + 1 << "  " << 2.1e11 << "  " << 0.0 << "  " << 1e4 << "\n";
	}
}

void Instance::BeamSectionTxT()
{
	//int SectionSize = pSection.size();
	//fout << SectionSize << "\n";
	//for (int i = 0; i < SectionSize; i++)
	//{
	//	fout << pSection[i].m_id << "  " << pSection[i].S << "  " << pSection[i].B_Iy << "  " << pSection[i].B_Iz << "  " << pSection[i].B_J << "\n";
	//}
	InterFace* pInterFace = Base::Get_InterFace();
	int SectionSize = pInterFace->Ms.size();
	fout << SectionSize << "\n";
	for (auto& i : pInterFace->Ms)
	{
		fout << i.second->m_id << "  " << i.second->S << "  " << i.second->B_Iy << "  " << i.second->B_Iz << "  " << i.second->B_J << "\n";
	}
}

void Instance::TrussSectionTxT()
{
	InterFace* pInterFace = Base::Get_InterFace();
	int SectionSize = pInterFace->Ms.size();
	fout << SectionSize << "\n";
	for (auto& i : pInterFace->Ms)
	{
		fout << i.second->m_id << "  " << i.second->S << "\n";
	}
}

void Instance::RestraintTxT()
{
	int a = 24;//只考虑塔脚的4个完全约束
	fout << a << "\n";
	cout << "start RestraintNode" << "\n";
	for (int i = 0; i < RestraintNode.size(); i++)
	{
		for (int j = 0; j < 6; j++)
		{
			fout << (j + 1) * (i + 1) << "  " << RestraintNode[i] << "  " << j << "  " << 0 << "\n";
		}
	}
}