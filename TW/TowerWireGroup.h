#pragma once
#include"Tower.h"
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkPoints.h>
#include <vtkLine.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkCellArray.h>
#include <vtkAutoInit.h> 
#include<vtkDoubleArray.h>
#include<vtkArrowSource.h>
#include <vtkTubeFilter.h>
#include<vtkTransform.h>
#include <vtkIdTypeArray.h>
#include<vtkDoubleArray.h>
#include<vtkArrowSource.h>
#include <vtkTubeFilter.h>
#include <QOpenGLWidget>//UINT_PTR
#include <vtkPointData.h>//->AddArray(Ptr)
class CreatWire;
class TowerWireGroup:public Instance
{
public:
	int Get_id()const;
	void SaveTo(QDataStream& fin)const;
	void Input(QDataStream& fin);
	int groupId = 0;
	vector<Node> m_Nodes;//�ڵ�ϼ�
	vector<Element> m_Elements;//��Ԫ�ϼ�
	vector<Element_Beam> m_Elements_beams;//����Ԫ�ϼ�
	vector<Element_Truss> m_Elements_Trusses;//�˵�Ԫ�ϼ�
	vector<int>SuspensionNode;//���ҵ�
	vector<int>realSuspoint;//��ʵ���ҵ�

	void VectorToMap();
	map<int, Node>NodeData;
	//�����
	void AddTowerToGroup(Tower* tower,int towerId, double dx, double dy, double dz,double dAngle);
	void AddTowerNode(Tower* tower, int towerId, double dx, double dy, double dz);
	void AddTowerElement(Tower* tower, int towerId);
	void AddSuspensionNode(Tower* tower);
	//�����
	void AddWireToGroup(CreatWire* wire);
	void AddWireNode(CreatWire* wire);
	void AddWireElement(CreatWire* wire);
	void rotation(double angle, int towerId);
	void Show_VTKtruss(vtkRenderer* renderer);
	void Show_VTKbeam(vtkRenderer* renderer);
	void Show_VTKnode(vtkRenderer* renderer);//��ʾ�ڵ�
	vtkSmartPointer<vtkPoints> m_pts;
	vtkSmartPointer<vtkActor> m_BeamActor;
	vtkSmartPointer<vtkActor> m_TrussActor;
	vtkSmartPointer<vtkActor> Node_actor;
	void ShowNode()const;//��ʾ���нڵ�����
	void ShowElement()const;//��ʾ���е�Ԫ
	void ShowTrussElement()const;//��ʾ���и˵�Ԫ
	void ShowBeamElement()const;//��ʾ��������Ԫ
	virtual enum Part_Type My_PartType()const
	{
		return ET_TowerWireGroup;
	}
	void Suspensioncombined();//���ҵ�����
	vector<std::pair<int, int>> combined;
	void addSpacer(Part_Base* base);
	void addSpacerNode(Part_Base* base);
	void addSpacerElement(Part_Base* base);
	void addSpacerSuspensionNode(Part_Base* base);
public:
	//����
	QTreeWidgetItem* Item = nullptr;//����Item�����ڲ���
};

