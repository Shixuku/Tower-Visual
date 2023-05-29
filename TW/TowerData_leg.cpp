#include "TowerData_leg.h"

double TowerData_leg::Get_PracticlH(int i) const
{
	double P_H = m_H1 - m_H[i];//实际高度
	return P_H;
}

double TowerData_leg::Get_PracticlL(int i) const
{
	double P_H = m_H1 - m_H[i];//实际高度
	double P_L = m_L2 + (m_L1 - m_L2) * P_H / m_H1;//实际跟开
	return P_L;
}

