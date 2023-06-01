#pragma once

#include <QDialog>
#include "ui_AddLoadForce.h"
#include"Tower.h"

class AddLoadForce : public QDialog
{
	Q_OBJECT

public:
	AddLoadForce(Tower*tower,QWidget *parent = nullptr);
	~AddLoadForce();

private:
	Ui::AddLoadForceClass ui;
};
