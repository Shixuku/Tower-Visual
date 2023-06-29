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
	double x ;//杆塔位移的x分量
	double y ;//杆塔位移的y分量
	double z ;//杆塔位移的z分量
	double angle ;//杆塔的转角
public slots:
	void onComboBoxIndexChanged(int index);
	
private:
	Ui::TowerWireGroupClass ui;
};
