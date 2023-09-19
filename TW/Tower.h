#pragma once
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkPoints.h>
#include <vtkLine.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkCellArray.h>
#include"Part_Base.h"
#include<Eigen/Dense>
#include<QDataStream>
#include<QTreeWidgetItem>
#include <vtkAutoInit.h> 
#include"LoadForce.h"
#include"ParameterConstraint.h"
#include <map>
#include <vtkPointData.h>
#include "Base.h"
#include<qtextstream.h>
#include<qfile.h>
#include"Instance.h"
using namespace Eigen;
class Tower:public Instance
{
public:
	//子类节点添加到父类里面
	Tower();

	void SaveTo(QDataStream& fin)const;
	void Input(QDataStream& fin);

	void ShowNode()const;//显示所有节点坐标
	void ShowElement()const;//显示所有单元
	void ShowTrussElement()const;//显示所有杆单元
	void ShowBeamElement()const;//显示所有梁单元
	void Show_Beam(int BeamID, int SectionClass, double a, double b);//没有调用
	void AddNewSection(int id);
	void AddNewSection(vector<int>& idSections) { pSection = idSections; }

	vector<int>TowerToGroup;//添加杆塔到塔线组里时暂存节点编号
	int FindGroupIdNode(int idNode) const;
	void addPart(Part_Base* part);
	void Check_Beam();
	Node* FindNode(int id);

	void move(double dx, double dy, double dz);
	void rotation( double angle);
	
	virtual enum Part_Type My_PartType()const
	{
		return ET_Tower;
	}

	//界面
	QTreeWidgetItem* Item = nullptr;
	

protected:
	void addNodeToTower(Part_Base* part);
	
	void addElementToTower(Part_Base* part);
	//void addSectionToTower(Part_Base* part);
	void addRestraintNode(Part_Base* part);
	void addSuspensionNode(Part_Base* part);

	//QFile Qf;
	//QTextStream Stream;
};
//xxxxxxxxx


