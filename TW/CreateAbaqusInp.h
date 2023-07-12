#pragma once
#include"Instance.h"
#include <iostream>
#include<fstream>
class CreateAbaqusInp
{
public:
	CreateAbaqusInp();

	Instance* tw = nullptr;
	void CreateInp(Instance* instance);//输出abaqus可使用的.inp文件
	ofstream Inpout;           //创建ofstream
	void NodeInp();//节点信息
	void TrussInp();//杆信息
	void BeamInp();//梁信息
	void ElementGroup();
	void TrussSectionInp();
	void BeamSectionInp();
	void MaterialInp();
};

