#pragma once
#include<iostream>
#include<QDataStream>
class Element
{
public:
	Element();
	Element(int id, int idNode1, int idNode2);

	int m_idElement;//��Ԫ���
	int m_idNode[2];//���ӵ�Ԫ�Ľڵ���

	int sectionID = 0;//�������
	int ClassSectionID = 0;//�����Ԫ�õĽ���ı��
	int MaterialID = 0;//��õĽ���ı��
	int groupTowerId = 0;
	void show()const;
	void showid()const;

	void SaveTo(QDataStream& fin) const;
	void Input(QDataStream& fin);

};

