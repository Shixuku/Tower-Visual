#pragma once
#include"WireData.h"
class CreatWire:public WireData
{
public:
	
	void CreateRealSus();//������ʵ���ҵ�
	vector<int>susPoint;
	void CreateWire();//���ɵ��ߵ�
	void CreateStrain();//�������Ŵ��ǲ���
	void Create_Mesh();
	//void AddAxialForceToInsulator();//����Ե��ʩ������
	vector<int>wireToGroup;
	int FindGroupIdNode(int idNode) const;
	virtual enum Part_Type My_PartType()const
	{
		return ET_Wire;
	}
};

