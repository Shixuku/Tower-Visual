#pragma once

#include <QDialog>
#include "ui_Creat_Loads.h"
#include<QString>
//#include<vector>
class InterFace;
class ConcentrateForce;
class Creat_Loads : public QDialog
{
	Q_OBJECT

public:
	Creat_Loads(InterFace* pInterFace, QWidget *parent = nullptr);
	~Creat_Loads();
	void btn_continue_clicked();
	InterFace* m_InterFace = nullptr;
	QString m_str;//ȡ����
	QString m_itemText;//ȡ�غ�����
private:
	Ui::Creat_LoadsClass ui;

};
