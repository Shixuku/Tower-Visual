#pragma once
using namespace std;


enum Part_Type :int
{
	ET_PartLeg, ET_PartBody, ET_PartSeptum, ET_PartCrossArm, ET_PartCrossArmSep, ET_Tower, ET_Section,ET_PartInsulator,ET_TowerWireGroup,
	ET_Wire
};
class Base
{
public:
	int m_id;
	Base() = default;
	//void 
};

