#pragma once
#include"Instance.h"
#include <iostream>
#include<fstream>
class CreateAbaqusInp
{
public:
	CreateAbaqusInp();

	Instance* tw = nullptr;
	void CreateInp(Instance* instance);//���abaqus��ʹ�õ�.inp�ļ�
	ofstream Inpout;           //����ofstream
	void NodeInp();//�ڵ���Ϣ
	void TrussInp();//����Ϣ
	void BeamInp();//����Ϣ
	void ElementGroup();
	void TrussSectionInp();
	void BeamSectionInp();
	void MaterialInp();
};

