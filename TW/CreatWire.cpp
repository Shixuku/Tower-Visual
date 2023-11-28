#include "CreatWire.h"

void CreatWire::CreateRealSus() 
{
	CreatRealNode();
	//生成节点
	int num = WireRealSus.size();
	int susnum = num * fenlie;//
	int* node = new int[susnum];
	int index = 0; // 节点索引
	for (int i = 0; i < num; i++)//
	{
		for (int j = 0; j < fenlie; j++)
		{
			double offsetX = 0; double offsetY = 0; double offsetZ = 0;
			if (fenlie == 1)
			{
				offsetX = 0.0;
				offsetZ = 0.0;
			}
			else if (fenlie == 2)
			{
				offsetX = (j == 0) ? 0.225 : -0.225;//三元运算符（如果j为0，x=0.225,反之为-0.225）
				offsetZ = 0.0;
			}
			else if (fenlie == 4)
			{
				double offsets[4][3] = { {0.225*cos(angle),-0.225*sin(angle), 0.225}, {0.225 * cos(angle),-0.225 * sin(angle), -0.225}, 
					{-0.225 * cos(angle),0.225 * sin(angle), -0.225}, {-0.225 * cos(angle),0.225 * sin(angle), 0.225} };
				offsetX = offsets[j][0];
				offsetY = offsets[j][1];
				offsetZ = offsets[j][2];
			}
			else if (fenlie == 6)
			{
				double offsets[6][2] = { {-0.2, 0.35}, {0.2, -0.35}, {0.4, 0.0}, {0.2, -0.35}, {-0.2, -0.35}, {-0.4, 0.0} };
				offsetX = offsets[j][0];
				offsetZ = offsets[j][1];
			}
			// 创建节点
			if (index < susnum)
			{
				node[index] = Creat_Node(WireRealSus[i].x + offsetX, WireRealSus[i].y + offsetY, WireRealSus[i].z + offsetZ, 0, 2);
				SaveSus({ node[index] }); // 放入悬挂点
			}
			else
			{
				// 处理索引超出范围的情况，例如抛出异常或打印错误信息
				// 并确保不会继续写入超出范围的节点
				break;
			}

			index++;
		}
		
	}
	allSus.insert(allSus.end(), WireRealSus.begin(), WireRealSus.end());
	// 在使用完后别忘了释放内存
	delete[]node;

}

