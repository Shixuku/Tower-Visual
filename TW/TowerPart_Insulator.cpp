#include "TowerPart_Insulator.h"

void TowerPart_Insulator::Get_Nodes4(double a,Node* n)
{
	if (m_node2->x > m_node1->x)
	{
		n[0] = *m_node1;		n[1] = *m_node2;
	}
	else if (m_node2->x < m_node1->x)
	{
		n[0] = *m_node2;		n[1] = *m_node1;
	}
	double disX = n[1].x - n[0].x;//两点之间长度
	n[2].x = n[0].x + disX / 2 - a / 2;		n[2].y = Get_y(n[2].x);		n[2].z = n[0].z - m_H;
	n[3].x = n[0].x + disX / 2 + a / 2;		n[3].y = Get_y(n[3].x);		n[3].z = n[1].z - m_H;
}

void TowerPart_Insulator::Get_Nodes4_I(double a, Node* n)
{
	n[0] = *m_node;				
	n[1].x = n[0].x;			n[1].y = n[0].y;	n[1].z = n[0].z - m_H;
	n[2].x = n[1].x - a / 2;	n[2].y = n[1].y;	n[2].z = n[0].z - m_H;
	n[3].x = n[1].x + a / 2;	n[3].y = n[1].y;	n[3].z = n[0].z - m_H;

}

