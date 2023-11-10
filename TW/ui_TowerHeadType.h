#pragma once

#include <QDialog>
#include "ui_ui_TowerHeadType.h"

class ui_TowerHeadType : public QDialog
{
	Q_OBJECT

public:
	ui_TowerHeadType(QWidget *parent = nullptr);
	~ui_TowerHeadType();

private:
	void Initialize();
	Ui::ui_TowerHeadTypeClass ui;
public slots:
	void Change_Picture_BU();
	void Change_Picture_S();
	void Change_Picture_gemian();
};
