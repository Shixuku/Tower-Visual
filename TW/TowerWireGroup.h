#pragma once
#include"Tower.h"
#include"TowerList.h"
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
	QString m_Name;
	void SaveTo(QDataStream& fin)const;
	void Input(QDataStream& fin);
	int groupId = 0;
	vector<int>SuspensionNode;//���ҵ�
	vector<int>realSuspoint;//��ʵ���ҵ�
	Manage_Entity<TowerList> TWG_TP;
	void VectorToMap();
	map<int, Node>NodeData;
	//�����
	void AddTowerToGroup(Tower* tower,int towerId, double dx, double dy, double dz,double dAngle);
	void AddTowerNode(Tower* tower, int towerId, double dx, double dy, double dz);
	void AddTowerElement(Tower* tower, int towerId);
	void addRestraintNode(Tower* tower);
	void AddSuspensionNode(Tower* tower);
	void addHangPoint(Tower* tower);
	//�����
	void AddWireToGroup(CreatWire* wire);
	void AddWireNode(CreatWire* wire);
	void AddWireElement(CreatWire* wire);
	void AddAxialForceToInsulator(CreatWire* wire);//����Ե��ʩ������
	void rotation(double angle, int towerId);

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

