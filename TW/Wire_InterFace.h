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
	void SetTableWideget(int row);//生产点列表信息
	void Initialize();//默认值
	void ui_Senior();
	void Get_Data(WireData& wd);
	void SaveSusPoint();//保存列表中的悬挂点信息

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
