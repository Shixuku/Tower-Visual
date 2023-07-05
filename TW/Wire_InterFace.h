#pragma once

#include <QDialog>
#include "InterFace.h"
#include"WireData.h"
#include"Node.h"
#include"ui_Wire_InterFace.h"
#pragma execution_character_set("utf-8")


class Senior;
class TowerWireGroup;
class Wire_InterFace : public QDialog
{
	Q_OBJECT

public:
	Wire_InterFace(TowerWireGroup* TowerWireGroup,QWidget *parent = nullptr);
	~Wire_InterFace();
	TowerWireGroup* towerWireGroup = nullptr;
	QStringList headertext;
	void SetTableWideget(int row);//�������б���Ϣ
	void Initialize();//Ĭ��ֵ
	void ui_Senior();
	void Get_Data(WireData& wd);
	void SaveSusPoint();//�����б��е����ҵ���Ϣ

	int N;
	double unitMass;
	double area;
	double stress;
	double strainL;
	double sag;

	std::vector<Node>WireSus;
	Senior* sen = nullptr;
private:
	Ui::Wire_InterFaceClass ui;
};
