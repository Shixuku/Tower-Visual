#include "Senior.h"

Senior::Senior(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	Initialize();
	connect(ui.OK_Btn, &QPushButton::clicked, this, [=]() {   this->close(); });
	connect(ui.Cancel_Btn, &QPushButton::clicked, this, [=]() {   this->close(); });
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