void TowerPart_Insulator::Get_split1()
{
	Node Node1[3];
	int node[3];
	Node1[0] = *m_node1;
	Node1[1] = *m_node2;
	double disX = m_node2->x - m_node1->x;//两点之间长度
	Node1[2].x = m_node1->x + disX / 2;		Node1[2].y= Get_y(Node1[2].x);		Node1[2].z = m_node1->z - m_H;//11
	for (int i = 0; i < 3; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	InPutSuspension({ node[2] ,wireLogo });
	realSuspoint.push_back(node[2]);
	//生成单元
	Creat_Trusses(m_Elements_Trusses, { node[0],node[2],node[1] });
	SuspensionElement.push_back(1);
	SuspensionElement.push_back(2);
	for (int i = 0; i < m_Elements_Trusses.size(); i++)
	{
		m_Elements_Trusses[i].ClassSectionID = 2;
		m_Elements_Trusses[i].MaterialID = 5;

	}
	SuspensionElementClass.push_back(2);
}

void TowerPart_Insulator::Get_split1_I()
{
	Node Node1[2];
	int node[2];
	Node1[0] = *m_node;
	Node1[1].x = Node1[0].x;
	Node1[1].y = Node1[0].y;
	Node1[1].z = Node1[0].z - m_H;//1111
	for (int i = 0; i < 2; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	InPutSuspension({ node[1],wireLogo });
	realSuspoint.push_back(node[1]);
	Creat_Trusses(m_Elements_Trusses, { node[0],node[1] });
	SuspensionElement.push_back(1);
	SuspensionElementClass.push_back(1);
	for (int i = 0; i < m_Elements_Trusses.size(); i++)
	{
		m_Elements_Trusses[i].ClassSectionID = 2;
		m_Elements_Trusses[i].MaterialID = 5;
	}
}

void TowerPart_Insulator::Get_split2()
{
	Node Node1[5];
	int node[5];
	Get_Nodes4(0.45, Node1);
	interpolation(Node1[2], Node1[3], 0.5, Node1[4]);///111
	for (int i = 0; i < 5; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	InPutSuspension({ node[4],wireLogo });
	realSuspoint.push_back(node[2]);
	realSuspoint.push_back(node[3]);
	Creat_Trusses(m_Elements_Trusses, { node[0],node[2], node[4],node[3],node[1] });
	SuspensionElement.push_back(1);
	SuspensionElement.push_back(4);
	SuspensionElementClass.push_back(2);
	for (int i = 0; i < m_Elements_Trusses.size(); i++)
	{
		m_Elements_Trusses[i].ClassSectionID = 2;
		m_Elements_Trusses[i].MaterialID = 5;

	}
}

void TowerPart_Insulator::Get_split2_I()
{
	Node Node1[4];
	int node[4];
	Get_Nodes4_I(0.45, Node1);
	for (int i = 0; i < 4; i++)///n[1]
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	InPutSuspension({ node[1], wireLogo });
	realSuspoint.push_back(node[2]);
	realSuspoint.push_back(node[3]);
	Creat_Trusses(m_Elements_Trusses, { node[0],node[1] });
	Creat_Trusses(m_Elements_Trusses, { node[2],node[1],node[3] });
	SuspensionElement.push_back(1);
	SuspensionElementClass.push_back(1);
	for (int i = 0; i < m_Elements_Trusses.size(); i++)
	{
		m_Elements_Trusses[i].ClassSectionID = 2;
		m_Elements_Trusses[i].MaterialID = 5;
	}
}

void TowerPart_Insulator::Get_split4(int m_type)
{
	Node Node1[7];
	int node[7];
	if (m_type == 1)
	{
		Get_Nodes4_I(0.45, Node1);
		Node1[4].x = Node1[2].x;	Node1[4].y = Node1[2].y;
		Node1[5].x = Node1[3].x;	Node1[5].y = Node1[3].y;
	}
	else if (m_type == 2)
	{
		Get_Nodes4(0.45, Node1);
		Node1[4].x = Node1[2].x;	Node1[4].y = Get_y(Node1[4].x);
		Node1[5].x = Node1[3].x;	Node1[5].y = Get_y(Node1[5].x);

	}
	Node1[4].z = Node1[2].z - 0.45;
	Node1[5].z = Node1[3].z - 0.45;
	interpolation(Node1[2], Node1[5], 0.5, Node1[6]);//11

	for (int i = 0; i < 7; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	InPutSuspension({ node[6],wireLogo });
	realSuspoint.push_back(node[2]);
	realSuspoint.push_back(node[3]);	
	realSuspoint.push_back(node[4]);
	realSuspoint.push_back(node[5]);
	if (m_type == 1)
	{
		Creat_Trusses(m_Elements_Trusses, { node[0],node[1] });
		CreatWireBeams(m_Elements_beams, { node[2],node[1],node[3] });
		CreatWireBeams(m_Elements_beams, { node[3],node[5],node[4],node[2] });
		SuspensionElement.push_back(1);
		SuspensionElementClass.push_back(1);
		for (int i = 0; i < m_Elements_Trusses.size(); i++)
		{
			m_Elements_Trusses[i].ClassSectionID = 2;
			m_Elements_Trusses[i].MaterialID = 5;
		}
		for (int i = 0; i < m_Elements_beams.size(); i++)
		{
			m_Elements_beams[i].ClassSectionID = 3;
			m_Elements_beams[i].MaterialID = 6;
		}

	}
	else if (m_type == 2)
	{
		//生成单元
		Creat_Trusses(m_Elements_Trusses, { node[0],node[2] });
		Creat_Trusses(m_Elements_Trusses, { node[3],node[1] });
		CreatWireBeams(m_Elements_beams, { node[3],node[5],node[4],node[2],node[3] });
		SuspensionElement.push_back(1);
		SuspensionElement.push_back(2);
		SuspensionElementClass.push_back(2);
		for (int i = 0; i < m_Elements_Trusses.size(); i++)
		{
			m_Elements_Trusses[i].ClassSectionID = 2;
			m_Elements_Trusses[i].MaterialID = 5;
		}
		for (int i = 0; i < m_Elements_beams.size(); i++)
		{
			m_Elements_beams[i].ClassSectionID = 3;
			m_Elements_beams[i].MaterialID = 6;
		}
	}
	
}

void TowerPart_Insulator::Get_split6(int m_type)
{
	Node Node1[9];
	int node[9];
	double b = 200 * sqrt(3);
	double c = 200;
	if (m_type == 1)
	{
		Get_Nodes4_I(400, Node1);
		Node1[4].x = Node1[2].x - c;
		Node1[4].y = Get_y_I(Node1[4].x);
		Node1[5].x = Node1[3].x + c;
		Node1[5].y = Get_y_I(Node1[5].x);
		Node1[6].x = Node1[2].x;
		Node1[6].y= Get_y_I(Node1[6].x);
		Node1[7].x = Node1[3].x;
		Node1[7].y = Get_y_I(Node1[7].x);
	}
	else if (m_type == 2)
	{
		Get_Nodes4(400, Node1);
		Node1[4].x = Node1[2].x - c;	Node1[4].y = Get_y(Node1[4].x);
		Node1[5].x = Node1[3].x + c;	Node1[5].y = Get_y(Node1[5].x);
		Node1[6].x = Node1[2].x;		Node1[6].y = Get_y(Node1[6].x);
		Node1[7].x = Node1[3].x;		Node1[7].y = Get_y(Node1[7].x);
	}

	Node1[4].z = Node1[2].z - b;
	Node1[5].z = Node1[3].z - b;
	Node1[6].z = Node1[2].z - 2 * b;
	Node1[7].z = Node1[3].z - 2 * b;
	interpolation(Node1[4], Node1[5], 0.5, Node1[8]);//11

	for (int i = 0; i < 9; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	InPutSuspension({ node[8] ,wireLogo });
	realSuspoint.push_back(node[2]);
	realSuspoint.push_back(node[3]);
	realSuspoint.push_back(node[4]);
	realSuspoint.push_back(node[5]);
	realSuspoint.push_back(node[6]);
	realSuspoint.push_back(node[7]);
	if (m_type == 1)
	{
		Creat_Trusses(m_Elements_Trusses, { node[0],node[1] });
		CreatWireBeams(m_Elements_beams, { node[2],node[1],node[3] });
		CreatWireBeams(m_Elements_beams, { node[2],node[4],node[6],node[7],node[5],node[3]});
		SuspensionElement.push_back(1);
		SuspensionElementClass.push_back(1);
		for (int i = 0; i < m_Elements_Trusses.size(); i++)
		{
			m_Elements_Trusses[i].ClassSectionID = 2;
			m_Elements_Trusses[i].MaterialID = 5;
		}
		for (int i = 0; i < m_Elements_beams.size(); i++)
		{
			m_Elements_beams[i].ClassSectionID = 3;
			m_Elements_beams[i].MaterialID = 6;
		}
	}
	else if (m_type == 2)
	{
		Creat_Trusses(m_Elements_Trusses, { node[0],node[2] });
		Creat_Trusses(m_Elements_Trusses, { node[3],node[1] });
		CreatWireBeams(m_Elements_beams, { node[2],node[4],node[6],node[7],node[5],node[3],node[2] });
		SuspensionElement.push_back(1);
		SuspensionElement.push_back(2);
		SuspensionElementClass.push_back(2);
		for (int i = 0; i < m_Elements_Trusses.size(); i++)
		{
			m_Elements_Trusses[i].ClassSectionID = 2;
			m_Elements_Trusses[i].MaterialID = 5;
		}
		for (int i = 0; i < m_Elements_beams.size(); i++)
		{
			m_Elements_beams[i].ClassSectionID = 3;
			m_Elements_beams[i].MaterialID = 6;
		}
	}

}

void TowerPart_Insulator::Get_split8(int m_type)
{
	Node Node1[11];
	int node[11];
	double b =sqrt(80000);
	if (m_type == 1)
	{
		Get_Nodes4_I(400, Node1);
		Node1[4].x = Node1[2].x - b;
		Node1[4].y = Get_y_I(Node1[4].x);
		Node1[5].x = Node1[3].x + b;
		Node1[5].y = Get_y_I(Node1[5].x);
		Node1[6].x = Node1[2].x - b;
		Node1[6].y = Get_y_I(Node1[6].x);
		Node1[7].x = Node1[3].x + b;
		Node1[7].y = Get_y_I(Node1[7].x);
		Node1[8].x = Node1[2].x;
		Node1[8].y = Get_y_I(Node1[8].x);
		Node1[9].x = Node1[3].x;
		Node1[9].y = Get_y_I(Node1[9].x);
	}
	else if (m_type == 2)
	{
		Get_Nodes4(400, Node1);
		Node1[4].x = Node1[2].x - b;	Node1[4].y = Get_y(Node1[4].x);
		Node1[5].x = Node1[3].x + b;	Node1[5].y = Get_y(Node1[5].x);
		Node1[6].x = Node1[2].x - b;	Node1[6].y = Get_y(Node1[6].x);
		Node1[7].x = Node1[3].x + b;	Node1[7].y = Get_y(Node1[7].x);
		Node1[8].x = Node1[2].x;		Node1[8].y = Get_y(Node1[8].x);
		Node1[9].x = Node1[3].x;		Node1[9].y = Get_y(Node1[9].x);
	}
	Node1[4].z = Node1[2].z - b;
	Node1[5].z = Node1[3].z - b;
	Node1[6].z = Node1[2].z - b - 400;
	Node1[7].z = Node1[3].z - b - 400;
	Node1[8].z = Node1[2].z - 2 * b - 400;
	Node1[9].z = Node1[2].z - 2 * b - 400;
	interpolation(Node1[2], Node1[9], 0.5, Node1[10]);//1111

	for (int i = 0; i < 11; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	InPutSuspension({ node[10], wireLogo });
	realSuspoint.push_back(node[2]);
	realSuspoint.push_back(node[3]);
	realSuspoint.push_back(node[4]);
	realSuspoint.push_back(node[5]);
	realSuspoint.push_back(node[6]);
	realSuspoint.push_back(node[7]);
	realSuspoint.push_back(node[8]);
	realSuspoint.push_back(node[9]);
	if (m_type == 1)
	{
		Creat_Trusses(m_Elements_Trusses, { node[0],node[1] });
		CreatWireBeams(m_Elements_beams, { node[2],node[1],node[3] });
		CreatWireBeams(m_Elements_beams, { node[2],node[4],node[6],node[8],node[9],node[7],node[5],node[3] });
		SuspensionElement.push_back(1);
		SuspensionElementClass.push_back(1);
		for (int i = 0; i < m_Elements_Trusses.size(); i++)
		{
			m_Elements_Trusses[i].ClassSectionID = 2;
			m_Elements_Trusses[i].MaterialID = 5;

		}
		for (int i = 0; i < m_Elements_beams.size(); i++)
		{
			m_Elements_beams[i].ClassSectionID = 3;
			m_Elements_beams[i].MaterialID = 6;
		}
	}
	else if (m_type == 2)
	{
		Creat_Trusses(m_Elements_Trusses, { node[0],node[2] });
		CreatWireBeams(m_Elements_beams, { node[3],node[1] });
		CreatWireBeams(m_Elements_beams, { node[2],node[4],node[6],node[8],node[9],node[7],node[5],node[3] ,node[2] });
		SuspensionElement.push_back(1);
		SuspensionElement.push_back(2);
		SuspensionElementClass.push_back(2);
		for (int i = 0; i < m_Elements_Trusses.size(); i++)
		{
			m_Elements_Trusses[i].ClassSectionID = 2;
			m_Elements_Trusses[i].MaterialID = 5;

		}
		for (int i = 0; i < m_Elements_beams.size(); i++)
		{
			m_Elements_beams[i].ClassSectionID = 3;
			m_Elements_beams[i].MaterialID = 6;
		}
	}
	
}

void TowerPart_Insulator::Create_Mesh()
{

	switch (m_splits)//塔腿类型
	{
	case 1:
		if (m_type == 1)		{ Get_split1_I(); }
		else if (m_type == 2)	{ Get_split1(); }
		break;

	case 2:
		if (m_type == 1)		{ Get_split2_I(); }
		else if (m_type == 2)	{ Get_split2(); }
		break;
	case 4:
		Get_split4(m_type);
		break;
	case 6:
		Get_split6(m_type);
		break;
	case 8:
		Get_split8(m_type);
		break;
	}
	
}