void CreatWire::CreateWire()
{
	rou = unitMass * 9.8 / area;//比载 N/mm^2*m
	int n = N;
	int num = WireRealSus.size() / wireQty - 1;

	for (int j = 0; j < wireQty; j++)
	{
		int* node = new int[num * (n + 1) * fenlie];

		for (int i = 0 + WireRealSus.size() / wireQty * j; i < WireRealSus.size() / wireQty * ( j + 1 ) - 1; i++)
		{
			double lxi = sqrt((WireRealSus[i].x - WireRealSus[i + 1].x) * (WireRealSus[i].x - WireRealSus[i + 1].x) + (WireRealSus[i].y - WireRealSus[i + 1].y) * (WireRealSus[i].y - WireRealSus[i + 1].y));//档距
			double wireL= sqrt((WireRealSus[i].x - WireRealSus[i + 1].x) * (WireRealSus[i].x - WireRealSus[i + 1].x) + 
				(WireRealSus[i].y - WireRealSus[i + 1].y) * (WireRealSus[i].y - WireRealSus[i + 1].y)+ 
				(WireRealSus[i].z - WireRealSus[i + 1].z) * (WireRealSus[i].z - WireRealSus[i + 1].z));//档距
			double  hi = WireRealSus[i + 1].z - WireRealSus[i].z;//高差
			double k = rou / stress;
			double Li = (2 / k) * sinh(k * lxi / 2); //线长
			double li = WireRealSus[i + 1].x - WireRealSus[i].x; //两点x之差
			double mi = WireRealSus[i + 1].y - WireRealSus[i].y; //两点y之差
			double nni = sqrt(li * li + mi * mi); //二维中两点之差
			double dxi = li / N;
			double dyi = mi / N;
			double dzi = nni / N;
			double y0i = WireRealSus[i].z + 1 / k * ((1 - cosh((k) * (nni / 2)) * sqrt(1 + (hi / Li) * (hi / Li)))) + hi / 2;
			double force = 0;//每个点对应的轴力
			double wireGravity = wireL * unitMass * 9.8 * fenlie;
			WireGravity.push_back(wireGravity);
			if (fenlie == 1)
			{
				for (int m = 0; m < N + 1; m++)
				{
					double Zi = m * dzi;
					double x = m * dxi + WireRealSus[i].x;
					double y = m * dyi + WireRealSus[i].y;
					double z = ((1. / k) * (hi / Li)) * (sinh(k * lxi / 2) + sinh(k * (2 * Zi - lxi) / 2)) - ((2 / k) * sinh(k * Zi / 2) *
						sinh(k * (lxi - Zi) / 2)) * sqrt(1 + (hi / Li) * (hi / Li)) + WireRealSus[i].z;
					force = area * (stress + rou * (z - y0i)) ;
					node[(i - WireRealSus.size() / wireQty * j) * (n + 1) + m] = Creat_Node(x, y, z, force,2);
				}
			}
			else if (fenlie == 2)
			{
				for (int m = 0; m < N + 1; m++)
				{
					double Zi = m * dzi;
					double x1 = m * dxi + WireRealSus[i].x + 0.225;
					double x2 = m * dxi + WireRealSus[i].x - 0.225;
					double y = m * dyi + WireRealSus[i].y;
					double z = ((1. / k) * (hi / Li)) * (sinh(k * lxi / 2) + sinh(k * (2 * Zi - lxi) / 2)) - ((2 / k) * sinh(k * Zi / 2) *
						sinh(k * (lxi - Zi) / 2)) * sqrt(1 + (hi / Li) * (hi / Li)) + WireRealSus[i].z;
					force = area * (stress + rou * (z - y0i)) / 1000;
					force = area * (stress + rou * (z - y0i));
					node[(i - WireRealSus.size() / wireQty * j) * (n + 1) + m] = Creat_Node(x1, y, z, force, 2);
					node[(i - WireRealSus.size() / wireQty * j) * (n + 1) + num * (N + 1) + m] = Creat_Node(x2, y, z, force, 2);
				}
			}
			else if (fenlie == 4)
			{
				for (int m = 0; m < N + 1; m++)
				{
					double Zi = m * dzi;
					double x1 = m * dxi + WireRealSus[i].x + 0.225;
					double x2 = m * dxi + WireRealSus[i].x - 0.225;
					double y = m * dyi + WireRealSus[i].y;
					double z1 = ((1. / k) * (hi / Li)) * (sinh(k * lxi / 2) + sinh(k * (2 * Zi - lxi) / 2)) - ((2 / k) * sinh(k * Zi / 2) *
						sinh(k * (lxi - Zi) / 2)) * sqrt(1 + (hi / Li) * (hi / Li)) + WireRealSus[i].z + 0.225;
					double z2 = ((1. / k) * (hi / Li)) * (sinh(k * lxi / 2) + sinh(k * (2 * Zi - lxi) / 2)) - ((2 / k) * sinh(k * Zi / 2) *
						sinh(k * (lxi - Zi) / 2)) * sqrt(1 + (hi / Li) * (hi / Li)) + WireRealSus[i].z - 0.225;
					force = area * (stress + rou * (z1 - y0i));
				//	segamai = area * (stress + rou * (z - y0i)) / 1000;
					node[(i - WireRealSus.size() / wireQty * j) * (n + 1) + m] = Creat_Node(x1, y, z1, force, 2);
					node[(i - WireRealSus.size() / wireQty * j) * (n + 1) + num * (N + 1) + m] = Creat_Node(x2, y, z1, force, 2);
					node[(i - WireRealSus.size() / wireQty * j) * (n + 1) + 2 * num * (N + 1) + m] = Creat_Node(x2, y, z2, force, 2);
					node[(i - WireRealSus.size() / wireQty * j) * (n + 1) + 3 * num * (N + 1) + m] = Creat_Node(x1, y, z2, force, 2);
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
			CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds, 1, "L", "F");
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
			CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds1,1, "L", "F");
			CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds2,1, "L", "F");
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
			CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds1,1, "L", "F");
			CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds2,1, "L", "F");
			CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds3,1, "L", "F");
			CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds4,1, "L", "F");
		}
		delete[]node;
	}
	for (int i = 0; i < m_Elements_Trusses.size(); i++)
	{
		m_Elements_Trusses[i].ClassSectionID = WireSectionId;
		m_Elements_Trusses[i].MaterialID = 4;
		m_Elements_Trusses[i].AxialForce = (m_Nodes[m_Elements_Trusses[i].m_idNode[0] - 1].F + m_Nodes[m_Elements_Trusses[i].m_idNode[1] - 1].F) / 2;
	}
}

