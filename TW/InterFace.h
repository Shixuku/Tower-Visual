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
#include<QFileDialog>
#include <QString>
#include <QDebug>
#include <QList>
#include<vector>
#include"Set_Section.h"
#include<vtkInteractorStyleTrackballCamera.h>
#include"TowerWireGroupAssembly.h"
#pragma execution_character_set("utf-8")

class T_Foot;
class T_Body;
class T_CrossArm;
class Tower_Assembly;
class MouseInteractorHighLightActor;
class Wire_InterFace;
class Interphase_spacer;
class AddLoadForce;
class Create_Constraint;
class ConcentrateForce;
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
    vtkRenderer* m_Renderer_2;//单塔实例的vtk窗口
    vtkRenderer* m_Renderer_3;//塔线组的vtk窗口
    vtkGenericOpenGLRenderWindow* m_renderWindow;
   
    void ResetCamera() const;
    void HiddeAllPart();
    void HiddeAllTower();
    void SubstaceActor(Part_Base* Part);
    void ShowLoadactor(Tower* tower);
    void switchRenderWindow(int index);
    void initMenu();
    void AddPartFunction(QTreeWidgetItem* item);
     Ui::InterFaceClass ui;

     Manage_Entity<TowerPart_leg> TP_leg;
     Manage_Entity<TowerPart_body> TP_body;
     Manage_Entity<TowerPart_CrossArm> TP_CrossArm;
     Manage_Entity<TowerPart_Insulator> towerPartInsulator;
     Manage_Entity<Tower> TP;
     Manage_Entity<Section> Ms;
     
     vector<T_Foot*> t_foots;
     vector<T_Body*> t_bodys;
     vector<T_CrossArm*> t_crossarms;
     vector<Tower_Assembly*> tower_assembles;
     vector<ConcentrateForce*> concentrateforce;
    
     std::vector<Section>MS;
     double* GetSectionData(int SectionGroup);
     std::vector<QStringList> Scetion_lists;

     //导线部分
     std::list<vtkSmartPointer<vtkActor>> m_NodeSelected;
     void Add_Select(vtkSmartPointer<vtkActor> pActor);
     void UnSelect_Nodes();
     void Get_SelectedNode(std::list<Node*>& Nodes); //node_list
     void Get_SelectedNode(std::set<Node*>& Nodes);
     void Point_Inqure();
     bool isChildOfTopLevelItem3(QTreeWidgetItem* item);//取载荷
     bool isChildOfTopLevelItem3OutPut(QTreeWidgetItem* item);//计算文件
     bool isChildOfTopLevelItem3Inp(QTreeWidgetItem* item);//ABAQUS计算文件
     bool isChildOfPartSetSection(QTreeWidgetItem* item);//选择赋予截面的item
     bool isChildOfPartSetAllSection(QTreeWidgetItem* item);//选择赋予全部截面的item
     bool isChildOfPartSetSpacer(QTreeWidgetItem* item);//将绝缘子放在横担下
     void Area_Inqure();//框选
     void Close_Point();
     vector<int> con_id;
     vector<Node>Con_Nodes;
     Node* Find_Node(int id, vector<Node>& node,vector<Node>&Con_node);
     void Insert_Data();
     void handleOkButtonClicked();
     QMap<int, QVector<double>> nodeCoordinates; // 存储节点坐标信息
     QMap<int, int> idMap; // 存储ID与索引的映射关系
     int index = 0;
  
signals://信号
    void Msg_Select_Nodes();//选择了节点--导线部分
    void Msg_CreateModel();
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
    void ui_Wire_InterFace();
    void ui_Constraint();
    void SaveFile();
    void OpenFile();
    void ui_Section();
    void ui_SetSection(QTreeWidgetItem* item);
    void ui_SetAllSection(QTreeWidgetItem* item);
    void Show_Part(Part_Base* part);
    void Show_Tower(Tower* tower);
    void GetData(QStringList&);
    void Test_mousePressEvent(QMouseEvent* event);
    void ui_AddLoadForce(QTreeWidgetItem* item);
    void CreateOutPut(QTreeWidgetItem* item);
    void CreateInp(QTreeWidgetItem* item);
    void Constraint_Tips();//点击创建边界条件弹出界面下放确认
    void Delete_Constraint();//点击约束删除Widget里面的控件]
  
    void ui_ConcentratedForce(QTreeWidgetItem* item);
    void ui_Creat_Loads();
    void ui_Manage_Loads();
    void ui_TowerWireGroup();
private:
    QFile Qf;
    QDataStream Stream;
    QHBoxLayout* layout;
    QVTKOpenGLNativeWidget* m_VtkWidget;
    // 菜单事件
    void contextMenuEvent(QContextMenuEvent* event) override;
    QMenu* m_pMenu = nullptr;
    QHBoxLayout* layout_Tips;
    QLabel* label;
    QLineEdit* Node_Edit;
    QPushButton* OK_Btn;
    QPushButton* Ensure_Btn;
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> m_MainStyle; // 保存主界面的交互器样式
    
    //treeWidget
    Part_Base* OnFindPart(const QTreeWidgetItem* Item);
    Tower* OnFindTower(const QTreeWidgetItem* Item);
    TowerData_CrossArm* OnFindCrossAem(const QTreeWidgetItem* Item);
};