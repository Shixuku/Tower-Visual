#pragma once

#include <QDialog>
#include "ui_Senior.h"

class Senior : public QDialog
{
	Q_OBJECT

public:
	Senior(QWidget *parent = nullptr);
	~Senior();
	void Initialize();//默认值
	void Draw_Wire(double* p1, double* p2, double* p3, double* p4, double* p5); //取导线参数(单位质量 应力 导线分段 截面积)

private:
	Ui::SeniorClass ui;
};
