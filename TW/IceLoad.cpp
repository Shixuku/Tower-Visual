#include "IceLoad.h"

IceLoad::IceLoad(QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);

	//缺省设置诱发脱冰参数不可编辑
	CheckBoxState(true);
	connect(ui.checkBox, &QCheckBox::stateChanged, this, [&](int state)
		{
			if (state == Qt::Checked)	CheckBoxState(false);
			else if (state == Qt::Unchecked)	CheckBoxState(true);
		});
	ui.radioBtn_directdeice->setChecked(true);
	connect(ui.radioBtn_directdeice, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(0); });
	connect(ui.radioBtn_choosedeice, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(1); });
	connect(ui.btn_ok, &QPushButton::clicked, this, &IceLoad::accept);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &IceLoad::reject);
}

IceLoad::~IceLoad()
{}

void IceLoad::CheckBoxState(bool state)
{
	ui.line_gx->setReadOnly(state);
	ui.line_gy->setReadOnly(state);
	ui.line_gz->setReadOnly(state);
}
