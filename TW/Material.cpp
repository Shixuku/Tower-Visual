#include "Material.h"

Material::Material(int id, QString iName, double iE, double iPoisson, double iDensity, double iThermal)
{
	m_id = id;
	m_Name= iName;
	E= iE;//����ģ��
	Poisson= iPoisson;//����
	Density= iDensity;//�ܶ�
	Thermal= iThermal;//������
}

Material::~Material()
{
}
