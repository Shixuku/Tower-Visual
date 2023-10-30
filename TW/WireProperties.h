#pragma once
#include<QDataStream>
#include <QRegularExpression>
#include <qDebug>
#include <vector>
#include "Base.h"


class InputWireInfor;

class WireProperties : public Base
{
public:
	int idTension = 0;
	int MaId = 0;
	bool isGround = false; //地线or导线
	double w_f = 0;
	double w_rou = 0;
	double w_area = 0;
	WireProperties() {};
	WireProperties(double m_f1, double m_rou1,  double m_area1)
	{
		w_f = m_f1;
		w_rou = m_rou1;
		w_area = m_area1;

	}
	static int GetSectionId(QString MaterialName, QString SectionName, InterFace* m_pInterFace);
	static void ReadInfoSus(InputWireInfor* in, QTextStream& ssin, std::vector<WireProperties>&Propertys);
	
};

