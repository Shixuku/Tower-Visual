#pragma once

#include <QDialog>
#include "ui_T_Foot.h"
#include "InterFace.h"
class TowerPart_leg;
class T_Foot : public QDialog
{
	Q_OBJECT

public:
	T_Foot(InterFace* InterFace, QWidget *parent = nullptr);
	~T_Foot();
	void Set_tableWideget();//���Ȳ������
	
	void btn_help();//������ť

	void Get_Data(TowerPart_leg& leg);//ȡ���������
	void Initialize();

private:
	Ui::T_FootClass ui;
	InterFace* m_InterFace = nullptr;
public slots:
	void Change_Picture_foot();
	void Change_Picture_gemian();

};
