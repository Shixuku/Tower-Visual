#include "ui_TowerLegParameter.h"
#include<QMessageBox.h>
#include"TowerPart_leg.h"
ui_TowerLegParameter::ui_TowerLegParameter(ui_TowerLegType* ui_TowerLegType, QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);
	m_pTowerLegType = ui_TowerLegType;
	Set_tableWideget();//设置下部的参数表格
	Initialize();//初始化合集

	connect(ui.btn_ok, &QPushButton::clicked, this, &ui_TowerLegParameter::Get_Data);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &ui_TowerLegParameter::reject);
	connect(ui.btn_help, &QPushButton::clicked, this, &ui_TowerLegParameter::btn_help);
}

void ui_TowerLegParameter::btn_help()
{
	QMessageBox help_pic;

	help_pic.setIconPixmap(QPixmap("./towerLegFootHelp.png"));
	QPushButton* agreeBut = help_pic.addButton("OK", QMessageBox::AcceptRole);
	help_pic.setWindowTitle("减腿象限");
	help_pic.exec();
	if (help_pic.clickedButton() == (QAbstractButton*)agreeBut)
	{
		help_pic.close();
	}
}

void ui_TowerLegParameter::Set_tableWideget()
{
	QStringList headertext, para, name;//表头、名称、初始值
	headertext << "参数名称" << "值(m)";
	name << "铁塔跟开 L1" << "基面标高 H1" << "减腿A" << "减腿B" << "减腿C" << "减腿D";
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
			//表格文字设置居中
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
		ui.tableWidget->item(i, 0)->setFlags(ui.tableWidget->item(i, 0)->flags() & (~Qt::ItemIsEditable));  // 设置只读属性
	}
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
}

void ui_TowerLegParameter::Initialize()
{
	ui.line_L2->setText("15.160");//设置初始值
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
	leg->m_L2 = ui.line_L2->text().toDouble();//顶部宽度
	leg->m_L1 = ui.tableWidget->item(0, 1)->text().toDouble(); //铁塔跟开
	leg->m_H1 = ui.tableWidget->item(1, 1)->text().toDouble();//基面标高
	for (int i = 0; i < 4; i++)
	{
		leg->m_H[i] = ui.tableWidget->item(i + 2, 1)->text().toDouble();//减腿A、B、C、D
	}
	leg->m_TypeSide = m_pTowerLegType->m_TypeSide;//侧面型号
	leg->m_TypeSeptum = m_pTowerLegType->m_TypeSeptum;//侧面型号

	item->setText(0, leg->m_Name);//子节点名称为自己命名的名称
	leg->Item = item;
	m_pTowerLegType->m_InterFace->m_Renderer->RemoveAllViewProps();

	leg->Create_Mesh();
	leg->m_id = parent->childCount();//编号
	leg->Show_VTKnode(m_pTowerLegType->m_InterFace->m_Renderer);
	leg->Show_VTKtruss(m_pTowerLegType->m_InterFace->m_Renderer);
	leg->Show_VTKbeam(m_pTowerLegType->m_InterFace->m_Renderer);

	m_pTowerLegType->m_InterFace->TP_leg.Add_Entity(leg);
	m_pTowerLegType->m_InterFace->m_Renderer->ResetCamera();
	m_pTowerLegType->m_InterFace->m_Renderer->GetRenderWindow()->Render();
	this->close();

}
