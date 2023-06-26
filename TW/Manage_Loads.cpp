#include "Manage_Loads.h"
#pragma execution_character_set("utf-8")
#include"Creat_Loads.h"
#include"ConcentrateForce.h"
#include<QMessageBox>
Manage_Loads::Manage_Loads(InterFace* InterFace, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_pInterFace = InterFace;
	m_pCreat_Loads = new Creat_Loads(InterFace);
	initialize();
	connect(ui.btn_ok, &QPushButton::clicked, this, &Manage_Loads::accept);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &Manage_Loads::reject);
	connect(ui.btn_modify, &QPushButton::clicked, this, &Manage_Loads::Modify_Loads);
	connect(ui.btn_delete, &QPushButton::clicked, this, &Manage_Loads::Delete_Loads);
}

Manage_Loads::~Manage_Loads()
{

}

void Manage_Loads::initialize()
{
	QStringList headertext;//��ͷ
	headertext << "���" << "����" << "����";
	ui.tableWidget->setColumnCount(headertext.count());
	ui.tableWidget->setHorizontalHeaderLabels(headertext);
	ui.tableWidget->verticalHeader()->setVisible(false);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

	//Ŀǰֻ�����˼�����
	int rowcount = m_pInterFace->concentrateforce.size();
	
	ui.tableWidget->setRowCount(rowcount);//��������
	for (int i = 0; i < rowcount; i++)
	{
		ConcentrateForce* cforce = m_pInterFace->concentrateforce[i];
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(cforce->m_pCreat_loads->m_str));
		ui.tableWidget->setItem(i, 2, new QTableWidgetItem(cforce->m_pCreat_loads->m_itemText));
		for (int j = 0; j < 3; j++)
		{
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//����������þ���
		}
	}
}

void Manage_Loads::Modify_Loads()
{
	int Index = ui.tableWidget->currentRow();//��ȡ�����������--currentRow()��0��ʼ
	if (ui.tableWidget->item(Index, 1) == nullptr || ui.tableWidget->item(Index, 1)->text().isEmpty())
	{
		QMessageBox::information(this, "Tips", "��ѡ��Ҫ�޸ĵ��غɣ�");
	}
	else
	{
		ConcentrateForce* cforce = m_pInterFace->concentrateforce[Index];
		cforce->show();

		ui.tableWidget->setItem(Index, 1, new QTableWidgetItem(cforce->m_pCreat_loads->m_str));
		for (int j = 0; j < 3; j++)//��ͷ;
		{
			ui.tableWidget->item(Index, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}
}

void Manage_Loads::Delete_Loads()
{//��ʱֻɾ����ָ�룬û��ɾ��ͼ��
	int Index = ui.tableWidget->currentRow();//��ȡ�����������--currentRow()��0��ʼ
	if (ui.tableWidget->item(Index, 1) == nullptr || ui.tableWidget->item(Index, 1)->text().isEmpty())
	{
		//��ʾ
		QMessageBox::information(this, "Tips", "��ѡ��Ҫɾ�����غɣ�");
	}
	else
	{
		QString name = ui.tableWidget->item(Index, 1)->text();
		//ȷ���Ƿ�ɾ��
		QMessageBox::StandardButton btn;
		btn = QMessageBox::question(this, "��ʾ", "ȷ��Ҫɾ�� " + name + " �غ���",
			QMessageBox::Yes | QMessageBox::No);
		if (btn == QMessageBox::Yes )
		{
			ui.tableWidget->removeRow(Index);//ɾ������е���һ��
			
			//��Ҫɾ�������ָ��
			std::vector<ConcentrateForce*>::iterator it = m_pInterFace->concentrateforce.begin();
			it = m_pInterFace->concentrateforce.begin() + Index;
			m_pInterFace->concentrateforce.erase(it);
		}
	}
}
