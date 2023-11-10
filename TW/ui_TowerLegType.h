#pragma once

#include <QDialog>
#include "ui_ui_TowerLegType.h"
#include"InterFace.h"
class ui_TowerLegType : public QDialog
{
	Q_OBJECT

public:
	ui_TowerLegType(InterFace* InterFace, QWidget *parent = nullptr);
	~ui_TowerLegType();
	void Initialize();
	void btnContinue();
	int m_TypeSide = 0;//侧面型号
	int m_TypeSeptum = 0;//侧面型号
	QString m_Name = nullptr;
	InterFace* m_InterFace = nullptr;
private:
	Ui::ui_TowerLegTypeClass ui;
	
public slots:
	void Change_Picture_foot();
	void Change_Picture_gemian();
};
