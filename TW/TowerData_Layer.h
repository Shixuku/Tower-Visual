#pragma once

class TowerData_Layer // 一层塔身所需参数
{
public:
	//塔身各部分型号
	int m_TypeFront;  //正面型号
	int m_TypeSide;  //侧面型号
	int m_TypeSeptumUp;//上部隔面型号
	int m_TypeSeptumMiddle;//中部隔面型号
	double m_h;//每一层的高

};

