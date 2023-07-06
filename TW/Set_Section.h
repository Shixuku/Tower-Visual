#pragma once

#include <QDialog>
#include<QTimer>
#include"ui_Set_Section.h"
#include"Section.h"
#include<vector>
#include<iostream>
#include"InterFace.h"
class Set_Section : public QDialog
{
	Q_OBJECT

public:
	Set_Section(InterFace* m_pInterFace, QWidget* parent = nullptr);
	InterFace* m_pInterFace = nullptr;
	~Set_Section();
	std::vector<Section>Classs;
	void Get_Data();
	//ȡ����
	string name;//����
	int ClassSection = 0;//��������
	int ClassMa = 0;//��������
	double a = 0;//�⾶���߳���
	double b = 0;//�ھ����߶̱�
signals:
	void SendData(QStringList&str);

private slots:
	void SendSlots();
	void Change_Picture_foot();
private:
	Ui::Set_SectionClass ui;

};
