#include "Insulator_Base.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)    
#pragma execution_character_set("utf-8")    
#endif

void Insulator_Base::ReadInfoSus(QString& strdata)
{
	QStringList strlist_node = strdata.split(QRegularExpression("[\t, ]"), Qt::SkipEmptyParts);//利用空格,分解字符串
	QString Type = strlist_node[0];
	Type.replace("\"", ""); // 去掉双引号
	//qDebug() << "Type: " << Type;
	switch (Type.toStdString()[0])
	{
	case 'H':
		TypeH(strlist_node);
		break;
	case 'G':
		TypeG(strlist_node);
		break;
	case 'V':
		TypeV(strlist_node);
		break;
	case 'O':
		TypeO(strlist_node);
		break;
	default:
		// 如果 Type 不匹配任何情况，可以选择处理默认情况或不采取任何操作
		break;
	}

}

void Insulator_Base::TypeH(QStringList& strlist)
{
	QString SusType = strlist[0];
	SusType.replace("\"", ""); // 去掉引号
	QString lineType = strlist[1]; //线路编号
	lineType.replace("\"", ""); // 去掉引号
	double tag = strlist[2].toDouble(); //串长度
	//int fenlie = strlist[3].toInt(); //分裂数
	int idSegment = strlist[3].toInt();
	double xi = strlist[4].toDouble();
	double yi = strlist[5].toDouble();
	double zi = strlist[6].toDouble();
	LineConversion(lineType);
	//生成挂点
	m_type = SusType;
	m_line = lineType.toInt();
	H = tag;
	m_x = xi;
	m_y = yi;
	m_z = zi;
	m_idSegment = idSegment;

	qDebug() << SusType << "  " << lineType << " " << tag << " " << m_idSegment << " " << xi << " " << yi << " " << zi;//输出，以便检查
}
void Insulator_Base::TypeV(QStringList& strlist)
{
	QString SusType = strlist[0];
	SusType.replace("\"", ""); // 去掉引号
	QString lineType = strlist[1]; //线路编号
	lineType.replace("\"", ""); // 去掉引号

	int idSegment = strlist[2].toInt();
	double angle1 = strlist[3].toDouble();
	double angle2 = strlist[4].toDouble();
	double x1 = strlist[5].toDouble();
	double y1 = strlist[6].toDouble();
	double z1 = strlist[7].toDouble();
	double x2 = strlist[8].toDouble();
	double y2 = strlist[9].toDouble();
	double z2 = strlist[10].toDouble();
	LineConversion(lineType);
	//生成挂点
	m_type = SusType;
	m_line = lineType.toInt();
	angle_1 = angle1;
	angle_2 = angle2;
	m_x1 = x1;
	m_y1 = y1;
	m_z1 = z1;

	m_x2 = x2;
	m_y2 = y2;
	m_z2 = z2;
	m_idSegment = idSegment;

	qDebug() << SusType << "  " << lineType << " " << m_idSegment << " " << angle1 << " " << angle2 << " " << x1 << " " << y1 << " " << z1 << " " << x2 << " " << y2 << " " << z2;//输出，以便检查
}
void Insulator_Base::TypeO(QStringList& strlist)
{
	QString SusType = strlist[0];
	SusType.replace("\"", ""); // 去掉引号
	QString lineType = strlist[1]; //线路编号
	lineType.replace("\"", ""); // 去掉引号
	double tag = strlist[2].toDouble(); //串长度
	//int fenlie = strlist[3].toInt(); //分裂数
	int idSegment = strlist[3].toInt();
	double xi = strlist[4].toDouble();
	double yi = strlist[5].toDouble();
	double zi = strlist[6].toDouble();
	LineConversion(lineType);
	lineType.replace("\"", ""); // 去掉引号
	//生成挂点
	m_type = SusType;
	m_line = lineType.toInt();
	H = tag;
	m_x = xi;
	m_y = yi;
	m_z = zi;
	m_idSegment = idSegment;

	qDebug() << SusType << "  " << lineType << " " << tag << " " << m_idSegment << " " << xi << " " << yi << " " << zi;//输出，以便检查
}
void Insulator_Base::TypeG(QStringList& strlist)
{
	QString SusType = strlist[0];
	SusType.replace("\"", ""); // 去掉引号
	QString lineType = strlist[1]; //线路编号
	lineType.replace("\"", ""); // 去掉引号
	double tag = strlist[2].toDouble(); //串长度
	int idSegment = strlist[3].toInt();
	double xi = strlist[4].toDouble();
	double yi = strlist[5].toDouble();
	double zi = strlist[6].toDouble();

	//生成挂点
	m_type = SusType;
	m_line = lineType.toInt();
	H = tag;
	m_x = xi;
	m_y = yi;
	m_z = zi;
	m_idSegment = idSegment;


	qDebug() << SusType << "  " << lineType  << " " << tag << " " << m_idSegment << " " << xi << " " << yi << " " << zi;//输出，以便检查
}
void Insulator_Base::LineConversion(QString &id)
{
	if (id == "1A")
	{
		id = "3";
	}
	else if (id == "1B")
	{
		id = "4";
	}
	else if (id == "1C")
	{
		id = "5";
	}
	else if (id == "2A")
	{
		id = "6";
	}
	else if (id == "2B")
	{
		id = "7";
	}
	else if (id == "2C")
	{
		id = "8";
	}
	
}
