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
#include <map>
#include <vtkPointData.h>
#include "Base.h"
using namespace Eigen;
class Tower :public Base
{
public:
	//����ڵ���ӵ���������
	Tower();

	QString m_name;
	void SaveTo(QDataStream& fin)const;
	void Input(QDataStream& fin);

	void ShowNode()const;//��ʾ���нڵ�����
	void ShowElement()const;//��ʾ���е�Ԫ
	void ShowTrussElement()const;//��ʾ���и˵�Ԫ
	void ShowBeamElement()const;//��ʾ��������Ԫ
	void Show_Beam(int BeamID, int SectionClass, double a, double b);
	void AddNewSection(int id);
	void AddNewSection(vector<int>& idSections) { pSection = idSections; }
	vector<Node> m_Nodes;//�ڵ�ϼ�

	vector<Element> m_Elements;//��Ԫ�ϼ�
	vector<Element_Beam> m_Elements_beams;//����Ԫ�ϼ�
	vector<Element_Truss> m_Elements_Trusses;//�˵�Ԫ�ϼ�
	vtkSmartPointer<vtkActor> Node_actor;//huangzhan
	vector<LoadForce>Load;//���ص�����
	vector<int>pSection;//���������
	vector<int>TowerToGroup;//��Ӹ�������������ʱ�ݴ�ڵ���
	int FindGroupIdNode(int idNode) const;
	void VectorToMap();
	map<int, Node>NodeData;
	map<int, Element_Beam>BeamData;
	map<int, Element_Truss>TrussData;
	map<int, Section>SectionData;
	void Show_VTKtruss(vtkRenderer* renderer);
	void Show_VTKbeam(vtkRenderer* renderer);
	void Show_VTKnode(vtkRenderer* renderer);//��ʾ�ڵ�

	//��������
	void DrawForceX(Node* n, int a, vtkRenderer* renderer);//a��1����-1
	void DrawForceY(Node* n, int a, vtkRenderer* renderer);//a��1����-1
	void DrawForceZ(Node* n, int a, vtkRenderer* renderer);//a��1����-1
	ofstream fout;           //����ofstream
	void CreateOutPut();//����txt�ļ�
	void NodeTxT();
	void BeamTxT();
	void TrussTxT();
	void ConcentrationTxT();//������
	void RestraintTxT();//Լ��
	void MaterialTxT();//����
	void BeamSectionTxT();//��������Ϣ
	void TrussSectionTxT();//�˽�����Ϣ
	vtkSmartPointer<vtkActor> m_BeamActor;
	vtkSmartPointer<vtkActor> m_TrussActor;
	std::vector<vtkSmartPointer<vtkActor>>m_LoadActor;
	std::vector<vtkSmartPointer<vtkActor>>Nactor;
	vector<int>RestraintNode;
	vector<int>SuspensionNode;
	void addPart(Part_Base* part);

	void Check();
	Node* FindNode(int id);

	void move(double dx, double dy, double dz);
	void rotation( double angle);
	
	virtual enum Part_Type My_PartType()const
	{
		return ET_Tower;
	}

	//����
	QTreeWidgetItem* Item = nullptr;
	vtkSmartPointer<vtkPoints> m_pts;
	//InterFace* m_pInterFace = nullptr;
protected:
	void addNodeToTower(Part_Base* part);
	
	void addElementToTower(Part_Base* part);
	//void addSectionToTower(Part_Base* part);
	void addRestraintNode(Part_Base* part);
	void addSuspensionNode(Part_Base* part);
};
//xxxxxxxxx


