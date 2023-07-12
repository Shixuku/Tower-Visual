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

	int m_Type=0;//�����Ǻᵣ����֧��  1--��ʾ֧�� 3--��ʾ�ᵣ

	double m_bodyButtomL = 0;//�����ײ����
	double m_bodyUpL = 0;//����㶥�����
	double m_bodyButtomH = 0;//�����ײ��߶�
	double m_bodyUpH = 0;//�����ײ��߶�

	double m_c1W = 0;//֧��--���۶˿��

	double m_c2Wb = 0;//�ᵣ--���۶˵ײ����
	double m_c2Wup = 0;//�ᵣ--���۶˶������
	double m_c2H = 0;//�ᵣ--���۶˸߶�

	int m_count = 0;//�γ�

	int m_position = 0;//λ�� 0--�ұ� 1--��� 2--����

	//�õ����γ����
	double Get_SumL();//x����Ķγ���---x
	double Get_SumLi(int i);//i��ǰ�γ���---x
	double Get_layerLi(int type, int i);//type 1--֧��ˮƽ 2--֧���� 3--�ᵣˮƽ 4--�ᵣ��
	double Get_layerWi(int type,int i);//type 1--֧��ˮƽ 2--֧���� 3--�ᵣˮƽ 4--�ᵣ��
	double Get_layerHi(int type,int i);//type 1--֧��ˮƽ 2--֧���� 3--�ᵣˮƽ 4--�ᵣ��

};

