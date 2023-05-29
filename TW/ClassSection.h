#pragma once
#include<iostream>

#include <cstring>
#include <string>
using namespace std;
class ClassSection
{
public:
	string name;
	int ClassSe = 0;
	double a = 0;
	double b = 0;
	int ClassM = 0;

	ClassSection(string str,int iClass, double ia, double ib, int iClassM)
	{
		name = str;
		ClassSe = iClass;
		a = ia;
		b = ib;
		ClassM = iClassM;
	}
};

