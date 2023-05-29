#pragma once
#include<vector>
#include"Node.h"
#include"Element_Beam.h"
#include"Element_Truss.h"
#include<iostream>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyDataMapper.h>
#include <vtkPoints.h>
#include <vtkLine.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include"QTreeWidget.h"
#include<QDataStream>
#include<vtkVertexGlyphFilter.h>//显示点
#include"Base.h"
#include"Section.h"
#include <map>
using namespace std;
class InterFace;

class Part_Base :public Base
{
public:
	int m_id;
	int Get_id()const;

	int m_nodeID = 0;//节点编号
	vector<Node> m_Nodes;//节点合集

	int m_elementsID = 0;//单元编号
	vector<Element> m_Elements;//单元合集

	int Beam_elementsID = 0;//梁单元编号
	int Truss_elementsID = 0;//杆单元编号

	vector<Element_Beam> m_Elements_beams;//梁单元合集
	vector<Element_Truss> m_Elements_Trusses;//杆单元合集

	int id_BeamSection = 0;
	int id_TrussSection = 0;

	vector<int>part_to_tower;
	int Find_tower_idNode(int idNode) const;

	InterFace* m_pInterFace = nullptr;
	vtkSmartPointer<vtkActor> m_BeamActor;
	vtkSmartPointer<vtkActor> m_TrussActor;
	std::vector<vtkSmartPointer<vtkActor>>Nactor;
	std::vector<Section>pMaterial;
	map<int, Section>pSection;
	vtkSmartPointer<vtkActor> Node_actor;//黄瞻

	void Show_VTKnode(vtkRenderer* renderer);//显示节点
	void Show_VTKtruss(vtkRenderer* renderer);
	void Show_VTKbeam(vtkRenderer* renderer);


	//void show_SectionBeam(double a, double b, int section, int M, int group, bool IsBeam);

	void SubstaceActor(Element_Beam& beam);
	void AssginSectionGroup(int ElementGroup, int SectionGroup);

	void AddAllSection(vector<Section>Ma);
	void AddNewSection(double ia, double ib, int id, int iClassSe, int iClassM);

	void SetL(Element_Beam& beam);
	void SetCir(Element_Beam& beam);

	void GetmL();

	virtual void Clear_Part(); //清除部件的节点和单元

public:
	static void interpolation(const Node& n1, const Node& n2, double num, Node& n);//计算节点--两点之间生成节点

	static void Node4_Creat1(const Node& n1, const Node& n2, const Node& n3, const Node& n4, Node& n);//计算四个点生成的两直线交叉点

	void Creat_Beams(vector<Element_Beam>& m_Elements_Beams, vector<int>ids);//生成梁单元
	void Creat_Beams1(vector<Element_Beam>& m_Elements_Beams, vector<int>ids);//生成梁单元

	void Creat_Trusses(vector<Element_Truss>& m_Elements_Trusses, vector<int>ids);//生成杆单元
	void Creat_Trusses1(vector<Element_Truss>& m_Elements_Trusses, vector<int>ids);//生成杆单元

	int Creat_Node(double x, double y, double z);//生成节点

	virtual void SaveTo(QDataStream& fin)const;

	virtual void Input(QDataStream& fin);

public:
	//界面
	QTreeWidgetItem* Item = nullptr;
	vtkSmartPointer<vtkPoints> m_pts;

};
enum Section_Type :int
{
	ET_L, ET_Circle
};

