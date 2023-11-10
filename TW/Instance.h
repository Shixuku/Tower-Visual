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
#include<vtkSmartPointer.h>
#include<qdebug.h>
#include"S_InterFace.h"
#include"dll.h"
#include"ParameterGravity.h"
#include"ParameterAnalysisStep.h"
#include"ParameterStableWind.h"
#include"ParameterIceType.h"
#include"HangList.h"
class Instance :public Base
{
public:
	Instance();
	virtual ~Instance() { 
		if (s)
		{
			delete s;
			qDebug() << "删除计算实例！";
		}

	}
	QString m_name = nullptr;
	vector<Node> m_Nodes;//节点合集
	vector<Element> m_Elements;//单元合集
	vector<Element_Beam> m_Elements_beams;//梁单元合集
	vector<Element_Truss> m_Elements_Trusses;//杆单元合集
	vector<HangList>m_HangList;
	vector<int>RestraintNode;//添加约束节点
	vector<int>SuspensionNode;//添加悬挂点
	vector<double>WireGravity;//每档导线的重力
	vector<int>WireLogo;
	void Show_VTKtruss(vtkRenderer* renderer);
	void Show_VTKbeam(vtkRenderer* renderer);
	void Show_VTKnode(vtkRenderer* renderer);//显示节点
	vtkSmartPointer<vtkPoints> m_pts;
	vtkSmartPointer<vtkActor> m_BeamActor;
	vtkSmartPointer<vtkActor> m_TrussActor;
	vtkSmartPointer<vtkPolyData> linesPolyData;
	std::vector<vtkSmartPointer<vtkActor>>m_LoadActor;//集中力actor
	std::vector<vtkSmartPointer<vtkActor>>m_ConstraintActor;//约束actor
	std::vector<vtkSmartPointer<vtkActor>>InstanceNactor;//暂时没发现有什么用
	vtkSmartPointer<vtkActor> Node_actor;//huangzhan
	map<int, Node>NodeData;
	map<int, Element_Beam>BeamData;
	map<int, Element_Truss>TrussData;
	map<int, Section>SectionData;
	map<int, vector<int>>m_EleWind;//存放风区单元集
	vector<int>susPoint;
	vector<int>realSuspoint;
	Manage_Entity<HangPoint> TP_HangPoint;
	int Truss_elementsID = 0;
	int TypeWind = 0;//风的类型
	int Splits= 0;//导线分裂数
	double WindCoefficient = 0;//风荷载系数
	double areaWire = 0;
	QString LandformsType;//地貌类型
	QTreeWidgetItem* Item = nullptr;
    int Creat_Node(double x, double y, double z);//生成点
    void SaveSus(vector<int>ids);//保存悬挂点的id
    void CreatWireEle(vector<Element_Truss>& m_Elements, vector<int> ids);
	virtual void SaveTo(QDataStream& fin)const;
	virtual void Input(QDataStream& fin);
	void VectorToMap();
	//创建输出文件
	vector<LoadForce>Load;//集中力的容器
	vector<ParameterGravity>m_Gravitys;//重力的容器
	vector<ParameterConstraint> m_Constraint;//约束的容器
	vector<ParameterStableWind>m_StableWind;//稳定风的容器
	vector<ParameterIceType> m_IceTypeElement;//冰单元类型
	vector<int>pSection;//截面的容器
	vector<int>TowerToGroup;//添加杆塔到塔线组里时暂存节点编号
	vector<int>SuspensionElement;//放悬挂的单元，用于后续添加轴力
	vector<int>SuspensionElementClass;//放单元类型，是I悬挂还是V悬挂
	QFile Qf;
	QTextStream Stream;         //创建ofstream
	vtkSmartPointer<vtkCellArray> m_lines;
	void CreateOutPut();//创建txt文件
	void NodeTxT();
	void BeamTxT();
	void TrussTxT();
	void AnalysisStepTxT();//分析步
	void ConcentrationTxT();//集中力
	void GravityTxT();//重力
	void WindTxT();//稳定风载荷
	void IceLoadTxT();
	void RestraintTxT();//约束
	void MaterialTxT();//材料
	void BeamSectionTxT();//梁截面信息
	void TrussSectionTxT();//杆截面信息
	void WindEleTxT();
	void Suspensioncombined();//悬挂点排序
	vector<std::pair<int, int>> combined;
	//画约束
	void Draw_fixed_Constrained(double x, double y, double z, vtkRenderer* renderer);
	void Draw_hinge_joint(double x, double y, double z, vtkRenderer* renderer);
	
	//计算
	S_InterFace* s = nullptr;
	S_InterFace_ice* s_ice = nullptr;
	void Section_Assign();//为各单元的截面指派情况
	void Axial_force();//初始轴力

	//画集中力
	void DrawForceX(Node* n, int a, vtkRenderer* renderer);//a传1或者-1
	void DrawForceY(Node* n, int a, vtkRenderer* renderer);//a传1或者-1
	void DrawForceZ(Node* n, int a, vtkRenderer* renderer);//a传1或者-1
};

