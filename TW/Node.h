#pragma once
#include<iostream>
#include<QDataStream>
class Node
{
public:
	int m_idNode = 0;//�ڵ���
	double x = 0;
	double y = 0;
	double z = 0;
	double F = 0;
	bool restraint = 0;
	int groupTowerId = 0;//��������ʵ�����ã���Ҫ������������������������
	Node() {};
	Node(int id, double ix, double iy, double iz, double iF);
	//Node(double ix, double iy, double iz, double iF)
	//{
	//	x = ix, y = iy, z = iz, F = iF;
	//}
	void show()const;

	void SaveTo(QDataStream& fin)const;

	void Input(QDataStream& fin);

};

