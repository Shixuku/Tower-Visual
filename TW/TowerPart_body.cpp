#include "TowerPart_body.h"

void TowerPart_body::Get_Node4(int iQuadrant, Node* n, int layer)//layer表示层数
{
	double z0 = Get_LayerH(layer - 1);
	double zn = Get_LayerH(layer);
	double L0 = Get_LayerL(layer - 1);//每层下宽度
	double Ln = Get_LayerL(layer);//每层上宽度

	double x0 = L0 / 2.0; double Xn = Ln / 2.0;
	if (iQuadrant == 0)//正视这个面
	{
		n[0].x = x0; n[0].y = x0; n[0].z = z0;
		n[1].x = Xn; n[1].y = Xn; n[1].z = zn;
		n[2].x = -x0; n[2].y = x0; n[2].z = z0;
		n[3].x = -Xn; n[3].y = Xn; n[3].z = zn;
	}
	else if (iQuadrant == 1) //后视
	{
		n[0].x = x0; n[0].y = -x0; n[0].z = z0;
		n[1].x = Xn; n[1].y = -Xn; n[1].z = zn;
		n[2].x = -x0; n[2].y = -x0; n[2].z = z0;
		n[3].x = -Xn; n[3].y = -Xn; n[3].z = zn;
	}
	else if (iQuadrant == 2)///右视图
	{
		n[0].x = x0; n[0].y = x0; n[0].z = z0;
		n[1].x = Xn; n[1].y = Xn; n[1].z = zn;
		n[2].x = x0; n[2].y = -x0; n[2].z = z0;
		n[3].x = Xn; n[3].y = -Xn; n[3].z = zn;
	}
	else if (iQuadrant == 3)//左视图
	{
		n[0].x = -x0; n[0].y = x0; n[0].z = z0;
		n[1].x = -Xn; n[1].y = Xn; n[1].z = zn;
		n[2].x = -x0; n[2].y = -x0; n[2].z = z0;
		n[3].x = -Xn; n[3].y = -Xn; n[3].z = zn;
	}

}
void TowerPart_body::type1(int iQuadrant, int layer)
{
	//生成节点
	Node Node1[5];
	int node[5];
	Get_Node4(iQuadrant, Node1, layer);
	interpolation(Node1[0], Node1[2], 0.5, Node1[4]);
	for (int i = 0; i < 5; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	if (iQuadrant == 0 || iQuadrant == 1)//正面类型生成梁 侧面类型不生成
	{
		Creat_Beams1(m_Elements_beams, { node[0],node[1],node[3],node[2] });//塔身立柱
	}
	Creat_Beams(m_Elements_beams, { node[1],node[4],node[3] });//中间梁
}
void TowerPart_body::type2(int iQuadrant, int layer)
{
	//生成节点
	Node Node1[9];
	int node[9];
	Get_Node4(iQuadrant, Node1, layer);
	interpolation(Node1[0], Node1[2], 0.5, Node1[4]);
	interpolation(Node1[0], Node1[1], 0.5, Node1[5]);
	interpolation(Node1[4], Node1[1], 0.5, Node1[6]);
	interpolation(Node1[2], Node1[3], 0.5, Node1[7]);
	interpolation(Node1[4], Node1[3], 0.5, Node1[8]);

	for (int i = 0; i < 9; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}

	if (iQuadrant == 0 || iQuadrant == 1)//正面类型生成梁 侧面类型不生成
	{
		Creat_Beams1(m_Elements_beams, { node[5],node[0],node[5],node[1],node[7],node[2],node[7] ,node[3] });//立柱
		//Creat_Beams(m_Elements_beams, { node[2],node[7],node[3] });//立柱
	}

	Creat_Beams(m_Elements_beams, { node[3],node[8],node[4],node[6],node[1] });//一串梁单元
	Creat_Trusses1(m_Elements_Trusses, { node[5],node[6],node[8],node[7] }); //杆单元
	Creat_Trusses1(m_Elements_Trusses, { node[2],node[8],node[0],node[6] });//杆单元
}
void TowerPart_body::type3(int iQuadrant, int layer)
{
	//生成节点
	Node Node1[13];
	int node[13];
	Get_Node4(iQuadrant, Node1, layer);
	interpolation(Node1[0], Node1[2], 0.5, Node1[4]);
	interpolation(Node1[0], Node1[1], 1. / 3, Node1[5]);
	interpolation(Node1[0], Node1[1], 2. / 3, Node1[6]);
	interpolation(Node1[4], Node1[1], 1. / 3, Node1[7]);
	interpolation(Node1[4], Node1[1], 2. / 3, Node1[8]);
	interpolation(Node1[2], Node1[3], 1. / 3, Node1[9]);
	interpolation(Node1[2], Node1[3], 2. / 3, Node1[10]);
	interpolation(Node1[4], Node1[3], 1. / 3, Node1[11]);
	interpolation(Node1[4], Node1[3], 2. / 3, Node1[12]);
	for (int i = 0; i < 13; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}

	if (iQuadrant == 0 || iQuadrant == 1)//正面类型生成梁 侧面类型不生成
	{
		Creat_Beams1(m_Elements_beams, { node[0],node[5],node[5],node[6],node[6],node[1],node[2],node[9],node[9],node[10],node[10],node[3] });//立柱
	}
	Creat_Trusses1(m_Elements_Trusses, { node[6],node[8],node[12],node[10]});//杆单元
	Creat_Trusses1(m_Elements_Trusses, { node[5],node[8],node[12],node[9]});//杆单元
	Creat_Trusses1(m_Elements_Trusses, { node[5],node[7],node[11],node[9]});//杆单元
	Creat_Trusses1(m_Elements_Trusses, { node[0],node[7],node[11],node[2]});//杆单元
	Creat_Beams(m_Elements_beams, { node[1],node[8],node[7],node[4],node[11],node[12],node[3] });//中间梁单元

}
void TowerPart_body::type4(int iQuadrant, int layer)
{
	//生成节点
	Node Node1[13];
	int node[13];
	Get_Node4(iQuadrant, Node1, layer);
	Node4_Creat1(Node1[0], Node1[3], Node1[1], Node1[2], Node1[4]);
	interpolation(Node1[0], Node1[1], 1. / 3, Node1[5]);
	interpolation(Node1[0], Node1[1], 2. / 3, Node1[6]);
	interpolation(Node1[2], Node1[3], 1. / 3, Node1[7]);
	interpolation(Node1[2], Node1[3], 2. / 3, Node1[8]);
	interpolation(Node1[4], Node1[0], 0.5, Node1[9]);
	interpolation(Node1[4], Node1[1], 0.5, Node1[10]);
	interpolation(Node1[4], Node1[2], 0.5, Node1[11]);
	interpolation(Node1[4], Node1[3], 0.5, Node1[12]);

	for (int i = 0; i < 13; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	//生成单元
	if (iQuadrant == 0 || iQuadrant == 1)//正面类型生成梁 侧面类型不生成
	{
		Creat_Beams1(m_Elements_beams, { node[0],node[5],node[5],node[6],node[6],node[1],node[2],node[7],node[7],node[8],node[8],node[3] });//立柱
	}
	Creat_Beams1(m_Elements_beams, { node[1],node[10],node[10],node[4],node[4],node[11],node[11],node[2], node[3],node[12],node[12],node[4],node[4],node[9],node[9],node[0] });//中间梁单元

	Creat_Trusses1(m_Elements_Trusses, {node[12],node[8],node[8],node[11],node[10],node[6],node[6],node[9] });//一串桁架单元

	Creat_Trusses1(m_Elements_Trusses, {node[12],node[11],node[10],node[9]});//一串桁架单元
	Creat_Trusses1(m_Elements_Trusses, {node[7],node[11],node[9],node[5]});//一串桁架单元
}
void TowerPart_body::type5(int iQuadrant, int layer)
{
	//生成节点
	Node Node1[11];
	int node[11];
	Get_Node4(iQuadrant, Node1, layer);//得到直接由数据给出的点
	Node4_Creat1(Node1[0], Node1[3], Node1[1], Node1[2], Node1[4]);
	interpolation(Node1[0], Node1[1], 0.5, Node1[5]);
	interpolation(Node1[2], Node1[3], 0.5, Node1[6]);
	interpolation(Node1[4], Node1[0], 0.5, Node1[7]);
	interpolation(Node1[4], Node1[1], 0.5, Node1[8]);
	interpolation(Node1[4], Node1[2], 0.5, Node1[9]);
	interpolation(Node1[4], Node1[3], 0.5, Node1[10]);
	for (int i = 0; i < 11; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	if (iQuadrant == 0 || iQuadrant == 1)//正面类型生成梁 侧面类型不生成
	{
		Creat_Beams1(m_Elements_beams, { node[0],node[5],node[5],node[1],node[2],node[6],node[6],node[3] });//立柱
	}

	Creat_Beams1(m_Elements_beams, { node[1],node[8],node[8],node[4],node[4],node[9],node[9],node[2],node[3],node[10],node[10],node[4],node[4],node[7],node[7],node[0] });//中间梁单元
	Creat_Trusses1(m_Elements_Trusses, { node[5],node[8],node[8],node[7],node[7],node[5],node[9],node[6],node[6],node[10],node[10],node[9] });//中间桁架单元
}
void TowerPart_body::type6(int iQuadrant, int layer)
{
	//生成节点
	Node Node1[5];
	int node[5];
	Get_Node4(iQuadrant, Node1, layer);
	Node4_Creat1(Node1[0], Node1[3], Node1[1], Node1[2], Node1[4]);
	for (int i = 0; i < 5; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	if (iQuadrant == 0 || iQuadrant == 1)//正面类型生成梁 侧面类型不生成
	{
		Creat_Beams1(m_Elements_beams, { node[0],node[1],node[2],node[3] });//立柱
	}
	Creat_Beams1(m_Elements_beams, { node[0],node[4],node[4],node[3],node[1],node[4],node[4],node[2] });//中间梁单元
	
}
void TowerPart_body::type7(int iQuadrant, int layer)
{
	//生成节点
	Node Node1[6];
	int node[6];
	Get_Node4(iQuadrant, Node1, layer);//得到直接由数据给出的点
	Node4_Creat1(Node1[0], Node1[3], Node1[1], Node1[2], Node1[4]);
	interpolation(Node1[0], Node1[2], 0.5, Node1[5]);
	for (int i = 0; i < 6; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	if (iQuadrant == 0 || iQuadrant == 1)//正面类型生成梁 侧面类型不生成
	{
		Creat_Beams1(m_Elements_beams, { node[0],node[1],node[2],node[3] });//立柱
	}
	Creat_Beams1(m_Elements_beams, { node[0],node[4],node[4],node[3],node[1],node[4],node[4],node[2] });//中间梁单元
	Creat_Trusses(m_Elements_Trusses, { node[4],node[5] });//一圈桁架单元
}
void TowerPart_body::type8(int iQuadrant, int layer)
{
	Node Node1[7];
	int node[7];
	Get_Node4(iQuadrant, Node1, layer);//得到直接由数据给出的点
	interpolation(Node1[0], Node1[2], 0.5, Node1[4]);
	interpolation(Node1[1], Node1[4], 0.5, Node1[5]);
	interpolation(Node1[3], Node1[4], 0.5, Node1[6]);
	for (int i = 0; i < 7; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	if (iQuadrant == 0 || iQuadrant == 1)//正面类型生成梁 侧面类型不生成
	{
		Creat_Beams1(m_Elements_beams, { node[0],node[1],node[2],node[3] });//立柱
	}
	Creat_Beams(m_Elements_beams, { node[1],node[5],node[4],node[6],node[3] });//中间梁单元
	Creat_Trusses1(m_Elements_Trusses, { node[0],node[5],node[2],node[6] });//中间桁架单元
	
}
void TowerPart_body::type9(int iQuadrant, int layer)
{
	//生成节点
	Node Node1[7];
	int node[7];
	Get_Node4(iQuadrant, Node1, layer);//得到直接由数据给出的点
	Node4_Creat1(Node1[0], Node1[3], Node1[1], Node1[2], Node1[4]);
	double h = Node1[3].z - Node1[4].z;
	double H = Node1[3].z - Node1[2].z;
	interpolation(Node1[1], Node1[0], h / H, Node1[5]);
	interpolation(Node1[3], Node1[2], h / H, Node1[6]);
	for (int i = 0; i < 7; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	if (iQuadrant == 0 || iQuadrant == 1)//正面类型生成梁 侧面类型不生成
	{
		Creat_Beams1(m_Elements_beams, { node[0],node[5],node[5],node[1],node[2],node[6],node[6],node[3] });//立柱
	}
	Creat_Beams1(m_Elements_beams, { node[4],node[0],node[4],node[1],node[4],node[2],node[4],node[3] });//中间梁单元
	Creat_Trusses(m_Elements_Trusses, { node[5],node[4],node[6] });//中间桁架单元

}
void TowerPart_body::type10(int iQuadrant, int layer)
{
	Node Node1[5];
	int node[5];
	Get_Node4(iQuadrant, Node1, layer);//得到直接由数据给出的点
	interpolation(Node1[3], Node1[1], 0.5, Node1[4]);
	for (int i = 0; i < 5; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	if (iQuadrant == 0 || iQuadrant == 1)//正面类型生成梁 侧面类型不生成
	{
		Creat_Beams1(m_Elements_beams, { node[0],node[1],node[2] ,node[3] });//立柱
	}
	Creat_Beams(m_Elements_beams, {node[2],node[4],node[0]});//一串梁单元
}

void TowerPart_body::type11(int iQuadrant, int layer)
{
	//生成节点
	Node Node1[13];
	int node[13];
	Get_Node4(iQuadrant, Node1, layer);
	interpolation(Node1[0], Node1[2], 0.5, Node1[4]);
	interpolation(Node1[0], Node1[4], 0.5, Node1[5]);
	interpolation(Node1[1], Node1[4], 0.5, Node1[6]);
	interpolation(Node1[2], Node1[4], 0.5, Node1[7]);
	interpolation(Node1[3], Node1[4], 0.5, Node1[8]);
	double h = Node1[1].z - Node1[6].z;
	double H = Node1[1].z - Node1[0].z;
	interpolation(Node1[1], Node1[0], h / H, Node1[9]);
	interpolation(Node1[3], Node1[2], h / H, Node1[10]);
	interpolation(Node1[5], Node1[9], 0.5, Node1[11]);
	interpolation(Node1[7], Node1[10], 0.5, Node1[12]);

	for (int i = 0; i < 13; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	if (iQuadrant == 0 || iQuadrant == 1)//正面类型生成梁 侧面类型不生成
	{
		Creat_Beams1(m_Elements_beams, { node[0],node[9],node[9],node[1],node[3],node[10],node[10],node[2] });//塔身立柱
	}
	Creat_Beams(m_Elements_beams, { node[1],node[6],node[4],node[8],node[3] });//中间梁
	Creat_Beams1(m_Elements_beams, { node[6],node[9],node[9],node[11],node[11],node[5],node[5],node[6],
		node[10],node[8],node[8],node[7],node[7],node[12],node[12],node[10] });
	Creat_Trusses1(m_Elements_Trusses, { node[0],node[11],node[2],node[10] });
}

void TowerPart_body::type12(int iQuadrant, int layer)
{
	//生成节点
	Node Node1[15];
	int node[15];
	Get_Node4(iQuadrant, Node1, layer);//得到直接由数据给出的点
	Node4_Creat1(Node1[0], Node1[3], Node1[2], Node1[1], Node1[4]);
	interpolation(Node1[0], Node1[4], 0.5, Node1[5]);
	interpolation(Node1[1], Node1[4], 0.5, Node1[6]);
	interpolation(Node1[2], Node1[4], 0.5, Node1[7]);
	interpolation(Node1[3], Node1[4], 0.5, Node1[8]);
	interpolation(Node1[0], Node1[1], 0.35, Node1[9]);
	interpolation(Node1[0], Node1[1], 0.675, Node1[10]);
	interpolation(Node1[2], Node1[3], 0.35, Node1[11]);
	interpolation(Node1[2], Node1[3], 0.675, Node1[12]);
	interpolation(Node1[5], Node1[10], 0.5, Node1[13]);
	interpolation(Node1[7], Node1[12], 0.5, Node1[14]);
	for (int i = 0; i < 15; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	if (iQuadrant == 0 || iQuadrant == 1)//正面类型生成梁 侧面类型不生成
	{
		Creat_Beams1(m_Elements_beams, { node[2],node[11],node[11],node[12],node[12],node[3],
			node[0],node[9],node[9],node[10],node[10],node[1], });//立柱
	}
	Creat_Beams1(m_Elements_beams, { node[3],node[8],node[8],node[4],node[4],node[5],node[5],node[0],
		node[1],node[6],node[6],node[4],node[4],node[7],node[7],node[2], });//中间梁单元

	Creat_Trusses1(m_Elements_Trusses, { node[5],node[13],node[13],node[10],node[10],node[6],node[6],node[5],
		node[7],node[14],node[14],node[12],node[12],node[8],node[8],node[7], });//一圈桁架单元
	Creat_Trusses1(m_Elements_Trusses, { node[7],node[11],node[5],node[9] });
	Creat_Trusses1(m_Elements_Trusses, { node[14],node[11],node[13],node[9] });
}

void TowerPart_body::type13(int iQuadrant, int layer)
{
	//生成节点
	Node Node1[12];
	int node[12];
	Get_Node4(iQuadrant, Node1, layer);//得到直接由数据给出的点
	Node4_Creat1(Node1[0], Node1[3], Node1[1], Node1[2], Node1[4]);
	interpolation(Node1[0], Node1[1], 0.5, Node1[5]);
	interpolation(Node1[2], Node1[3], 0.5, Node1[6]);
	interpolation(Node1[4], Node1[0], 0.5, Node1[7]);
	interpolation(Node1[4], Node1[1], 0.5, Node1[8]);
	interpolation(Node1[4], Node1[2], 0.5, Node1[9]);
	interpolation(Node1[4], Node1[3], 0.5, Node1[10]);
	interpolation(Node1[0], Node1[2], 0.5, Node1[11]);
	for (int i = 0; i < 12; i++)
	{
		node[i] = Creat_Node(Node1[i].x, Node1[i].y, Node1[i].z);
	}
	if (iQuadrant == 0 || iQuadrant == 1)//正面类型生成梁 侧面类型不生成
	{
		Creat_Beams1(m_Elements_beams, { node[0],node[5],node[5],node[1],node[2],node[6],node[6],node[3] });//立柱
	}

	Creat_Beams1(m_Elements_beams, { node[1],node[8],node[8],node[4],node[4],node[9],node[9],node[2],node[3],node[10],node[10],node[4],node[4],node[7],node[7],node[0] });//中间梁单元
	Creat_Trusses1(m_Elements_Trusses, { node[5],node[8],node[8],node[7],node[7],node[5],node[9],node[6],node[6],node[10],node[10],node[9]});//中间桁架单元
	Creat_Trusses(m_Elements_Trusses, { node[4],node[11] });
}

void TowerPart_body::G_Get_Node4(int Tier, Node* n,int pos)
{
	double h = 0.0;
	double l = 0.0;
	if (pos == 0)//底面或者顶面
	{
		h = Get_LayerH(Tier);
		l = Get_LayerL(Tier);

	}
	else if (pos == 1)//中间
	{
		Node Node1[7];
		int node[7];
		//塔身一面的宽度和高度
		double z0 = Get_LayerH(Tier - 1);
		double zn = Get_LayerH(Tier);
		double L0 = Get_LayerL(Tier - 1);//每层下宽度
		double Ln = Get_LayerL(Tier);//每层上宽度
		//塔身一面四个点的坐标
		double x0 = L0 / 2.0; double Xn = Ln / 2.0;
		Node1[0].x = x0; Node1[0].y = x0; Node1[0].z = z0;
		Node1[1].x = Xn; Node1[1].y = Xn; Node1[1].z = zn;
		Node1[2].x = -x0; Node1[2].y = x0; Node1[2].z = z0;
		Node1[3].x = -Xn; Node1[3].y = Xn; Node1[3].z = zn;
		//计算中间点
		Node4_Creat1(Node1[0], Node1[3], Node1[1], Node1[2], Node1[4]);
		//计算经过中间点的水平线与塔身边的交点
		double hh = Node1[3].z - Node1[4].z;
		double H = Node1[3].z - Node1[2].z;
		interpolation(Node1[1], Node1[0], hh / H, Node1[5]);
		interpolation(Node1[3], Node1[2], hh / H, Node1[6]);

		h = Node1[4].z;
		l = Node1[5].x - Node1[6].x;
	}
	double a = l / 2.0;
	n[0].x = a; n[0].y = a;
	n[1].x = -a; n[1].y = a;
	n[2].x = -a; n[2].y = -a;
	n[3].x = a; n[3].y = -a;
	for (int i = 0; i < 4; i++)
	{
		n[i].z = h;
	}
}

void TowerPart_body::G_Type1(int tier, int pos)
{
	Node node1[24];
	int node[24];
	G_Get_Node4(tier, node1, pos);

	double l = node1[0].x - node1[1].x;
	double h = node1[0].z;
	double a = l / 2.0;
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

	Creat_Beams1(m_Elements_beams, { node[3],node[22], node[0],node[16],node[1],node[18],node[2],node[20] });//一串梁单元
	Creat_Beams1(m_Elements_beams, { node[14],node[23],node[5],node[17],node[8],node[19],node[11],node[21] });//一串梁单元

}

void TowerPart_body::G_Type2(int tier, int pos)
{
	Node node2[5];
	int node[5];
	G_Get_Node4(tier, node2, pos);
	Node4_Creat1(node2[0], node2[2], node2[1], node2[3], node2[4]);
	for (int i = 0; i < 5; i++)
	{
		node[i] = Creat_Node(node2[i].x, node2[i].y, node2[i].z);
	}
	//生成单元
	Creat_Beams(m_Elements_beams, { node[0],node[1],node[2],node[3],node[0] });//一圈梁单元
	Creat_Beams1(m_Elements_beams, { node[4],node[0],node[4],node[1],node[4],node[2],node[4],node[3] });//一串梁单元
}

void TowerPart_body::G_Type3(int tier, int pos)
{
	Node node2[9];
	int node[9];
	G_Get_Node4(tier, node2, pos);
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
	//Creat_Beams(m_Elements_beams, { node[4],node[8],node[6] });//一圈梁单元
	Creat_Beams1(m_Elements_beams, { node[8],node[4],node[8],node[5], node[8],node[6],node[8],node[7] });//一圈梁单元
	//Creat_Beams(m_Elements_beams, { node[5],node[8],node[7] });//一串梁单元

}

void TowerPart_body::G_Type4(int tier, int pos)
{
	Node node2[12];
	int node[12];
	G_Get_Node4(tier, node2, pos);
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
	Creat_Beams1(m_Elements_beams, { node[0],node[8],node[1],node[9],node[2],node[10],node[3],node[11] });//一串梁单元


}

void TowerPart_body::G_Type5(int tier, int pos)
{
	Node node2[12];
	int node[12];
	G_Get_Node4(tier, node2, pos);
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
	Creat_Beams1(m_Elements_beams, { node[0],node[8],node[1],node[9],node[2],node[10],node[3],node[11] });//一串梁单元
	Creat_Beams1(m_Elements_beams, { node[8],node[11],node[9],node[10] });//一串梁单元
}

void TowerPart_body::G_Type6(int Tier, int pos)
{
	Node node2[4];
	int node[4];
	G_Get_Node4(Tier, node2, pos);
	for (int i = 0; i < 4; i++)
	{
		node[i] = Creat_Node(node2[i].x, node2[i].y, node2[i].z);
	}
	//生成单元
	Creat_Beams(m_Elements_beams, { node[0],node[1],node[2],node[3],node[0] });//一圈梁单元
}

void TowerPart_body::Create_Mesh()
{
	if (m_Tier == 0) return;
	//塔身层类型
	for (int i = 1; i < m_Tier + 1; i++)//i--层数 从第1层开始算
	{//数组里面[i]个位置表示第i+1层型号
		int f_type = m_layer[i - 1].m_TypeFront;//正面类型
		int s_type = m_layer[i - 1].m_TypeSide;//侧面类型
		int g_mType = m_layer[i - 1].m_TypeSeptumMiddle;//中间隔面类型
		int g_type = m_layer[i - 1].m_TypeSeptumUp;//上部隔面类型
		//正面型号
		switch (f_type)
		{
		case 1:
			type1(0, i);
			type1(1, i);
			break;
		case 2:
			type2(0, i);
			type2(1, i);
			break;
		case 3:
			type3(0, i);
			type3(1, i);
			break;
		case 4:
			type4(0, i);
			type4(1, i);
			break;
		case 5:
			type5(0, i);
			type5(1, i);
			break;
		case 6:
			type6(0, i);
			type6(1, i); break;
		case 7:
			type7(0, i);
			type7(1, i); break;
		case 8:
			type8(0, i);
			type8(1, i); break;
		case 9:
			type9(0, i);
			type9(1, i); break;
		case 10:
			type10(0, i);
			type10(1, i); break;
		case 11:
			type11(0, i);
			type11(1, i); break;
		case 12:
			type12(0, i);
			type12(1, i); break;
		case 13:
			type13(0, i);
			type13(1, i); break;
		default:break;
		}
		//侧面型号
		switch (s_type)
		{
		case 1:
			type1(2, i);
			type1(3, i); break;
		case 2:
			type2(2, i);
			type2(3, i); break;
		case 3:
			type3(2, i);
			type3(3, i); break;
		case 4:
			type4(2, i);
			type4(3, i); break;
		case 5:
			type5(2, i);
			type5(3, i); break;
		case 6:
			type6(2, i);
			type6(3, i); break;
		case 7:
			type7(2, i);
			type7(3, i); break;
		case 8:
			type8(2, i);
			type8(3, i); break;
		case 9:
			type9(2, i);
			type9(3, i); break;
		case 10:
			type10(2, i);
			type10(3, i); break;
		case 11:
			type11(2, i);
			type11(3, i); break;
		case 12:
			type12(2, i);
			type12(3, i); break;
		case 13:
			type13(2, i);
			type13(3, i); break;
		default:break;
		}
		//中间隔面类型
		switch (g_mType)
		{
		case 1:
			G_Type1(i, 1);
			break;
		case 2:
			G_Type2(i, 1);
			break;
		case 3:
			G_Type3(i, 1);
			break;
		case 4:
			G_Type4(i, 1);
			break;
		case 5:
			G_Type5(i, 1);
			break;
		default:break;
		}
		//上部隔面类型
		switch (g_type)
		{
		case 1:
			G_Type1(i, 0);
			break;
		case 2:
			G_Type2(i, 0);
			break;
		case 3:
			G_Type3(i, 0);
			break;
		case 4:
			G_Type4(i, 0);
			break;
		case 5:
			G_Type5(i, 0);
			break;
		case 6:
			G_Type6(i, 0);
			break;
		default:break;
		}
	}
}

