#pragma once
class LoadForce
{
public:
	int id_force;//���������
	int id_node;//�ڵ���
	int DirectionForce;//����
	double Force;//��С
	double StartTime;//��ʼʱ��
	double EndTime;//����ʱ��
	LoadForce(int id, int i_node,double i_direction, double i_force, double i_stime,double i_etime)
	{
		id_force = id;
		id_node = i_node;
		DirectionForce = i_direction;
		Force = i_force;
		StartTime = i_stime;
		EndTime = i_etime;
	}
};

