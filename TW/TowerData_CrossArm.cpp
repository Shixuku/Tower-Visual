#include"TowerData_CrossArm.h"

double TowerData_CrossArm::Get_SumL()
{
	double sumL = 0;
	for (int i = 0; i < m_count; i++)
	{
		sumL += m_layerArm[i].l;//总段长和
	}
	return sumL;
}

double TowerData_CrossArm::Get_SumLi(int i)
{
	double l = 0;
	for (int j = 0; j < i; j++)
	{
		l += m_layerArm[j].l;//i段前段长和
	}
	return l;
}

double TowerData_CrossArm::Get_layerLi(int type, int i)
{
	//type 1--支架水平 2--支架下 3--横担水平 4--横担上
	double sumLi = Get_SumLi(i);
	double Li = 0;//初始定义长
	switch (type)
	{
	case 1:
		Li = m_bodyUpL / 2 + sumLi;
		break;
	case 2:
		if (i == 0)
		{
			Li = m_bodyButtomL / 2 + sumLi;
		}
		else 
		{
			Li = m_bodyUpL / 2 + sumLi;
		}
		
		break;
	case 3:
		if (i == 0)
		{
			Li = m_bodyButtomL / 2 + sumLi;
		}
		else
		{
			Li = m_bodyUpL / 2 + sumLi;
		}
		break;
	case 4:
		Li = m_bodyUpL / 2 + sumLi;
		break;
	default:break;
	}
	return Li;
}

double TowerData_CrossArm::Get_layerWi(int type, int i)
{
	//type 1--支架水平 2--支架下 3--横担水平 4--横担上
	double suml = Get_SumL();
	double sumLi = Get_SumLi(i);
	double n = (m_bodyButtomL - m_bodyUpL) / 2;
	double Wi = 0;//初始定义宽
	switch (type)
	{
	case 1:
		Wi = m_c1W + (suml - sumLi) * (m_bodyUpL - m_c1W) / suml;
		break;
	case 2:
		if (i == 0) Wi = m_bodyButtomL;
		else
		{
			Wi = m_c1W + (suml - sumLi) * (m_bodyButtomL - m_c1W) / (suml - n);
		}
		break;
	case 3:
		Wi = m_c2Wb + (suml - sumLi) * (m_bodyButtomL - m_c2Wb) / suml;
		break;
	case 4:
		if (i == 0) Wi = m_bodyUpL;
		else
		{
			Wi = m_c2Wup + (suml - sumLi) * (m_bodyUpL - m_c2Wup) / (suml + n);
		}
		break;
	default:break;
	}
	return Wi;
}

double TowerData_CrossArm::Get_layerHi(int type, int i)
{
	//type 1--支架水平 2--支架下 3--横担水平 4--横担上
	double suml = Get_SumL();
	double sumLi = Get_SumLi(i);
	double n = (m_bodyButtomL - m_bodyUpL) / 2;
	double Hi = 0;
	switch (type)
	{
	case 1:
		Hi = m_bodyUpH;
		break;
	case 2:
		if (i == 0)
		{
			Hi = m_bodyButtomH;
		}
		else
		{
			Hi = (sumLi - n) * (m_bodyUpH - m_bodyButtomH) / (suml - n) + m_bodyButtomH;
		}
		
		break;
	case 3:
		Hi = m_bodyButtomH;
		break;
	case 4:
		if (i == 0)
		{
			Hi = m_bodyUpH;
		}
		else
		{
			Hi = m_bodyUpH - ((m_bodyUpH - m_bodyButtomH) - m_c2H) * (sumLi + n) / (suml + n);
		}
		break;
	default:break;
	}
	return Hi;
}


//double TowerData_CrossArm::Get_m_bodyButtomL()
//{
//	m_bodyButtomL = m_databodyTier->Get_LayerL(m_bodyTier - 1);
//	return m_bodyButtomL;
//}
//double TowerData_CrossArm::Get_m_bodyUpL()
//{
//	m_bodyUpL= m_databodyTier->Get_LayerL(m_bodyTier);
//	return m_bodyUpL;
//}
//double TowerData_CrossArm::Get_m_bodyButtomH()
//{
//	m_bodyButtomH = m_databodyTier->Get_LayerH(m_bodyTier - 1);
//	return m_bodyButtomH;
//}
//double TowerData_CrossArm::Get_m_bodyUpH()
//{
//	m_bodyUpH = m_databodyTier->Get_LayerH(m_bodyTier);
//	return m_bodyUpH;
//}


