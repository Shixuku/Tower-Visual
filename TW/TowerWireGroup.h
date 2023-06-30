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
class TowerWireGroup
{
public:
	int Get_id()const;
	void SaveTo(QDataStream& fin)const;
	void Input(QDataStream& fin);
	int groupId;
	vector<Node> m_Nodes;//节点合集
	vector<Element> m_Elements;//单元合集
	vector<Element_Beam> m_Elements_beams;//梁单元合集
	vector<Element_Truss> m_Elements_Trusses;//杆单元合集
	vector<int>SuspensionNode;//悬挂点

	void VectorToMap();
	map<int, Node>NodeData;

	void AddTowerToGroup(Tower* tower,int towerId, double dx, double dy, double dz,double dAngle);
	void AddTowerNode(Tower* tower, int towerId, double dx, double dy, double dz);
	void AddTowerElement(Tower* tower, int towerId);
	void AddSuspensionNode(Tower* tower);
	void rotation(double angle, int towerId);
	void Show_VTKtruss(vtkRenderer* renderer);
	void Show_VTKbeam(vtkRenderer* renderer);
	void Show_VTKnode(vtkRenderer* renderer);//显示节点
	vtkSmartPointer<vtkPoints> m_pts;
	vtkSmartPointer<vtkActor> m_BeamActor;
	vtkSmartPointer<vtkActor> m_TrussActor;
	vtkSmartPointer<vtkActor> Node_actor;
	void ShowNode()const;//显示所有节点坐标
	void ShowElement()const;//显示所有单元
	void ShowTrussElement()const;//显示所有杆单元
	void ShowBeamElement()const;//显示所有梁单元
	virtual enum Part_Type My_PartType()const
	{
		return ET_TowerWireGroup;
	}
public:
	//界面
	QTreeWidgetItem* Item = nullptr;//保存Item，用于查找
};

