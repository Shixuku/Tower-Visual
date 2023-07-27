#include "WireData.h"

int WireData::Creat_Node(double x, double y, double z, double f)
{

	int SIZE = realSuspoint.size();
	int judg = 0;
	for (int i = 0; i < SIZE; ++i)
	{
		if (((abs(NodeData[realSuspoint[i]].x - x) < 1e-8)) &&
			((abs(NodeData[realSuspoint[i]].y - y)) < 1e-8) &&
			((abs(NodeData[realSuspoint[i]].z - z)) < 1e-8))
		{
			m_Nodes[realSuspoint[i] - 1].F = f;
			return NodeData[realSuspoint[i]].m_idNode; //重节点
		}
		else
		{
			judg++;

		}

	}

	if (judg == SIZE)
	{
		int id = m_Nodes.size() + 1;
		m_Nodes.push_back(Node(id, x, y, z, f));
		NodeData[id] = m_Nodes[id - 1];
		return m_Nodes[id - 1].m_idNode;
	}
	return 0;
}

void WireData::CreatRealNode()
{
	//确定向量（you）
	double x = WireListSus[1].x - WireListSus[0].x;
	double y = WireListSus[1].y - WireListSus[0].y;
	angle = ((atan2(x, y) * 180) / 3.1415926);
	angle = vtkMath::RadiansFromDegrees(angle);
	for (int i = 1; i < wireQty + 1; i++)
	{
		if (endpoinType1[i-1] == 0 && endpoinType2[i-1] == 0)//两端均为悬垂端点
		{
			WireRealSus.assign(WireListSus.begin(), WireListSus.end());
		}
		else if (endpoinType1[i-1] == 1 && endpoinType2[i-1] == 0)//端点一耐张 端点二悬垂
		{
			CreateTempWireNode(i, 0, WireListSus);
		}
		else if (endpoinType1[i-1] == 0 && endpoinType2[i-1] == 1)//端点二耐张 端点一悬垂
		{
			CreateTempWireNode(i, 1, WireListSus);
		}
		else if (endpoinType1[i-1] == 1 && endpoinType2[i-1] == 1)//端点均为耐张
		{
			CreateTempWireNode(i, 2, WireListSus);
		}

	}
	
}

void WireData::CreateTempWireNode(int wireLogo, int choose, vector<Node>& sus)
{
	vector<Node>TempListNodes;
	int oneWireLogoNum = sus.size() / wireQty; //一条线路悬挂点的个数
	rou = unitMass * 9.8 / area;//比载 N/mm^2*m
	for (int i = (wireLogo - 1) * oneWireLogoNum; i < wireLogo * oneWireLogoNum-1; i++)
	{
		double lxi = sqrt((sus[i].x - sus[i + 1].x) * (sus[i].x - sus[i + 1].x) + (sus[i].y - sus[i + 1].y) * (sus[i].y - sus[i + 1].y));//档距
		double  hi = sus[i + 1].z - sus[i].z;//高差
		double k = rou / stress;
		double Li = (2 / k) * sinh(k * lxi / 2); //线长
		double li = sus[i + 1].x - sus[i].x; //两点x之差
		double mi = sus[i + 1].y - sus[i].y; //两点y之差
		double nni = sqrt(li * li + mi * mi); //二维中两点之差
		double dxi = li / N;
		double dyi = mi / N;
		double dzi = nni / N;
		double y0i = k * ((1 - cosh((1 / k) * (li / 2)) * sqrt(1 + (hi / Li) * (hi / Li)))) + hi / 2;

		//if (i > 0 && i < sus.size())TempListNodes.pop_back(); //删除重节点
		for (int j = 0; j < N + 1; j++)
		{
			double Zi = j * dzi;
			double x = j * dxi + sus[i].x;
			double y = j * dyi + sus[i].y;
			double z = ((1. / k) * (hi / Li)) * (sinh(k * lxi / 2) + sinh(k * (2 * Zi - lxi) / 2)) - ((2 / k) * sinh(k * Zi / 2) *
				sinh(k * (lxi - Zi) / 2)) * sqrt(1 + (hi / Li) * (hi / Li)) + sus[i].z;
			TempListNodes.push_back(Node(1, x, y, z, 0));
		}
		
	}
	FindRealSus(wireLogo, choose, TempListNodes);
}

