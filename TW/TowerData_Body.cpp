#include "TowerData_Body.h"

double TowerData_Body::Get_SumH()
{
	double sumH = m_Z0;
	for (int i = 0; i < m_Tier; i++)
	{
		sumH += m_layer[i].m_h;
	}
	return sumH;
}

double TowerData_Body::Get_LayerH(int i)
{
	double h = m_Z0;
	for (int j = 0; j < i; j++)
	{
		h += m_layer[j].m_h;//h是前几层的和
	}
	return h;
}

double TowerData_Body::Get_LayerL(int i)//num--表示层数
{
	double h = Get_LayerH(i);
	double sumh = Get_SumH();
	double H = sumh - m_Z0;
	double L = m_Ln + (m_L0 - m_Ln) * (sumh - h) / H;
	return L;//得到i层宽度L
}




