#pragma once

#include <QDialog>
#include"InterFace.h"
#include "ui_SetAllSection.h"
#include"Tower.h"

class InterFace;
class SetAllSection : public QDialog
{
	Q_OBJECT

public:
	SetAllSection(InterFace* InterFace, QWidget* parent = nullptr);
	~SetAllSection();
	void Create_combobox();
	void ShowComboVTK();
	void Assgin(Tower* tower);
	InterFace* m_pInterFace = nullptr;
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
