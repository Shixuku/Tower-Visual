#include "StrainInfor.h"

void StrainInfor::ReadInfoSus(QString& strdata)
{
	QStringList strlist_node = strdata.split(QRegularExpression("[\t, ]"), Qt::SkipEmptyParts);//利用空格,分解字符串

}
