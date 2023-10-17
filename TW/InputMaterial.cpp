#include "InputMaterial.h"
#include"Material.h"
#include"Section_L.h"
#include"Section_C.h"
void InputMaterial::ReadMaterial(InterFace* InterFace)
{
	m_pInterFace = InterFace;
	str = "../material/Material.txt";
	if (!str.isEmpty())
	{
		qDebug() << str;

		Qf1.setFileName(str);
		if (!Qf1.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "文件打开失败！";
			return;
		}
		Stream1.setDevice(&Qf1);

		QString keyword;
		while (!Stream1.atEnd())
		{
			QString qLine = Stream1.readLine();

			if (qLine.startsWith("**") || qLine.isEmpty())
			{
				// 跳过说明行和空行
				continue;
			}
			else if (qLine.startsWith("*"))
			{
				QStringList parts = qLine.mid(1).split(",", Qt::SkipEmptyParts);
				keyword = parts[0].trimmed();
				if (keyword.compare("LineMaterial", Qt::CaseInsensitive) == 0)
				{
					MaterialList(parts);
				}
				else if (keyword.compare("GroundMaterial", Qt::CaseInsensitive) == 0)
				{
					MaterialList(parts);
				}
				else if (keyword.compare("StringMaterial", Qt::CaseInsensitive) == 0)
				{
					MaterialList(parts);
				}
				else if (keyword.compare("Material", Qt::CaseInsensitive) == 0)
				{
					MaterialList(parts);
				}
				//else if (keyword.compare("Section_L", Qt::CaseInsensitive) == 0)
				//{
				//	Section_LList(parts);
				//}
			}
		}
		Qf1.close();
	}
}

void InputMaterial::MaterialList(QStringList parts)
{
	int count = parts[1].toInt();//得到行数
	int processedCount = 0; // 记录已处理的数据行数量
	while (processedCount < count)
	{
		QString materialLine = Stream1.readLine();
		if (materialLine.isEmpty() || materialLine.startsWith("**"))
		{
			// 跳过空行和说明行
			continue;
		}

		QStringList materialParts = materialLine.split(QRegExp("[\\s,]+"), Qt::SkipEmptyParts);
		int materialNumber = materialParts[0].toInt();//编号
		QString materialName = materialParts[1];//名称
		double elasticModulus = materialParts[2].toDouble();//弹性模量
		double poissonRatio = materialParts[3].toDouble();//泊松比
		double density = materialParts[4].toDouble();//质量密度
		double Stress = materialParts[5].toDouble();//质量密度
		double thermalExpansion = materialParts[6].toDouble();//热膨胀系数
		int size = m_pInterFace->ME_Material.size()+1;
		Material* material = new Material(size, materialName, elasticModulus, poissonRatio, density, Stress,thermalExpansion);
		m_pInterFace->ME_Material.Add_Entity(material);
		++processedCount;
	}
}


