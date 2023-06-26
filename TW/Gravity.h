#pragma once

#include <QDialog>
#include "ui_Gravity.h"

class Gravity : public QDialog
{
	Q_OBJECT

public:
	Gravity(QWidget *parent = nullptr);
	~Gravity();

private:
	Ui::GravityClass ui;
};
