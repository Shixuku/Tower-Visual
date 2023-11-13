#include "ui_TowerBodyType.h"

ui_TowerBodyType::ui_TowerBodyType(QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);
	Set_No1_picture();//设置首张图片
	connect(ui.combo_b, SIGNAL(currentIndexChanged(int)), this, SLOT(Change_Picture_body()));
	connect(ui.combo_g, SIGNAL(currentIndexChanged(int)), this, SLOT(Change_Picture_gem()));
}

ui_TowerBodyType::~ui_TowerBodyType()
{}

void ui_TowerBodyType::Set_No1_picture()
{
	ui.tabWidget->setCurrentIndex(0);//设置默认page
	QImage* img = new QImage;
	img->load("./shen1.png");
	ui.label_b->resize(img->width(), img->height());
	ui.label_b->setPixmap(QPixmap::fromImage(*img));

	QImage* img_gem = new QImage;
	img_gem->load("./gemian1.png");
	ui.label_g->resize(img_gem->width(), img_gem->height());
	ui.label_g->setPixmap(QPixmap::fromImage(*img_gem));
}

void ui_TowerBodyType::Change_Picture_gem()
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

void ui_TowerBodyType::Change_Picture_body()
{
	int Index = ui.combo_b->currentIndex();
	QImage* img = new QImage;
	switch (Index)
	{
	case 0:
		img->load("./shen1.png");
		break;
	case 1:
		img->load("./shen2.png");
		break;
	case 2:
		img->load("./shen3.png");
		break;
	case 3:
		img->load("./shen4.png");
		break;
	case 4:
		img->load("./shen5.png");
		break;
	case 5:
		img->load("./shen6.png");
		break;
	case 6:
		img->load("./shen7.png");
		break;
	case 7:
		img->load("./shen8.png");
		break;
	case 8:
		img->load("./shen9.png");
		break;
	case 9:
		img->load("./shen10.png");
		break;
	}
	ui.label_b->resize(img->width(), img->height());
	ui.label_b->setPixmap(QPixmap::fromImage(*img));
}