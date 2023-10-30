#pragma once
#include<QStringList>
#include<QFile>
#include<QTextStream>
#include<QString>
#include <io.h>
#include <iostream>
#include <string>
#include <vector>
#pragma execution_character_set("utf-8")
using namespace std;
class InterFace;
class Part_Base;
class TowerWireGroup;
class Tower;
class TowerList;
class WireWiring
{
public:
	void GetAllPartTXT(string path, vector<string>& files);
	void ReadWireWiring(InterFace* InterFace);
	InterFace* m_pInterFace = nullptr;
	QString str;
	QFile Qf1;
	QTextStream Stream1;
	TowerWireGroup* towerWire = nullptr;
	TowerList* tower = nullptr;
	void PartList(QStringList parts);
	void NodeList(Part_Base* part, QStringList parts);
	void Element_Beam3DList(Part_Base* part, QStringList parts);
	int GetSectionId1(QString MaterialName, QString SectionName);//在姜瑞丰基础之上按照我的逻辑又写了一遍
	//后面四个参数要传指针才可以在函数外部继续用
	void GetLIxyz(double a, double b, double *Iu, double *Iv, double *J, double *S);
	void HighList(QStringList parts);
	void HangPointList(QStringList parts);
};


