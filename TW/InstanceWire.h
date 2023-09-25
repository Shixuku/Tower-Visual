#pragma once
#include <iostream>
#include<QStringList>
#include<QFile>
#include<QTextStream>
#include<QString>
#include <io.h>
#include <string>
#include <vector>
#include<QStringList>
#include<QTextStream>
#pragma execution_character_set("utf-8")
using namespace std;
class InterFace;
class Part_Base;
class TowerWireGroup;
class Tower;
class TowerList;
class InstanceWire
{
public:
	void GetAllInstanceTXT(string path, vector<string>& files);
	void ReadInstanceWire(InterFace* InterFace);
	InterFace* m_pInterFace = nullptr;
	QString str;
	QFile Qf1;
	QTextStream Stream1;
	TowerWireGroup* towerWire = nullptr;
	Tower* tower = nullptr;
	TowerList* towerList = nullptr;
	int Tension = 0;//耐张段数
	int Line = 0;//直线塔或者耐张塔
	int TowerSize = 0;
	int Spilt = 0;
	double x = 0;
	double y = 0;
	double z = 0;
	double dAngle = 0;
	void instanceList(QStringList parts);
	void TowerPosition(QStringList parts);
	void Angle(QStringList parts);
	void NomHeight(QStringList parts);
	void LegList();
	void showGroup(TowerWireGroup* towerWireGroup);
};

