#include "Manage_Loads.h"
#pragma execution_character_set("utf-8")
Manage_Loads::Manage_Loads(QWidget *parent)
	: QDialog(parent)
{
	initialize();
	ui.setupUi(this);
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

}

