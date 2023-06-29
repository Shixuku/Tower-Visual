#pragma once

#include <QDialog>
#include "ui_Manage_Loads.h"
class Manage_Loads : public QDialog
{
	Q_OBJECT

public:
	Manage_Loads(QWidget *parent = nullptr);
	~Manage_Loads();
	void initialize();

private:
	Ui::Manage_LoadsClass ui;

};
