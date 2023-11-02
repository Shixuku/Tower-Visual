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
	int MaterialID = 0;//获得的截面的编号
    double AxialForce = 0;//轴力
	int groupTowerId = 0;
	void show()const;
	void showid()const;
	QString Type = "T";
	virtual void SaveTo(QDataStream& fin) const;
	virtual void Input(QDataStream& fin);

};

