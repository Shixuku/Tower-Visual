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
    double AxialForce = 0;//����
	int groupTowerId = 0;
	void show()const;
	void showid()const;
	QString Type = "T";
	virtual void SaveTo(QDataStream& fin) const;
	virtual void Input(QDataStream& fin);

};

