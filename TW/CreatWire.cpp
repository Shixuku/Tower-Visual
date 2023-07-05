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
	int num = WireSus.size() - 1;
	int* node = new int[num * (n + 1)];
	for (int i = 0; i < WireSus.size() - 1; i++)
	{
		double lxi = sqrt((WireSus[i].x - WireSus[i + 1].x) * (WireSus[i].x - WireSus[i + 1].x) + (WireSus[i].y - WireSus[i + 1].y) * (WireSus[i].y - WireSus[i + 1].y));//档距
		double  hi = WireSus[i + 1].z - WireSus[i].z;//高差
		double k = rou;
		double Li = (2 / k) * sinh(k * lxi / 2); //线长
		double li = WireSus[i + 1].x - WireSus[i].x; //两点x之差
		double mi = WireSus[i + 1].y - WireSus[i].y; //两点y之差
		double nni = sqrt(li * li + mi * mi); //二维中两点之差
		double dxi = li / N;
		double dyi = mi / N;
		double dzi = nni / N;
		double y0i = k * ((1 - cosh((1 / k) * (li / 2)) * sqrt(1 + (hi / Li) * (hi / Li)))) + hi / 2;
		//if (i > 0 && i < WireSus.size())node.pop_back(); //删除重节点
		for (int j = 0; j < N + 1; j++)
		{
			double Zi = j * dzi;
			double x = j * dxi + WireSus[i].x;
			double y = j * dyi + WireSus[i].y;
			double z = ((1. / k) * (hi / Li)) * (sinh(k * lxi / 2) + sinh(k * (2 * Zi - lxi) / 2)) - ((2 / k) * sinh(k * Zi / 2) *
				sinh(k * (lxi - Zi) / 2)) * sqrt(1 + (hi / Li) * (hi / Li)) + WireSus[i].z;
			node[(i * (N + 1)) + j] = Creat_Node(x, y, z);
			cout << node[j] << "\n";
		}
	}
	
	
	std::vector<int> nodeIds;
	for (int i = 0; i < num * N + 1; i++)
	{
		nodeIds.push_back(node[i]);
	}
    cout << "test node" << m_Nodes.size() << "\n";

	CreatWireEle(m_Elements_Trusses, nodeIds);
	for (int i = 0; i < m_Nodes.size(); i++)
	{
		cout << m_Nodes[i].m_idNode << "  " << m_Nodes[i].x << "  " << m_Nodes[i].y << "  " << m_Nodes[i].z << "\n";
	}
	delete[]node;
}

void CreatWire::Create_Mesh()
{
	CreatWireSus();
	CreatWirePoint();
}