void WireData::FindRealSus(int wireLogo, int choose, vector<Node>m_Nodes)
{
	std::vector<Node> tempWireRealSus;
	int oneWireLogoNum = (WireListSus.size() / wireQty) ; //一条线路挡位的悬挂点个数
	
	for (const auto& node : m_Nodes)
	{
		if (choose == 0) // 左耐张
		{
			double error = 0.499 * sqrt(pow((WireListSus[(wireLogo - 1) * oneWireLogoNum + 1].x - WireListSus[(wireLogo - 1) * oneWireLogoNum].x), 2) + pow((WireListSus[(wireLogo - 1) * oneWireLogoNum + 1].y - WireListSus[(wireLogo - 1) * oneWireLogoNum].y), 2) + pow((WireListSus[(wireLogo - 1) * oneWireLogoNum + 1].z - WireListSus[(wireLogo - 1) * oneWireLogoNum].z), 2)) / N;
			double Li = sqrt(pow((node.x - WireListSus[(wireLogo - 1) * oneWireLogoNum].x), 2) + pow((node.y - WireListSus[(wireLogo - 1) * oneWireLogoNum].y), 2) + pow((node.z - WireListSus[(wireLogo - 1) * oneWireLogoNum].z), 2));

			if (abs(Li - strainLength) < error)
			{
				tempWireRealSus.push_back(Node(1, node.x, node.y, node.z, 0));
				for (int j = (wireLogo - 1) * oneWireLogoNum + 1; j < wireLogo * oneWireLogoNum; j++)
				{
					tempWireRealSus.push_back(Node(1, WireListSus[j].x, WireListSus[j].y, WireListSus[j].z, 0));
				}
				break; // 结束循环，只添加一个节点
			}
		}
		else if (choose == 1) // 右耐张
		{
			int size = wireLogo * oneWireLogoNum - 1;
			double error = 0.499 * sqrt(pow((WireListSus[size].x - WireListSus[size - 1].x), 2) + pow((WireListSus[size].y - WireListSus[size - 1].y), 2) + pow((WireListSus[size].z - WireListSus[size - 1].z), 2)) / N;
			double nz = sqrt(pow((node.x - WireListSus[size].x), 2) + pow((node.y - WireListSus[size].y), 2) + pow((node.z - WireListSus[size].z), 2));

			if (abs(nz - strainLength) < error)
			{
				for (int j = (wireLogo - 1) * oneWireLogoNum; j < wireLogo * oneWireLogoNum - 1; j++)
				{
					tempWireRealSus.push_back(Node(1, WireListSus[j].x, WireListSus[j].y, WireListSus[j].z, 0));
				}
				tempWireRealSus.push_back(Node(1, node.x, node.y, node.z, 0));
				break; // 结束循环，只添加一个节点
			}
		}
		else if (choose == 2) // 两边耐张
		{
			double error1 = 0.499 * sqrt(pow((WireListSus[(wireLogo - 1) * oneWireLogoNum + 1].x - WireListSus[(wireLogo - 1) * oneWireLogoNum].x), 2) + pow((WireListSus[(wireLogo - 1) * oneWireLogoNum + 1].y - WireListSus[(wireLogo - 1) * oneWireLogoNum].y), 2) + pow((WireListSus[(wireLogo - 1) * oneWireLogoNum + 1].z - WireListSus[(wireLogo - 1) * oneWireLogoNum].z), 2)) / N;
			int size = wireLogo * oneWireLogoNum - 1;
			double error2 = 0.499 * sqrt(pow((WireListSus[size].x - WireListSus[size - 1].x), 2) + pow((WireListSus[size].y - WireListSus[size - 1].y), 2) + pow((WireListSus[size].z - WireListSus[size - 1].z), 2)) / N;
			double L1i = sqrt(pow((node.x - WireListSus[(wireLogo - 1) * oneWireLogoNum ].x), 2) + pow((node.y - WireListSus[(wireLogo - 1) * oneWireLogoNum ].y), 2) + pow((node.z - WireListSus[(wireLogo - 1) * oneWireLogoNum ].z), 2));
			double L2i = sqrt(pow((node.x - WireListSus[size].x), 2) + pow((node.y - WireListSus[size].y), 2) + pow((node.z - WireListSus[size].z), 2));

			if (abs(L1i - strainLength) < error1)
			{
				tempWireRealSus.push_back(Node(1, node.x, node.y, node.z, 0));
				for (int j = (wireLogo - 1) * oneWireLogoNum + 1; j < wireLogo * oneWireLogoNum -1; j++)
				{
					tempWireRealSus.push_back(Node(1, WireListSus[j].x, WireListSus[j].y, WireListSus[j].z, 0));
				}
			}
			if (abs(L2i - strainLength) < error2)
			{
				tempWireRealSus.push_back(Node(1, node.x, node.y, node.z, 0));
			}
		}
	}

	// 将临时结果一次性添加到 WireRealSus 中
	WireRealSus.insert(WireRealSus.end(), tempWireRealSus.begin(), tempWireRealSus.end());//放入了所有线路的真实悬挂点
}

