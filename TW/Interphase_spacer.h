#pragma once

#include <QDialog>
#include "ui_Interphase_spacer.h"
#include"Node.h";
class InterFace;
class TowerPart_Insulator;
class Interphase_spacer : public QDialog
{
	Q_OBJECT

public:
	Interphase_spacer(QWidget *parent = nullptr);
	~Interphase_spacer();

	InterFace* m_pInterFace = nullptr;
	void Get_PickedNode();
	double x1, y1, z1;
	double x2, y2, z2;
	void Get_Data(TowerPart_Insulator* TP_insulator);

private:
	Ui::Interphase_spacerClass ui;
};
