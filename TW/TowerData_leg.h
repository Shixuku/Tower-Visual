#pragma once
#include"Part_Base.h"
class TowerData_leg :public Part_Base
{
public:
	//塔腿的基本几何参数

	double m_L1;  //铁塔跟开
	double m_L2;  //塔腿上宽
	double m_H1;  //基面标高
	double m_H[4];  //四个减腿高度
	int m_TypeSide;  //侧面型号
	int m_TypeSeptum;//隔面型号

	double Get_PracticlH(int i)const;//得到塔腿下部实际的H1
	double Get_PracticlL(int i)const;//得到塔腿下部实际的L1

};

