#pragma once

#include <QDialog>
#include "ui_AddLoadForce.h"
#include"Tower.h"
class InterFace;
class Tower;
class AddLoadForce : public QDialog
{
	Q_OBJECT

public:
	AddLoadForce(Tower* tower, QWidget* parent = nullptr);
	~AddLoadForce();
	InterFace* m_pInterFace = nullptr;
	Tower* m_tower = new Tower;
	void Get_Nodes();//ѡ��
	void Get_Data();//ȡ������
private:
	Ui::AddLoadForceClass ui;
};
