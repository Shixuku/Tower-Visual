#pragma once

#include <QDialog>
#include "ui_TowerCaculate.h"
#include<vector>
class TowerCaculate : public QDialog
{
	Q_OBJECT

public:
	TowerCaculate(QWidget *parent = nullptr);
	~TowerCaculate();
	void BtnOpen();
	void BtnOK();
	int static_stepnum = 0;//静力加载步数
	double dynamic_step = 0;//动力步长
	int dynamic_stepnum = 0;//动力加载步数
	std::vector<int> IdNode;
	double stol = 0.0;
	int MaxIterations = 0;
	QString m_str;

	bool dynamic = false;//动力计算
	bool galloping = false;//舞动
	bool windage = false;//风偏
	bool eq = false;//自平衡

private slots:
	void set_dynamic(bool flag) { dynamic = flag; }
	void set_galloping(bool flag) { galloping = flag; }
	void set_windage(bool flag) { windage = flag; }
	void set_eq(bool flag) { eq = flag; }

private:
	Ui::TowerCaculateClass ui;
};
