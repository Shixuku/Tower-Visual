#include "ui_TowerLegType.h"
#include"ui_TowerLegParameter.h"
ui_TowerLegType::ui_TowerLegType(InterFace* InterFace, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_InterFace = InterFace;
	Initialize();
	connect(ui.btn_continue, &QPushButton::clicked, this, &ui_TowerLegType::btnContinue);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &ui_TowerLegType::reject);
	connect(ui.combo_f, SIGNAL(currentIndexChanged(int)), this, SLOT(Change_Picture_foot()));
	connect(ui.combo_g, SIGNAL(currentIndexChanged(int)), this, SLOT(Change_Picture_gemian()));
}

ui_TowerLegType::~ui_TowerLegType()
{}
void ui_TowerLegType::Initialize()
{
	int T_legs = m_InterFace->TP_leg.size() + 1;//塔腿数量
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

void ui_TowerLegType::btnContinue()
{
	m_TypeSide = ui.combo_f->currentIndex() + 1;//侧面型号
	m_TypeSeptum = ui.combo_g->currentIndex() + 1;//侧面型号
	m_Name= ui.part_name->text();
	this->close();
	ui_TowerLegParameter* towerLegParameter = new ui_TowerLegParameter(this);
	towerLegParameter->show();

}

void ui_TowerLegType::Change_Picture_gemian()
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

void ui_TowerLegType::Change_Picture_foot()
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
