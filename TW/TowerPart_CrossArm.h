#pragma once
#include"TowerData_CrossArm.h"

class TowerPart_CrossArm : public TowerData_CrossArm
{
public:
	
	//type==1(֧��)/3(�ᵣ)  iQuadrant=0(��ǰ)=1(�Һ�)=2(��ǰ)=3(���)
	void Get_SideNode3(int type, Node* n,int iQuadrant, int layer); 
	//type=1(֧��ˮƽ)=2(֧����)=3(�ᵣˮƽ)=4(�ᵣ��) iQuadrant=0(��)=1(��)
	void Get_BandUNode4(int type,Node* n,int iQuadrant,int layer);

	void SideType1(int type, int iQuadrant, int layer);
	void SideType2(int type, int iQuadrant, int layer);
	void SideType3(int type, int iQuadrant, int layer);
	void BandU_Type1(int type,int iQuadrant, int layer);
	void BandU_Type2(int type,int iQuadrant, int layer);
	void BandU_Type3(int type,int iQuadrant, int layer);
	void BandU_Type4(int type,int iQuadrant, int layer);
	void BandU_Type5(int type, int iQuadrant, int layer);

	void G_Get_Node4(int type, Node* n, int iQuadrant, int layer);//�õ�ֱ�������ݸ�����4����
	void G_Create_point(const Node& n1, const Node& n2, const Node& n3, const Node& n4, Node& n);
	void G_Type1(int type, int iQuadrant, int layer); //��������1
	void G_Type2(int type, int iQuadrant, int layer); //��������2
	void G_Type3(int type, int iQuadrant, int layer); //��������3
	void G_Type4(int type, int iQuadrant, int layer); //��������4
	void G_Type5(int type, int iQuadrant, int layer); //��������5

	void Create_Mesh();

	//QString m_name;
	virtual enum Part_Type My_PartType()const
	{
		return ET_PartCrossArm;
	}
};

