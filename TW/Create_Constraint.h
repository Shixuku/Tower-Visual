#pragma once
#include <QDialog>
#include "ui_Create_Constraint.h"
#include<iostream>
#include"Tower.h"
class InterFace;

class Create_Constraint : public QDialog
{
	Q_OBJECT

public:
	Create_Constraint(Instance* instance, QWidget *parent = nullptr);
	~Create_Constraint();

	void Draw_Con1();//�̶�Լ��
	void Insert_Data();
	InterFace* m_pInterFace = nullptr;

	//�������޸ĵĲ���

	//Tower* m_pTower = new Tower;
	Instance* m_pInstance = new Instance;

	std::list<Node>Con_Nodes;
	QMap<int, QVector<double>> nodeCoordinates; // �洢�ڵ�������Ϣ
	QMap<int, int> idMap; // �洢ID��������ӳ���ϵ
	int index = 0;
private:
	Ui::Create_ConstraintClass ui;
};
