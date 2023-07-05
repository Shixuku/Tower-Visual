#pragma once
#include<map>
#include"Part_Base.h"
//#include"LoadForce.h"
#include<vtkTransform.h>
#include <vtkIdTypeArray.h>
#include <QOpenGLWidget>//UINT_PTR
#include <vtkPointData.h>//->AddArray(Ptr
class Instance
{
public:
	int m_id;
	int Get_id()const;

	vector<Node> m_Nodes;//�ڵ�ϼ�
	vector<Element> m_Elements;//��Ԫ�ϼ�
	vector<Element_Beam> m_Elements_beams;//����Ԫ�ϼ�
	vector<Element_Truss> m_Elements_Trusses;//�˵�Ԫ�ϼ�
	vector<int>RestraintNode;//���Լ���ڵ�
	vector<int>SuspensionNode;//������ҵ�
	void Show_VTKtruss(vtkRenderer* renderer);
	void Show_VTKbeam(vtkRenderer* renderer);
	void Show_VTKnode(vtkRenderer* renderer);//��ʾ�ڵ�
	int Creat_Node(double x, double y, double z);//���ɵ�
	vtkSmartPointer<vtkPoints> m_pts;
	vtkSmartPointer<vtkActor> m_BeamActor;
	vtkSmartPointer<vtkActor> m_TrussActor;
	std::vector<vtkSmartPointer<vtkActor>>m_LoadActor;
	std::vector<vtkSmartPointer<vtkActor>>Nactor;
	vtkSmartPointer<vtkActor> Node_actor;//huangzhan
	void SaveSus(vector<int>ids);//�������ҵ��id
	vector<int>susPoint;
	map<int, Node>NodeData;
	void CreatWireEle(vector<Element_Truss>& m_Elements, vector<int> ids);
	int Truss_elementsID = 0;
	QTreeWidgetItem* Item = nullptr;


	virtual void SaveTo(QDataStream& fin)const;
	virtual void Input(QDataStream& fin);
};

