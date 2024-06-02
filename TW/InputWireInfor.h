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
	void OpenReadWireInfor(InterFace* InterFace);//�����
	void ReadWireInfor(InterFace* InterFace);//Ĭ�ϴ�
	void GetAllHangPointTXT(string path, vector<string>& files);
	bool ReadLine(QTextStream& ssin, QString& str)
	{//���ļ��ж�ȡһ����Ч����
		while (!ssin.atEnd())
		{
			str = ssin.readLine();
			if (str.left(2).compare("**") == 0) continue;//ע���У�������ȡ��һ��

			str = str.trimmed();//ȥ����ͷ��ĩβ�Ŀո�
			if (str.isEmpty()) continue;//ע���У�������ȡ��һ��

			return true;//��ȡ����Ч���ݣ����һ�ж�ȡ
		}

		return false;//�ļ����꣬û�еõ���Ч��������
	}

	bool Input_FemData(const QString& FileName);
	bool OpenInput_FemData(const QString& FileName);//����򿪶�ȡ
	//static  bool cmp(const Insulator_Base& left, const Insulator_Base& right);
	static  void sort_line(vector<Insulator_Base>& object);


	WireData* wd = nullptr;
};

