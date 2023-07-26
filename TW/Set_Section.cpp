#include "Set_Section.h"
#include <QMouseEvent>
#include<iostream>
#include<string>
#pragma execution_character_set("utf-8")
using namespace std;

Set_Section::Set_Section(InterFace* InterFace,QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);
	m_pInterFace = InterFace;
	connect(ui.btn_ok, &QPushButton::clicked, this, &Set_Section::GetData);

	connect(ui.btn_cancle, &QPushButton::clicked, this, &Set_Section::reject);

	void (QComboBox:: * IndexChanged)(int) = &QComboBox::currentIndexChanged;
	connect(ui.comboBox, IndexChanged, this, &Set_Section::Change_Picture_foot);

	QImage* img = new QImage;
	img->load("./ab2.png");
	ui.label_2->resize(img->width(), img->height());
	ui.label_2->setPixmap(QPixmap::fromImage(*img));
	ui.LEdit_a->setText("0.200");
	ui.LEdit_b->setText("0.020");
	ui.line_name->setText("Section-" + QString::number(m_pInterFace->Ms.size() + 1));
}


Set_Section::~Set_Section()
{
}

void Set_Section::Change_Picture_foot()
{
	int Index = ui.comboBox->currentIndex();
	QImage* img = new QImage;
	switch (Index)
	{
	case 0:
		img->load("./ab2.png");
		break;
	case 1:
		img->load("./ab3.png");
		break;
	}
	ui.label_2->resize(img->width(), img->height());
	ui.label_2->setPixmap(QPixmap::fromImage(*img));
}


void Set_Section::GetData()
{
	QString name = ui.line_name->text();
	double a = ui.LEdit_a->text().toDouble();
	double b = ui.LEdit_b->text().toDouble();
	int id = m_pInterFace->Ms.size() + 1;
	int ClassSection = ui.comboBox->currentIndex();
	int iM = ui.comboBox_2->currentIndex() + 1;

	Section* section = new Section(name, a, b, id, ClassSection, iM);
	m_pInterFace->Ms.Add_Entity(section);
	
	this->accept();
	//第一次调用完之后，删除数据
	ui.LEdit_a->clear();
	ui.LEdit_b->clear();
	ui.line_name->clear();

}




