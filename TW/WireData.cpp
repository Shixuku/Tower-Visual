#include "WireData.h"

int WireData::Creat_Node(double x, double y, double z, double f)
{

	int SIZE = realSuspoint.size();
	std::cout << "size:" << SIZE << std::endl;
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
	CreateTempWireNode(WireListSus, TempListNodes);
	if (endpoinType1 == 0 && endpoinType2 == 0)//两端均为悬垂端点
	{
		WireRealSus = WireListSus;
	}
	else if (endpoinType1 == 1 && endpoinType2 == 0)//端点一耐张 端点二悬垂
	{
		FindRealSus(0);
	}
	else if (endpoinType1 == 0 && endpoinType2 == 1)//端点二耐张 端点一悬垂
	{
		FindRealSus(1);
	}
	else if (endpoinType1 == 1 && endpoinType2 == 1)//端点均为耐张
	{
		FindRealSus(2);
	}
}

void WireData::CreateTempWireNode(vector<Node>&sus, vector<Node>&Nodes)
{
	rou = unitMass * 9.8 / area;//比载 N/mm^2*m
	for (int i = 0; i < sus.size() - 1; i++)
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

		if (i > 0 && i < sus.size())TempListNodes.pop_back(); //删除重节点
		for (int j = 0; j < N + 1; j++)
		{
			double Zi = j * dzi;
			double x = j * dxi + sus[i].x;
			double y = j * dyi + sus[i].y;
			double z = ((1. / k) * (hi / Li)) * (sinh(k * lxi / 2) + sinh(k * (2 * Zi - lxi) / 2)) - ((2 / k) * sinh(k * Zi / 2) *
				sinh(k * (lxi - Zi) / 2)) * sqrt(1 + (hi / Li) * (hi / Li)) + sus[i].z;
			Nodes.push_back(Node(1, x, y, z, 0));
		}
	}
}

void WireData::FindRealSus(int choose)
{
	std::vector<Node> tempWireRealSus;

	for (const auto& node : TempListNodes)
	{
		if (choose == 0) // 左耐张
		{
			double error = 0.499 * sqrt(pow((WireListSus[1].x - WireListSus[0].x), 2) + pow((WireListSus[1].y - WireListSus[0].y), 2) + pow((WireListSus[1].z - WireListSus[0].z), 2)) / N;
			double Li = sqrt(pow((node.x - WireListSus[0].x), 2) + pow((node.y - WireListSus[0].y), 2) + pow((node.z - WireListSus[0].z), 2));

			if (abs(Li - strainLength) < error)
			{
				tempWireRealSus.push_back(Node(1, node.x, node.y, node.z, 0));
				for (int j = 1; j < WireListSus.size(); j++)
				{
					tempWireRealSus.push_back(Node(1, WireListSus[j].x, WireListSus[j].y, WireListSus[j].z, 0));
				}
				break; // 结束循环，只添加一个节点
			}
		}
		else if (choose == 1) // 右耐张
		{
			int size = WireListSus.size() - 1;
			double error = 0.499 * sqrt(pow((WireListSus[size].x - WireListSus[size - 1].x), 2) + pow((WireListSus[size].y - WireListSus[size - 1].y), 2) + pow((WireListSus[size].z - WireListSus[size - 1].z), 2)) / N;
			double nz = sqrt(pow((node.x - WireListSus[size].x), 2) + pow((node.y - WireListSus[size].y), 2) + pow((node.z - WireListSus[size].z), 2));

			if (abs(nz - strainLength) < error)
			{
				for (int j = 0; j < WireListSus.size() - 1; j++)
				{
					tempWireRealSus.push_back(Node(1, WireListSus[j].x, WireListSus[j].y, WireListSus[j].z, 0));
				}
				tempWireRealSus.push_back(Node(1, node.x, node.y, node.z, 0));
				break; // 结束循环，只添加一个节点
			}
			
		}
		else if (choose == 2) // 两边耐张
		{
			double error1 = 0.499 * sqrt(pow((WireListSus[1].x - WireListSus[0].x), 2) + pow((WireListSus[1].y - WireListSus[0].y), 2) + pow((WireListSus[1].z - WireListSus[0].z), 2)) / N;
			int size = WireListSus.size() - 1;
			double error2 = 0.499 * sqrt(pow((WireListSus[size].x - WireListSus[size - 1].x), 2) + pow((WireListSus[size].y - WireListSus[size - 1].y), 2) + pow((WireListSus[size].z - WireListSus[size - 1].z), 2)) / N;
			double L1i = sqrt(pow((node.x - WireListSus[0].x), 2) + pow((node.y - WireListSus[0].y), 2) + pow((node.z - WireListSus[0].z), 2));
			double L2i = sqrt(pow((node.x - WireListSus[size].x), 2) + pow((node.y - WireListSus[size].y), 2) + pow((node.z - WireListSus[size].z), 2));

			if (abs(L1i - strainLength) < error1)
			{
				tempWireRealSus.push_back(Node(1, node.x, node.y, node.z, 0));
				for (int j = 1; j < WireListSus.size() - 1; j++)
				{
					tempWireRealSus.push_back(Node(1, WireListSus[j].x, WireListSus[j].y, WireListSus[j].z, 0));
				}
				
			}
			if (abs(L2i - strainLength) < error2)
			{
				tempWireRealSus.push_back(Node(1, node.x, node.y, node.z, 0));
			}
			//break; // 结束循环，只添加一个节点
		}
	}

	// 将临时结果一次性添加到 WireRealSus 中
	WireRealSus.insert(WireRealSus.end(), tempWireRealSus.begin(), tempWireRealSus.end());
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
		idelement++;
	}
}

void WireData::CreateSpacerDistance()
{
	if (fenlie < 4)return;
	if (SpacerNum.size() == 0)return;
	int s = 0;
	int L = 0;
	int d = 0;
	
	if (ChooseWay == 0)//等间距
	{
		for (int i = 0; i < WireRealSus.size() - 1; i++)
		{
			double l = sqrt(pow(WireRealSus[i + 1].x - WireRealSus[i].x, 2) + pow(WireRealSus[i + 1].y - WireRealSus[i].y, 2));//档距
			s = l / (SpacerNum[i] + 1);
			for (int j = 1; j < SpacerNum[i] + 1; j++)
			{
				L = s * j;
				d = i + 1;
				SpacerL.push_back(L);
				SpacerD.push_back(d);
			}
		}
	}
}

vector<int> WireData::FindSpacerL(int d,int L)
{
	vector<int> idnodes;
	for (auto& i : TempRealNodes)
	{
		double error = 0.500 * sqrt(pow((WireRealSus[d].x - WireRealSus[d - 1].x), 2) + pow((WireRealSus[d].y - WireRealSus[d - 1].y), 2)) / N;
		double Lm = sqrt(pow(i.x - WireRealSus[d - 1].x, 2) + pow(i.y - WireRealSus[d - 1].y, 2));
		if (i.y >= WireRealSus[d - 1].y)
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
						//cout << " 间隔棒的点" << idnodes[0] << "  " << idnodes[1] << "  " << idnodes[2] << "  " << idnodes[3] << "\n";
						break;
					}
				
				}

			}
		}

	}
	return idnodes;
}




