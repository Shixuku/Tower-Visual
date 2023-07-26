#include "Gravity.h"
#include"Creat_Loads.h"
#include"Tower.h"
#pragma execution_character_set("utf-8")
Gravity::Gravity(Creat_Loads* creat_loads, QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);
	m_pCreat_loads = creat_loads;
	m_pTower = m_pCreat_loads->m_tower;//调用父类的tower

	connect(ui.btn_ok, &QPushButton::clicked, this, &Gravity::Get_ui_Data);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &Gravity::reject);

	ui.label_name->setText("名称：" + m_pCreat_loads->m_str);//显示父窗口的名称
	this->setWindowFlags(Qt::WindowStaysOnTopHint);//设置窗口在最上层
}

Gravity::~Gravity()
{}

void Gravity::Get_ui_Data()
{
	int id = m_pTower->m_Gravitys.size() + 1;
	int direction = ui.line_direction->text().toInt();
	double g = ui.line_g->text().toDouble();
	m_pTower->m_Gravitys.push_back(ParameterGravity(id, direction, g));
	this->accept();
}
