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
	void Set_Lists(int N);//�������ҵ������ĵ���
	void Get_Data();
	std::vector<int>Spacer_Num;
	int choose;//combobox���������װ��ʽ��
private:
	Ui::SpacerClass ui;
	QStringList headertext;

};
