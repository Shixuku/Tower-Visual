#pragma once
#include"TowerData_Layer.h"
#include<iostream>
using namespace std;
#include<vector>
#include"Part_Base.h"
class TowerData_Body:public Part_Base   //塔身段的参数
{
public:

	//塔身的基本几何参数
	int m_Tier;  //总层数

	double m_L0=0 ; double m_Z0=0;//底部宽度、底部z坐标
	double m_Ln=0; //double m_Zn=0;//顶部宽度、顶部z坐标(顶部z坐标可以不要)

	vector<TowerData_Layer> m_layer;
	double Get_SumH();
	double Get_LayerH(int i);//得到i层高度
	double Get_LayerL(int i);//得到i层上宽度

};

