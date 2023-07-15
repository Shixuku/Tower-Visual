#include "TowerCaculate.h"
#include <QFileDialog>
TowerCaculate::TowerCaculate(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	ui.stackedWidget->setCurrentIndex(0);//设置默认page
	connect(ui.radio_static, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(0); });
	connect(ui.radio_dynamic, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(1); });
	connect(ui.radio_dynamic, &QRadioButton::clicked, this, &TowerCaculate::set_dynamic);

	connect(ui.radioButton_galloping, &QRadioButton::clicked, this, &TowerCaculate::set_galloping);
	connect(ui.radioButton_windage, &QRadioButton::clicked, this, &TowerCaculate::set_windage);
	connect(ui.checkBox_eq, &QCheckBox::setChecked, this, &TowerCaculate::set_eq);


	connect(ui.btn_ok, &QPushButton::clicked, this, &TowerCaculate::BtnOK);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &TowerCaculate::reject);
	connect(ui.btn_open, &QPushButton::clicked, this, &TowerCaculate::BtnOpen);
	
	QString beta = 0x03B2;
	QString gama = 0x03B3;
	ui.label_beta->setText(beta);
	ui.label_gama->setText(gama);

	ui.line_beta->setText("0.25");
	ui.line_gama->setText("0.5");
	ui.lineEdit_dynamic_step->setText("0.1");
	ui.line_dynamic_stepnum->setText("100");

	ui.line_static_step->setText("2");
	ui.line_stol->setText("1e-6");
	ui.line_MaxIterations->setText("20");
}

TowerCaculate::~TowerCaculate()
{}

void TowerCaculate::BtnOpen()
{
	QString str = QFileDialog::getOpenFileName(this, "打开", "/", "textfile(*.txt);;All file(*.*)");
	if(str!=nullptr) ui.line_path->setText(str);
}

void TowerCaculate::BtnOK()
{
	static_stepnum = ui.line_static_step->text().toInt();
	dynamic_step = ui.lineEdit_dynamic_step->text().toDouble();
	dynamic_stepnum = ui.line_dynamic_stepnum->text().toInt();;
	int idNode = ui.line_idNode->text().toInt();
	IdNode.push_back(idNode);
	stol= ui.line_stol->text().toDouble();
	MaxIterations=ui.line_MaxIterations->text().toInt();
	m_str = ui.line_path->text();


	this->accept();
}
