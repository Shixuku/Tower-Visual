#pragma once
class ParameterConstraint
{
public:
	int m_idConstraint;
	int m_idNode;
	int m_Direction;
	ParameterConstraint(int idconstraint, int idnode, int direction)
	{
		m_idConstraint = idconstraint;
		m_idNode = idnode;
		m_Direction = direction;
	}
};

