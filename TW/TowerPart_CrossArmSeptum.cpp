#include "TowerPart_CrossArmSeptum.h"

void TowerPart_CrossArmSeptum::Get_Node4(int type, Node* n, int iQuadrant, int layer)
{
	double W1 = 0, W2 = 0, H1 = 0, H2 = 0;
	if (type == 1)//支架
	{
		W1 =Get_layerWi(type + 1, layer);//支架下宽
		W2 =Get_layerWi(type, layer);//支架水平宽
		H1 =Get_layerHi(type + 1, layer);//支架下高
		H2 =Get_layerHi(type , layer);//支架水平高
	}
	else if (type == 3)//横担
	{
		W1 =Get_layerWi(type, layer);//横担水平
		W2 =Get_layerWi(type + 1, layer);//横担上宽
		H1 =Get_layerHi(type, layer);//横担水平高
		H2 =Get_layerHi(type + 1, layer);//横担上高
	}
	double X =Get_layerLi(type, layer);//x坐标

	double w1 = W1 / 2; double w2 = W2 / 2;

	//右边点的坐标
	n[0].x = X; n[0].y = -w1; n[0].z = H1;
	n[1].x = X; n[1].y = w1; n[1].z = H1;
	n[2].x = X; n[2].y = w2; n[2].z = H2;
	n[3].x = X; n[3].y = -w2; n[3].z = H2;
	//左边点坐标-只有x值发生改变
	if (iQuadrant == 1)//左
	{
		n[0].x *= -1;	 n[1].x *= -1;	 n[2].x *= -1;	n[3].x *= -1;
	}
}

void TowerPart_CrossArmSeptum::Create_point(const Node& n1, const Node& n2, const Node& n3, const Node& n4, Node& n)
{
	double k1 = (n2.z - n1.z) / (n2.y - n1.y);
	double b1 = n1.z - k1 * n1.y;
	double k2 = (n4.z - n3.z) / (n4.y - n3.y);
	double b2 = n3.z - k2 * n3.y;
	n.x = n1.x;
	n.y = (b2 - b1) / (k1 - k2);
	n.z = k1 * n.y + b1;
}

void TowerPart_CrossArmSeptum::Type1(int type, int iQuadrant, int layer)
{
	Node node1[24];
	int node[24];
	Get_Node4(type, node1, iQuadrant, layer);
	for (size_t i = 4; i < 16; i++)
	{
		if (i < 7)
		{
			interpolation(node1[0], node1[1], (i - 3) * 0.25, node1[i]);
		}
		else if (i < 10)
		{
			interpolation(node1[1], node1[2], (i - 6) * 0.25, node1[i]);
		}
		else if (i < 13)
		{
			interpolation(node1[2], node1[3], (i - 9) * 0.25, node1[i]);
		}
		else if (i < 16)
		{
			interpolation(node1[3], node1[0], (i - 12) * 0.25, node1[i]);
		}
	}
	Create_point(node1[4], node1[13], node1[6], node1[15], node1[16]);
	Create_point(node1[4], node1[7], node1[6], node1[15], node1[17]);
	Create_point(node1[4], node1[7], node1[6], node1[9], node1[18]);
	Create_point(node1[10], node1[7], node1[6], node1[9], node1[19]);
	Create_point(node1[10], node1[7], node1[12], node1[9], node1[20]);
	Create_point(node1[10], node1[13], node1[12], node1[9], node1[21]);
	Create_point(node1[10], node1[13], node1[12], node1[15], node1[22]);
	Create_point(node1[4], node1[13], node1[12], node1[15], node1[23]);
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

void TowerPart_CrossArmSeptum::Type2(int type, int iQuadrant, int layer)
{
	Node node2[5];
	int node[5];
	Get_Node4(type, node2, iQuadrant, layer);
	Create_point(node2[0], node2[2], node2[1], node2[3], node2[4]);
	for (int i = 0; i < 5; i++)
	{
		node[i] = Creat_Node(node2[i].x, node2[i].y, node2[i].z);

	}
	//生成单元
	Creat_Beams(m_Elements_beams, { node[0],node[1],node[2],node[3],node[0] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[0],node[4],node[1] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[2],node[4],node[3] });//一串梁单元
}

void TowerPart_CrossArmSeptum::Type3(int type, int iQuadrant, int layer)
{
	Node node2[9];
	int node[9];
	Get_Node4(type, node2, iQuadrant, layer);
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

void TowerPart_CrossArmSeptum::Type4(int type, int iQuadrant, int layer)
{
	Node node2[12];
	int node[12];
	Get_Node4(type, node2, iQuadrant, layer);
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

void TowerPart_CrossArmSeptum::Type5(int type, int iQuadrant, int layer)
{
	Node node2[12];
	int node[12];
	Get_Node4(type, node2, iQuadrant, layer);
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

void TowerPart_CrossArmSeptum::Create_Mesh()
{
	for (int i = 0; i <m_count; i++)//从底面开始
	{
		int g_type =m_layerArm[i].m_TypeSeptum;//隔面型号
		int type =m_Type; // 支架1或横担3
		int postion =m_position;//位置 0--右边 1--左边 2--两侧
		if (postion == 0 || postion == 1)//   ||表示或者
		{
			switch (g_type)
			{
			case 1:
				Type1(type, postion, i);
				break;
			case 2:
				Type2(type, postion, i);
				break;
			case 3:
				Type3(type, postion, i);
				break;
			case 4:
				Type4(type, postion, i);
				break;
			case 5:
				Type5(type, postion, i);
				break;
			default:break;
			}
		}
		else if (postion == 2)
		{
			switch (g_type)
			{
			case 1:
				Type1(type, 0, i);
				break;
			case 2:
				Type2(type, 0, i);
				break;
			case 3:
				Type3(type, 0, i);
				break;
			case 4:
				Type4(type, 0, i);
				break;
			case 5:
				Type5(type, 0, i);
				break;
			default:break;
			}
			switch (g_type)
			{
			case 1:
				Type1(type, 1, i);
				break;
			case 2:
				Type2(type, 1, i);
				break;
			case 3:
				Type3(type, 1, i);
				break;
			case 4:
				Type4(type, 1, i);
				break;
			case 5:
				Type5(type, 1, i);
				break;
			default:break;
			}
		}
	}
}
