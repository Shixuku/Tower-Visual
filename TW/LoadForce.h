#pragma once
#include"Base.h"
class LoadForce:public Base
{
public:
	int id_force;//���������
	int id_node;//�ڵ���
	int DirectionForce;//����
	double Force;//��С
	double StartTime;//��ʼʱ��
	double EndTime;//����ʱ��
	LoadForce();
	LoadForce(int id, int i_node, double i_direction, double i_force, double i_stime, double i_etime);
	virtual enum Part_Type My_PartType()const
	{
		return ET_LoadForce;
	}
	virtual void SaveTo(QDataStream& fin)const;
	virtual void Input(QDataStream& fin);
};

