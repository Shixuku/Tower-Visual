#pragma once

#include <QDialog>
#include "ui_Senior.h"
#include <QObject>


class Wire_InterFace;
class Spacer;
class Senior : public QDialog
{
	Q_OBJECT

public:
	Senior(Wire_InterFace* Wire_InterFace,QWidget *parent = nullptr);
	~Senior();
	void Initialize();
	int N;
	double unitMass;
	double area;
	double stress;
	double strainL;
	double sag;
	int ChooseWay = 0;//间隔棒安装方式 0-等间距 1-不等间距
	std::vector<int>SpacerNum;//存储间隔棒个数
	Wire_InterFace* wireInterFace = nullptr;
	Spacer* sp = nullptr;
	void CreatData(); // 将CreatData()函数声明为槽函数
public slots:
	void ui_Spacer();

private:
	Ui::SeniorClass ui;
};
