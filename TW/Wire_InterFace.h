#pragma once

#include <QDialog>
#include "ui_Wire_InterFace.h"
#include"Creat_WireData.h"
#include"Draw_Wire.h"
#include"TowerWireGroup.h"

class InterFace;
class Senior;
class Wire_InterFace : public QDialog
{
	Q_OBJECT

public:
	Wire_InterFace(TowerWireGroup* TowerWireGroup,QWidget *parent = nullptr);
	~Wire_InterFace();

	void Insert_Point_Infor(int N);//列表点相关信息(设置列表行数 几个悬挂点设置几行)
	void Insert_Pts_to_vector();//将悬挂点放入容器
	void Initialize();//默认值
	void Insert_Data();//鼠标点击 将相关点信息加入列表（通过鼠标点击后，将点信息显示到列表）
	//xgd_List(表格中的悬挂点) xgd_Real（加了耐张串的实际悬挂点）direction=0，1，2,3（直线 左耐张 右耐张,两边都耐张）
	void Line_Segment(vector<Node>xgd_List, vector<Node>xgd_Real, int direction);//(列表中的悬挂点，真实的悬挂点，选择哪个方向耐张(这个函数生成导线))
	void Show_Senior();//点击高级弹出界面
	void Creat_Distance(vector<Node>xgd_real, vector<int>num, vector<int>& l_Spacer, vector<int>& d_Spacer);//创建间隔棒离端点的距离
	void Creat_Spacer(vector<int>L, vector<int>num,  vector<Node>xgd_Real);//画出间隔棒
	void OK();//点击确定之后的函数
	int fenlie;//分裂数
	double Rou;//单位质量
	double yingli;//应力
	double area;//截面积
	int N; //导线分段数
	double rou;//bizai
	double Sag;//弧垂
	int nz_len;//耐张串的长度

	vector<int>L_Spacer;//每个间隔棒离前面这个悬挂点的距离
	vector<int>D_Spacer;//间隔棒在第几档导线中
	vector<int>Extreme_Point;//判断端点的类型
	vector<Node>xgd; //列表悬挂点
	vector<Node>xgd_real; //实际悬挂点
	vector<Node> m_Nodes;//所有点存放的容器
	vector<Node>Node_Temp;//未分裂的基础导线节点 用于后续找间隔棒
	vector<Node>Node_Base;//未分裂的列表悬挂点 用于找真实悬挂点
	vector<Element_Wire> m_Elements;//所有单元合集
	vector<Element_Wire> m_Elements_Wire;//导线单元合集
	vector<Element_Wire> m_Elements_Spacer;//间隔棒单元合集
	vector<Element_Wire> m_Elements_Strain;//耐张串单元合集
	
	InterFace* m_pInterFace = nullptr;
	TowerWireGroup* towerWireGroup = nullptr;
	Senior* sen = nullptr;//高级的指针
	Creat_WireData* cd; 

private:
	Ui::Wire_InterFaceClass ui;
	QStringList headertext;
};
