#pragma once
#include"TowerData_Layer.h"
#include<iostream>
using namespace std;
#include<vector>
#include"Part_Base.h"
class TowerData_Body:public Part_Base   //����εĲ���
{
public:

	//����Ļ������β���
	int m_Tier;  //�ܲ���

	double m_L0=0 ; double m_Z0=0;//�ײ���ȡ��ײ�z����
	double m_Ln=0; //double m_Zn=0;//������ȡ�����z����(����z������Բ�Ҫ)

	vector<TowerData_Layer> m_layer;
	double Get_SumH();
	double Get_LayerH(int i);//�õ�i��߶�
	double Get_LayerL(int i);//�õ�i���Ͽ��

};

