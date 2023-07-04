#pragma once

#include "EntityBase.h"
#include <QDataStream>
#include <QTextStream>
#include <vector>

using namespace std;
class Element_Base:public EntityBase
{
public:
	vector<int> m_idNode;//½Úµã±àºÅ
	int m_idMaterial;
	int m_idSection;

	Element_Base();

	virtual void Input(QDataStream& fin);
	virtual void Input(QTextStream& fin);
	void Input_inp(QTextStream& fin);
	virtual void Disp();
	virtual void SaveTo(QDataStream& fin) const;

	virtual enum Entity_Type My_EntityType() const
	{
		return ET_LineElement;
	}
};

