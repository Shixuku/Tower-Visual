#pragma once
#include<iostream>
#include<QDataStream>
class Node
{
public:
	//0-�����ڵ� 1-���ҵ�ڵ� 2-���߽ڵ�
	int m_idNode = 0;//�ڵ���
	double x = 0;
	double y = 0;
	double z = 0;
	double F = 0;
	int Type = 0;
	bool restraint = 0;
	int groupTowerId = 0;//��������ʵ�����ã���Ҫ������������������������
	Node() {};
	Node(int id, double ix, double iy, double iz, double iF);
	Node(int id, double ix, double iy, double iz, double iF, int iType);
	void show()const;

	void SaveTo(QDataStream& fin)const;

	void Input(QDataStream& fin);

};

