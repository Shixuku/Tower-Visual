#pragma once
#include"WireData.h"
class CreatWire:public WireData
{
public:
	
	void CreatWireSus();//�������ҵ�
	vector<int>susPoint;
	void CreatWirePoint();
	void Create_Mesh();
	virtual enum Part_Type My_PartType()const
	{
		return ET_Wire;
	}
};

