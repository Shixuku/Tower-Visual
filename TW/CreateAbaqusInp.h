#pragma once
#include"Tower.h"
#include <iostream>
#include<fstream>
class CreateAbaqusInp
{
public:
	CreateAbaqusInp();

	Tower* tw = nullptr;
	void CreateInp(Tower* tower);//���abaqus��ʹ�õ�.inp�ļ�
	ofstream Inpout;           //����ofstream
	void NodeInp();
};

