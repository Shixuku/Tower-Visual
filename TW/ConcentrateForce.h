#pragma once

#include <QDialog>
#include "ui_ConcentrateForce.h"
#include<vector>
#include<iostream>
#include"Node.h"

class Creat_Loads;
class Instance;
class ConcentrateForce : public QDialog
{
	Q_OBJECT

public:
	ConcentrateForce(Creat_Loads* creat_loads, QWidget *parent = nullptr);
	~ConcentrateForce();
	Creat_Loads* m_pCreat_loads = nullptr;
	Instance* m_pInstance = nullptr;
	
	std::list<Node*>m_ffNodes;//µã¼¯ºÏ
	void initialization();
	void Get_Nodes();
	void draw();
private:
	Ui::ConcentrateForceClass ui;
};
