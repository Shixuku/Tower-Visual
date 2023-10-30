#pragma once
#include<QDataStream>
#include <QRegularExpression>
#include <qDebug>

class Insulator_Base
{
public:
	QString m_type;
	int m_idSegment; //���Ŷ�
	double H = 0;
	double angle_1 = 0;
	double angle_2 = 0;
	double m_x = 0;
	double m_y = 0;
	double m_z = 0;
	double m_x1 = 0;
	double m_y1 = 0;
	double m_z1 = 0;
	double m_x2 = 0;
	double m_y2 = 0;
	double m_z2 = 0;
	int m_line = 0;//��·��ʶ��
	Insulator_Base() {};
	Insulator_Base(QString type,double x1, double y1, double z1, double x2, double y2, double z2, double a1, double a2, int line) //V��
	{
		m_type = type;
		m_x1 = x1;
		m_y1 = y1;
		m_z1 = z1;
		m_x2 = x2;
		m_y2 = y2;
		m_z2 = z2;
		angle_1 = a1;
		angle_2 = a2;
		m_line = line;
	}
	Insulator_Base(QString type, double x, double y, double z, double h, int line)//����
	{
		m_type = type;
		m_x = x;
		m_y = y;
		m_z = z;
		H = h;
		m_line = line;
	}

	void ReadInfoSus(QString& strlist_node);

	void TypeH(QStringList& strlist);//����
	void TypeV(QStringList& strlist);//V��
	void TypeO(QStringList& strlist);//����
	void TypeG(QStringList& strlist);//����
	void LineConversion(QString &id);
};

