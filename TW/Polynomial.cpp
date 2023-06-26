#include "Polynomial.h"

Polynomial::Polynomial(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.btn_ok, &QPushButton::clicked, this, &Polynomial::accept);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &Polynomial::reject);
}

Polynomial::~Polynomial()
{}
