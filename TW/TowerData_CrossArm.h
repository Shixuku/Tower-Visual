#pragma once
#include<iostream>
using namespace std;
#include<vector>
#include"TowerData_LayerArm.h"
#include"Part_Base.h"
#include"TowerData_Body.h"
class TowerData_CrossArm:public Part_Base
{
public:
	vector<TowerData_LayerArm> m_layerArm;

	TowerData_Body* m_databodyTier = nullptr;//ȡ�������

	int m_Type;//�����Ǻᵣ����֧��  1--��ʾ֧�� 3--��ʾ�ᵣ

	double m_bodyButtomL;//�����ײ����
	double m_bodyUpL;//����㶥�����
	double m_bodyButtomH;//�����ײ��߶�
	double m_bodyUpH;//�����ײ��߶�

	double m_c1W;//֧��--���۶˿��

	double m_c2Wb;//�ᵣ--���۶˵ײ����
	double m_c2Wup;//�ᵣ--���۶˶������
	double m_c2H;//�ᵣ--���۶˸߶�

	int m_count;//�γ�

	int m_position;//λ�� 0--�ұ� 1--��� 2--����

	//�õ����γ����
	double Get_SumL();//x����Ķγ���---x
	double Get_SumLi(int i);//i��ǰ�γ���---x
	double Get_layerLi(int type, int i);//type 1--֧��ˮƽ 2--֧���� 3--�ᵣˮƽ 4--�ᵣ��
	double Get_layerWi(int type,int i);//type 1--֧��ˮƽ 2--֧���� 3--�ᵣˮƽ 4--�ᵣ��
	double Get_layerHi(int type,int i);//type 1--֧��ˮƽ 2--֧���� 3--�ᵣˮƽ 4--�ᵣ��

};

