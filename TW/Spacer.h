#pragma once

#include <QDialog>
#include "ui_Spacer.h"
#pragma execution_character_set("utf-8")
class Senior;
class Spacer : public QDialog
{
	Q_OBJECT

public:
	Spacer(Senior*sen,QWidget *parent = nullptr);
	~Spacer();
	void Set_Lists(int N); //跟进悬挂点生产的档数
	void GetData();
	
	Senior* m_sen = nullptr;
private:
	Ui::SpacerClass ui;
};
