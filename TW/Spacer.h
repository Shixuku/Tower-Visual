#pragma once

#include <QDialog>
#include "ui_Spacer.h"
#pragma execution_character_set("utf-8")

class Spacer : public QDialog
{
	Q_OBJECT

public:
	Spacer(QWidget *parent = nullptr);
	~Spacer();
	void Set_Lists(int N);//跟进悬挂点生产的档数
	void Get_Data();
	std::vector<int>Spacer_Num;
	int choose;//combobox（间隔棒安装方式）
private:
	Ui::SpacerClass ui;
	QStringList headertext;

};
