#pragma once

#include <QDialog>
#include "ui_Creat_Loads.h"
#include<QString>
#include"Tower.h"
class InterFace;
class Instance;
class Creat_Loads : public QDialog
{
	Q_OBJECT

public:
	Creat_Loads(Instance* instance, QWidget* parent = nullptr);
	~Creat_Loads();
	void btn_continue_clicked();
	int index;
	InterFace* m_pInterFace = nullptr;
	Instance* m_instance = new Tower;
	QString m_str;//È¡Ãû³Æ
private:
	Ui::Creat_LoadsClass ui;

};
