#pragma once
#include <iostream>
#include <map>
#include <set>
#include <QString>

enum Entity_Type :int
{
	ET_Node, ET_Dependent, ET_LineElement, ET_Beam3D, ET_Beam2D, ET_Truss3D, ET_Truss2D,ET_Cable3D, ET_Cable3N, 
	ET_Material, ET_Force_Node, ET_Force_Gravity, ET_Force_Node_harmonic, ET_Force_Node_Function, ET_Force_Wind, ET_Force_Ice, ET_Constraint,
	ET_Section_Beam2D, ET_Section_Beam3D, ET_Section_Truss, ET_Section_Cable, ET_ReaderWriter, ET_Section_Assign, ET_AnalysisStep, ET_Line
};

class Structure;

class EntityBase
{//�������
private:
	static std::map<int, Structure*> s_pStructures; // �ṹָ��

	virtual enum Entity_Type My_EntityType() const = 0;

public:
	static const double pi;

	int m_id; // ���

	int m_idStructure = -1; // ģ�ͱ��

	void Set_idStructure(int id)
	{
		m_idStructure = id;
	}
	int Get_idStructure() const
	{
		return m_idStructure;
	}

	int Get_id()
	{
		return m_id;
	}

	static void AddToStructures(Structure* pStructure);

	Structure* Get_Structure() const
	{
		if (m_idStructure == -1)
		{
			std::cout << "�ṹָ��û�����ã�\n";
			exit(1);
		}
		return s_pStructures[m_idStructure];
	}
};


