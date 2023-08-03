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
	double CountUz(double h);//�����ѹϵ��
	void CountElePara();//���㵥Ԫ��ز���
	std::vector<double> CountWindForce();//������غ�
	int ExampleNum = 0;
	int num_node;//����ʵ���Ľڵ����
	int num_ele;//����ʵ���ĵ�Ԫ����
	double alf = 0;
	std::vector<double>Uz_node;//ÿ���ڵ�ķ�ѹϵ��
	std::vector<double>Uz_ele;//ÿ����Ԫ�ķ�ѹϵ��
	std::vector<double>L_ele;//ÿ����Ԫ�ĳ���
	std::vector<double>Wx_values;//���غ�����
	//std::vector<double>Interpolation_V;//����Ĳ�ֵ�ķ���
	//InterFace* m_pInterFace = nullptr;
	TowerWireGroup* m_pcreatWire = nullptr;  // Tower ���͵ĳ�Ա���������ڱ��� Show_Tower �����Ľ��
	WindList* w_l = nullptr;
	RandomWind* ran = nullptr;
	Creat_Loads* m_pCreat_loads = nullptr;
	Instance* m_pInstance = nullptr;

	//Test
	double WindSpeedInterpolation(int pointIndex, double time);//������
private:
	Ui::WindClass ui;
	//QVBoxLayout* layout;
	//vtkSmartPointer<vtkRenderWindow> renderWindow;
	//QVTKOpenGLNativeWidget* m_VtkWidget;
	//vtkSmartPointer<vtkRenderer> m_Renderer;

};
