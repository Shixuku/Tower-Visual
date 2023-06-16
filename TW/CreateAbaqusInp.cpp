#include "CreateAbaqusInp.h"

CreateAbaqusInp::CreateAbaqusInp()
{

}

void CreateAbaqusInp::CreateInp(Tower* tower)
{
	tw = tower;
	Inpout.open("../model.inp");
	Inpout << "*Heading" << "\n";
	Inpout << "*Preprint, echo=NO, model=NO, history=NO, contact=NO" << "\n";
	Inpout << "*Part, name=Part-1" << "\n";
	Inpout << "*Assembly, name=Assembly" << "\n";
	Inpout << "*End Part" << "\n";
	Inpout << "*Instance, name=Part-1-1, part=Part-1" << "\n";
	NodeInp();
	Inpout.close();

}

void CreateAbaqusInp::NodeInp()
{
	Inpout << "*Node" << "\n";
	for (int i = 0; i < tw->m_Nodes.size(); i++)
	{
		Inpout  << tw->m_Nodes[i].m_idNode << "," << tw->m_Nodes[i].x * 1e-3 << "," << tw->m_Nodes[i].y * 1e-3 <<
			"," << tw->m_Nodes[i].z * 1e-3 << "\n";
	}
}
