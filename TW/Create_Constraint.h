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

	void Draw_Con1();//固定约束
	void Insert_Data();
	InterFace* m_pInterFace = nullptr;

	//以下是修改的部分

	//Tower* m_pTower = new Tower;
	Instance* m_pInstance = new Instance;

	std::list<Node>Con_Nodes;
	QMap<int, QVector<double>> nodeCoordinates; // 存储节点坐标信息
	QMap<int, int> idMap; // 存储ID与索引的映射关系
	int index = 0;
private:
	Ui::Create_ConstraintClass ui;
};
