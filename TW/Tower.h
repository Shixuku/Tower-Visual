#pragma once
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
#include"D:/VTK/Use_VTK.h"
#include"Part_Base.h"
#include<Eigen/Dense>
#include<QDataStream>
#include<QTreeWidgetItem>
using namespace Eigen;
class Tower
{
public:
	//子类节点添加到父类里面
	Tower();

	int m_id;
	int Get_id()const;

	QString m_name;
	void SaveTo(QDataStream& fin)const;
	void Input(QDataStream& fin);

	void ShowNode()const;//显示所有节点坐标
	void ShowElement()const;//显示所有单元
	void ShowTrussElement()const;//显示所有杆单元
	void ShowBeamElement()const;//显示所有梁单元
	void Show_Beam(int BeamID, int SectionClass, double a, double b);

	vector<Node> m_Nodes;//节点合集

	vector<Element> m_Elements;//单元合集
	vector<Element> m_Elements_beams;//梁单元合集
	vector<Element> m_Elements_Trusses;//杆单元合集
	vtkSmartPointer<vtkActor> Node_actor;//huangzhan

	void Show_VTKtruss(vtkRenderer* renderer);
	void Show_VTKbeam(vtkRenderer* renderer);
	void Show_VTKnode(vtkRenderer* renderer);//显示节点

	vtkSmartPointer<vtkActor> m_BeamActor;
	vtkSmartPointer<vtkActor> m_TrussActor;
	//std::vector<vtkSmartPointer<vtkActor>>NactorTower;
	std::vector<vtkSmartPointer<vtkActor>>Nactor;

	void addPart(Part_Base* part);

	void Check();
	Node* FindNode(int id);

	Tower moved(double dx, double dy, double dz) const;
	void move(double dx, double dy, double dz);
	Tower rotationed(Vector3d fai, double angle) const;
	void rotation( double angle);


	virtual enum Part_Type My_PartType()const
	{
		return ET_Tower;
	}

	//界面
	QTreeWidgetItem* Item = nullptr;
	vtkSmartPointer<vtkPoints> m_pts;
protected:
	void addNodeToTower(Part_Base* part);
	void addElementToTower(Part_Base* part);

};