void WireData::CreateTempRealWireNode(int wireLogo, vector<Node>& sus)
{
	vector<Node>TempRealNodes;
	int oneWireLogoNum = sus.size() / wireQty; //一条线路悬挂点的个数
	rou = unitMass * 9.8 / area;//比载 N/mm^2*m
	for (int i = (wireLogo - 1) * oneWireLogoNum; i < wireLogo * oneWireLogoNum - 1; i++)
	{
		double lxi = sqrt((sus[i].x - sus[i + 1].x) * (sus[i].x - sus[i + 1].x) + (sus[i].y - sus[i + 1].y) * (sus[i].y - sus[i + 1].y));//档距
		double  hi = sus[i + 1].z - sus[i].z;//高差
		double k = rou / stress;
		double Li = (2 / k) * sinh(k * lxi / 2); //线长
		double li = sus[i + 1].x - sus[i].x; //两点x之差
		double mi = sus[i + 1].y - sus[i].y; //两点y之差
		double nni = sqrt(li * li + mi * mi); //二维中两点之差
		double dxi = li / N;
		double dyi = mi / N;
		double dzi = nni / N;
		double y0i = k * ((1 - cosh((1 / k) * (li / 2)) * sqrt(1 + (hi / Li) * (hi / Li)))) + hi / 2;

		//if (i > 0 && i < sus.size())TempListNodes.pop_back(); //删除重节点
		for (int j = 0; j < N + 1; j++)
		{
			double Zi = j * dzi;
			double x = j * dxi + sus[i].x;
			double y = j * dyi + sus[i].y;
			double z = ((1. / k) * (hi / Li)) * (sinh(k * lxi / 2) + sinh(k * (2 * Zi - lxi) / 2)) - ((2 / k) * sinh(k * Zi / 2) *
				sinh(k * (lxi - Zi) / 2)) * sqrt(1 + (hi / Li) * (hi / Li)) + sus[i].z;
			TempRealNodes.push_back(Node(1, x, y, z, 0));
		}
	}
	CreateSpacerDistance(TempRealNodes,wireLogo);
	
	
}

void WireData::CreatSpacer(vector<Element_Beam>& m_Elements_Beams, vector<int> ids)
{
	if (ids.size() < 4)return;
	ids.push_back(ids[0]);

	int id1 = ids[0];
	int idelement = m_Elements_Beams.size();
	size_t size = ids.size();
	for (int i = 1; i < size; i++)
	{
		int id2 = ids[i];
		double iDirection[3] = { 3.141595, 1.75691, 0.84178 };
		m_Elements_Beams.push_back(Element_Beam(idelement + 1, id1, id2, 0, iDirection));//总的单元
		id1 = id2;
		idelement++;
	}
	for (int i = 0; i < m_Elements_Beams.size(); i++)
	{
		m_Elements_Beams[i].ClassSectionID = 3;
		m_Elements_Beams[i].MaterialID = 6;

	}

}

