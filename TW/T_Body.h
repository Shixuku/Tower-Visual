#pragma once
#include <QDialog>
#include "ui_T_Body.h"
#include"TowerData_Layer.h"
#include"TowerData_Body.h"
#include"InterFace.h"
class TowerPart_body;

class T_Body : public QDialog
{
	Q_OBJECT

public:
	T_Body(InterFace* InterFace, QWidget *parent = nullptr);
	~T_Body();
	void Set_headertext();//���ñ�ͷ
	void Set_No1_picture();//������������ͼ��ʾ

	void Set_body1();//���������1
	void Set_body2();//���������1

	void Set_table(int tier);//���ñ��
	void Get_Data(TowerPart_body* body);
	QString Get_line_Zn(int tier);
private:
	Ui::T_BodyClass ui;
	InterFace* m_InterFace = nullptr;
public slots:
	void Change_Picture_body();
	void Change_Picture_gem();
	
};
