#pragma once

#include <QDialog>
#include "ui_Interphase_spacer.h"
#include"Node.h"
#include"Part_Base.h"
#include"TowerPart_CrossArm.h"
class InterFace;
class TowerPart_Insulator;
class TowerWireGroup;
class Interphase_spacer : public QDialog
{
	Q_OBJECT

public:
	Interphase_spacer(TowerData_CrossArm* CrossArm, TowerWireGroup* towerWireGroup,QWidget* parent = nullptr);
	~Interphase_spacer();

	InterFace* m_pInterFace = nullptr;
	TowerPart_Insulator* TP_insulator = nullptr;
	TowerWireGroup* towerWireGroup = nullptr;
	void Get_Nodes();
	void Create_Nodes();
	void Get_Data();
	Part_Base* crossarm = nullptr;
	TowerData_CrossArm* Arm = nullptr;
	vector<Node*>m_Nodes;
	TowerPart_Insulator* ReturnInsulator();

private:
	Ui::Interphase_spacerClass ui;
};
