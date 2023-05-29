#include "TowerPart_septum.h"
void TowerPart_septum::Get_Node4(int Tier, Node* n)
{
	double h =Get_LayerH(Tier);
	double l =Get_LayerL(Tier);

	double a = l / 2;
	n[0].x = a; n[0].y = a;
	n[1].x = -a; n[1].y = a;
	n[2].x = -a; n[2].y = -a;
	n[3].x = a; n[3].y = -a;
	for (int i = 0; i < 4; i++)
	{
		n[i].z = h;
	}
}

void TowerPart_septum::Type1(int tier)
{
	Node node1[24];
	int node[24];
	Get_Node4(tier, node1);

	double l =Get_LayerL(tier);
	double h =Get_LayerH(tier);
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

void TowerPart_septum::Type2(int tier)
{
	Node node2[5];
	int node[5];
	Get_Node4(tier, node2);
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

void TowerPart_septum::Type3(int tier)
{
	Node node2[9];
	int node[9];
	Get_Node4(tier, node2);
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

void TowerPart_septum::Type4(int tier)
{
	Node node2[12];
	int node[12];
	Get_Node4(tier, node2);
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

void TowerPart_septum::Type5(int tier)
{
	Node node2[12];
	int node[12];
	Get_Node4(tier, node2);
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

void TowerPart_septum::Create_Mesh()
{
	//如果循环里面i==1开始，身部的隔面加在上部
	//如果循环里面i==0开始，身部的隔面加在下部

	for (int i = 0; i <m_Tier; i++)//i--层数 从第1层开始算
	{
		int g_type =m_layer[i].m_TypeSeptum;//数组里面[i]个位置表示第i+1层型号
		switch (g_type)
		{
		case 1:
			Type1(i);
			break;
		case 2:
			Type2(i);
			break;
		case 3:
			Type3(i);
			break;
		case 4:
			Type4(i);
			break;
		case 5:
			Type5(i);
			break;
		default:break;
		}
	}
}