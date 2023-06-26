#pragma once

#include <QDialog>
#include "ui_ConcentrateForce.h"
#include<vector>
#include<iostream>
#include"Node.h"
class test_pic_focusforce;
class Creat_Loads;
class ConcentrateForce : public QDialog
{
	Q_OBJECT

public:
	ConcentrateForce(Creat_Loads* creat_loads, QWidget *parent = nullptr);
	~ConcentrateForce();
	Creat_Loads* m_pCreat_loads = nullptr;
	test_pic_focusforce* ff = nullptr;//画集中力的类
	std::vector< Node*>m_ffNodes;//点集合
	void initialization();
	void Get_Nodes();
	void draw();
private:
	Ui::ConcentrateForceClass ui;
};
