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
	int static_stepnum = 0;//�������ز���
	double dynamic_step = 0;//��������
	int dynamic_stepnum = 0;//�������ز���
	std::vector<int> IdNode;
	double stol = 0.0;
	int MaxIterations = 0;
	QString m_str;

	bool dynamic = false;//��������
	bool galloping = false;//�趯
	bool windage = false;//��ƫ
	bool eq = false;//��ƽ��

private slots:
	void set_dynamic(bool flag) { dynamic = flag; }
	void set_galloping(bool flag) { galloping = flag; }
	void set_windage(bool flag) { windage = flag; }
	void set_eq(bool flag) { eq = flag; }

private:
	Ui::TowerCaculateClass ui;
};
