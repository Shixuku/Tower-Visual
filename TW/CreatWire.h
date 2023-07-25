#pragma once
#include"WireData.h"
class CreatWire:public WireData
{
public:
	
	void CreateRealSus();//生成真实悬挂点
	vector<int>susPoint;
	void CreateWire();//生成导线点
	void CreateStrain();//生成耐张串那部分
	void Create_Mesh();
	//void AddAxialForceToInsulator();//给绝缘子施加轴力
	vector<int>wireToGroup;
	int FindGroupIdNode(int idNode) const;
	virtual enum Part_Type My_PartType()const
	{
		return ET_Wire;
	}
};

