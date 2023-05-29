#include "TowerPart_leg.h"

void TowerPart_leg::Get_Node4(int iQuadrant, Node* n)
{
	double a = m_L2 / 2;
	if (iQuadrant == 0)//第一象限
	{
		double x = Get_PracticlL(0) / 2; double z = Get_PracticlH(0);
		n[0].x = x;			n[0].y = x;			n[0].z = -z;
		n[1].x = a;			n[1].y = a;			n[1].z = 0;
		n[2].x = a;			n[2].y = 0;			n[2].z = 0;
		n[3].x = 0;			n[3].y = a;			n[3].z = 0;
	}
	else if (iQuadrant == 1)//第二象限
	{
		double x = Get_PracticlL(1) / 2; double z = Get_PracticlH(1);
		n[0].x = -x;		n[0].y = x;			n[0].z = -z;
		n[1].x = -a;		n[1].y = a;			n[1].z = 0;
		n[2].x = 0;			n[2].y = a;			n[2].z = 0;
		n[3].x = -a;		n[3].y = 0;			n[3].z = 0;
	}
	else if (iQuadrant == 2)//第三象限
	{
		double x = Get_PracticlL(2) / 2; double z = Get_PracticlH(2);
		n[0].x = -x;		n[0].y = -x;		n[0].z = -z;
		n[1].x = -a;		n[1].y = -a;		n[1].z = 0;
		n[2].x = -a;		n[2].y = 0;			n[2].z = 0;
		n[3].x = 0;			n[3].y = -a;		n[3].z = 0;
	}
	else//第四象限
	{
		double x = Get_PracticlL(3) / 2; double z = Get_PracticlH(3);
		n[0].x = x;			n[0].y = -x;		n[0].z = -z;
		n[1].x = a;			n[1].y = -a;		n[1].z = 0;
		n[2].x = 0;			n[2].y = -a;		n[2].z = 0;
		n[3].x = a;			n[3].y = 0;			n[3].z = 0;
	}
}



void TowerPart_leg::Type1(int iQuadrant)
{
	//生成节点
	Node Node1[12];
	int node[12];
	Get_Node4(iQuadrant, Node1);//得到直接由数据给出的点

	//根据两点算插值点
	interpolation(Node1[1], Node1[2], 0.5, Node1[4]);
	interpolation(Node1[1], Node1[3], 0.5, Node1[5]);

	interpolation(Node1[0], Node1[1], 0.35, Node1[6]);
	interpolation(Node1[0], Node1[1], 0.675, Node1[7]);

	interpolation(Node1[0], Node1[2], 0.35, Node1[8]);
	interpolation(Node1[0], Node1[2], 0.675, Node1[9]);

	interpolation(Node1[0], Node1[3], 0.35, Node1[10]);
	interpolation(Node1[0], Node1[3], 0.675, Node1[11]);

	for (int i = 0; i < 12; i++)//
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);

	}
	//生成单元
	Creat_Beams(m_Elements_beams, { node[0],node[6],node[7],node[1] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[0],node[8],node[9],node[2] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[0],node[10],node[11],node[3] });//一串梁单元

	Creat_Trusses(m_Elements_Trusses, { node[8],node[6],node[10] });//一圈桁架单元
	Creat_Trusses(m_Elements_Trusses, { node[8],node[10] });//一圈桁架单元
	Creat_Trusses(m_Elements_Trusses, { node[9],node[7],node[11] });//一圈桁架单元
	Creat_Trusses(m_Elements_Trusses, { node[9],node[11] });//一圈桁架单元
	Creat_Trusses(m_Elements_Trusses, { node[9],node[4] });//一圈桁架单元
	Creat_Trusses(m_Elements_Trusses, { node[5],node[11] });//一圈桁架单元
	Creat_Trusses(m_Elements_Trusses, { node[4],node[7],node[5] });//一串桁架单元

}

void TowerPart_leg::Type2(int iQuadrant)
{
	//生成节点
	Node Node2[10];
	int node[10];
	Get_Node4(iQuadrant, Node2);//得到直接由数据给出的点

	//根据两点算插值点

	interpolation(Node2[0], Node2[1], 0.35, Node2[4]);
	interpolation(Node2[0], Node2[1], 0.675, Node2[5]);

	interpolation(Node2[0], Node2[2], 0.35, Node2[6]);
	interpolation(Node2[0], Node2[2], 0.675, Node2[7]);

	interpolation(Node2[0], Node2[3], 0.35, Node2[8]);
	interpolation(Node2[0], Node2[3], 0.675, Node2[9]);

	for (int i = 0; i < 10; i++)
	{
		node[i] = Creat_Node(Node2[i].x, Node2[i].y, Node2[i].z);
	}

	//生成单元

	Creat_Beams(m_Elements_beams, { node[0],node[4],node[5],node[1] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[0],node[6],node[7],node[2] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[0],node[8],node[9],node[3] });//一串梁单元

	Creat_Trusses(m_Elements_Trusses, { node[6],node[4],node[8] });
	Creat_Trusses(m_Elements_Trusses, { node[6],node[8] });
	Creat_Trusses(m_Elements_Trusses, { node[7],node[5],node[9] });
	Creat_Trusses(m_Elements_Trusses, { node[7],node[9] });

	Creat_Trusses(m_Elements_Trusses, { node[6],node[5],node[8] });//一串桁架单元
	Creat_Trusses(m_Elements_Trusses, { node[7],node[1],node[9] });//一串桁架单元

}

