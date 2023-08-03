#pragma once
class Ele_Location
{
public:
	Ele_Location();
	~Ele_Location();
	int m_id;
	double m_x;
	double m_y;
	double m_z;
	Ele_Location(int id, double x, double y, double z)
	{
		m_id = id;
		m_x = x;
		m_x = y;
		m_x = z;

	}

};

