#pragma once

#include <QDialog>
#include"TowerWireGroup.h"
#include "ui_TowerWireGroupAssembly.h"
class InterFace;
class Tower;
class TowerWireGroupAssembly : public QDialog
{
	Q_OBJECT

public:
	TowerWireGroupAssembly(InterFace*InterFace, TowerWireGroup* TowerWireGroup,QWidget *parent = nullptr);
	~TowerWireGroupAssembly();
	InterFace* m_pInterFace = nullptr;
	TowerWireGroup* towerWireGroup = nullptr;
	Tower* mTower = nullptr;
	void CreateComBox();
	void TableTowerLists();
	void GetTowerSetData();
	QStringList headertext;
	QString selectedItemName;
	double x ;//����λ�Ƶ�x����
	double y ;//����λ�Ƶ�y����
	double z ;//����λ�Ƶ�z����
	double angle ;//������ת��
public slots:
	void onComboBoxIndexChanged(int index);
	
private:
	Ui::TowerWireGroupClass ui;
};
