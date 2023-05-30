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
#include "Tower.h"
#include<QFileDialog>
#include <QString>
#include <QDebug>
#include <QList>
#include<vector>
#include"Set_Section.h"
#pragma execution_character_set("utf-8")

class T_Foot;
class T_Body;
class T_CrossArm;
class Tower_Assembly;
class MouseInteractorHighLightActor;
class Wire_InterFace;

class InterFace : public QMainWindow
{
    Q_OBJECT
public:
    InterFace(QWidget* parent = Q_NULLPTR);
    ~InterFace();

    void SetupCentralWidget();//右窗口
    void TreeWidgetShow();//左窗口

    vtkRenderer* m_Renderer;
    vtkGenericOpenGLRenderWindow* m_renderWindow;
    void ResetCamera() const;
    void HiddeAll();
    void SubstaceActor(Part_Base* Part);
     Ui::InterFaceClass ui;

     //Set_Section* m_ab = nullptr;
     Manage_Entity<TowerPart_leg> TP_leg;
     Manage_Entity<TowerPart_body> TP_body;
     Manage_Entity<TowerPart_CrossArm> TP_CrossArm;
     Manage_Entity<Tower> TP;
     Manage_Entity<Section> Ms;
     vector<T_Foot*> t_foots;
     vector<T_Body*> t_bodys;
     vector<T_CrossArm*> t_crossarms;
     vector<Tower_Assembly*> tower_assembles;

     std::vector<Section>MS;
     double* GetSectionData(int SectionGroup);
     std::vector<QStringList> Scetion_lists;

     //导线部分
     std::list<vtkSmartPointer<vtkActor>> m_NodeSelected;
     void Add_Select(vtkSmartPointer<vtkActor> pActor);
     void UnSelect_Nodes();
     void Get_SelectedNode(std::list<Node*>& Nodes); //node_list
     void Point_Inqure();
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
    void ui_Interphase_spacer();
    void ui_Wire_InterFace();
    void SaveFile();
    void OpenFile();
    void ui_Section();
    void ui_SetSection();
    void ui_SetAllSection();
    void Show_Part(Part_Base* part);
    void Show_Tower(Tower* tower);
    void GetData(QStringList&);
    void Test_mousePressEvent(QMouseEvent* event);
private:
    QFile Qf;
    QDataStream Stream;
    QHBoxLayout* layout;
    QVTKOpenGLNativeWidget* m_VtkWidget;
    
    //treeWidget
    Part_Base* OnFindPart(const QTreeWidgetItem* Item);
    Tower* OnFindTower(const QTreeWidgetItem* Item);
   
};