void WireData::CreateStrainLine(double x, double y, double z, vector<int> ids)
{
	int id = Creat_Node(x, y, z, 0);
	int idelement = m_Elements_Trusses.size();
	size_t size = ids.size();
	for (int i = 0; i < size; i++)
	{
		int id2 = ids[i];
		m_Elements_Trusses.push_back(Element_Truss(idelement + 1, id, id2, 2, 5));//材料和轴力后续需要改
		int id = m_Elements_Trusses.size() - 1;
		m_Elements_Trusses[id].AxialForce = 1;//初始轴力
		idelement++;
	}
}

void WireData::CreateSpacerDistance(vector<Node>m_TempNodes, int wireLogo)
{
	if (fenlie < 4)return;
	if (SpacerNum.size() == 0)return;
	std::vector<int>SpacerL;//间隔棒离左边端点的距离 
	std::vector<int>SpacerD;//间隔棒所在的档位
	int s = 0;
	int L = 0;
	int d = 0;
	int oneWireLogoNum = WireRealSus.size() / wireQty; //一条线路挡位的个数
	if (ChooseWay == 0)//等间距
	{
		for (int i = (wireLogo - 1) * oneWireLogoNum; i < wireLogo * oneWireLogoNum - 1; i++)
		{
			double l = sqrt(pow(WireRealSus[i + 1].x - WireRealSus[i].x, 2) + pow(WireRealSus[i + 1].y - WireRealSus[i].y, 2));//档距
			s = l / (SpacerNum[i - oneWireLogoNum * (wireLogo - 1)] + 1);
			for (int j = 1; j < SpacerNum[i - oneWireLogoNum * (wireLogo - 1)] + 1; j++)
			{
				L = s * j;
				d = (i- (wireLogo-1)* oneWireLogoNum) + 1;
				SpacerL.push_back(L);
				SpacerD.push_back(d);
			}
		}
	}
	int a1 = 0; int a2 = SpacerNum[0];
	for (int j = 0; j < SpacerNum.size(); j++)
	{
		for (int i = a1; i < a2; i++)
		{
			vector<int> ids;
			ids = FindSpacerL(m_TempNodes, SpacerD[i], SpacerL[i], wireLogo);
			CreatSpacer(m_Elements_beams, ids);
		}
		a1 += SpacerNum[j];
		a2 += SpacerNum[j];
	}
}

vector<int> WireData::FindSpacerL(vector<Node>m_TempNodes, int d, int L, int wireLogo)
{
	vector<int> idnodes;
	int oneWireLogoNum = WireRealSus.size() / wireQty; //一条线路悬挂点的个数
	int num = (wireLogo - 1) * oneWireLogoNum;
	for (auto& i : m_TempNodes)
	{
		double error = 0.500 * sqrt(pow((WireRealSus[num + d].x - WireRealSus[num + d - 1].x), 2) + pow((WireRealSus[num + d].y - WireRealSus[num + d - 1].y), 2)) / N;
		double Lm = sqrt(pow(i.x - WireRealSus[num + d - 1].x, 2) + pow(i.y - WireRealSus[num + d - 1].y, 2));
		if (i.y >= WireRealSus[num + d - 1].y)
		{
			if (abs(Lm - L) < error)
			{
				double x = i.x + 0.225 * cos(angle);
				double y = i.y;
				double z = i.z + 0.225;

				for (auto& j : m_Nodes)
				{
					if ((abs(x - j.x) < 1e-1) && (abs(y - j.y) < 1e-1) && (abs(z - j.z) < 1e-1))
					{
						for (int k = 0; k < fenlie; ++k)
						{
							idnodes.push_back(j.m_idNode + k);
						}
					
						break;
					}
				
				}

			}
		}

	}
	return idnodes;
}




