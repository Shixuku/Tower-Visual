#pragma once

#include <QDialog>
#include "ui_CreateSingleTower.h"
class Tower;
class InterFace;
class CreateSingleTower : public QDialog
{
	Q_OBJECT

public:
	
	CreateSingleTower(InterFace*m_pinterface,QWidget *parent = nullptr);
	~CreateSingleTower();
	void Initialize();
	void BtnOK(Tower* tower);
	InterFace* m_pInterface = nullptr;
	void CreateTowerLeg();
private:
	Ui::CreateSingleTowerClass ui;
public slots:
	void ChangeTowerType();//改变塔的类型
	void ChangeTowerHugao();//改变塔的呼高
};
