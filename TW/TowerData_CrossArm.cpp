#include"TowerData_CrossArm.h"
#include"TowerPart_Insulator.h"
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
void TowerData_CrossArm::addInsulator(TowerPart_Insulator* insulator)
{
	addInsulatorNode(insulator);
	addInsulatorElement(insulator);
	addNodevector(insulator);
	insulator->part_to_tower.clear();
}

void TowerData_CrossArm::addInsulatorNode(TowerPart_Insulator* insulator)
{
	size_t partNode = insulator->m_Nodes.size();//part中的节点
	cout << typeid(*insulator).name() << ": " << partNode << "\n";

	//add
	for (size_t i = 0; i < partNode; ++i)
	{

		int a = this->m_Nodes.size();
		int Judg = 0;
		if (a == 0)
		{
			this->m_Nodes.push_back(insulator->m_Nodes[i]);
			this->m_Nodes[0].m_idNode = 1;
			insulator->part_to_tower.push_back(1);
		}
		else
		{
			for (int j = 0; j < a; j++)
			{
				if (abs(this->m_Nodes[j].x - insulator->m_Nodes[i].x) < 1e-2 && abs(this->m_Nodes[j].y - insulator->m_Nodes[i].y) < 1e-2 && abs(this->m_Nodes[j].z - insulator->m_Nodes[i].z) < 1e-2)
				{
					insulator->part_to_tower.push_back(this->m_Nodes[j].m_idNode);
					j = a;

				}
				else
				{
					Judg++;

				}

			}
			if (Judg == a)
			{
				this->m_Nodes.push_back(insulator->m_Nodes[i]);
				size_t total = this->m_Nodes.size() - 1;//
				this->m_Nodes[total].m_idNode = total + 1;
				//part-tower 节点对应关系
				insulator->part_to_tower.push_back(total + 1);
			}

		}
	}
}

void TowerData_CrossArm::addInsulatorElement(TowerPart_Insulator* insulator)
{
	size_t tpart = insulator->m_Elements_Trusses.size();

	for (size_t i = 0; i < tpart; ++i)//Truss
	{
		Element_Truss* pE = &insulator->m_Elements_Trusses[i];
		this->m_Elements.push_back(insulator->m_Elements_Trusses[i]);
		size_t total = this->m_Elements.size() - 1;
		this->m_Elements[total].m_idElement = total + 1;//放入实例的总单元
		this->m_Elements[total].m_idNode[0] = insulator->Find_tower_idNode(pE->m_idNode[0]);
		this->m_Elements[total].m_idNode[1] = insulator->Find_tower_idNode(pE->m_idNode[1]);

		this->m_Elements_Trusses.push_back(insulator->m_Elements_Trusses[i]);
		size_t totalT = this->m_Elements_Trusses.size() - 1;
		this->m_Elements_Trusses[totalT].m_idElement = total + 1;//放入实例的杆单元
		this->m_Elements_Trusses[totalT].m_idNode[0] = insulator->Find_tower_idNode(pE->m_idNode[0]);
		this->m_Elements_Trusses[totalT].m_idNode[1] = insulator->Find_tower_idNode(pE->m_idNode[1]);
		this->m_Elements_Trusses[totalT].ClassSectionID = pE->ClassSectionID;
		this->m_Elements_Trusses[totalT].MaterialID = pE->MaterialID;
	}
}

void TowerData_CrossArm::addNodevector(TowerPart_Insulator* insulator)
{
	size_t SusNode = insulator->SuspensionNode.size();
	for (int i = 0; i < SusNode; i++)
	{
		this->SuspensionNode.push_back(insulator->SuspensionNode[i]);
		this->WireLoge.push_back(insulator->WireLoge[i]);
		size_t totalT = this->SuspensionNode.size() - 1;
		SuspensionNode[totalT] = insulator->Find_tower_idNode(insulator->SuspensionNode[i]);
	}

	size_t realSusNode = insulator->realSuspoint.size();
	for (int i = 0; i < realSusNode; i++)
	{
		this->realSuspoint.push_back(insulator->realSuspoint[i]);
		size_t totalT = this->realSuspoint.size() - 1;
		realSuspoint[totalT] = insulator->Find_tower_idNode(insulator->realSuspoint[i]);
	}
}
