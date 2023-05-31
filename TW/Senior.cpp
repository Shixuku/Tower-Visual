#include "Senior.h"
#include"Spacer.h"

Senior::Senior(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	Initialize();
	connect(ui.OK_Btn, &QPushButton::clicked, this, [=]() {   Get_Data(); this->close(); });
	connect(ui.Cancel_Btn, &QPushButton::clicked, this, [=]() {   this->close(); });
	connect(ui.Sp_choose, SIGNAL(currentIndexChanged(int)), this, SLOT(Show_Spacer()));
}

Senior::~Senior()
{}

void Senior::Initialize()
{
	ui.N_pts->setText("100");
	ui.bizai_Btn->setText("1.6855");
	ui.Btn_Area->setText("531.68");
	ui.li_Btn->setText("10");
	ui.nz_Len->setText("4000");
}

void Senior::Draw_Wire(double* p1, double* p2, double* p3, double* p4, double* p5)
{
	p1[0] = ui.bizai_Btn->text().toDouble();
	p2[0] = ui.li_Btn->text().toDouble();
	p3[0] = ui.N_pts->text().toDouble();
	p4[0] = ui.Btn_Area->text().toDouble();
	p5[0] = ui.nz_Len->text().toInt();
}

void Senior::Get_Data()
{
	choose = ui.Sp_choose->currentIndex();
	if (choose == 1)//公式
	{
		Num_Spacer.insert(Num_Spacer.end(), sp->Spacer_Num.begin(), sp->Spacer_Num.end());
		choose_way = sp->choose;
	}
}

void Senior::Show_Spacer()
{
	int Index = ui.Sp_choose->currentIndex();
	//公式
	if (Index == 1)
	{
		sp = new Spacer(this);
		sp->Set_Lists(N_Pts - 1);
		Qt::WindowFlags flags = sp->windowFlags();
		sp->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);
		sp->show();
	}
	//自定义
	else if (Index == 2)
	{
	}
}