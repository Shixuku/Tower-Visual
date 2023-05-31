#pragma once
#include"Node.h"
#include"Element_Wire.h"
#include<vector>
#include<iostream>
using namespace std;
class Creat_WireData
{
public:

	vector<Node> m_NodesLeg;//�ڵ�ϼ�

	int m_elementsID = 0;
	vector<Element_Wire> m_Elements;//��Ԫ�ϼ�

	Creat_WireData() {};
	Creat_WireData(const std::vector<Node>& xuanguadian, double bizai, double yingli, int fenduan, vector<Node>& node)
	{
		pt = xuanguadian;
		gama = bizai;
		stress = yingli;
		N = fenduan;
		CreateWire(node);

	}
	std::vector<Node> pt;//xuanguadian
	double gama;//bizai
	double stress;//yingli
	int N;

	Node* Find_Node(int id, vector<Node>& node);

	void CreateWire(vector<Node>& node);//���ɵ��ߵ�
	void CreatWire_ele(int a, int b, vector<Element_Wire>& m_Elements);//���ɵ��ߵ�Ԫ
	vector<int> Find_Spacer(int fenli, double S, int N, vector<Node>xgd_List, vector<Node>xgd, vector<Node>wire0, int dangwei, vector<Node>& node)const;

	void Creat_Spacer(vector<Element_Wire>& spacer, vector<int> ids);
	void Creat_Strain_Strand(vector<Node>nz_pts, int c, vector<int>ids, vector<Node>& node, vector<Element_Wire>& m_Elements);
	void Find_Real_XGD(vector<Node>wire_temp, double L, int N, int choose, vector<Node>list_xgd, vector<Node>& real_xgd);

};

