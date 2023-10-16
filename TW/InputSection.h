#pragma once
#include"InterFace.h"
#include<QStringList>
#include<QFile>
#include<QTextStream>
#include<QString>
class InputSection
{
public:
	void ReadSection(InterFace* m_pInterFace);
	InterFace* m_pInterFace = nullptr;
	QString str;
	QFile Qf1;
	QTextStream Stream1;

};

