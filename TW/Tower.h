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
#include"Part_Base.h"
#include<Eigen/Dense>
#include<QDataStream>
#include<QTreeWidgetItem>
#include <vtkAutoInit.h> 
#include"LoadForce.h"
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
	void AddNewSection(double ia, double ib, int id, int iClassSe, int iClassM);
	vector<Node> m_Nodes;//节点合集

	vector<Element> m_Elements;//单元合集
	vector<Element_Beam> m_Elements_beams;//梁单元合集
	vector<Element_Truss> m_Elements_Trusses;//杆单元合集
	vtkSmartPointer<vtkActor> Node_actor;//huangzhan
	vector<LoadForce>Load;//荷载的容器
	vector<Section>pSection;//截面的容器
	void Show_VTKtruss(vtkRenderer* renderer);
	void Show_VTKbeam(vtkRenderer* renderer);
	void Show_VTKnode(vtkRenderer* renderer);//显示节点
	void Show_Load(vtkRenderer* renderer);//画力的箭头
	ofstream fout;           //创建ofstream
	void CreateOutPut();//创建txt文件
	void NodeTxT();
	void BeamTxT();
	void TrussTxT();
	void ConcentrationTxT();//集中力
	void RestraintTxT();//约束
	void MaterialTxT();//材料
	void BeamSectionTxT();//梁截面信息
	void TrussSectionTxT();//杆截面信息
	vtkSmartPointer<vtkActor> m_BeamActor;
	vtkSmartPointer<vtkActor> m_TrussActor;
	std::vector<vtkSmartPointer<vtkActor>>m_LoadActor;
	//std::vector<vtkSmartPointer<vtkActor>>NactorTower;
	std::vector<vtkSmartPointer<vtkActor>>Nactor;
	vector<int>RestraintNode;
	void addPart(Part_Base* part);

	void Check();
	Node* FindNode(int id);

	void move(double dx, double dy, double dz);
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
	void addSectionToTower(Part_Base* part);
	void addRestraintNode(Part_Base* part);
};
//xxxxxxxxx


