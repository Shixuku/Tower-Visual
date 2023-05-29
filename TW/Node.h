#pragma once
#include<iostream>
#include<QDataStream>
class Node
{
public:
	int m_idNode = 0;//½Úµã±àºÅ
	double x = 0;
	double y = 0;
	double z = 0;

	Node() {};
	Node(int id, double ix, double iy, double iz);
	void show()const;

	void SaveTo(QDataStream& fin)const;

	void Input(QDataStream& fin);

};

