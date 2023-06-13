#include "T_Foot.h"
#include<QMessageBox.h>
#pragma execution_character_set("utf-8")
#include"TowerPart_leg.h"
T_Foot::T_Foot(InterFace* InterFace, QWidget *parent): QDialog(parent)
{

	ui.setupUi(this);
	m_InterFace = InterFace;

	Set_tableWideget();//设置下部的参数表格
	Initialize();//初始化合集

	connect(ui.btn_ok, &QPushButton::clicked, this, &T_Foot::accept);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &T_Foot::reject);
	connect(ui.btn_help, &QPushButton::clicked, this, &T_Foot::btn_help);
	connect(ui.combo_f, SIGNAL(currentIndexChanged(int)), this, SLOT(Change_Picture_foot()));
	connect(ui.combo_g, SIGNAL(currentIndexChanged(int)), this, SLOT(Change_Picture_gemian()));
	ui.tabWidget->setCurrentIndex(0);//设置tab第一页最先显示

}

T_Foot::~T_Foot()
{}

void T_Foot::Set_tableWideget()
{
	QStringList headertext, para, name;//表头、名称、初始值
	headertext << "参数名称" << "值(mm)";
	name << "铁塔跟开 L1"  << "基面标高 H1" << "减腿A" << "减腿B" << "减腿C" << "减腿D";
	para << "18240" << "14000" << "0" << "0" << "0" << "0";
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
	}
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
}

void T_Foot::btn_help()
{
	QMessageBox help_pic;

	help_pic.setIconPixmap(QPixmap("./ax2.png"));
	QPushButton* agreeBut = help_pic.addButton("OK", QMessageBox::AcceptRole);
	help_pic.exec();
	if (help_pic.clickedButton() == (QAbstractButton*)agreeBut)
	{
		help_pic.close();
	}
}

void T_Foot::Get_Data(TowerPart_leg& leg)
{
	leg.m_Name = ui.part_name->text();
	leg.m_L2= ui.line_L2->text().toDouble();//顶部宽度
	leg.m_L1 = ui.tableWidget->item(0, 1)->text().toDouble(); //铁塔跟开
	leg.m_H1= ui.tableWidget->item(1, 1)->text().toDouble();//基面标高
	for (int i = 0; i < 4; i++)
	{
		leg.m_H[i] = ui.tableWidget->item(i + 2, 1)->text().toDouble();//减腿A、B、C、D
	}
	leg.m_TypeSide = ui.combo_f->currentIndex() + 1;//侧面型号
	leg.m_TypeSeptum = ui.combo_g->currentIndex() + 1;//侧面型号
}

void T_Foot::Initialize()
{
	ui.line_L2->setText("15160");//设置初始值

	int T_legs = m_InterFace->ui.treeWidget->topLevelItem(0)->child(0)->childCount() + 1;//塔腿数量
	QString str("塔腿部件-");
	str += QString::number(T_legs);     //str转字符
	ui.part_name->setText(str);//设置初始值

	//塔腿类型图
	QImage* img_f = new QImage;
	img_f->load("./tui1.png");
	ui.label_f->resize(img_f->width(), img_f->height());
	ui.label_f->setPixmap(QPixmap::fromImage(*img_f));
	//隔面类型图
	QImage* img_g = new QImage;
	img_g->load("./gemian1.png");
	ui.label_g->resize(img_g->width(), img_g->height());
	ui.label_g->setPixmap(QPixmap::fromImage(*img_g));
}

void T_Foot::Change_Picture_gemian()
{
	int Index = ui.combo_g->currentIndex();
	QImage* img = new QImage;
	switch (Index)
	{
	case 0:
		img->load("./gemian1.png");
		break;
	case 1:
		img->load("./gemian2.png");
		break;
	case 2:
		img->load("./gemian3.png");
		break;
	case 3:
		img->load("./gemian4.png");
		break;
	case 4:
		img->load("./gemian5.png");
		break;
	}
	ui.label_g->resize(img->width(), img->height());
	ui.label_g->setPixmap(QPixmap::fromImage(*img));
}

void T_Foot::Change_Picture_foot()
{
	int Index = ui.combo_f->currentIndex();
	QImage* img = new QImage;
	switch (Index)
	{
	case 0:
		img->load("./tui1.png");
		break;
	case 1:
		img->load("./tui2.png");
		break;
	case 2:
		img->load("./tui3.png");
		break;
	}
	ui.label_f->resize(img->width(), img->height());
	ui.label_f->setPixmap(QPixmap::fromImage(*img));
}



