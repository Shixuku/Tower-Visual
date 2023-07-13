#pragma once

#include <QDialog>
#include "ui_Senior.h"
#include <QObject>


class Wire_InterFace;
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
	Wire_InterFace* wireInterFace = nullptr;
	void CreatData(); // 将CreatData()函数声明为槽函数
	
signals:
	void valuesReturned(int N, double unitMass, double area, double stress, double strainL, double sag);

private:
	Ui::SeniorClass ui;
};
