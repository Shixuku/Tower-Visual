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
		double Stress = materialParts[5].toDouble();//�����ܶ�
		double thermalExpansion = materialParts[6].toDouble();//������ϵ��
		int size = m_pInterFace->ME_Material.size()+1;
		Material* material = new Material(size, materialName, elasticModulus, poissonRatio, density, Stress,thermalExpansion);
		m_pInterFace->ME_Material.Add_Entity(material);
		++processedCount;
	}
}


