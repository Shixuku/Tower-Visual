#pragma once
#include"Part_Base.h"
class TowerData_leg :public Part_Base
{
public:
	//���ȵĻ������β���

	double m_L1;  //��������
	double m_L2;  //�����Ͽ�
	double m_H1;  //������
	double m_H[4];  //�ĸ����ȸ߶�
	int m_TypeSide;  //�����ͺ�
	int m_TypeSeptum;//�����ͺ�

	double Get_PracticlH(int i)const;//�õ������²�ʵ�ʵ�H1
	double Get_PracticlL(int i)const;//�õ������²�ʵ�ʵ�L1

};

