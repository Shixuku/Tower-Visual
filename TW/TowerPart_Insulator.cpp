#include "TowerPart_Insulator.h"

void TowerPart_Insulator::Get_Nodes4(double a,Node* n)
{
	n[0]= *m_node1;
	n[1]= *m_node2;
	double disX = n[1].x - n[0].x;//两点之间长度
	n[2].x = n[0].x + disX / 2 - a / 2;
	n[2].y = Get_y(n[2].x);
	n[2].z = n[0].z - m_H;

	n[3].x = n[0].x + disX / 2 + a / 2;
	n[3].y = Get_y(n[3].x);
	n[3].z = n[1].z - m_H;
}

void TowerPart_Insulator::Get_split1()
{
	Node Node1[3];
	int node[3];
	Node1[0] = *m_node1;
	Node1[1] = *m_node2;
	double disX = m_node2->x - m_node1->x;//两点之间长度
	Node1[2].x = m_node1->x + disX / 2;
	Node1[2].y= Get_y(Node1[3].x);
	Node1[2].z = m_node1->z - m_H;
	for (int i = 0; i < 3; i++)//
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	//生成单元
	Creat_Trusses(m_Elements_Trusses, { node[0],node[2],node[1] });
}

void TowerPart_Insulator::Get_split2()
{
	Node Node1[5];
	int node[5];
	Get_Nodes4(450, Node1);
	interpolation(Node1[2], Node1[3], 0.5, Node1[4]);
	for (int i = 0; i < 5; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	Creat_Trusses(m_Elements_Trusses, { node[0],node[2], node[4],node[3],node[1] });
}

void TowerPart_Insulator::Get_split4()
{
	Node Node1[7];
	int node[7];
	Get_Nodes4(450, Node1);
	Node1[4].x = Node1[2].x;
	Node1[4].y= Get_y(Node1[2].x);
	Node1[4].z = Node1[2].z - 450;

	Node1[5].x = Node1[3].x;
	Node1[5].x= Get_y(Node1[3].x);
	Node1[5].z = Node1[3].z - 450;

	Node1[6].x = Node1[2].x + 450 / 2;
	Node1[6].y = Get_y(Node1[6].x);
	Node1[6].z = Node1[2].z - 450 / 2;

	for (int i = 0; i < 7; i++)//
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	//生成单元
	Creat_Trusses(m_Elements_Trusses, { node[0],node[2],node[3],node[1] });
	Creat_Trusses(m_Elements_Trusses, { node[3],node[5],node[4],node[2] });
}

void TowerPart_Insulator::Get_split6()
{
	Node Node1[9];
	int node[9];
	Get_Nodes4(400, Node1);
	double b = 200 * sqrt(3);
	double c = 200;

	Node1[4].x = Node1[2].x - c;	Node1[4].y = Get_y(Node1[4].x);		Node1[4].z = Node1[2].z - b;

	Node1[5].x = Node1[3].x + c;	Node1[5].y = Get_y(Node1[5].x);		Node1[5].z = Node1[3].z - b;

	Node1[6].x = Node1[2].x;		Node1[6].y = Get_y(Node1[6].x);		Node1[6].z = Node1[2].z - 2 * b;

	Node1[7].x = Node1[3].x;		Node1[7].y = Get_y(Node1[7].x);		Node1[7].z = Node1[3].z - 2 * b;

	interpolation(Node1[4], Node1[5], 0.5, Node1[8]);
	for (int i = 0; i < 9; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	Creat_Trusses(m_Elements_Trusses, { node[0],node[2],node[3],node[1] });
	Creat_Trusses(m_Elements_Trusses, { node[2],node[4],node[6],node[7],node[5],node[3] });

}

void TowerPart_Insulator::Get_split8()
{
	Node Node1[11];
	int node[11];
	Get_Nodes4(400, Node1);
	double b =sqrt(80000);
	Node1[4].x = Node1[2].x - b;	Node1[4].y = Get_y(Node1[4].x);		Node1[4].z = Node1[2].z - b;

	Node1[5].x = Node1[3].x + b;	Node1[5].y = Get_y(Node1[5].x);		Node1[5].z = Node1[3].z - b;

	Node1[6].x = Node1[2].x - b;	Node1[6].y = Get_y(Node1[6].x);		Node1[6].z = Node1[2].z - b - 400;

	Node1[7].x = Node1[3].x + b;	Node1[7].y = Get_y(Node1[7].x);		Node1[7].z = Node1[3].z - b - 400;

	Node1[8].x = Node1[2].x;		Node1[8].y = Get_y(Node1[8].x);		Node1[8].z = Node1[2].z - 2 * b - 400;

	Node1[9].x = Node1[3].x;		Node1[9].y = Get_y(Node1[9].x);		Node1[9].z = Node1[2].z - 2 * b - 400;

	interpolation(Node1[2], Node1[9], 0.5, Node1[10]);

	for (int i = 0; i < 11; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	Creat_Trusses(m_Elements_Trusses, { node[0],node[2],node[3],node[1] });
	Creat_Trusses(m_Elements_Trusses, { node[2],node[4],node[6],node[8],node[9],node[7],node[5],node[3] });
}

void TowerPart_Insulator::Create_Mesh()
{
	switch (m_splits)//塔腿类型
	{
	case 1:
		Get_split1();
		break;
	case 2:
		Get_split2();
		break;
	case 4:
		Get_split4();
		break;
	case 6:
		Get_split6();
		break;
	case 8:
		Get_split8();
		break;
	}
}
