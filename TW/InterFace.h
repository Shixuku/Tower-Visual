#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_InterFace.h"
#include<vtkGenericOpenGLRenderWindow.h>//m_renderWindow
#include<vtkRenderer.h>//m_Renderer
#include<vtkAxesActor.h>//Axes
#include<vtkOrientationMarkerWidget.h>//widgetAxes
#include<QVTKOpenGLNativeWidget.h>//m_VtkWidget
#include"Manage_Entity.h"
#include "TowerPart_leg.h"
#include "TowerPart_body.h"
#include "TowerPart_CrossArm.h"
#include"TowerPart_Insulator.h"
#include "Tower.h"
#include"TowerWireGroup.h"
#include<QFileDialog>
#include <QString>
#include <QDebug>
#include <QList>
#include<vector>
#include"Set_Section.h"
#include<vtkInteractorStyleTrackballCamera.h>
#include"TowerWireGroupAssembly.h"
#include"resultVisualize.h"
#include <vtkAppendPolyData.h>
//fl_Dll
#include"S_InterFace.h"
#include"Node_Base.h"
#include"InputWireInfor.h"

#pragma execution_character_set("utf-8")

class T_Foot;
class T_Body;
class T_CrossArm;
class Tower_Assembly;
class MouseInteractorHighLightActor;
class Interphase_spacer;
class Create_Constraint;
class ConcentrateForce;
class Creat_Loads;
class Wind;
class TowerWireGroup;
class Wire_InterFace;
class CreatWire;
class TowerCaculate;
class ParameterIceElement;
class Element_Ice;
class InteractorStyle;
class ui_AnalysisStep;
class Manage_Section;
class Instance_Calculate;
class Material;
class Section_L;
class Section_C;
class CreateStrainWire;
class CreateSingleTower;
//class Manage_Loads;
class InterFace : public QMainWindow
{
    Q_OBJECT
public:
    InterFace(QWidget* parent = Q_NULLPTR);
    ~InterFace();

    void SetupCentralWidget();//�Ҵ���
    void TreeWidgetShow();//�󴰿�

    vtkRenderer* m_Renderer;//�Ų�����vtk����
   //���Զ����ж�ȡ��·�ļ�
    void ReadMaterialTXT();
    void ReadSectionTXT();
    void ReadPartTXT();
    void ReadInstanceTXT();

    vtkGenericOpenGLRenderWindow* m_renderWindow;
    QTreeWidgetItem* creat_towerwire_instance;//���������item��Ϊ��Ա����������װ�������ʱ�򷽱����
   
    void ShowSubstaceActor(Part_Base* Part);
    void initMenu();
    void AddPartFunction(QTreeWidgetItem* item);
     Ui::InterFaceClass ui;

     Manage_Entity<TowerPart_leg> TP_leg;
     Manage_Entity<TowerPart_body> TP_body;
     Manage_Entity<TowerPart_CrossArm> TP_CrossArm;
     Manage_Entity<TowerPart_Insulator> towerPartInsulator;
     Manage_Entity<Tower> TP;
     Manage_Entity<Section> Ms;//����ϼ�
     Manage_Entity<ParameterAnalysisStep> ME_AnalysisSteps;//�������ϼ�
     Manage_Entity<TowerWireGroup> TWG;
     Manage_Entity<TowerWireGroup> CalculationModel;//��TWG���֣����㵼���ģ�ͷ��ڲ�ͬ�ļ�������
     Manage_Entity < Material> ME_Material;
     Manage_Entity < Section_L> ME_Section_L;
     Manage_Entity < Section_C> ME_Section_C;
    // Manage_Entity<TowerWireGroup> creatWire;
    // Manage_Entity<CreatWire> creatWire;
     vector<T_Foot*> t_foots;
     vector<T_Body*> t_bodys;
     vector<T_CrossArm*> t_crossarms;
     vector<Tower_Assembly*> tower_assembles;
     void Caculate();
     void Display();

