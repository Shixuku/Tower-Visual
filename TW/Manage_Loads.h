#pragma once

#include <QDialog>
#include "ui_Manage_Loads.h"
#include"InterFace.h"
class Creat_Loads;
class Manage_Loads : public QDialog
{
	Q_OBJECT

public:
	Manage_Loads(InterFace* InterFace, QWidget *parent = nullptr);
	~Manage_Loads();
	void initialize();
	void Modify_Loads();
	void Delete_Loads();
private:
	Ui::Manage_LoadsClass ui;
	InterFace* m_pInterFace = nullptr;
	Creat_Loads* m_pCreat_Loads = nullptr;
};
