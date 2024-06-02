#include "Manage_Section.h"
#include"InterFace.h"
#include"Section.h"
#include<QMessageBox.h>

Manage_Section::Manage_Section(InterFace*m_InterFace,QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);
	m_pInterFace = m_InterFace;
	Set_headertext();
	connect(ui.btn_ok, &QPushButton::clicked, this, &Manage_Section::accept);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &Manage_Section::reject);
	//connect(ui.btn_modify, &QPushButton::clicked, this, &Manage_Section::Modify_Data);
	//connect(ui.btn_delete, &QPushButton::clicked, this, &Manage_Section::DeleteSection);
}

Manage_Section::~Manage_Section()
{}

void Manage_Section::Set_headertext()
{
	QStringList headertext;//��ͷ
	headertext << "���" << "����" << "����";
	ui.tableWidget->setColumnCount(headertext.count());
	ui.tableWidget->setHorizontalHeaderLabels(headertext);
	ui.tableWidget->verticalHeader()->setVisible(false);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

	int rowcount = m_pInterFace->Ms.size();
	ui.tableWidget->setRowCount(rowcount);
	//int id = m_pInterFace->Ms.Find_Entity(1)->m_id;
	QVector<Section*> ve_section;
	m_pInterFace->Ms.Get_Array(ve_section, true);

	for (int i = 0; i < rowcount; i++)
	{
		Section* section = ve_section[i];
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(section->m_id)));//���
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(section->m_Name));//����
		QString Material;
		if (section->ClassM == 1)
		{
			Material = "Q235";
		}
		else if (section->ClassM == 2)
		{
			Material = "Q345";
		}
		else if (section->ClassM == 3)
		{
			Material = "Q420";
		}
		QString type;
		if (section->ClassSe == 0)//L��
		{
			type = "L";
		}
		else if (section->ClassSe == 1)
		{
			//type = "\u03A6";//��дfai--��ʾ��������
			type = "D";
		}
		QString data = QString::number((section->a)*1000) + "X" + QString::number((section->b)*1000);
		ui.tableWidget->setItem(i, 2, new QTableWidgetItem(Material + type + data));
		for (int j = 0; j < 3; j++)//��ͷ;
		{
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}
}

void Manage_Section::DeleteSection()
{
	int Index = ui.tableWidget->currentRow();//��ȡ�����������--currentRow()��0��ʼ
	int id = ui.tableWidget->item(Index, 0)->text().toInt();
	if (ui.tableWidget->item(Index, 1) == nullptr || ui.tableWidget->item(Index, 1)->text().isEmpty())
	{
		//��ʾ
		QMessageBox::information(this, "Tips", "��ѡ��Ҫɾ���Ľ��棡");
	}
	else
	{
		QString name = ui.tableWidget->item(Index, 1)->text();
		//ȷ���Ƿ�ɾ��
		QMessageBox::StandardButton btn;
		btn = QMessageBox::question(this, "��ʾ", "ȷ��Ҫɾ�� " + name + " ��������",
			QMessageBox::Yes | QMessageBox::No);
		if (btn == QMessageBox::Yes)
		{
			m_pInterFace->Ms.Delete_Entity(id);
			ui.tableWidget->removeRow(Index);//ɾ������е���һ��
		}
	}
}
