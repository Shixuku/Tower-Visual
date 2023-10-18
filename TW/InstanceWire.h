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
	QFile Qf;  // ���� QFile ����ָ���ļ�·��
	QTextStream Stream1;
	QTextStream Stream;         //����ofstream
	TowerWireGroup* towerWire = nullptr;
	Tower* tower = nullptr;
	TowerList* towerList = nullptr;
	int Tension = 0;//���Ŷ���
	int Line = 0;//ֱ��������������
	int TowerSize = 0;
	int Ground = 0;//��������
	int Conductor = 0;//����
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
	void CreatTowerWierGroupItem(QString lineName);//�����洴�������item
	void WriteHangList();
	int LineSegment1 = 0;
	int LineSegment2 = 0;

};

