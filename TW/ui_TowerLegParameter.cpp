#include "ui_TowerLegParameter.h"
#include<QMessageBox.h>
#include"TowerPart_leg.h"
ui_TowerLegParameter::ui_TowerLegParameter(ui_TowerLegType* ui_TowerLegType, QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);
	m_pTowerLegType = ui_TowerLegType;
	Set_tableWideget();//�����²��Ĳ������
	Initialize();//��ʼ���ϼ�

	connect(ui.btn_ok, &QPushButton::clicked, this, &ui_TowerLegParameter::Get_Data);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &ui_TowerLegParameter::reject);
	connect(ui.btn_help, &QPushButton::clicked, this, &ui_TowerLegParameter::btn_help);
}

void ui_TowerLegParameter::btn_help()
{
	QMessageBox help_pic;

	help_pic.setIconPixmap(QPixmap("./towerLegFootHelp.png"));
	QPushButton* agreeBut = help_pic.addButton("OK", QMessageBox::AcceptRole);
	help_pic.setWindowTitle("��������");
	help_pic.exec();
	if (help_pic.clickedButton() == (QAbstractButton*)agreeBut)
	{
		help_pic.close();
	}
}

void ui_TowerLegParameter::Set_tableWideget()
{
	QStringList headertext, para, name;//��ͷ�����ơ���ʼֵ
	headertext << "��������" << "ֵ(m)";
	name << "�������� L1" << "������ H1" << "����A" << "����B" << "����C" << "����D";
	para << "18.240" << "14.000" << "0" << "0" << "0" << "0";
	ui.tableWidget->setColumnCount(headertext.count());
	ui.tableWidget->setRowCount(name.count());
	ui.tableWidget->setHorizontalHeaderLabels(headertext);
	ui.tableWidget->verticalHeader()->setVisible(false);
	for (int i = 0; i < name.count(); i++)
	{
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(name[i]));
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(para[i]));
		for (int j = 0; j < headertext.count(); j++)
		{
			//����������þ���
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
		ui.tableWidget->item(i, 0)->setFlags(ui.tableWidget->item(i, 0)->flags() & (~Qt::ItemIsEditable));  // ����ֻ������
	}
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
}

void ui_TowerLegParameter::Initialize()
{
	ui.line_L2->setText("15.160");//���ó�ʼֵ
}

ui_TowerLegParameter::~ui_TowerLegParameter()
{}

void ui_TowerLegParameter::Get_Data()
{
	QTreeWidgetItem* parent = m_pTowerLegType->m_InterFace->ui.treeWidget->topLevelItem(0)->child(0);
	QTreeWidgetItem* item = new QTreeWidgetItem(parent);
	m_pTowerLegType->m_InterFace->AddPartFunction(item);
	TowerPart_leg* leg = new TowerPart_leg;
	leg->m_Name = m_pTowerLegType->m_Name;
	leg->m_L2 = ui.line_L2->text().toDouble();//�������
	leg->m_L1 = ui.tableWidget->item(0, 1)->text().toDouble(); //��������
	leg->m_H1 = ui.tableWidget->item(1, 1)->text().toDouble();//������
	for (int i = 0; i < 4; i++)
	{
		leg->m_H[i] = ui.tableWidget->item(i + 2, 1)->text().toDouble();//����A��B��C��D
	}
	leg->m_TypeSide = m_pTowerLegType->m_TypeSide;//�����ͺ�
	leg->m_TypeSeptum = m_pTowerLegType->m_TypeSeptum;//�����ͺ�

	item->setText(0, leg->m_Name);//�ӽڵ�����Ϊ�Լ�����������
	leg->Item = item;
	m_pTowerLegType->m_InterFace->m_Renderer->RemoveAllViewProps();

	leg->Create_Mesh();
	leg->m_id = parent->childCount();//���
	leg->Show_VTKnode(m_pTowerLegType->m_InterFace->m_Renderer);
	leg->Show_VTKtruss(m_pTowerLegType->m_InterFace->m_Renderer);
	leg->Show_VTKbeam(m_pTowerLegType->m_InterFace->m_Renderer);

	m_pTowerLegType->m_InterFace->TP_leg.Add_Entity(leg);
	m_pTowerLegType->m_InterFace->m_Renderer->ResetCamera();
	m_pTowerLegType->m_InterFace->m_Renderer->GetRenderWindow()->Render();
	this->close();

}
