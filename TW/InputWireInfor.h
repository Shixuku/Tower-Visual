#pragma once
#include<QStringList>
#include<QFile>
#include<QTextStream>
#include<QString>
#include <io.h>
#include <iostream>
#include <string>
#include <vector>
#include<QFileDialog>
#include"Insulator_Base.h"
#include"WireProperties.h"
#pragma execution_character_set("utf-8")
using namespace std;
class InterFace;
class WireData;
class TowerWireGroup;
class InputWireInfor
{
public:
	TowerWireGroup* towerWire = nullptr;
	InterFace* m_pInterFace = nullptr;
	void OpenReadWireInfor(InterFace* InterFace);//界面打开
	void ReadWireInfor(InterFace* InterFace);//默认打开
	void GetAllHangPointTXT(string path, vector<string>& files);
	bool ReadLine(QTextStream& ssin, QString& str)
	{//从文件中读取一行有效数据
		while (!ssin.atEnd())
		{
			str = ssin.readLine();
			if (str.left(2).compare("**") == 0) continue;//注释行，继续读取下一行

			str = str.trimmed();//去除开头和末尾的空格
			if (str.isEmpty()) continue;//注释行，继续读取下一行

			return true;//读取到有效数据，完成一行读取
		}

		return false;//文件读完，没有得到有效的数据行
	}

	bool Input_FemData(const QString& FileName);
	bool OpenInput_FemData(const QString& FileName);//界面打开读取
	//static  bool cmp(const Insulator_Base& left, const Insulator_Base& right);
	static  void sort_line(vector<Insulator_Base>& object);


	WireData* wd = nullptr;
};

