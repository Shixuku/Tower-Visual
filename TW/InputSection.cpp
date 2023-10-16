#include "InputSection.h"
#include "Section_L.h"
void InputSection::ReadSection(InterFace* InterFace)
{
	m_pInterFace = InterFace;
	str = "../section/Sections.txt";
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
			QString section_LLine = Stream1.readLine();
			QStringList section_LParts = section_LLine.split(QRegExp("[\\s,]+"), Qt::SkipEmptyParts);
			QString section_LName = section_LParts[0];//����
			double Iu = section_LParts[1].toDouble();//����ģ��
			double Iv = section_LParts[2].toDouble();//���ɱ�
			double J = section_LParts[3].toDouble();//���ɱ�
			double A = section_LParts[4].toDouble();//���ɱ�
			int size = m_pInterFace->ME_Section_L.size() + 1;
			Section_L* section = new Section_L(size, section_LName, Iu, Iv, J,A);
			m_pInterFace->ME_Section_L.Add_Entity(section);
		}
		Qf1.close();
	}
}
