#pragma once
#include<iostream>
using namespace std;
#include<vector>
#include"TowerData_LayerArm.h"
#include"Part_Base.h"
#include"TowerData_Body.h"
class TowerData_CrossArm:public Part_Base
{
public:
	vector<TowerData_LayerArm> m_layerArm;

	TowerData_Body* m_databodyTier = nullptr;//取塔身层数

	int m_Type;//区分是横担还是支架  1--表示支架 3--表示横担

	double m_bodyButtomL;//塔身层底部宽度
	double m_bodyUpL;//塔身层顶部宽度
	double m_bodyButtomH;//塔身层底部高度
	double m_bodyUpH;//塔身层底部高度

	double m_c1W;//支架--悬臂端宽度

	double m_c2Wb;//横担--悬臂端底部宽度
	double m_c2Wup;//横担--悬臂端顶部宽度
	double m_c2H;//横担--悬臂端高度

	int m_count;//段长

	int m_position;//位置 0--右边 1--左边 2--两侧

	//得到各段长宽高
	double Get_SumL();//x方向的段长和---x
	double Get_SumLi(int i);//i段前段长和---x
	double Get_layerLi(int type, int i);//type 1--支架水平 2--支架下 3--横担水平 4--横担上
	double Get_layerWi(int type,int i);//type 1--支架水平 2--支架下 3--横担水平 4--横担上
	double Get_layerHi(int type,int i);//type 1--支架水平 2--支架下 3--横担水平 4--横担上

};