void TowerPart_leg::Type3(int iQuadrant)
{
	//生成节点
	Node Node3[7];
	int node[7];
	Get_Node4(iQuadrant, Node3);//得到直接由数据给出的点

	//根据两点算插值点
	interpolation(Node3[0], Node3[1], 0.5, Node3[4]);
	interpolation(Node3[0], Node3[2], 0.5, Node3[5]);
	interpolation(Node3[0], Node3[3], 0.5, Node3[6]);

	for (int i = 0; i < 7; i++)
	{
		node[i] = Creat_Node(Node3[i].x, Node3[i].y, Node3[i].z);
	}

	//生成单元
	Creat_Beams(m_Elements_beams, { node[0],node[4],node[1] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[0],node[5],node[2] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[0],node[6],node[3] });//一串梁单元

	Creat_Trusses(m_Elements_Trusses, { node[5],node[4],node[6] });//一圈桁架单元
	Creat_Trusses(m_Elements_Trusses, { node[6],node[1],node[5] });//一串桁架单元
	Creat_Trusses(m_Elements_Trusses, { node[6],node[5] });//一串桁架单元

}

void TowerPart_leg::G_Get_Node4(Node* n)
{
	double a = m_L2 / 2;
	n[0].x = a; n[0].y = a;
	n[1].x = -a; n[1].y = a;
	n[2].x = -a; n[2].y = -a;
	n[3].x = a; n[3].y = -a;
	for (int i = 0; i < 4; i++)
	{
		n[i].z = 0;
	}
}
void TowerPart_leg::G_Type1()
{
	Node node1[24];
	int node[24];
	G_Get_Node4(node1);

	double l = node1[0].x - node1[1].x;
	double h = node1[0].z;
	int a = l / 2;
	for (size_t i = 4; i < 16; i++)
	{
		if (i < 7)
		{
			node1[i].x = a - (i - 3) * 0.5 * a;
			node1[i].y = a;
		}
		else if (i < 10)
		{
			node1[i].x = -a;
			node1[i].y = a - (i - 6) * 0.5 * a;
		}
		else if (i < 13)
		{
			node1[i].x = -a + (i - 9) * 0.5 * a;
			node1[i].y = -a;
		}
		else if (i < 16)
		{
			node1[i].x = a;
			node1[i].y = -a + (i - 12) * 0.5 * a;
		}
		node1[i].z = h;
	}

	Node4_Creat1(node1[4], node1[13], node1[6], node1[15], node1[16]);
	Node4_Creat1(node1[4], node1[7], node1[6], node1[15], node1[17]);
	Node4_Creat1(node1[4], node1[7], node1[6], node1[9], node1[18]);
	Node4_Creat1(node1[10], node1[7], node1[6], node1[9], node1[19]);
	Node4_Creat1(node1[10], node1[7], node1[12], node1[9], node1[20]);
	Node4_Creat1(node1[10], node1[13], node1[12], node1[9], node1[21]);
	Node4_Creat1(node1[10], node1[13], node1[12], node1[15], node1[22]);
	Node4_Creat1(node1[4], node1[13], node1[12], node1[15], node1[23]);
	for (int i = 0; i < 24; i++)
	{
		node[i] = Creat_Node(node1[i].x, node1[i].y, node1[i].z);
	}
	//生成单元
	Creat_Beams(m_Elements_beams, { node[0],node[4],node[5],node[6],node[1],node[7],node[8],node[9],node[2],node[10],node[11],node[12],node[3],node[13],node[14],node[15],node[0] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[15],node[16],node[17],node[6],node[18],node[19],node[9],node[20],node[21],node[12],node[22],node[23],node[15] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[13],node[23],node[16],node[4],node[17],node[18],node[7],node[19],node[20],node[10],node[21],node[22],node[13] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[3],node[22] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[14],node[23] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[0],node[16] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[5],node[17] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[1],node[18] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[8],node[19] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[2],node[20] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[11],node[21] });//一串梁单元
}

