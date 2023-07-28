#pragma once

#include <QDialog>
#include "ui_IceLoad.h"
class Creat_Loads;
class Instance;
class IceLoad : public QDialog
{
	Q_OBJECT

public:
	IceLoad(Creat_Loads* creat_loads, QWidget *parent = nullptr);
	~IceLoad();
	Creat_Loads* m_pCreat_loads = nullptr;
	Instance* m_pInstance = nullptr;
	//QComboBox* AnalysisStep1 = nullptr;
	//QComboBox* AnalysisStep2 = nullptr;
	int step1;
	int step2;
	void Initialization();
	void GetData();
private:
	Ui::IceLoadClass ui;
};