     std::vector<QStringList> Scetion_lists;
     //��ʾ����ʵ���µĵ���
     void ShowGroupTower(QTreeWidgetItem* item);
     //���߲���
     std::list<vtkSmartPointer<vtkActor>> m_NodeSelected;
     void Add_Select(vtkSmartPointer<vtkActor> pActor);
     void UnSelect_Nodes();
     void Get_SelectedNode(std::list<Node*>& Nodes); //node_list
     void Get_SelectedNode(std::set<Node*>& Nodes);
     void Point_Inqure();
     bool isChildOfTower(int idParent,QTreeWidgetItem* item, int childNumber);//ȡ�غ�
     bool isChildOfGroup(QTreeWidgetItem* item, int childNumber);//ȡ�غ�
     bool isChildOfPartSetSection(QTreeWidgetItem* item);//ѡ��������item
     bool isChildOfPartSetAllSection(QTreeWidgetItem* item);//ѡ����ȫ�������item
     bool isChildOfPartSetSpacer(QTreeWidgetItem* item);//����Ե�ӷ��ںᵣ��
     void Area_Inqure();//��ѡpoint
     void AreaElement_Inqure();//��ѡElement
     bool isChildOfSingleWire(QTreeWidgetItem* item, int childNumber);
     bool isChildOfGroupTower(QTreeWidgetItem* item);
     void Close_Point();
     //��
     Wind* wd = nullptr;
     vtkRenderer* m_CurrentRenderer; // ��¼��ǰѡ�е� m_Renderer
     vtkNew<InteractorStyle> style;
     vtkNew<vtkAppendPolyData> appendFilter;
signals://�ź�
    void Msg_Select_Nodes();//ѡ���˽ڵ�--���߲���
    void Msg_CreateModel();
    void SignalsPickElements();

public slots:
    void onTreeitemClicked(QTreeWidgetItem* item);
    void onTreeitemDoubleClicked(QTreeWidgetItem* item);
    void ui_Foot();
    void ui_Body();
    void ui_CrossArm();
    void ui_Tower();
    void ui_Management_PartData();
    void ui_Management_InsData();
    void ui_Interphase_spacer(QTreeWidgetItem* item);
    void ui_Wire_InterFace(QTreeWidgetItem* item);
    void SaveFile();
    void OpenFile();
    void ui_Section();
    void ui_SetSection(QTreeWidgetItem* item);
    void ui_SetAllSection(QTreeWidgetItem* item);
    void Show_Part(Part_Base* part);
    void Show_Tower(Instance* instance);
    void Show_Wire(Instance* instance);
    void ui_CreatLoads(QTreeWidgetItem* item);
    void CreateOutPut(QTreeWidgetItem* item);

    void ui_Constraint(QTreeWidgetItem* item);//��������߽��������������·�ȷ��
    void CreateTowerInp(QTreeWidgetItem* item);
    void CreateGroupInp(QTreeWidgetItem* item);

    void ui_SingleWire();
    void ui_TowerWireGroup();

    void ui_SingleWireSpacer(QTreeWidgetItem* item);
    void ui_CreateSingleTower();
private:
    Outputter* m_Outputter=nullptr;
    resultVisualize* display=nullptr;
    QHBoxLayout* layout;
    QVTKOpenGLNativeWidget* m_VtkWidget;
    // �˵��¼�
    void contextMenuEvent(QContextMenuEvent* event) override;
    QMenu* m_pMenu = nullptr;
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> m_MainStyle; // ����������Ľ�������ʽ
    
    //treeWidget
    Instance* OnFindInstance(const QTreeWidgetItem* Item);
    Part_Base* OnFindPart(const QTreeWidgetItem* Item);
    Tower* OnFindTower(const QTreeWidgetItem* Item);
    TowerData_CrossArm* OnFindCrossAem(const QTreeWidgetItem* Item);
    TowerWireGroup* OnFindGroup(const QTreeWidgetItem* Item);
    vtkAxesActor* Axes;
    vtkOrientationMarkerWidget* widgetAxes;
    bool isAxesCreated = false;
   
    Instance_Calculate* ui_calculate = nullptr;
    void ReadWireInforTxT();
    void CreateStrain(QTreeWidgetItem* item);
 
};