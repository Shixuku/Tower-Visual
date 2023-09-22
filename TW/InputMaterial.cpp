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
				if (keyword.compare("Material", Qt::CaseInsensitive) == 0)
				{
					MaterialList(parts);
				}
				else if (keyword.compare("Section_L", Qt::CaseInsensitive) == 0)
				{
					Section_LList(parts);
				}
			}
		}
	}
	//for (const auto& j : m_pInterFace->ME_Material)
	//{
	//	qDebug() << j.second->m_id << "  " << j.second->m_Name << "/t";
	//}
	//for (const auto& j : m_pInterFace->ME_Section_L)
	//{
	//	qDebug() << j.second->m_id << "  " << j.second->name << "/t";
	//}
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
		double thermalExpansion = materialParts[5].toDouble();//热膨胀系数
		Material* material = new Material(materialNumber, materialName, elasticModulus, poissonRatio, density, thermalExpansion);
		m_pInterFace->ME_Material.Add_Entity(material);
		++processedCount;
	}
}

void InputMaterial::Section_LList(QStringList parts)
{
	int count = parts[1].toInt();//得到行数
	int processedCount = 0; // 记录已处理的数据行数量
	while (processedCount < count)
	{
		QString section_LLine = Stream1.readLine();
		if (section_LLine.isEmpty() || section_LLine.startsWith("**"))
		{
			// 跳过空行和说明行
			continue;
		}
		QStringList section_LParts = section_LLine.split(QRegExp("[\\s,]+"), Qt::SkipEmptyParts);
		QString section_LName = section_LParts[0];//名称
		double Iu = section_LParts[1].toDouble();//弹性模量
		double Iv = section_LParts[2].toDouble();//泊松比
		double J = section_LParts[3].toDouble();//泊松比
		Section_L* section = new Section_L(processedCount + 1, section_LName, Iu, Iv, J);
		m_pInterFace->ME_Section_L.Add_Entity(section);
		processedCount++;
	}
}

void InputMaterial::Section_CList(QStringList parts)
{
	int count = parts[1].toInt();//得到行数
	int processedCount = 0; // 记录已处理的数据行数量
	while (processedCount < count)
	{
		QString section_CLine = Stream1.readLine();
		if (section_CLine.isEmpty() || section_CLine.startsWith("**"))
		{
			// 跳过空行和说明行
			continue;
		}
		QStringList section_CParts = section_CLine.split(QRegExp("[\\s,]+"), Qt::SkipEmptyParts);
		QString section_LName = section_CParts[0];//名称
		double Iu = section_CParts[1].toDouble();//弹性模量
		double Iv = section_CParts[2].toDouble();//泊松比
		double J = section_CParts[3].toDouble();//泊松比
		Section_C* section = new Section_C(processedCount + 1, section_LName, Iu, Iv, J);
		m_pInterFace->ME_Section_C.Add_Entity(section);
		processedCount++;
	}
}
