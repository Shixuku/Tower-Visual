#include "Senior.h"
#include"Wire_InterFace.h"
Senior::Senior(Wire_InterFace* Wire_InterFace,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	wireInterFace = Wire_InterFace;
	Initialize();
	connect(ui.rad_stress, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(0); });
	connect(ui.rad_sag, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(1); });
	connect(ui.OK_Btn, &QPushButton::clicked, this, [=]() {CreatData(); this->close(); });
}

Senior::~Senior()
{}

void Senior::Initialize()
{
	ui.N_pts->setText("100");
	ui.bizai_Btn->setText("1.6855");
	ui.Btn_Area->setText("531.68");
	ui.li_Btn->setText("10");
	ui.nz_Len->setText("1");
	ui.sag_edit->setText("4000");
}

void Senior::CreatData()
{
	wireInterFace->N = ui.N_pts->text().toInt();
	wireInterFace->unitMass = ui.bizai_Btn->text().toDouble();
	wireInterFace->area = ui.Btn_Area->text().toDouble();
	wireInterFace->stress = ui.li_Btn->text().toDouble();
	wireInterFace->strainL = ui.nz_Len->text().toDouble();
	wireInterFace->sag = ui.sag_edit->text().toDouble();

	//// 发射信号并传递数值
	//emit valuesReturned(N, unitMass, area, stress, strainL, sag);
}



