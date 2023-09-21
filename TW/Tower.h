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
	//����ڵ���ӵ���������
	Tower();

	void SaveTo(QDataStream& fin)const;
	void Input(QDataStream& fin);

	void ShowNode()const;//��ʾ���нڵ�����
	void ShowElement()const;//��ʾ���е�Ԫ
	void ShowTrussElement()const;//��ʾ���и˵�Ԫ
	void ShowBeamElement()const;//��ʾ��������Ԫ
	void Show_Beam(int BeamID, int SectionClass, double a, double b);//û�е���
	void AddNewSection(int id);
	void AddNewSection(vector<int>& idSections) { pSection = idSections; }

	vector<int>TowerToGroup;//��Ӹ�������������ʱ�ݴ�ڵ���
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

	//����
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


