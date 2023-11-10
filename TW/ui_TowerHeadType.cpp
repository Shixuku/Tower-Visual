#include "ui_TowerHeadType.h"

ui_TowerHeadType::ui_TowerHeadType(QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);
	Initialize();
	connect(ui.combo_BU, SIGNAL(currentIndexChanged(int)), this, SLOT(Change_Picture_BU()));
	connect(ui.combo_S, SIGNAL(currentIndexChanged(int)), this, SLOT(Change_Picture_S()));
	connect(ui.combo_g, SIGNAL(currentIndexChanged(int)), this, SLOT(Change_Picture_gemian()));
}

ui_TowerHeadType::~ui_TowerHeadType()
{}

void ui_TowerHeadType::Initialize()
{
	//BU类型图
	QImage* img_BU = new QImage;
	img_BU->load("./headBU1.png");
	ui.label_BU->resize(img_BU->width(), img_BU->height());
	ui.label_BU->setPixmap(QPixmap::fromImage(*img_BU));
	//S类型图
	QImage* img_S = new QImage;
	img_S->load("./headS1.png");
	ui.label_S->resize(img_S->width(), img_S->height());
	ui.label_S->setPixmap(QPixmap::fromImage(*img_S));
	//隔面类型图
	QImage* img_g = new QImage;
	img_g->load("./gemian1.png");
	ui.label_g->resize(img_g->width(), img_g->height());
	ui.label_g->setPixmap(QPixmap::fromImage(*img_g));
}
void ui_TowerHeadType::Change_Picture_BU()
{
	int Index = ui.combo_BU->currentIndex();
	QImage* img = new QImage;
	switch (Index)
	{
	case 0:
		img->load("./headBU1.png");
		break;
	case 1:
		img->load("./headBU2.png");
		break;
	case 2:
		img->load("./headBU3.png");
		break;
	case 3:
		img->load("./headBU4.png");
		break;
	}
	ui.label_BU->resize(img->width(), img->height());
	ui.label_BU->setPixmap(QPixmap::fromImage(*img));
}

void ui_TowerHeadType::Change_Picture_S()
{
	int Index = ui.combo_S->currentIndex();
	QImage* img = new QImage;
	switch (Index)
	{
	case 0:
		img->load("./headS1.png");
		break;
	case 1:
		img->load("./headS2.png");
		break;
	case 2:
		img->load("./headS3.png");
		break;

	}
	ui.label_S->resize(img->width(), img->height());
	ui.label_S->setPixmap(QPixmap::fromImage(*img));
}

void ui_TowerHeadType::Change_Picture_gemian()
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