void CreatWire::CreateStrain()
{
	//找耐张段那个间隔棒
	vector<int> ids;
	vector<Element_Beam>TempTruss;
	
	int num = WireListSus.size() - 1;
	int SusSize = WireListSus.size();//所有列表悬挂点的个数
	int realSusSize = WireRealSus.size();
	for (int j = 0; j < endpoinType1.size(); j++)
	{
		if (endpoinType1[j] == 1 && endpoinType2[j] == 0)//端点一耐张 端点二悬垂
		{
			vector<int> ids;
			for (int i = 0; i < fenlie; i++)
			{
				ids.push_back(fenlie*j* SusSize / wireQty + i+1);
			}
			double x = WireListSus[0 + SusSize / wireQty * j].x;
			double y = WireListSus[0 + SusSize / wireQty * j].y;
			double z = WireListSus[0 + SusSize / wireQty * j].z;
			//CreateStrainLine(TempTruss, x, y, z, ids);
			CreatSpacer(m_Elements_beams,3,ids);
		}
		else if (endpoinType1[j] == 0 && endpoinType2[j] == 1)//端点一悬垂 端点二耐张
		{
			vector<int> ids;
			for (int i = 0; i < fenlie; i++)
			{
				ids.push_back(fenlie * (j+1) * SusSize / wireQty-(fenlie-(i+1)));
			}
			double x = WireListSus[SusSize / wireQty * (j + 1) - 1].x;
			double y = WireListSus[SusSize / wireQty * (j + 1) - 1].y;
			double z = WireListSus[SusSize / wireQty * (j + 1) - 1].z;
			//CreateStrainLine(TempTruss, x, y, z, ids);
			CreatSpacer(m_Elements_beams,3, ids);
		}
		else if (endpoinType1[j] == 1 && endpoinType2[j] == 1)//端点一耐张 端点二耐张
		{
			vector<int> start_ids;
			vector<int> end_ids;
			for (int i = 0; i < fenlie; i++)
			{
				start_ids.push_back(fenlie * j * SusSize / wireQty + i + 1);
				end_ids.push_back(fenlie * (j + 1) * SusSize / wireQty - (fenlie - (i + 1)));
			}
			double x1 = WireListSus[0 + SusSize / wireQty * j].x;
			double y1 = WireListSus[0 + SusSize / wireQty * j].y;
			double z1 = WireListSus[0 + SusSize / wireQty * j].z;
			//CreateStrainLine(TempTruss, x1, y1, z1, start_ids);
			double x2 = WireListSus[SusSize / wireQty * (j + 1) - 1].x;
			double y2 = WireListSus[SusSize / wireQty * (j + 1) - 1].y;
			double z2 = WireListSus[SusSize / wireQty * (j + 1) - 1].z;
			//CreateStrainLine(TempTruss, x2, y2, z2, end_ids);
			CreatSpacer(m_Elements_beams,3,start_ids);
			CreatSpacer(m_Elements_beams,3,end_ids);
		}
	}
	for (int i = 0; i < TempTruss.size(); i++)
	{
		TempTruss[i].ClassSectionID = WireSectionId;
		TempTruss[i].AxialForce = 6000;
		TempTruss[i].MaterialID = 4;
	}
	m_Elements_beams.insert(m_Elements_beams.end(), TempTruss.begin(), TempTruss.end());
}

void CreatWire::Create_Mesh()
{
	CreateRealSus();
	CreateWire();
	CreateStrain();
	for (int i = 0; i < wireQty; i++)
	{
		CreateTempRealWireNode(i + 1, WireRealSus);
	}
}



int CreatWire::FindGroupIdNode(int idNode) const
{
	return wireToGroup[idNode - 1];;
}
