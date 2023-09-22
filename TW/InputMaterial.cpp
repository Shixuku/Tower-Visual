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
			qDebug() << "�ļ���ʧ�ܣ�";
			return;
		}
		Stream1.setDevice(&Qf1);

		QString keyword;
		while (!Stream1.atEnd())
		{
			QString qLine = Stream1.readLine();

			if (qLine.startsWith("**") || qLine.isEmpty())
			{
				// ����˵���кͿ���
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
	int count = parts[1].toInt();//�õ�����
	int processedCount = 0; // ��¼�Ѵ��������������
	while (processedCount < count)
	{
		QString materialLine = Stream1.readLine();
		if (materialLine.isEmpty() || materialLine.startsWith("**"))
		{
			// �������к�˵����
			continue;
		}

		QStringList materialParts = materialLine.split(QRegExp("[\\s,]+"), Qt::SkipEmptyParts);
		int materialNumber = materialParts[0].toInt();//���
		QString materialName = materialParts[1];//����
		double elasticModulus = materialParts[2].toDouble();//����ģ��
		double poissonRatio = materialParts[3].toDouble();//���ɱ�
		double density = materialParts[4].toDouble();//�����ܶ�
		double thermalExpansion = materialParts[5].toDouble();//������ϵ��
		Material* material = new Material(materialNumber, materialName, elasticModulus, poissonRatio, density, thermalExpansion);
		m_pInterFace->ME_Material.Add_Entity(material);
		++processedCount;
	}
}

void InputMaterial::Section_LList(QStringList parts)
{
	int count = parts[1].toInt();//�õ�����
	int processedCount = 0; // ��¼�Ѵ��������������
	while (processedCount < count)
	{
		QString section_LLine = Stream1.readLine();
		if (section_LLine.isEmpty() || section_LLine.startsWith("**"))
		{
			// �������к�˵����
			continue;
		}
		QStringList section_LParts = section_LLine.split(QRegExp("[\\s,]+"), Qt::SkipEmptyParts);
		QString section_LName = section_LParts[0];//����
		double Iu = section_LParts[1].toDouble();//����ģ��
		double Iv = section_LParts[2].toDouble();//���ɱ�
		double J = section_LParts[3].toDouble();//���ɱ�
		Section_L* section = new Section_L(processedCount + 1, section_LName, Iu, Iv, J);
		m_pInterFace->ME_Section_L.Add_Entity(section);
		processedCount++;
	}
}

void InputMaterial::Section_CList(QStringList parts)
{
	int count = parts[1].toInt();//�õ�����
	int processedCount = 0; // ��¼�Ѵ��������������
	while (processedCount < count)
	{
		QString section_CLine = Stream1.readLine();
		if (section_CLine.isEmpty() || section_CLine.startsWith("**"))
		{
			// �������к�˵����
			continue;
		}
		QStringList section_CParts = section_CLine.split(QRegExp("[\\s,]+"), Qt::SkipEmptyParts);
		QString section_LName = section_CParts[0];//����
		double Iu = section_CParts[1].toDouble();//����ģ��
		double Iv = section_CParts[2].toDouble();//���ɱ�
		double J = section_CParts[3].toDouble();//���ɱ�
		Section_C* section = new Section_C(processedCount + 1, section_LName, Iu, Iv, J);
		m_pInterFace->ME_Section_C.Add_Entity(section);
		processedCount++;
	}
}
