#pragma once
#include <iostream>
#include <QString>

enum Entity_Type :int
{
	ET_Node, ET_Dependent, ET_LineElement, ET_Beam3D, ET_Beam2D, ET_Truss3D, ET_Truss2D,ET_Cable3D, ET_Cable3N, 
	ET_Material, ET_Force_Node, ET_Force_Gravity, ET_Force_Node_harmonic, ET_Force_Node_Function, ET_Force_Wind,ET_Constraint,
	ET_Section_Beam2D, ET_Section_Beam3D, ET_Section_Truss, ET_Section_Cable, ET_ReaderWriter, ET_Section_Assign, ET_AnalysisStep
};

class Structure;

class EntityBase
{//对象基类
private:
	static Structure* s_pStructure;

	virtual enum Entity_Type My_EntityType() const = 0;

public:
	int m_id;//编号

	static const double pi;

	int Get_id()
	{
		return m_id;
	}

	static void Set_Structure(Structure* pStructure) 
	{
		s_pStructure = pStructure;
	}

	static Structure* Get_Structure() 
	{
		if (s_pStructure==nullptr)
		{
			std::cout << "结构指针没有设置！\n";
			exit(1);
		}
		return s_pStructure; 
	}
};


