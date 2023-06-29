#pragma once

#include <QDialog>
#include "ui_Wind.h"

class Wind : public QDialog
{
	Q_OBJECT

public:
	Wind(QWidget *parent = nullptr);
	~Wind();

private:
	Ui::WindClass ui;
};
