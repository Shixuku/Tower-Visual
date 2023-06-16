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
#include<vtkInteractorStyleTrackballCamera.h>

#pragma execution_character_set("utf-8")

class T_Foot;
class T_Body;
class T_CrossArm;
class Tower_Assembly;
class MouseInteractorHighLightActor;
class Wire_InterFace;
class Interphase_spacer;
class AddLoadForce;
class InterFace : public QMainWindow
{
    Q_OBJECT
public:
    InterFace(QWidget* parent = Q_NULLPTR);
    ~InterFace();

    void SetupCentralWidget();//�Ҵ���
    void TreeWidgetShow();//�󴰿�

    vtkRenderer* m_Renderer;//�Ų�����vtk����
    vtkRenderer* m_Renderer_2;//����ʵ����vtk����
    vtkRenderer* m_Renderer_3;//�������vtk����
    vtkGenericOpenGLRenderWindow* m_renderWindow;
   
    void ResetCamera() const;
    void HiddeAllPart();
    void HiddeAllTower();
    void SubstaceActor(Part_Base* Part);
    void ShowLoadactor(Tower* tower);
    void switchRenderWindow(int index);
    void initMenu();
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

     //���߲���
     std::list<vtkSmartPointer<vtkActor>> m_NodeSelected;
     void Add_Select(vtkSmartPointer<vtkActor> pActor);
     void UnSelect_Nodes();
     void Get_SelectedNode(std::list<Node*>& Nodes); //node_list
     void Point_Inqure();
     bool isChildOfTopLevelItem3(QTreeWidgetItem* item);
     bool isChildOfTopLevelItem3OutPut(QTreeWidgetItem* item);
     bool isChildOfTopLevelItem3Inp(QTreeWidgetItem* item);
     void Area_Inqure();//��ѡ
     void Close_Point();
signals://�ź�
    void Msg_Select_Nodes();//ѡ���˽ڵ�--���߲���
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
    void ui_AddLoadForce(QTreeWidgetItem* item);
    void CreateOutPut(QTreeWidgetItem* item);
    void CreateInp(QTreeWidgetItem* item);
private:
    QFile Qf;
    QDataStream Stream;
    QHBoxLayout* layout;
    QVTKOpenGLNativeWidget* m_VtkWidget;
    // �˵��¼�
    void contextMenuEvent(QContextMenuEvent* event) override;
    QMenu* m_pMenu = nullptr;
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> m_MainStyle; // ����������Ľ�������ʽ
    
    //treeWidget
    Part_Base* OnFindPart(const QTreeWidgetItem* Item);
    Tower* OnFindTower(const QTreeWidgetItem* Item);
   
};