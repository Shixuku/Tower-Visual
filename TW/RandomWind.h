#pragma once

#include <QDialog>
#include "ui_RandomWind.h"

class RandomWind : public QDialog
{
	Q_OBJECT

public:
	RandomWind(QWidget *parent = nullptr);
	~RandomWind();

private:
	Ui::RandomWindClass ui;
};
