#include "Gravity.h"
#include"Creat_Loads.h"
#include"InterFace.h"
#include"Tower.h"
#pragma execution_character_set("utf-8")
Gravity::Gravity(Creat_Loads* creat_loads, QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);
	m_pCreat_loads = creat_loads;
	m_pInstance = m_pCreat_loads->m_instance;//���ø����tower
	initialization();
	connect(ui.btn_ok, &QPushButton::clicked, this, &Gravity::Get_ui_Data);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &Gravity::reject);


}

Gravity::~Gravity()
{}

void Gravity::initialization()
{
	ui.label_name->setText("���ƣ�" + m_pCreat_loads->m_str);//��ʾ�����ڵ�����
	this->setWindowFlags(Qt::WindowStaysOnTopHint);//���ô��������ϲ�
	//��������combobox
	int size = m_pCreat_loads->m_pInterFace->ME_AnalysisSteps.size();
	for (int i = 1; i < size + 1; i++)
	{
		ui.comboBox->addItem(m_pCreat_loads->m_pInterFace->ME_AnalysisSteps.Find_Entity(i)->m_Name);
	}
}

void Gravity::Get_ui_Data()
{
	int id = m_pInstance->m_Gravitys.size() + 1;
	//������
	int AnalysisStep = ui.comboBox->currentIndex() + 1;
	int direction = ui.comboBox_2->currentIndex();//0 1 2 ��Ӧx y z����
	double g = ui.line_g->text().toDouble();
	m_pInstance->m_Gravitys.push_back(ParameterGravity(id, AnalysisStep, direction, g));
	this->accept();
}
