#pragma once
#include"InterFace.h"
#include<QStringList>
#include<QFile>
#include<QTextStream>
#include<QString>
class InputMaterial
{
public:
	void ReadMaterial(InterFace* m_pInterFace);
	InterFace* m_pInterFace = nullptr;
	QString str;
	QFile Qf1;
	QTextStream Stream1;
	void MaterialList(QStringList parts);
	void Section_LList(QStringList parts);
	void Section_CList(QStringList parts);
};

