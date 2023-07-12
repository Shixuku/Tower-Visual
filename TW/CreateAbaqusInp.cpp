#include "CreateAbaqusInp.h"
#include<string>
CreateAbaqusInp::CreateAbaqusInp()
{

}

void CreateAbaqusInp::CreateInp(Instance* instance)
{
	tw = instance;
	tw->VectorToMap();
	Inpout.open("../model.inp");
	Inpout << "*Heading" << "\n";
	Inpout << "*Preprint, echo=NO, model=NO, history=NO, contact=NO" << "\n";
	Inpout << "*Part, name=Part-1" << "\n";
	Inpout << "*Assembly, name=Assembly" << "\n";
	Inpout << "*End Part" << "\n";
	Inpout << "*Assembly, name=Assembly" << "\n";
	Inpout << "*Instance, name=Part-1-1, part=Part-1" << "\n";
	NodeInp();
	TrussInp();
	BeamInp();
	ElementGroup();
	TrussSectionInp();
	BeamSectionInp();
	Inpout << "*End Instance" << "\n";
	Inpout << "*End Assembly" << "\n";
	MaterialInp();
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

void CreateAbaqusInp::TrussInp()
{
	if (tw->m_Elements_Trusses.size() != 0)
	{
		Inpout << "*Element, type=T3D2" << "\n";
		for (int i = 0; i < tw->m_Elements_Trusses.size(); i++)
		{
			Inpout << tw->m_Elements_Trusses[i].m_idElement << "," << tw->m_Elements_Trusses[i].m_idNode[0] << "," << tw->m_Elements_Trusses[i].m_idNode[1]
				<< "\n";
		}
	}
}

void CreateAbaqusInp::BeamInp()
{
	if (tw->m_Elements_beams.size() != 0)
	{
		Inpout << "**Element, type=B33" << "\n";
		for (int i = 0; i < tw->m_Elements_beams.size(); i++)
		{
			Inpout << tw->m_Elements_beams[i].m_idElement << "," << tw->m_Elements_beams[i].m_idNode[0] << "," << tw->m_Elements_beams[i].m_idNode[1]
				<< "\n";
		}
	}
}

void CreateAbaqusInp::ElementGroup()
{
	for (int i = 0; i < tw->m_Elements.size(); i++)
	{
		Inpout <<"*Elset,elset=Set-" << tw->m_Elements[i].m_idElement << "\n";
		Inpout << tw->m_Elements[i].m_idElement << "\n";
	}
}

void CreateAbaqusInp::TrussSectionInp()
{
	for (int i = 0; i < tw->m_Elements_Trusses.size(); i++)
	{
		Inpout << "*Solid Section,elset =Set-" << tw->m_Elements_Trusses[i].m_idElement << ",material =Material-" << tw->m_Elements_Trusses[i].MaterialID
			<< "\n";
		Inpout << tw->SectionData[tw->m_Elements_Trusses[i].ClassSectionID].S*1e-6 << "\n";//用杆的材料信息去材料库里找到截面信息
	}
}

void CreateAbaqusInp::BeamSectionInp()
{
	for (int i = 0; i < tw->m_Elements_beams.size(); i++)
	{
		string SectionClass;
		int SectionID = tw->m_Elements_beams[i].ClassSectionID;
		if (tw->SectionData[SectionID].ClassSe == 0)
		{
			SectionClass = "L";
		}
		Inpout << "*Beam Section, elset=Set-" << tw->m_Elements_beams[i].m_idElement << ",material =Material-" << tw->m_Elements_beams[i].MaterialID
			<<",section ="<< SectionClass << "\n";
		Inpout << tw->SectionData[SectionID].a * 1e-3 <<"," << tw->SectionData[SectionID].a * 1e-3 << ","
			<< tw->SectionData[SectionID].b * 1e-3 << "," << tw->SectionData[SectionID].b * 1e-3 << "\n";//用杆的材料信息去材料库里找到截面信息
		Inpout << tw->m_Elements_beams[i].direction[1] << "," << tw->m_Elements_beams[i].direction[0] << "," 
			<< tw->m_Elements_beams[i].direction[2] << "\n";
	}
}

void CreateAbaqusInp::MaterialInp()
{
	Inpout << "*Material, name = Material-1" << "\n";
	Inpout << "*Density" << "\n";
	Inpout << "7800," << "\n";
	Inpout << "*Elastic" << "\n";
	Inpout << "2e+11,0.3" << "\n";
}
