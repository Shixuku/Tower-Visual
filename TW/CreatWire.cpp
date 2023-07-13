#include "CreatWire.h"

void CreatWire::CreatWireSus()
{
	//生成节点
	int num = WireSus.size();
	int* node = new int[num * fenlie];
	int index = 0; // 节点索引
	for (int i = 0; i < num; i++)//
	{
		for (int j = 0; j < fenlie; j++)
		{
			double offsetX, offsetZ;
			if (fenlie == 1)
			{
				offsetX = 0.0;
				offsetZ = 0.0;
			}
			else if (fenlie == 2)
			{
				offsetX = (j == 0) ? 0.225 : -0.225;
				offsetZ = 0.0;
			}
			else if (fenlie == 4)
			{
				double offsets[4][2] = { {0.225, 0.225}, {0.225, -0.225}, {-0.225, -0.225}, {-0.225, 0.225} };
				offsetX = offsets[j][0];
				offsetZ = offsets[j][1];
			}
			else if (fenlie == 6)
			{
				double offsets[6][2] = { {-0.2, 0.35}, {0.2, -0.35}, {0.4, 0.0}, {0.2, -0.35}, {-0.2, -0.35}, {-0.4, 0.0} };
				offsetX = offsets[j][0];
				offsetZ = offsets[j][1];
			}
			// 创建节点
			node[index] = Creat_Node(WireSus[i].x + offsetX, WireSus[i].y, WireSus[i].z + offsetZ);
			SaveSus({ node[index] }); // 放入悬挂点
			index++;
		}

	}
	// 在使用完后别忘了释放内存
	delete[]node;

}


