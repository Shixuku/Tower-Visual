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
	int idStep; //分析步号
	int m_direction;
	bool overTime = false; //是否随时间变化
	bool isInherit = true; //是否传递

	virtual void Input(QDataStream& fin) = 0;
	virtual void Input(QTextStream& fin) = 0;
	virtual void Disp() = 0;
	virtual void Assemble_Force(VectorXd& Force, const double t) = 0;

};

