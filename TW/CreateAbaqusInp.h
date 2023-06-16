#pragma once
#include"Tower.h"
#include <iostream>
#include<fstream>
class CreateAbaqusInp
{
public:
	CreateAbaqusInp();

	Tower* tw = nullptr;
	void CreateInp(Tower* tower);//输出abaqus可使用的.inp文件
	ofstream Inpout;           //创建ofstream
	void NodeInp();
};

