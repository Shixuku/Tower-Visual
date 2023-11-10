#pragma once

#include <QDialog>
#include "ui_ui_TowerLegParameter.h"
#include"ui_TowerLegType.h"
class TowerPart_leg;
class ui_TowerLegParameter : public QDialog
{
	Q_OBJECT

public:
	ui_TowerLegParameter(ui_TowerLegType* ui_TowerLegType, QWidget* parent = nullptr);
	~ui_TowerLegParameter();
	ui_TowerLegType* m_pTowerLegType = nullptr;
	void Get_Data();//ȡ���������
private:
	Ui::ui_TowerLegParameterClass ui;
	void btn_help();//������ť
	void Set_tableWideget();//���Ȳ������
	void Initialize();
};
