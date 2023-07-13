#pragma once
#include<map>
#include"Part_Base.h"
#include<vtkTransform.h>
#include <vtkIdTypeArray.h>
#include <QOpenGLWidget>//UINT_PTR
#include <vtkPointData.h>//->AddArray(Ptr
#include"LoadForce.h"
#include<qtextstream.h>
#include<qfile.h>
#include"ParameterConstraint.h"
class Instance :public Base
{
public:
	QString m_name = nullptr;
	vector<Node> m_Nodes;//�ڵ�ϼ�
	vector<Element> m_Elements;//��Ԫ�ϼ�
	vector<Element_Beam> m_Elements_beams;//����Ԫ�ϼ�
	vector<Element_Truss> m_Elements_Trusses;//�˵�Ԫ�ϼ�
	vector<int>RestraintNode;//���Լ���ڵ�
	vector<int>SuspensionNode;//������ҵ�
	vector<int>WireLogo;
	void Show_VTKtruss(vtkRenderer* renderer);
	void Show_VTKbeam(vtkRenderer* renderer);
	void Show_VTKnode(vtkRenderer* renderer);//��ʾ�ڵ�
	vtkSmartPointer<vtkPoints> m_pts;
	vtkSmartPointer<vtkActor> m_BeamActor;
	vtkSmartPointer<vtkActor> m_TrussActor;
	std::vector<vtkSmartPointer<vtkActor>>m_LoadActor;
	std::vector<vtkSmartPointer<vtkActor>>m_ConstraintActor;//Լ��actor
	std::vector<vtkSmartPointer<vtkActor>>Nactor;
	vtkSmartPointer<vtkActor> Node_actor;//huangzhan
	map<int, Node>NodeData;
	map<int, Element_Beam>BeamData;
	map<int, Element_Truss>TrussData;
	map<int, Section>SectionData;
	vector<int>susPoint;
	vector<int>realSuspoint;
	int Truss_elementsID = 0;
	QTreeWidgetItem* Item = nullptr;
    int Creat_Node(double x, double y, double z);//���ɵ�
    void SaveSus(vector<int>ids);//�������ҵ��id
    void CreatWireEle(vector<Element_Truss>& m_Elements, vector<int> ids);
	virtual void SaveTo(QDataStream& fin)const;
	virtual void Input(QDataStream& fin);
	void VectorToMap();
	//��������ļ�
	vector<LoadForce>Load;//���ص�����
	vector<ParameterConstraint> m_Constraint;//Լ��������
	vector<int>pSection;//���������
	vector<int>TowerToGroup;//��Ӹ�������������ʱ�ݴ�ڵ���
	QFile Qf;
	QTextStream Stream;         //����ofstream
	void CreateOutPut();//����txt�ļ�
	void NodeTxT();
	void BeamTxT();
	void TrussTxT();
	void ConcentrationTxT();//������
	void RestraintTxT();//Լ��
	void MaterialTxT();//����
	void BeamSectionTxT();//��������Ϣ
	void TrussSectionTxT();//�˽�����Ϣ
};

