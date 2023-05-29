#pragma once
#include<iostream>
#include<QDataStream>
class Element
{
public:
	Element();
	Element(int id, int idNode1, int idNode2);

	int m_idElement;//单元编号
	int m_idNode[2];//连接单元的节点编号

	int sectionID = 0;//截面组号
	int ClassSectionID = 0;//这个单元用的截面的编号

	void show()const;
	void showid()const;

	void SaveTo(QDataStream& fin) const;
	void Input(QDataStream& fin);

};

