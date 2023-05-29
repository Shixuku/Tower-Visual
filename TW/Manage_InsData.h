#pragma once

#include <QDialog>
#include "ui_Manage_InsData.h"
#include"InterFace.h"
#include"Tower.h"
#include"Tower_Assembly.h"
class Manage_InsData : public QDialog
{
	Q_OBJECT

public:
	Manage_InsData(InterFace* InterFace, QWidget* parent = nullptr);
	~Manage_InsData();
	void Set_headertext();
	void Modify_Data();//�޸�����
	void Delete_Data();//ɾ������

private:
	Ui::Manage_InsDataClass ui;
	InterFace* m_InterFace = nullptr;
};
