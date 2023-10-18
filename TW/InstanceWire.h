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
#include <iostream>
#include<fstream>
#pragma execution_character_set("utf-8")
using namespace std;
class InterFace;
class Part_Base;
class TowerWireGroup;
class Tower;
class TowerList;
class HangPoint;
class InstanceWire
{
public:
	void GetAllInstanceTXT(string path, vector<string>& files);
	void ReadInstanceWire(InterFace* InterFace);
	InterFace* m_pInterFace = nullptr;
	QString str;
	QFile Qf1;
	QFile Qf;  // 创建 QFile 对象并指定文件路径
	QTextStream Stream1;
	QTextStream Stream;         //创建ofstream
	TowerWireGroup* towerWire = nullptr;
	Tower* tower = nullptr;
	TowerList* towerList = nullptr;
	int Tension = 0;//耐张段数
	int Line = 0;//直线塔或者耐张塔
	int TowerSize = 0;
	int Ground = 0;//地线数量
	int Conductor = 0;//导线
	int Spilt = 0;
	double x = 0;
	double y = 0;
	double z = 0;
	double dAngle = 0;
	void SectionList(QStringList parts);
	void GroungLineList(QStringList parts);
	void instanceList(QStringList parts);
	void TowerPosition(QStringList parts);
	void Angle(QStringList parts);
	void NomHeight(QStringList parts);
	void LegList(QStringList parts);
	void StringList(QStringList parts);
	void showGroup(TowerWireGroup* towerWireGroup);
	void CreatTowerWierGroupItem(QString lineName);//主界面创建点击的item
	void WriteHangList();
	int LineSegment1 = 0;
	int LineSegment2 = 0;

};

