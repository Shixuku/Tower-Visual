#pragma once

#include <QDialog>
#include"InterFace.h"
#include "ui_SetAllSection.h"
#include"Tower.h"
#include"Part_Base.h"
#include<QList>
#include<QSet>
class InterFace;
class SetAllSection : public QDialog
{
	Q_OBJECT

public:
	SetAllSection(InterFace* InterFace, Part_Base*Part, QWidget* parent = nullptr);
	~SetAllSection();
	void Assgin(Part_Base* part);
	InterFace* m_pInterFace = nullptr;
	Part_Base* m_Part = nullptr;
	void Get_Data();
	void table_Section_Lists();
	int T_legs = 0;
	int T_bodys = 0;//塔身数量
	int T_heads = 0;//塔头数量
	int TowerSize = 0;
	int id_Part = 0;
private:
	Ui::SetAllSectionClass ui;
	QStringList headertext;
};
