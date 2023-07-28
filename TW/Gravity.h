#pragma once

#include <QDialog>
#include "ui_Gravity.h"

class Creat_Loads;
class Instance;
class Gravity : public QDialog
{
	Q_OBJECT

public:
	Gravity(Creat_Loads* creat_loads, QWidget *parent = nullptr);
	~Gravity();
	Creat_Loads* m_pCreat_loads = nullptr;
	Instance* m_pInstance = nullptr;
	void initialization();
	void Get_ui_Data();

private:
	Ui::GravityClass ui;
};
