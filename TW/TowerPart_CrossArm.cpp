#include "TowerPart_CrossArm.h"

void TowerPart_CrossArm::Get_SideNode3(int type, Node* n, int iQuadrant, int layer)//改变方向
{
	double L0 = Get_layerLi(type, layer - 1);	double L1 = Get_layerLi(type, layer);	double L2 = Get_layerLi(type + 1, layer);
	double W0 = Get_layerWi(type, layer - 1);	double W1 = Get_layerWi(type, layer);	double W2 = Get_layerWi(type + 1, layer);
	double H0 = Get_layerHi(type, layer - 1);	double H1 = Get_layerHi(type, layer);	double H2 = Get_layerHi(type + 1, layer);
	n[0].x = L0;	n[0].y = W0 / 2;	n[0].z = H0;
	n[1].x = L1;	n[1].y = W1 / 2;	n[1].z = H1;
	n[2].x = L2;	n[2].y = W2 / 2;	n[2].z = H2;
	if (iQuadrant == 1)//右 后视图--改变y坐标的符号
	{
		 n[0].y *= -1;
		 n[1].y *= -1;
		 n[2].y *= -1;
	}
	else if (iQuadrant == 2)//左 正试图--改变x坐标的符号
	{
		 n[0].x *= -1;
		 n[1].x *= -1;
		 n[2].x *= -1;
	}
	else if (iQuadrant == 3)//左 后视图--改变xy坐标的符号
	{
		 n[0].x *= -1;	n[0].y *= -1;
		 n[1].x *= -1;  n[1].y *= -1;
		 n[2].x *= -1;	n[2].y *= -1;
	}
}

void TowerPart_CrossArm::Get_BandUNode4(int type, Node* n, int iQuadrant, int layer)//type==1/2/3/4
{
	double L0 = Get_layerLi(type, layer - 1); double L1 = Get_layerLi(type, layer);
	double W0 = Get_layerWi(type, layer - 1); double W1 = Get_layerWi(type, layer);
	double H0 = Get_layerHi(type, layer - 1); double H1 = Get_layerHi(type, layer);
	n[0].x = L0; n[0].y = W0 / 2; n[0].z = H0;
	n[1].x = L1; n[1].y = W1 / 2; n[1].z = H1;
	n[2].x = L0; n[2].y = -W0 / 2; n[2].z = H0;
	n[3].x = L1; n[3].y = -W1 / 2; n[3].z = H1;
	if (iQuadrant == 1)//左
	{
		n[0].x *= -1;	 n[1].x *= -1;	 n[2].x *= -1;	n[3].x *= -1;
	}
}

