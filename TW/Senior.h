#pragma once

#include <QDialog>
#include "ui_Senior.h"
class Spacer;

class Senior : public QDialog
{
	Q_OBJECT

public:
	Senior(QWidget *parent = nullptr);
	~Senior();
	void Initialize();//默认值
	void Draw_Wire(double* p1, double* p2, double* p3, double* p4, double* p5, double* p6); //取导线参数(单位质量 应力 导线分段 截面积)
	void Get_Data();
	int N_Pts;
	int choose;//间隔棒安装方式（指派or公式）
	int choose_way;//等间距or不等间距
	int Type_find = 0; //找型方式
	std::vector<int>Num_Spacer;//存储间隔棒个数
	Spacer* sp = nullptr;
public slots:
	void Show_Spacer();
private:
	Ui::SeniorClass ui;
};
