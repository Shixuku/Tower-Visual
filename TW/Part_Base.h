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
#include<vtkVertexGlyphFilter.h>//��ʾ��
#include"Base.h"
#include"Section.h"
#include <map>
using namespace std;
class InterFace;

class Part_Base :public Base
{
public:
	int m_nodeID = 0;//�ڵ���
	vector<Node> m_Nodes;//�ڵ�ϼ�

	int m_elementsID = 0;//��Ԫ���
	vector<Element> m_Elements;//��Ԫ�ϼ�

	int Beam_elementsID = 0;//����Ԫ���
	int Truss_elementsID = 0;//�˵�Ԫ���

	vector<Element_Beam> m_Elements_beams;//����Ԫ�ϼ�
	vector<Element_Truss> m_Elements_Trusses;//�˵�Ԫ�ϼ�

	int id_BeamSection = 0;
	int id_TrussSection = 0;

	vector<int>part_to_tower;
	int Find_tower_idNode(int idNode) const;

	InterFace* m_pInterFace = nullptr;
	vtkSmartPointer<vtkActor> m_BeamActor;//��actor�ϼ�
	vtkSmartPointer<vtkActor> m_TrussActor;
	std::vector<vtkSmartPointer<vtkActor>>Nactor;
	//std::vector<Section>pSection;
	std::vector<int>pSection;
	//map<int, Section>pSection;
	vtkSmartPointer<vtkActor> Node_actor;//��հ
	vector<int>RestraintNode;//����Լ���ڵ�
	vector<int>SuspensionNode;//������ߵ�ı�ʶ
	vector<int>WireLoge;//������������ҵ�����һ����·
	vector<int>realSuspoint;//���ѵ�����ʵ�õĵ�
	void Show_VTKnode(vtkRenderer* renderer);//��ʾ�ڵ�
	void Show_VTKtruss(vtkRenderer* renderer);
	void Show_VTKbeam(vtkRenderer* renderer);
	void ShowMessage();

	void SubstaceActor(Element_Beam& beam);
	void AssginSectionGroup(QSet<int> Group, int SectionGroup);

	void AddNewSection(int id);
	void AddNewSection(vector<int>& idSections) { pSection = idSections; }

	void SetL(Element_Beam& beam);
	void SetCir(Element_Beam& beam);

	virtual void Clear_Part(); //��������Ľڵ�͵�Ԫ

public:
	static void interpolation(const Node& n1, const Node& n2, double num, Node& n);//����ڵ�--����֮�����ɽڵ�

	static void Node4_Creat1(const Node& n1, const Node& n2, const Node& n3, const Node& n4, Node& n);//�����ĸ������ɵ���ֱ�߽����

	void Creat_Beams(vector<Element_Beam>& m_Elements_Beams, vector<int>ids);//��������Ԫ
	void Creat_Beams1(vector<Element_Beam>& m_Elements_Beams, vector<int>ids);//��������Ԫ

	void Creat_Trusses(vector<Element_Truss>& m_Elements_Trusses, vector<int>ids);//���ɸ˵�Ԫ
	void Creat_Trusses1(vector<Element_Truss>& m_Elements_Trusses, vector<int>ids);//���ɸ˵�Ԫ
	void InPutRestraintNode(vector<int>ids);//��4���ŵ���뵥Ԫ
	void InPutSuspension(vector<int>ids);//�����ҵ����
	int Creat_Node(double x, double y, double z);//���ɽڵ�

	virtual void SaveTo(QDataStream& fin)const;

	virtual void Input(QDataStream& fin);

public:
	//����
	QTreeWidgetItem* Item = nullptr;
	vtkSmartPointer<vtkPoints> m_pts;

};
enum Section_Type :int
{
	ET_L, ET_Circle
};

