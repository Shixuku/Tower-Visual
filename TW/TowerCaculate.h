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
	int step = 0;
	std::vector<int> IdNode;
	double stol = 0.0;
	int MaxIterations = 0;
	QString m_str;
private:
	Ui::TowerCaculateClass ui;
};
