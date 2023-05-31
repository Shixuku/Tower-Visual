#include "TowerPart_Insulator.h"

void TowerPart_Insulator::Get_split4()
{
	Node Node1[6];
	int node[6];
	double disX = m_node2->x - m_node1->x;//����֮�䳤��
	double a = 450;//�ķ��ѳ���
	Node1[0].x = m_node1->x + disX / 2 - a / 2;
	Node1[1].x = m_node1->x + disX / 2 + a / 2;
	Node1[2].x = Node1[1].x;
	Node1[3].x = Node1[0].x;
	Node1[0].y = Node1[3].y = Get_y(Node1[0].x);
	Node1[2].y = Node1[1].y = Get_y(Node1[2].x);
	Node1[0].z = Node1[1].z = m_node1->z - m_H;
	Node1[2].z = Node1[3].z = m_node1->z - m_H - a;
	Node1[4] =*m_node1;
	Node1[5] =*m_node2;
	for (int i = 0; i < 6; i++)//
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	//���ɵ�Ԫ
	Creat_Trusses(m_Elements_Trusses, { node[0],node[1], node[2],node[3],node[0] });
	Creat_Trusses(m_Elements_Trusses, { node[0],node[4]});
	Creat_Trusses(m_Elements_Trusses, { node[1],node[5]});
}

void TowerPart_Insulator::Create_Mesh()
{
	switch (m_splits)//��������
	{
	case 1:
		break;
	case 2:
		break;
	case 4:
		Get_split4();
		break;
	case 6:
		break;
	case 8:
		break;
	}
}
