#pragma once
#include "EntityBase.h"
#include <QDataStream>
#include <QTextStream>
#include <Eigen/Dense>
using namespace Eigen;

class ForceBase :
	public EntityBase
{
public:
	int idStep; //��������
	int m_direction;
	bool overTime = false; //�Ƿ���ʱ��仯
	bool isInherit = true; //�Ƿ񴫵�

	virtual void Input(QDataStream& fin) = 0;
	virtual void Input(QTextStream& fin) = 0;
	virtual void Disp() = 0;
	virtual void Assemble_Force(VectorXd& Force, const double t) = 0;

};

