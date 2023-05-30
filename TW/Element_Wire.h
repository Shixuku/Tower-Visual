#pragma once
#include<iostream>
class Element_Wire
{
public:
	Element_Wire() {};
	Element_Wire(int id, int idNode1, int idNode2)
	{
		m_idElement = id;
		m_idNode[0] = idNode1;
		m_idNode[1] = idNode2;
	}

	int m_idElement;//单元编号
	int m_idNode[2];//连接单元的节点编号
	bool isbeam = false;


	void show()
	{
		std::cout << m_idElement << " " << m_idNode[0] << " " << m_idNode[1] << "\n";
	}
};

