#include "WireData.h"

int WireData::Creat_Node(double x, double y, double z, double f)
{

	int SIZE = susPoint.size();
	std::cout << "size:" << SIZE << std::endl;
	int judg = 0;
	for (int i = 0; i < SIZE; ++i)
	{
		if (((abs(NodeData[susPoint[i]].x - x) < 1e-8)) &&
			((abs(NodeData[susPoint[i]].y - y)) < 1e-8) &&
			((abs(NodeData[susPoint[i]].z - z)) < 1e-8))
		{
			m_Nodes[susPoint[i] - 1].F = f;
			return NodeData[susPoint[i]].m_idNode; //ÖØ½Úµã
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
