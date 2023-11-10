#pragma once

#include <QDialog>
#include "ui_ui_TowerBodyType.h"

class ui_TowerBodyType : public QDialog
{
	Q_OBJECT

public:
	ui_TowerBodyType(QWidget* parent = nullptr);
	~ui_TowerBodyType();

private:
	Ui::ui_TowerBodyTypeClass ui;
	void Set_No1_picture();//设置首张类型图显示
public slots:
	void Change_Picture_body();
	void Change_Picture_gem();
};
