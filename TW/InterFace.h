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

    void SetupCentralWidget();//右窗口
    void TreeWidgetShow();//左窗口

    vtkRenderer* m_Renderer;//放部件的vtk窗口
   //打开自动运行读取线路文件
    void ReadMaterialTXT();
    void ReadSectionTXT();
    void ReadPartTXT();
    void ReadInstanceTXT();

    vtkGenericOpenGLRenderWindow* m_renderWindow;
    QTreeWidgetItem* creat_towerwire_instance;//将塔线组的item作为成员变量，在组装塔线组的时候方便调用
   
    void ShowSubstaceActor(Part_Base* Part);
    void initMenu();
    void AddPartFunction(QTreeWidgetItem* item);
     Ui::InterFaceClass ui;

     Manage_Entity<TowerPart_leg> TP_leg;
     Manage_Entity<TowerPart_body> TP_body;
     Manage_Entity<TowerPart_CrossArm> TP_CrossArm;
     Manage_Entity<TowerPart_Insulator> towerPartInsulator;
     Manage_Entity<Tower> TP;
     Manage_Entity<Section> Ms;//截面合集
     Manage_Entity<ParameterAnalysisStep> ME_AnalysisSteps;//分析步合集
     Manage_Entity<TowerWireGroup> TWG;
     Manage_Entity<TowerWireGroup> CalculationModel;//与TWG区分，计算导入的模型放在不同文件夹里面
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
     //显示塔线实例下的单塔
     void ShowGroupTower(QTreeWidgetItem* item);
     //导线部分
     std::list<vtkSmartPointer<vtkActor>> m_NodeSelected;
     void Add_Select(vtkSmartPointer<vtkActor> pActor);
     void UnSelect_Nodes();
     void Get_SelectedNode(std::list<Node*>& Nodes); //node_list
     void Get_SelectedNode(std::set<Node*>& Nodes);
     void Point_Inqure();
     bool isChildOfTower(int idParent,QTreeWidgetItem* item, int childNumber);//取载荷
     bool isChildOfGroup(QTreeWidgetItem* item, int childNumber);//取载荷
     bool isChildOfPartSetSection(QTreeWidgetItem* item);//选择赋予截面的item
     bool isChildOfPartSetAllSection(QTreeWidgetItem* item);//选择赋予全部截面的item
     bool isChildOfPartSetSpacer(QTreeWidgetItem* item);//将绝缘子放在横担下
     void Area_Inqure();//框选point
     void AreaElement_Inqure();//框选Element
     bool isChildOfSingleWire(QTreeWidgetItem* item, int childNumber);
     bool isChildOfGroupTower(QTreeWidgetItem* item);
     void Close_Point();
     //风
     Wind* wd = nullptr;
     vtkRenderer* m_CurrentRenderer; // 记录当前选中的 m_Renderer
     vtkNew<InteractorStyle> style;
     vtkNew<vtkAppendPolyData> appendFilter;
signals://信号
    void Msg_Select_Nodes();//选择了节点--导线部分
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

    void ui_Constraint(QTreeWidgetItem* item);//点击创建边界条件弹出界面下放确认
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
    // 菜单事件
    void contextMenuEvent(QContextMenuEvent* event) override;
    QMenu* m_pMenu = nullptr;
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> m_MainStyle; // 保存主界面的交互器样式
    
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