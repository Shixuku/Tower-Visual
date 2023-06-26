#pragma once

#include <QDialog>
#include "ui_Polynomial.h"

class Polynomial : public QDialog
{
	Q_OBJECT

public:
	Polynomial(QWidget *parent = nullptr);
	~Polynomial();

private:
	Ui::PolynomialClass ui;
};
