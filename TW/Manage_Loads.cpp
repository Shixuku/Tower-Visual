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
	QStringList headertext;//表头
	headertext << "序号" << "名称" << "类型";
	ui.tableWidget->setColumnCount(headertext.count());
	ui.tableWidget->setHorizontalHeaderLabels(headertext);
	ui.tableWidget->verticalHeader()->setVisible(false);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

	//目前只考虑了集中力
	int rowcount = m_pInterFace->concentrateforce.size();
	
	ui.tableWidget->setRowCount(rowcount);//设置行数
	for (int i = 0; i < rowcount; i++)
	{
		ConcentrateForce* cforce = m_pInterFace->concentrateforce[i];
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(cforce->m_pCreat_loads->m_str));
		ui.tableWidget->setItem(i, 2, new QTableWidgetItem(cforce->m_pCreat_loads->m_itemText));
		for (int j = 0; j < 3; j++)
		{
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//表格文字设置居中
		}
	}
}

void Manage_Loads::Modify_Loads()
{
	int Index = ui.tableWidget->currentRow();//获取表格所在行数--currentRow()从0开始
	if (ui.tableWidget->item(Index, 1) == nullptr || ui.tableWidget->item(Index, 1)->text().isEmpty())
	{
		QMessageBox::information(this, "Tips", "请选择要修改的载荷！");
	}
	else
	{
		ConcentrateForce* cforce = m_pInterFace->concentrateforce[Index];
		cforce->show();

		ui.tableWidget->setItem(Index, 1, new QTableWidgetItem(cforce->m_pCreat_loads->m_str));
		for (int j = 0; j < 3; j++)//表头;
		{
			ui.tableWidget->item(Index, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}
}

void Manage_Loads::Delete_Loads()
{//暂时只删除了指针，没有删除图形
	int Index = ui.tableWidget->currentRow();//获取表格所在行数--currentRow()从0开始
	if (ui.tableWidget->item(Index, 1) == nullptr || ui.tableWidget->item(Index, 1)->text().isEmpty())
	{
		//提示
		QMessageBox::information(this, "Tips", "请选择要删除的载荷！");
	}
	else
	{
		QString name = ui.tableWidget->item(Index, 1)->text();
		//确认是否删除
		QMessageBox::StandardButton btn;
		btn = QMessageBox::question(this, "提示", "确定要删除 " + name + " 载荷吗？",
			QMessageBox::Yes | QMessageBox::No);
		if (btn == QMessageBox::Yes )
		{
			ui.tableWidget->removeRow(Index);//删掉表格中的那一行
			
			//需要删掉界面的指针
			std::vector<ConcentrateForce*>::iterator it = m_pInterFace->concentrateforce.begin();
			it = m_pInterFace->concentrateforce.begin() + Index;
			m_pInterFace->concentrateforce.erase(it);
		}
	}
}