void TowerPart_CrossArm::SideType1(int type, int iQuadrant, int layer)
{
	Node Node1[5];
	int node[5];
	Get_SideNode3(type, Node1, iQuadrant, layer);
	interpolation(Node1[0], Node1[1], 0.5, Node1[3]);
	interpolation(Node1[0], Node1[2], 0.5, Node1[4]);
	for (int i = 0; i < 5; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	Creat_Beams(m_Elements_beams, { node[0], node[4], node[2], node[1] });
	Creat_Trusses(m_Elements_Trusses, { node[1],node[4],node[3] });
}

void TowerPart_CrossArm::SideType2(int type, int iQuadrant, int layer)
{
	Node Node1[3];
	int node[3];
	Get_SideNode3(type, Node1, iQuadrant, layer);
	for (int i = 0; i < 3; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	Creat_Beams(m_Elements_beams, { node[0], node[2], node[1] });
}

void TowerPart_CrossArm::SideType3(int type, int iQuadrant, int layer)
{
	Node Node1[3];
	int node[3];
	Get_SideNode3(type, Node1, iQuadrant, layer);
	for (int i = 0; i < 3; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	Creat_Beams(m_Elements_beams, { node[0], node[2], node[1] });
}

void TowerPart_CrossArm::BandU_Type1(int type, int iQuadrant, int layer)
{
	Node Node1[11];
	int node[11];
	Get_BandUNode4(type, Node1, iQuadrant, layer);
	Node4_Creat1(Node1[0], Node1[3], Node1[1], Node1[2], Node1[4]);
	interpolation(Node1[0], Node1[4], 0.5, Node1[5]);
	interpolation(Node1[1], Node1[4], 0.5, Node1[6]);
	interpolation(Node1[2], Node1[4], 0.5, Node1[7]);
	interpolation(Node1[3], Node1[4], 0.5, Node1[8]);
	interpolation(Node1[0], Node1[1], 0.5, Node1[9]);
	interpolation(Node1[2], Node1[3], 0.5, Node1[10]);
	for (int i = 0; i < 11; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	Creat_Beams(m_Elements_beams, { node[0],node[9],node[1],node[6],node[4],node[5],node[0] });//一圈桁架单元
	Creat_Beams(m_Elements_beams, { node[2],node[10],node[3],node[8],node[4],node[7],node[2] });//一圈桁架单元
	Creat_Trusses(m_Elements_Trusses, { node[9],node[5],node[6],node[9] });//一圈桁架单元
	Creat_Trusses(m_Elements_Trusses, { node[8],node[7],node[10],node[8] });//一圈桁架单元
}

void TowerPart_CrossArm::BandU_Type2(int type, int iQuadrant, int layer)
{
	Node Node1[5];
	int node[5];
	Get_BandUNode4(type, Node1, iQuadrant, layer);
	Node4_Creat1(Node1[0], Node1[3], Node1[1], Node1[2], Node1[4]);
	for (int i = 0; i < 5; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	Creat_Beams(m_Elements_beams, { node[0],node[1],node[4],node[2],node[3],node[4],node[0] });
}

void TowerPart_CrossArm::BandU_Type3(int type, int iQuadrant, int layer)
{
	Node Node1[5];
	int node[5];
	Get_BandUNode4(type, Node1, iQuadrant, layer);
	interpolation(Node1[1], Node1[3], 0.5, Node1[4]);
	for (int i = 0; i < 5; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	Creat_Beams(m_Elements_beams, { node[0],node[1],node[4],node[3],node[2],node[4],node[0] });
}

void TowerPart_CrossArm::BandU_Type4(int type, int iQuadrant, int layer)
{
	Node Node1[5];
	int node[5];
	Get_BandUNode4(type, Node1, iQuadrant, layer);
	Node4_Creat1(Node1[0], Node1[3], Node1[1], Node1[2], Node1[4]);
	for (int i = 0; i < 5; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	Creat_Beams(m_Elements_beams, { node[0],node[1],node[4],node[2],node[3],node[4],node[0] });
	Creat_Beams(m_Elements_beams, { node[1],node[3] });
}

void TowerPart_CrossArm::G_Get_Node4(int type, Node* n, int iQuadrant, int layer)
{
	double W1 = 0, W2 = 0, H1 = 0, H2 = 0;
	if (type == 1)//支架
	{
		W1 = Get_layerWi(type + 1, layer);//支架下宽
		W2 = Get_layerWi(type, layer);//支架水平宽
		H1 = Get_layerHi(type + 1, layer);//支架下高
		H2 = Get_layerHi(type, layer);//支架水平高
	}
	else if (type == 3)//横担
	{
		W1 = Get_layerWi(type, layer);//横担水平
		W2 = Get_layerWi(type + 1, layer);//横担上宽
		H1 = Get_layerHi(type, layer);//横担水平高
		H2 = Get_layerHi(type + 1, layer);//横担上高
	}
	double X = Get_layerLi(type, layer);//x坐标

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

void TowerPart_CrossArm::G_Create_point(const Node& n1, const Node& n2, const Node& n3, const Node& n4, Node& n)
{
	double k1 = (n2.z - n1.z) / (n2.y - n1.y);
	double b1 = n1.z - k1 * n1.y;
	double k2 = (n4.z - n3.z) / (n4.y - n3.y);
	double b2 = n3.z - k2 * n3.y;
	n.x = n1.x;
	n.y = (b2 - b1) / (k1 - k2);
	n.z = k1 * n.y + b1;
}

void TowerPart_CrossArm::G_Type1(int type, int iQuadrant, int layer)
{
	Node node1[24];
	int node[24];
	G_Get_Node4(type, node1, iQuadrant, layer);
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
	G_Create_point(node1[4], node1[13], node1[6], node1[15], node1[16]);
	G_Create_point(node1[4], node1[7], node1[6], node1[15], node1[17]);
	G_Create_point(node1[4], node1[7], node1[6], node1[9], node1[18]);
	G_Create_point(node1[10], node1[7], node1[6], node1[9], node1[19]);
	G_Create_point(node1[10], node1[7], node1[12], node1[9], node1[20]);
	G_Create_point(node1[10], node1[13], node1[12], node1[9], node1[21]);
	G_Create_point(node1[10], node1[13], node1[12], node1[15], node1[22]);
	G_Create_point(node1[4], node1[13], node1[12], node1[15], node1[23]);
	for (int i = 0; i < 24; i++)
	{
		node[i] = Creat_Node(node1[i].x, node1[i].y, node1[i].z);
	}
	//生成单元
	Creat_Beams(m_Elements_beams, { node[0],node[4],node[5],node[6],node[1] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[2],node[10],node[11],node[12],node[3] });//一圈梁单元
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

void TowerPart_CrossArm::G_Type2(int type, int iQuadrant, int layer)
{
	Node node2[5];
	int node[5];
	G_Get_Node4(type, node2, iQuadrant, layer);
	G_Create_point(node2[0], node2[2], node2[1], node2[3], node2[4]);
	for (int i = 0; i < 5; i++)
	{
		node[i] = Creat_Node(node2[i].x, node2[i].y, node2[i].z);

	}
	//生成单元
	Creat_Beams(m_Elements_beams, { node[0],node[1] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[2],node[3] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[0],node[4],node[1] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[2],node[4],node[3] });//一串梁单元
}

void TowerPart_CrossArm::G_Type3(int type, int iQuadrant, int layer)
{
	Node node2[9];
	int node[9];
	G_Get_Node4(type, node2, iQuadrant, layer);
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
	Creat_Beams(m_Elements_beams, { node[0],node[4],node[1] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[2],node[6],node[3] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[4],node[5],node[6],node[7],node[4] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[4],node[8],node[6] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[5],node[8],node[7] });//一串梁单元
}

void TowerPart_CrossArm::G_Type4(int type, int iQuadrant, int layer)
{
	Node node2[12];
	int node[12];
	G_Get_Node4(type, node2, iQuadrant, layer);
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
	Creat_Beams(m_Elements_beams, { node[0],node[4],node[1] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[2],node[6],node[3] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[8],node[4],node[9],node[5],node[10],node[6],node[11],node[7],node[8] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[8],node[9],node[10],node[11],node[8] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[0],node[8] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[1],node[9] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[2],node[10] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[3],node[11] });//一串梁单元
}

void TowerPart_CrossArm::G_Type5(int type, int iQuadrant, int layer)
{
	Node node2[12];
	int node[12];
	G_Get_Node4(type, node2, iQuadrant, layer);
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
	Creat_Beams(m_Elements_beams, { node[0],node[4],node[1] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[2],node[6],node[3] });//一圈梁单元
	Creat_Beams(m_Elements_beams, { node[8],node[4],node[9],node[5],node[10],node[6],node[11],node[7],node[8] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[8],node[11] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[9],node[10] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[0],node[8] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[1],node[9] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[2],node[10] });//一串梁单元
	Creat_Beams(m_Elements_beams, { node[3],node[11] });//一串梁单元
}


void TowerPart_CrossArm::Create_Mesh()
{
	for (int i = 1; i < m_count + 1; i++)
	{
		int up_type = m_layerArm[i - 1].m_TypeUp;
		int bu_type = m_layerArm[i - 1].m_TypeButtom;
		int side_type = m_layerArm[i - 1].m_TypeSide;
		int g_type = m_layerArm[i - 1].m_TypeSeptum;//隔面型号
		if (m_Type == 1)//支架
		{
			//顶面 
			switch (up_type)
			{
			case 1:
				BandU_Type1(1, m_position, i);// 0表示右
				break;
			case 2:
				BandU_Type2(1, m_position, i);
				break;
			case 3:
				BandU_Type3(1, m_position, i);
				break;
			case 4:
				BandU_Type4(1, m_position, i);
				break;
			default:break;
			}
			//底面
			switch (bu_type)
			{
			case 1:
				BandU_Type1(2, m_position, i);// 0表示右
				break;
			case 2:
				BandU_Type2(2, m_position, i);
				break;
			case 3:
				BandU_Type3(2, m_position, i);
				break;
			case 4:
				BandU_Type4(2, m_position, i);
				break;
			default:break;
			}
		}
		else if (m_Type == 3)//横担
		{
			//顶面 
			switch (up_type)
			{
			case 1:
				BandU_Type1(4, m_position, i);// 0表示右
				break;
			case 2:
				BandU_Type2(4, m_position, i);
				break;
			case 3:
				BandU_Type3(4, m_position, i);
				break;
			case 4:
				BandU_Type4(4, m_position, i);
				break;
			default:break;
			}
			//底面
			switch (bu_type)
			{
			case 1:
				BandU_Type1(3, m_position, i);// 0表示右
				break;
			case 2:
				BandU_Type2(3, m_position, i);
				break;
			case 3:
				BandU_Type3(3, m_position, i);
				break;
			case 4:
				BandU_Type4(3, m_position, i);
				break;
			default:break;
			}
		}

		//右边侧面
		if (m_position == 0)
		{
			switch (side_type)
			{
			case 1:
				SideType1(m_Type, 0, i);//0 右前
				SideType1(m_Type, 1, i);//1 右后
				break;
			case 2:
				SideType2(m_Type, 0, i);//0 右前
				SideType2(m_Type, 1, i);//1 右后
				break;
			case 3:
				SideType3(m_Type, 0, i);//0 右前
				SideType3(m_Type, 1, i);//1 右后
				break;
			default: break;
			}
		}
		//左边侧面
		else if (m_position == 1)
		{
			switch (side_type)
			{
			case 1:
				SideType1(m_Type, 2, i);//左前
				SideType1(m_Type, 3, i);//左后
				break;
			case 2:
				SideType2(m_Type, 2, i);//左前
				SideType2(m_Type, 3, i);//左后
				break;
			case 3:
				SideType3(m_Type, 2, i);//左前
				SideType3(m_Type, 3, i);//左后
				break;
			default: break;
			}
		}
		//左右侧面
		else if (m_position == 2)
		{
			//顶面和底面
			for (int p = 0; p < 2; p++)
			{
				if (m_Type == 1)//支架
				{
					//顶面 
					switch (up_type)
					{
					case 1:
						BandU_Type1(1, p, i);// 0表示右
						break;
					case 2:
						BandU_Type2(1, p, i);
						break;
					case 3:
						BandU_Type3(1, p, i);
						break;
					case 4:
						BandU_Type4(1, p, i);
						break;
					default:break;
					}
					//底面
					switch (bu_type)
					{
					case 1:
						BandU_Type1(2, p, i);// 0表示右
						break;
					case 2:
						BandU_Type2(2, p, i);
						break;
					case 3:
						BandU_Type3(2, p, i);
						break;
					case 4:
						BandU_Type4(2, p, i);
						break;
					default:break;
					}
				}
				else if (m_Type == 3)//横担
				{
					//顶面 
					switch (up_type)
					{
					case 1:
						BandU_Type1(4, p, i);// 0表示右
						break;
					case 2:
						BandU_Type2(4, p, i);
						break;
					case 3:
						BandU_Type3(4, p, i);
						break;
					case 4:
						BandU_Type4(4, p, i);
						break;
					default:break;
					}
					//底面
					switch (bu_type)
					{
					case 1:
						BandU_Type1(3, p, i);// 0表示右
						break;
					case 2:
						BandU_Type2(3, p, i);
						break;
					case 3:
						BandU_Type3(3, p, i);
						break;
					case 4:
						BandU_Type4(3, p, i);
						break;
					default:break;
					}
				}
			}
			//侧面
			switch (side_type)
			{
			case 1:
				SideType1(m_Type, 0, i);//0 右前
				SideType1(m_Type, 1, i);//1 右后
				break;
			case 2:
				SideType2(m_Type, 0, i);//0 右前
				SideType2(m_Type, 1, i);//1 右后
				break;
			case 3:
				SideType3(m_Type, 0, i);//0 右前
				SideType3(m_Type, 1, i);//1 右后
				break;
			default: break;
			}
			switch (side_type)
			{
			case 1:
				SideType1(m_Type, 2, i);//左前
				SideType1(m_Type, 3, i);//左后
				break;
			case 2:
				SideType2(m_Type, 2, i);//左前
				SideType2(m_Type, 3, i);//左后
				break;
			case 3:
				SideType3(m_Type, 2, i);//左前
				SideType3(m_Type, 3, i);//左后
				break;
			default: break;
			}
		}

		//隔面
		if (m_position == 0 || m_position == 1)//   ||表示或者
		{
			switch (g_type)
			{
			case 1:
				G_Type1(m_Type, m_position, i);
				break;
			case 2:
				G_Type2(m_Type, m_position, i);
				break;
			case 3:
				G_Type3(m_Type, m_position, i);
				break;
			case 4:
				G_Type4(m_Type, m_position, i);
				break;
			case 5:
				G_Type5(m_Type, m_position, i);
				break;
			default:break;
			}
		}
		else if (m_position == 2)
		{
			switch (g_type)
			{
			case 1:
				G_Type1(m_Type, 0, i);
				break;
			case 2:
				G_Type2(m_Type, 0, i);
				break;
			case 3:
				G_Type3(m_Type, 0, i);
				break;
			case 4:
				G_Type4(m_Type, 0, i);
				break;
			case 5:
				G_Type5(m_Type, 0, i);
				break;
			default:break;
			}
			switch (g_type)
			{
			case 1:
				G_Type1(m_Type, 1, i);
				break;
			case 2:
				G_Type2(m_Type, 1, i);
				break;
			case 3:
				G_Type3(m_Type, 1, i);
				break;
			case 4:
				G_Type4(m_Type, 1, i);
				break;
			case 5:
				G_Type5(m_Type, 1, i);
				break;
			default:break;
			}
		}
	}

}



