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
	//����ڵ���ӵ���������
	Tower();

	int m_id;
	int Get_id()const;

	QString m_name;
	void SaveTo(QDataStream& fin)const;
	void Input(QDataStream& fin);

	void ShowNode()const;//��ʾ���нڵ�����
	void ShowElement()const;//��ʾ���е�Ԫ
	void ShowTrussElement()const;//��ʾ���и˵�Ԫ
	void ShowBeamElement()const;//��ʾ��������Ԫ
	void Show_Beam(int BeamID, int SectionClass, double a, double b);

	vector<Node> m_Nodes;//�ڵ�ϼ�

	vector<Element> m_Elements;//��Ԫ�ϼ�
	vector<Element> m_Elements_beams;//����Ԫ�ϼ�
	vector<Element> m_Elements_Trusses;//�˵�Ԫ�ϼ�
	vtkSmartPointer<vtkActor> Node_actor;//huangzhan

	void Show_VTKtruss(vtkRenderer* renderer);
	void Show_VTKbeam(vtkRenderer* renderer);
	void Show_VTKnode(vtkRenderer* renderer);//��ʾ�ڵ�

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

	//����
	QTreeWidgetItem* Item = nullptr;
	vtkSmartPointer<vtkPoints> m_pts;
protected:
	void addNodeToTower(Part_Base* part);
	void addElementToTower(Part_Base* part);

};



