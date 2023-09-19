#include "ui_AnalysisStep.h"
#include"InterFace.h"
#include"ParameterAnalysisStep.h"
#pragma execution_character_set("utf-8")
ui_AnalysisStep::ui_AnalysisStep(InterFace* m_Interface, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_pInterFace = m_Interface;
	int size = m_pInterFace->ME_AnalysisSteps.size();
	Initialization();
	ui.line_name->setText("分析步 " + QString::number(size + 1));
	ui.line_zxrxz->setText("1e-5");
	ui.line_zdddcs->setText("16");

	connect(ui.radio_static, &QRadioButton::clicked, this, &ui_AnalysisStep::Initialization);
	connect(ui.radio_dynamic, &QRadioButton::clicked, this, &ui_AnalysisStep::InitializationDynamic);
	connect(ui.btn_ok, &QPushButton::clicked, this, &ui_AnalysisStep::GetData);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &ui_AnalysisStep::reject);

}

ui_AnalysisStep::~ui_AnalysisStep()
{}

void ui_AnalysisStep::Initialization()
{
	ui.line_sjd->setText("1");
	ui.line_sjzl->setText("1");
}

void ui_AnalysisStep::InitializationDynamic()
{
	ui.line_sjd->setText("10");
	ui.line_sjzl->setText("0.01");
}

void ui_AnalysisStep::GetData()
{
	QString name = ui.line_name->text();
	int id = m_pInterFace->ME_AnalysisSteps.size() + 1;

	QString Type = "Static";//默认静力
	if(ui.radio_dynamic->isChecked()) Type = "Dynamic";

	double TimeSlot = ui.line_sjd->text().toDouble();
	double TimeIncrement=ui.line_sjzl->text().toDouble();
	double MinAllowableValue=ui.line_zxrxz->text().toDouble();
	int MaxEpoch = ui.line_zdddcs->text().toInt();

	ParameterAnalysisStep* parameterAnalysisStep = new ParameterAnalysisStep(name, id, Type, TimeSlot, TimeIncrement, MinAllowableValue, MaxEpoch);
	m_pInterFace->ME_AnalysisSteps.Add_Entity(parameterAnalysisStep);

	this->accept();
}