void CreatWire::CreatWirePoint()
{
	rou = unitMass * 10 / area;
	int n = N;
	int num = WireSus.size() / wireQty - 1;
	
	for (int j = 0; j < wireQty; j++)
	{
		int* node = new int[num * (n + 1) * fenlie];

		for (int i = 0 + WireSus.size() / wireQty * j; i < WireSus.size() / wireQty + WireSus.size() / wireQty * j - 1; i++)
		{
			cout << i << "\n";
			double lxi = sqrt((WireSus[i].x - WireSus[i + 1].x) * (WireSus[i].x - WireSus[i + 1].x) + (WireSus[i].y - WireSus[i + 1].y) * (WireSus[i].y - WireSus[i + 1].y));//档距
			double  hi = WireSus[i + 1].z - WireSus[i].z;//高差
			double k = rou / stress;
			double Li = (2 / k) * sinh(k * lxi / 2); //线长
			double li = WireSus[i + 1].x - WireSus[i].x; //两点x之差
			double mi = WireSus[i + 1].y - WireSus[i].y; //两点y之差
			double nni = sqrt(li * li + mi * mi); //二维中两点之差
			double dxi = li / N;
			double dyi = mi / N;
			double dzi = nni / N;


			if (fenlie == 1)
			{
				for (int m = 0; m < N + 1; m++)
				{
					double Zi = m * dzi;
					double x = m * dxi + WireSus[i].x;
					double y = m * dyi + WireSus[i].y;
					double z = ((1. / k) * (hi / Li)) * (sinh(k * lxi / 2) + sinh(k * (2 * Zi - lxi) / 2)) - ((2 / k) * sinh(k * Zi / 2) *
						sinh(k * (lxi - Zi) / 2)) * sqrt(1 + (hi / Li) * (hi / Li)) + WireSus[i].z;
					node[(i - WireSus.size() / wireQty * j) * (n + 1) + m] = Creat_Node(x, y, z);
				}
			}
			else if (fenlie == 2)
			{
				for (int m = 0; m < N + 1; m++)
				{
					double Zi = m * dzi;
					double x1 = m * dxi + WireSus[i].x + 0.225;
					double x2 = m * dxi + WireSus[i].x - 0.225;
					double y = m * dyi + WireSus[i].y;
					double z = ((1. / k) * (hi / Li)) * (sinh(k * lxi / 2) + sinh(k * (2 * Zi - lxi) / 2)) - ((2 / k) * sinh(k * Zi / 2) *
						sinh(k * (lxi - Zi) / 2)) * sqrt(1 + (hi / Li) * (hi / Li)) + WireSus[i].z;
					node[(i - WireSus.size() / wireQty * j) * (n + 1) + m] = Creat_Node(x1, y, z);
					node[(i - WireSus.size() / wireQty * j) * (n + 1) + num * (N + 1) + m] = Creat_Node(x2, y, z);
				}
			}
			else if (fenlie == 4)
			{
				for (int m = 0; m < N + 1; m++)
				{
					double Zi = m * dzi;
					double x1 = m * dxi + WireSus[i].x + 0.225;
					double x2 = m * dxi + WireSus[i].x - 0.225;
					double y = m * dyi + WireSus[i].y;
					double z1 = ((1. / k) * (hi / Li)) * (sinh(k * lxi / 2) + sinh(k * (2 * Zi - lxi) / 2)) - ((2 / k) * sinh(k * Zi / 2) *
						sinh(k * (lxi - Zi) / 2)) * sqrt(1 + (hi / Li) * (hi / Li)) + WireSus[i].z + 0.225;
					double z2 = ((1. / k) * (hi / Li)) * (sinh(k * lxi / 2) + sinh(k * (2 * Zi - lxi) / 2)) - ((2 / k) * sinh(k * Zi / 2) *
						sinh(k * (lxi - Zi) / 2)) * sqrt(1 + (hi / Li) * (hi / Li)) + WireSus[i].z - 0.225;
					node[(i - WireSus.size() / wireQty * j) * (n + 1) + m] = Creat_Node(x1, y, z1);
					node[(i - WireSus.size() / wireQty * j) * (n + 1) + num * (N + 1) + m] = Creat_Node(x2, y, z1);
					node[(i - WireSus.size() / wireQty * j) * (n + 1) + 2 * num * (N + 1) + m] = Creat_Node(x2, y, z2);
					node[(i - WireSus.size() / wireQty * j) * (n + 1) + 3 * num * (N + 1) + m] = Creat_Node(x1, y, z2);
				}
			}
		}
		if (fenlie == 1)
		{
			std::vector<int> nodeIds;
			for (int i = 0; i < num * (N + 1); i++)
			{
				nodeIds.push_back(node[i]);
			}
			CreatWireEle(m_Elements_Trusses, nodeIds);
		}
		else if (fenlie == 2)
		{
			std::vector<int> nodeIds1;
			std::vector<int> nodeIds2;
			for (int i = 0; i < num * (N + 1); i++)
			{
				nodeIds1.push_back(node[i]);
				nodeIds2.push_back(node[num * (N + 1) + i]);
			}
			CreatWireEle(m_Elements_Trusses, nodeIds1);
			CreatWireEle(m_Elements_Trusses, nodeIds2);
		}
		else if (fenlie == 4)
		{
			std::vector<int> nodeIds1;
			std::vector<int> nodeIds2;
			std::vector<int> nodeIds3;
			std::vector<int> nodeIds4;
			for (int i = 0; i < num * (N + 1); i++)
			{
				nodeIds1.push_back(node[i]);
				nodeIds2.push_back(node[num * (N + 1) + i]);
				nodeIds3.push_back(node[2 * num * (N + 1) + i]);
				nodeIds4.push_back(node[3 * num * (N + 1) + i]);
			}
			CreatWireEle(m_Elements_Trusses, nodeIds1);
			CreatWireEle(m_Elements_Trusses, nodeIds2);
			CreatWireEle(m_Elements_Trusses, nodeIds3);
			CreatWireEle(m_Elements_Trusses, nodeIds4);
		}
		delete[]node;
	}
	
	
	


	for (int i = 0; i < m_Elements_Trusses.size(); i++)
	{
		m_Elements_Trusses[i].ClassSectionID = WireSectionId;
	}
}

void CreatWire::Create_Mesh()
{
	CreatWireSus();
	CreatWirePoint();
}

int CreatWire::FindGroupIdNode(int idNode) const
{
	return wireToGroup[idNode - 1];;
}
