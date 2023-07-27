#pragma once

#include <QDialog>
#include"WireData.h"
#include"Node.h"
#include"ui_Wire_InterFace.h"
#pragma execution_character_set("utf-8")

class InterFace;
class Senior;
class TowerWireGroup;
class Wire_InterFace : public QDialog
{
	Q_OBJECT

public:
	Wire_InterFace(TowerWireGroup* TowerWireGroup,QWidget *parent = nullptr);
	~Wire_InterFace();
	InterFace* m_pInterFace;
	TowerWireGroup* towerWireGroup = nullptr;
	QStringList headertext;
	void SetTableWideget(int row);//生产点列表信息
	void ui_Senior();
	void Get_Data(WireData& wd);
	void SaveSusPoint();//保存列表中的悬挂点信息
	int a = 0;//生成的点的数量
	int N;//分段数
	double unitMass;//
	double area;//面积
	double stress;//最低点应力
	double strainLength;//耐张串长度
	double sag;//弧垂
	double wireLogoQty;
	vector<int> chooseType1 ;//端点一类型
	vector<int> chooseType2 ;//端点二类型
	std::vector<int>SpacerNum;//存储间隔棒个数
	std::vector<Node>WireSusList;//表格中给的悬挂点
	int  ChooseWay = 0;//间隔棒安装方式 0-等间距 1-不等间距
	Senior* sen = nullptr;
private:
	Ui::Wire_InterFaceClass ui;
};
