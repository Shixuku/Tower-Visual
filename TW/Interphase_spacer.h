#pragma once

#include <QDialog>
#include "ui_Interphase_spacer.h"
#include"Node.h"
#include"Part_Base.h"
#include"TowerPart_CrossArm.h"
class InterFace;
class TowerPart_Insulator;
class Interphase_spacer : public QDialog
{
	Q_OBJECT

public:
	Interphase_spacer(TowerData_CrossArm* CrossArm, QWidget* parent = nullptr);
	~Interphase_spacer();

	InterFace* m_pInterFace = nullptr;
	TowerPart_Insulator* TP_insulator = nullptr;
	void Get_Nodes();
	void Get_Data();
	Part_Base* crossarm = nullptr;
	TowerData_CrossArm* Arm = nullptr;
private:
	Ui::Interphase_spacerClass ui;
};
