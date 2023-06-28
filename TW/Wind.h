#pragma once

#include <QDialog>
#include "ui_Wind.h"
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <QVTKOpenGLNativeWidget.h>
#include"Tower.h"
#pragma execution_character_set("utf-8")
class InterFace;
class WindList;
class RandomWind;


class Wind : public QDialog
{
	Q_OBJECT

public:
	Wind(InterFace* pInterFace, QWidget *parent = nullptr);
	~Wind();
	
	void Initialize();
	void CreateCombobox();
	void ShowObject();
	void ui_Speed();
	void ReadData();
	void BtnOk();
	double CountUz(double h);//计算风压系数
	int ExampleNum = 0;
	int id_Part = 0;
	int num;//分析实例的节点个数
	InterFace* m_pInterFace = nullptr;
	WindList* w_l = nullptr;
	RandomWind* ran = nullptr;
private:
	Ui::WindClass ui;
	QVBoxLayout* layout;
	vtkSmartPointer<vtkRenderWindow> renderWindow;
	QVTKOpenGLNativeWidget* m_VtkWidget;
	vtkSmartPointer<vtkRenderer> m_Renderer;
	Tower* m_pTower=nullptr;  // Tower 类型的成员变量，用于保存 Show_Tower 方法的结果
};
