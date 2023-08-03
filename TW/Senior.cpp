#include "Senior.h"
#include"Wire_InterFace.h"
#include"Spacer.h"
Senior::Senior(Wire_InterFace* Wire_InterFace,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	wireInterFace = Wire_InterFace;
	Initialize();
	connect(ui.rad_stress, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(0); });
	connect(ui.rad_sag, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(1); });
	connect(ui.OK_Btn, &QPushButton::clicked, this, [=]() {CreatData(); this->close(); });
	connect(ui.Sp_choose, SIGNAL(currentIndexChanged(int)), this, SLOT(ui_Spacer()));
}

Senior::~Senior()
{}

void Senior::Initialize()
{
	ui.N_pts->setText("100");
	ui.bizai_Btn->setText("2.06");
	ui.Btn_Area->setText("666.55");
	ui.li_Btn->setText("10");
	ui.nz_Len->setText("1");
	ui.sag_edit->setText("4");
}

void Senior::ui_Spacer()
{
	int Index = ui.Sp_choose->currentIndex();
	if (Index == 1)
	{
		if (sp == nullptr)
		{
			sp = new Spacer(this);
			sp->Set_Lists(wireInterFace->a / wireInterFace->wireLogoQty);
			sp->exec();
		}
		else
		{
			sp->Set_Lists(wireInterFace->a / wireInterFace->wireLogoQty);
			sp->exec();
		}
	}
	else if (Index == 2)
	{

	}
}
void Senior::CreatData()
{
	wireInterFace->N = ui.N_pts->text().toInt();
	wireInterFace->unitMass = ui.bizai_Btn->text().toDouble();
	wireInterFace->area = ui.Btn_Area->text().toDouble();
	wireInterFace->stress = ui.li_Btn->text().toDouble();
	wireInterFace->strainLength = ui.nz_Len->text().toDouble();
	wireInterFace->sag = ui.sag_edit->text().toDouble();
	wireInterFace->SpacerNum = SpacerNum;
	wireInterFace->ChooseWay = ChooseWay;

	//// 发射信号并传递数值
	//emit valuesReturned(N, unitMass, area, stress, strainL, sag);
}



