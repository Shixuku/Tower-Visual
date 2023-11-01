#pragma once
#include<iostream>
#include<QDataStream>
class Node
{
public:
	//0-杆塔节点 1-悬挂点节点 2-导线节点
	int m_idNode = 0;//节点编号
	double x = 0;
	double y = 0;
	double z = 0;
	double F = 0;
	int Type = 0;
	bool restraint = 0;
	int groupTowerId = 0;//部件和塔实例不用，主要放在塔线里区分属于哪座塔
	Node() {};
	Node(int id, double ix, double iy, double iz, double iF);
	Node(int id, double ix, double iy, double iz, double iF, int iType);
	void show()const;

	void SaveTo(QDataStream& fin)const;

	void Input(QDataStream& fin);

};

