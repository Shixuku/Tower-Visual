#include "Creat_Loads.h"
#include <iostream> // cout
#include"ConcentrateForce.h"
#include"Gravity.h"
#include"Wind.h"
#include"Polynomial.h"
#include"InterFace.h"
#include"IceLoad.h"

#pragma execution_character_set("utf-8")
Creat_Loads::Creat_Loads(Tower* tower,QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);

	m_pInterFace = dynamic_cast<InterFace*>(parent);
	Q_ASSERT(m_pInterFace != nullptr);
	m_tower = tower;

	ui.line_name->setText("Load - ");
	//��ť��Ӧ�¼�
	connect(ui.btn_continue, &QPushButton::clicked, this, &Creat_Loads::btn_continue_clicked);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &Creat_Loads::reject);	
}

Creat_Loads::~Creat_Loads()
{}

void Creat_Loads::btn_continue_clicked()
{
	int index = ui.listWidget->currentRow();//ȡѡ���غ�
	m_str = ui.line_name->text();
	
	if (index == 0)
	{//������
		ConcentrateForce* concentrateForce = new ConcentrateForce(this);
		concentrateForce->show();
	}
	else if (index == 1)
	{//����
		Gravity* gravity = new Gravity(this);
		gravity->show();
	}
	else if (index == 2)
	{//����
		IceLoad* iceLoad = new IceLoad(this);
		iceLoad->show();
	}
	else if (index == 3)
	{//����ʽ����
		Polynomial* polynomial = new Polynomial;
		polynomial->show();
	}
	else if (index == 4)
	{
	}
	this->accept();
}
