#pragma once

#include <QDialog>
#include "ui_ui_AnalysisStep.h"
class InterFace;
class ui_AnalysisStep : public QDialog
{
	Q_OBJECT

public:
	ui_AnalysisStep(InterFace* m_Interface, QWidget* parent = nullptr);
	~ui_AnalysisStep();
	void Initialization();
	void InitializationDynamic();
	
	void GetData();
	InterFace* m_pInterFace = nullptr;
	
private:
	Ui::ui_AnalysisStepClass ui;
};
