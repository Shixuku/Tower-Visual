#pragma once

#include <QDialog>
#include "ui_Senior.h"

class Senior : public QDialog
{
	Q_OBJECT

public:
	Senior(QWidget *parent = nullptr);
	~Senior();
	void Initialize();//Ĭ��ֵ
	void Draw_Wire(double* p1, double* p2, double* p3, double* p4, double* p5); //ȡ���߲���(��λ���� Ӧ�� ���߷ֶ� �����)

private:
	Ui::SeniorClass ui;
};
