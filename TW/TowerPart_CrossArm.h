#pragma once
#include"TowerData_CrossArm.h"

class TowerPart_CrossArm : public TowerData_CrossArm
{
public:
	
	//type==1(支架)/3(横担)  iQuadrant=0(右前)=1(右后)=2(左前)=3(左后)
	void Get_SideNode3(int type, Node* n,int iQuadrant, int layer); 
	//type=1(支架水平)=2(支架下)=3(横担水平)=4(横担上) iQuadrant=0(右)=1(左)
	void Get_BandUNode4(int type,Node* n,int iQuadrant,int layer);

	void SideType1(int type, int iQuadrant, int layer);
	void SideType2(int type, int iQuadrant, int layer);
	void SideType3(int type, int iQuadrant, int layer);
	void BandU_Type1(int type,int iQuadrant, int layer);
	void BandU_Type2(int type,int iQuadrant, int layer);
	void BandU_Type3(int type,int iQuadrant, int layer);
	void BandU_Type4(int type,int iQuadrant, int layer);
	void BandU_Type5(int type, int iQuadrant, int layer);

	void G_Get_Node4(int type, Node* n, int iQuadrant, int layer);//得到直接由数据给出的4个点
	void G_Create_point(const Node& n1, const Node& n2, const Node& n3, const Node& n4, Node& n);
	void G_Type1(int type, int iQuadrant, int layer); //隔面类型1
	void G_Type2(int type, int iQuadrant, int layer); //隔面类型2
	void G_Type3(int type, int iQuadrant, int layer); //隔面类型3
	void G_Type4(int type, int iQuadrant, int layer); //隔面类型4
	void G_Type5(int type, int iQuadrant, int layer); //隔面类型5

	void Create_Mesh();

	//QString m_name;
	virtual enum Part_Type My_PartType()const
	{
		return ET_PartCrossArm;
	}
};

