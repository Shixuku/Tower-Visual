#pragma once
#include "EntityBase.h"
#include <iostream>
#include <QDataStream>
#include <QTextStream>

#include "dll_ansys_global.h"


class DLL_ANSYS_EXPORT Node_Base : public EntityBase
{
public:
	double m_x = 0, m_y = 0, m_z = 0;

	Node_Base(){}
	Node_Base(int id, double x, double y, double z)
	{
		m_id = id, m_x = x, m_y = y, m_z = z;
	}

	void Input(QDataStream& fin)
	{
		fin >> m_id >> m_x >> m_y >> m_z;
	}

	void Input(QTextStream& fin)
	{
		fin >> m_id >> m_x >> m_y >> m_z;
	}

	void SaveTo(QDataStream& fin) const
	{
		fin << m_id << m_x << m_y << m_z;
	}
	void Disp()
	{
		std::cout << m_id << " " << m_x << " " << m_y << " " << m_z << "\n";
	}

	virtual enum Entity_Type My_EntityType() const
	{
		return ET_Node;
	}
};

