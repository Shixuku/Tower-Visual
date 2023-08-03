#pragma once

#include <QDialog>
#include "ui_Wind.h"
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <QVTKOpenGLNativeWidget.h>
#include"Tower.h"
#include"TowerWireGroup.h"
#pragma execution_character_set("utf-8")
class Creat_Loads;
class WindList;
class RandomWind;
class Instance;

class Wind : public QDialog
{
	Q_OBJECT

public:
	Wind(Creat_Loads* creat_loads, QWidget *parent = nullptr);
	~Wind();
	
	void Initialize();
	void CreateCombobox();
	void ShowObject();
	void ui_Speed();
	void ReadStableData();
	void ReadRandomData();
	void BtnOk();
	void Get_ui_Data();
	double CountUz(double h);//计算风压系数
	void CountElePara();//计算单元相关参数
	std::vector<double> CountWindForce();//计算风载荷
	int ExampleNum = 0;
	int num_node;//分析实例的节点个数
	int num_ele;//分析实例的单元个数
	double alf = 0;
	std::vector<double>Uz_node;//每个节点的风压系数
	std::vector<double>Uz_ele;//每个单元的风压系数
	std::vector<double>L_ele;//每个单元的长度
	std::vector<double>Wx_values;//风载荷容器
	//std::vector<double>Interpolation_V;//计算的插值的风速
	//InterFace* m_pInterFace = nullptr;
	TowerWireGroup* m_pcreatWire = nullptr;  // Tower 类型的成员变量，用于保存 Show_Tower 方法的结果
	WindList* w_l = nullptr;
	RandomWind* ran = nullptr;
	Creat_Loads* m_pCreat_loads = nullptr;
	Instance* m_pInstance = nullptr;

	//Test
	double WindSpeedInterpolation(int pointIndex, double time);//找区间
private:
	Ui::WindClass ui;
	//QVBoxLayout* layout;
	//vtkSmartPointer<vtkRenderWindow> renderWindow;
	//QVTKOpenGLNativeWidget* m_VtkWidget;
	//vtkSmartPointer<vtkRenderer> m_Renderer;

};
