#include "Element_Ice.h"
#include"InterFace.h"
#include<iostream>
Element_Ice::Element_Ice(InterFace* pInterFace, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_pInterFace = pInterFace;
	connect(ui.btn_ok, &QPushButton::clicked, this, &Element_Ice::GetData);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &Element_Ice::reject);
	//³õÊ¼»¯
	ui.line_addice->setText("1");
	ui.line_eraseice->setText("2");
	ui.line_d->setText("0.15");
}

Element_Ice::~Element_Ice()
{}

void Element_Ice::GetData()
{
	//std::cout << "start " << m_pInterFace->ME_ElementIce.size() << "\n";
	int id = m_pInterFace->ME_ElementIce.size() + 1;
	int startTime = ui.line_addice->text().toInt();
	int endTime = ui.line_eraseice->text().toInt();
	double thickness = ui.line_d->text().toDouble();
	ParameterIceElement *pParameterIceElement = new ParameterIceElement(id, startTime, endTime, thickness);
	m_pInterFace->ME_ElementIce.push_back(pParameterIceElement);
	this->accept();
	//std::cout << "end " << m_pInterFace->ME_ElementIce.size() << "\n";

}
