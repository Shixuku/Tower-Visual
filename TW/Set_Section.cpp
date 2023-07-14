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
	connect(ui.btn_ok, &QPushButton::clicked, this, &Set_Section::SendSlots);

	connect(ui.btn_cancle, &QPushButton::clicked, this, &Set_Section::reject);

	connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(Change_Picture_foot()));

	QImage* img = new QImage;
	img->load("./ab2.png");
	ui.label_2->resize(img->width(), img->height());
	ui.label_2->setPixmap(QPixmap::fromImage(*img));
	ui.LEdit_a->setText("0.200");
	ui.LEdit_b->setText("0.020");

}


Set_Section::~Set_Section()
{

}

void Set_Section::Get_Data()
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


void Set_Section::SendSlots()
{
	QStringList strs;
	ClassSection = ui.comboBox->currentIndex();
	ClassMa = ui.comboBox_2->currentIndex() + 1;
	QString Ma = QString::number(ClassMa);
	QString s;
	if (ClassSection == 0)
	{
		s = "L型";
	}
	else
	{
		s = "圆环";
	}
	strs << ui.LEdit_name->text() << s << ui.LEdit_a->text() << ui.LEdit_b->text()<< Ma;
	emit SendData(strs);///!!数据是取出来了
	QDialog::accept();
	name = ui.LEdit_name->text().toStdString();
	
	a = ui.LEdit_a->text().toDouble();
	b = ui.LEdit_b->text().toDouble();
	int iM = ClassMa;

	//修改的
	int id = m_pInterFace->Ms.size() + 1;
	Section* section = new Section(a, b, id, ClassSection, iM);
	m_pInterFace->Ms.Add_Entity(section);

	//第一次调用完之后，删除数据
	ui.LEdit_a->clear();
	ui.LEdit_b->clear();
	ui.LEdit_name->clear();

}




