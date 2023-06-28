#include "Creat_Loads.h"
#include <iostream> // cout
#include"ConcentrateForce.h"
#include"Gravity.h"
#include"Wind.h"
#include"Polynomial.h"
#include"InterFace.h"

#pragma execution_character_set("utf-8")
Creat_Loads::Creat_Loads(InterFace* pInterFace, QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);
	m_InterFace = pInterFace;
	
	//命名方式暂时处理成这样
	ui.line_name->setText("Load-" + QString::number(m_InterFace->concentrateforce.size() + 1));
	//按钮响应事件
	connect(ui.btn_continue, &QPushButton::clicked, this, [=]()
		{
			btn_continue_clicked();
			this->accept();
		});
	connect(ui.btn_cancle, &QPushButton::clicked, this, &Creat_Loads::reject);
	

}

Creat_Loads::~Creat_Loads()
{

}

void Creat_Loads::btn_continue_clicked()
{
	int index = ui.listWidget->currentRow();//取选的载荷
	m_str = ui.line_name->text();
	QList<QListWidgetItem*> selectedItems = ui.listWidget->selectedItems();
	for (QListWidgetItem* item : selectedItems) {
		m_itemText = item->text();
	}
	
	if (index == 0)
	{//集中力
		ConcentrateForce* concentrateForce = new ConcentrateForce(this);
		concentrateForce->show();
	}
	else if (index == 1)
	{//重力
		Gravity* gravity = new Gravity;
		gravity->show();
	}
	else if (index == 2)
	{//稳定风
		//Wind* wind = new Wind;
		//wind->show();
	}
	else if (index == 3)
	{//多项式函数
		Polynomial* polynomial = new Polynomial;
		polynomial->show();
	}
	
}
