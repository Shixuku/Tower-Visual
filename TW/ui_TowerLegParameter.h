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
	void Get_Data();//取界面的数据
private:
	Ui::ui_TowerLegParameterClass ui;
	void btn_help();//帮助按钮
	void Set_tableWideget();//塔腿参数表格
	void Initialize();
};
