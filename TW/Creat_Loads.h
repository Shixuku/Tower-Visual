#pragma once

#include <QDialog>
#include "ui_Creat_Loads.h"
#include<QString>
#include"Tower.h"
class InterFace;
class Tower;
class Creat_Loads : public QDialog
{
	Q_OBJECT

public:
	Creat_Loads(Tower* tower, QWidget* parent = nullptr);
	~Creat_Loads();
	void btn_continue_clicked();
	InterFace* m_pInterFace = nullptr;
	Tower* m_tower = new Tower;
	QString m_str;//È¡Ãû³Æ
private:
	Ui::Creat_LoadsClass ui;

};
