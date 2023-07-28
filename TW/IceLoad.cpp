#include "IceLoad.h"
#include"Creat_Loads.h"
#include"Tower.h"
#include"InterFace.h"
#include"InteractorStyle.h"
#include"ParameterIceElement.h"
#pragma execution_character_set("utf-8")

IceLoad::IceLoad(Creat_Loads * creat_loads, QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);

	m_pCreat_loads = creat_loads;
	m_pTower = m_pCreat_loads->m_tower;//���ø����tower
	Initialization();//combox��ʼ��
	this->setWindowFlags(Qt::WindowStaysOnTopHint);//���ô��������ϲ�
	connect(ui.btn_ok, &QPushButton::clicked, this, &IceLoad::GetData);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &IceLoad::reject);
	
}

IceLoad::~IceLoad()
{}

void IceLoad::Initialization()
{
	ui.label_name->setText("���ƣ�" + m_pCreat_loads->m_str);//��ʾ�����ڵ�����
	//����combobox
	QStringList headertext;//��ͷ
	headertext << "����������" << "�ѱ�������"<<"�������(m)";
	ui.tableWidget->setColumnCount(headertext.count());//��
	ui.tableWidget->setRowCount(1);//��
	ui.tableWidget->setHorizontalHeaderLabels(headertext);
	ui.tableWidget->verticalHeader()->setVisible(false);

	QComboBox *AnalysisStep1 = new QComboBox;
	int size1 = m_pCreat_loads->m_pInterFace->ME_AnalysisSteps.size();
	for (int i = 1; i < size1 + 1; i++)
	{
		AnalysisStep1->addItem(m_pCreat_loads->m_pInterFace->ME_AnalysisSteps.Find_Entity(i)->m_Name);
	}
	ui.tableWidget->setCellWidget(0, 0, (QWidget*)AnalysisStep1);

	QComboBox*AnalysisStep2 = new QComboBox;
	int size2 = m_pCreat_loads->m_pInterFace->ME_AnalysisSteps.size();
	for (int i = 1; i < size2 + 1; i++)
	{
		AnalysisStep2->addItem(m_pCreat_loads->m_pInterFace->ME_AnalysisSteps.Find_Entity(i)->m_Name);
	}
	ui.tableWidget->setCellWidget(0, 1, (QWidget*)AnalysisStep2);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
}

void IceLoad::GetData()
{
	int size = m_pTower->m_Elements.size();//124
	for (int i = 1; i < size+1; i++)
	{
		int m_id = i + size;//����Ԫ���
		int Element = i;//��Ԫ���
		QWidget* widget_Start = ui.tableWidget->cellWidget(0, 0);
		QWidget* widget_End = ui.tableWidget->cellWidget(0, 1);
		QComboBox* combox1 = (QComboBox*)widget_Start;
		QComboBox* combox2 = (QComboBox*)widget_End;
		int addIce = combox1->currentIndex() + 1;//����������
		int reduceIce = combox2->currentIndex() + 1;//�ѱ�������
		double m_thickness = ui.tableWidget->item(0, 2)->text().toDouble();//�������
		m_pTower->m_IceElement.push_back(ParameterIceElement(m_id, Element, addIce, reduceIce, m_thickness));
	}
	this->accept();
}

