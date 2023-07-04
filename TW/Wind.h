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
	double CountUz(double h);//�����ѹϵ��
	void CountElePara();//���㵥Ԫ��ز���
	std::vector<double> CountWindForce();//������غ�
	int ExampleNum = 0;
	int id_Part = 0;
	int num_node;//����ʵ���Ľڵ����
	int num_ele;//����ʵ���ĵ�Ԫ����
	double alf;
	std::vector<double>Uz_node;//ÿ���ڵ�ķ�ѹϵ��
	std::vector<double>Uz_ele;//ÿ����Ԫ�ķ�ѹϵ��
	std::vector<double>L_ele;//ÿ����Ԫ�ĳ���
	std::vector<double>Wx_values;
	InterFace* m_pInterFace = nullptr;
	WindList* w_l = nullptr;
	RandomWind* ran = nullptr;
private:
	Ui::WindClass ui;
	QVBoxLayout* layout;
	vtkSmartPointer<vtkRenderWindow> renderWindow;
	QVTKOpenGLNativeWidget* m_VtkWidget;
	vtkSmartPointer<vtkRenderer> m_Renderer;
	Tower* m_pTower=nullptr;  // Tower ���͵ĳ�Ա���������ڱ��� Show_Tower �����Ľ��
};
