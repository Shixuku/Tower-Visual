#pragma once

#include <QDialog>
#include "ui_Tower_Assembly.h"
#include"InterFace.h"
#include<QTableWidget>
#include<QTableWidgetItem>
#include<qvector.h>

class Tower_Assembly : public QDialog
{
	Q_OBJECT

public:
	Tower_Assembly(InterFace* InterFace, QWidget* parent = nullptr);
	~Tower_Assembly();

	void Create_combobox();
	QString Get_name();//������ȡ���������
	
	QVector<int> m_ArryLeg;//��������
	QVector<int> m_ArryBody;//���������
	QVector<int> m_ArryHead;//��ͷ����

	void Get_Movedata(double& x,double& y,double& z,double& angle);
	
private:
	Ui::Tower_AssemblyClass ui;

	InterFace* m_InterFace = nullptr;

	void Get_PartData();
};
