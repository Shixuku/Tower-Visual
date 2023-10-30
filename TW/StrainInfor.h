#pragma once
#include<QDataStream>
#include <QRegularExpression>
#include <qDebug>
#include<vector>
class StrainInfor
{
public:
	std::vector<double>m_f;//最低点应力
	std::vector<QString>m_Ma;//材料名称
	std::vector<QString>m_R;//直径
	StrainInfor() {};
	StrainInfor(std::vector<double>f, std::vector<QString>Ma, std::vector<QString>R)
	{
		m_f = f;
		m_Ma = Ma;
		m_R = R;
	}
	void ReadInfoSus(QString& strlist_node);
	std::vector<StrainInfor>aa;
};

