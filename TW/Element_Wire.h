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

	int m_idElement;//��Ԫ���
	int m_idNode[2];//���ӵ�Ԫ�Ľڵ���
	bool isbeam = false;


	void show()
	{
		std::cout << m_idElement << " " << m_idNode[0] << " " << m_idNode[1] << "\n";
	}
};