void TowerPart_leg::G_Type2()
{
	Node node2[5];
	int node[5];
	G_Get_Node4(node2);
	Node4_Creat1(node2[0], node2[2], node2[1], node2[3], node2[4]);
	for (int i = 0; i < 5; i++)
	{
		node[i] = Creat_Node(node2[i].x, node2[i].y, node2[i].z);
	}
	//生成单元
	Creat_Beams(m_Elements_beams, { node[0],node[1],node[2],node[3],node[0] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[0],node[4],node[1] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[2],node[4],node[3] });//一串梁单元
	
}

void TowerPart_leg::G_Type3()
{
	Node node2[9];
	int node[9];
	G_Get_Node4(node2);
	interpolation(node2[0], node2[1], 0.5, node2[4]);
	interpolation(node2[2], node2[1], 0.5, node2[5]);
	interpolation(node2[2], node2[3], 0.5, node2[6]);
	interpolation(node2[0], node2[3], 0.5, node2[7]);
	interpolation(node2[5], node2[7], 0.5, node2[8]);
	for (int i = 0; i < 9; i++)
	{
		node[i] = Creat_Node(node2[i].x, node2[i].y, node2[i].z);
	}
	//生成单元
	Creat_Beams(m_Elements_beams, { node[0],node[4],node[1],node[5],node[2],node[6],node[3],node[7],node[0] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[4],node[5],node[6],node[7],node[4] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[4],node[8],node[6] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[5],node[8],node[7] });//一串梁单元
}

void TowerPart_leg::G_Type4()
{
	Node node2[12];
	int node[12];
	G_Get_Node4(node2);
	interpolation(node2[0], node2[1], 0.5, node2[4]);
	interpolation(node2[2], node2[1], 0.5, node2[5]);
	interpolation(node2[2], node2[3], 0.5, node2[6]);
	interpolation(node2[0], node2[3], 0.5, node2[7]);
	interpolation(node2[4], node2[7], 0.5, node2[8]);
	interpolation(node2[4], node2[5], 0.5, node2[9]);
	interpolation(node2[6], node2[5], 0.5, node2[10]);
	interpolation(node2[6], node2[7], 0.5, node2[11]);
	for (int i = 0; i < 12; i++)
	{
		node[i] = Creat_Node(node2[i].x, node2[i].y, node2[i].z);
	}
	//生成单元
	Creat_Beams(m_Elements_beams, { node[0],node[4],node[1],node[5],node[2],node[6],node[3],node[7],node[0] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[8],node[4],node[9],node[5],node[10],node[6],node[11],node[7],node[8] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[8],node[9],node[10],node[11],node[8] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[0],node[8] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[1],node[9] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[2],node[10] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[3],node[11] });//一串梁单元
}

void TowerPart_leg::G_Type5()
{
	Node node2[12];
	int node[12];
	G_Get_Node4(node2);
	interpolation(node2[0], node2[1], 0.5, node2[4]);
	interpolation(node2[2], node2[1], 0.5, node2[5]);
	interpolation(node2[2], node2[3], 0.5, node2[6]);
	interpolation(node2[0], node2[3], 0.5, node2[7]);
	interpolation(node2[4], node2[7], 0.5, node2[8]);
	interpolation(node2[4], node2[5], 0.5, node2[9]);
	interpolation(node2[6], node2[5], 0.5, node2[10]);
	interpolation(node2[6], node2[7], 0.5, node2[11]);
	for (int i = 0; i < 12; i++)
	{
		node[i] = Creat_Node(node2[i].x, node2[i].y, node2[i].z);
	}
	//生成单元
	Creat_Beams(m_Elements_beams, { node[0],node[4],node[1],node[5],node[2],node[6],node[3],node[7],node[0] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[8],node[4],node[9],node[5],node[10],node[6],node[11],node[7],node[8] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[8],node[11] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[9],node[10] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[0],node[8] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[1],node[9] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[2],node[10] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[3],node[11] });//一串梁单元
}


void TowerPart_leg::Create_Mesh()
{
	switch (m_TypeSide)//塔腿类型
	{
	case 1:
		Type1(0);
		Type1(1);
		Type1(2);
		Type1(3);
		break;
	case 2:
		Type2(0);
		Type2(1);
		Type2(2);
		Type2(3);
		break;
	case 3:
		Type3(0);
		Type3(1);
		Type3(2);
		Type3(3);
		break;
	}
	switch (m_TypeSeptum)//隔面类型
	{
	case 1:
		G_Type1();
		break;
	case 2:
		G_Type2();
		break;
	case 3:
		G_Type3();
		break;
	case 4:
		G_Type4();
		break;
	case 5:
		G_Type5();
		break;
	}
}

TowerPart_leg::TowerPart_leg()
{
}

TowerPart_leg::~TowerPart_leg()
{
}
