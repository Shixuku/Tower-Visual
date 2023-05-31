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
	Interphase_spacer(QWidget* parent = nullptr);
	~Interphase_spacer();

	InterFace* m_pInterFace = nullptr;
	TowerPart_Insulator* TP_insulator;
	void Get_Nodes();
	void Get_Data();

private:
	Ui::Interphase_spacerClass ui;
};
