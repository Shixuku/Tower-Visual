#include "CreatWire.h"

void CreatWire::CreatWireSus()
{
	//生成节点
	int num = WireSus.size();
	int* node = new int[num];
	for (int i = 0; i < num; i++)//
	{
		node[i] = Creat_Node(WireSus[i].x, WireSus[i].y, WireSus[i].z);
		SaveSus({ node[i] });//放入悬挂点
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
		int* node = new int[num * (n + 1)];
		cout << j << "\n";
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
			double y0i = k * ((1 - cosh((1 / k) * (li / 2)) * sqrt(1 + (hi / Li) * (hi / Li)))) + hi / 2;
			//if (i > 0 && i < WireSus.size())node.pop_back(); //删除重节点
			for (int a = 0; a < N + 1; a++)
			{
				double Zi = a * dzi;
				double x = a * dxi + WireSus[i].x;
				double y = a * dyi + WireSus[i].y;
				double z = ((1. / k) * (hi / Li)) * (sinh(k * lxi / 2) + sinh(k * (2 * Zi - lxi) / 2)) - ((2 / k) * sinh(k * Zi / 2) *
					sinh(k * (lxi - Zi) / 2)) * sqrt(1 + (hi / Li) * (hi / Li)) + WireSus[i].z;
				node[((i- WireSus.size() / wireQty * j) * (N + 1)) + a ] = Creat_Node(x, y, z);
				cout << node[a] << "\n";
			}
		}
		std::vector<int> nodeIds;
		for (int i = 0; i < num * N + 1; i++)
		{
			nodeIds.push_back(node[i]);
		}
		CreatWireEle(m_Elements_Trusses, nodeIds);
		delete[]node;
		nodeIds.clear();
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
