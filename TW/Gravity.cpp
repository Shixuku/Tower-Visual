#include "Gravity.h"

Gravity::Gravity(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.btn_ok, &QPushButton::clicked, this, &Gravity::accept);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &Gravity::reject);
}

Gravity::~Gravity()
{}
