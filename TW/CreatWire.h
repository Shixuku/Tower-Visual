#pragma once
#include"WireData.h"
class CreatWire:public WireData
{
public:
	
	void CreatWireSus();//Éú³ÉÐü¹Òµã
	vector<int>susPoint;
	void CreatWirePoint();
	void Create_Mesh();
	vector<int>wireToGroup;
	int FindGroupIdNode(int idNode) const;
	virtual enum Part_Type My_PartType()const
	{
		return ET_Wire;
	}
};

