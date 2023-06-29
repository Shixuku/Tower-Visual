#include "Wind.h"

Wind::Wind(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.btn_ok, &QPushButton::clicked, this, &Wind::accept);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &Wind::reject);
}

Wind::~Wind()
{}
