#include "InstanceWire.h"
#include"InterFace.h"
#include "NominalHeight.h"
#pragma execution_character_set("utf-8")
void InstanceWire::GetAllInstanceTXT(string path, vector<string>& files)
{
	intptr_t hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//�����Ŀ¼,�ݹ����
			//�������,���ļ�����·������vector��
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (string(fileinfo.name).substr(string(fileinfo.name).find_last_of(".") + 1) == "txt")
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void InstanceWire::ReadInstanceWire(InterFace* InterFace)
{
	m_pInterFace = InterFace;
	vector<string> fileNames;
	string path("../InstanceTXT"); 	//�Լ�ѡ��Ŀ¼����
	GetAllInstanceTXT(path, fileNames);
	//for (const auto& ph : fileNames) {
	//	std::cout << ph << "\n";
	//}
	//qDebug() << "fileNames.size()��" << fileNames.size() << "\t";
	//for (const auto& j : m_pInterFace->TWG)
	//{
	//	qDebug() << j.second->m_Name << "\t";
	//	for (const auto& i : j.second->TWG_TP)
	//	{
	//		qDebug() << i.second->m_Name << "  " << i.second->TP_Height.size() << "\t";
	//		for (const auto& l : i.second->TP_Height)
	//		{
	//			for (int i = 0; i < l.second->BodyList.size(); i++)
	//			{
	//				qDebug() << l.second->BodyList[i] << "\t";
	//			}
	//		}
	//	}
	//}
	for (int i = 0; i < fileNames.size(); i++)
	{
		str = QString::fromStdString(fileNames[i]);
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
					if (keyword.compare("Line", Qt::CaseInsensitive) == 0)
					{
						QString lineName = parts[1].trimmed();
						TowerSize = parts[2].toInt();
						Spilt= parts[3].toInt();
						for (const auto& j : m_pInterFace->TWG)
						{
							if (j.second->m_Name == lineName)
							{
								towerWire = j.second;
								break;
							}
						}
					}
					else if (keyword.compare("Instance", Qt::CaseInsensitive) == 0)
					{
						tower = new Tower();
						tower->m_id= parts[1].toInt();
						QString TowerName = parts[2];
						Line = parts[3].toInt();
						for (const auto& j : towerWire->TWG_TP)
						{
							if (j.second->m_Name == TowerName)
							{
								towerList = j.second;
								break;
							}
						}
						qDebug() << towerList->TP_Height.size() << "\t";
						if (towerList == nullptr)
						{
							qDebug() << "y������" << "\t";
						}
						if (Line == 0)
						{
							Tension++;
						}
						instanceList(parts);
					}
				}
			}
		}
	}
	showGroup(towerWire);
}

void InstanceWire::instanceList(QStringList parts)
{
	int instanceID = parts[1].toInt();
	int processedCount = 0;// ��¼�Ѵ���ĺ�������
	QString keyword;
	while (processedCount < 4)
	{
		QString line = Stream1.readLine();
		if (line.startsWith("**") || line.isEmpty())
		{
			// ������ "**" ��ͷ���кͿ���
			continue;
		}
		// ��������
		else if (line.startsWith("*"))
		{
			QStringList parts = line.mid(1).split(",", Qt::SkipEmptyParts);
			keyword = parts[0].trimmed();
			if (keyword.compare("TowerPosition", Qt::CaseInsensitive) == 0)
			{
				TowerPosition(parts);
			}
			else if (keyword.compare("Angle", Qt::CaseInsensitive) == 0)
			{
				Angle(parts);
			}
			else if (keyword.compare("Body", Qt::CaseInsensitive) == 0)
			{
				NomHeight(parts);
			}
			else if (keyword.compare("Leg", Qt::CaseInsensitive) == 0)
			{
				LegList();
			}
			processedCount++;
		}

	}
	if (tower != nullptr)
	{
		towerWire->AddTowerToGroup(tower, instanceID, x*1000, y*1000, z*1000, dAngle);
	}

}

void InstanceWire::TowerPosition(QStringList parts)
{
	int processedCount = 0;
	while (processedCount < 1)
	{
		QString line = Stream1.readLine();
		if (line.startsWith("**") || line.isEmpty())
		{
			// ������ "**" ��ͷ���кͿ���
			continue;
		}
		QStringList xyzLine = line.split(QRegExp("[\\s,]+"), Qt::SkipEmptyParts);
		x = xyzLine[0].toDouble();
		y = xyzLine[1].toDouble();
		z = xyzLine[2].toDouble();
		processedCount++;
	}
}

void InstanceWire::Angle(QStringList parts)
{
	int processedCount = 0;
	while (processedCount < 1)
	{
		QString line = Stream1.readLine();
		if (line.startsWith("**") || line.isEmpty())
		{
			// ������ "**" ��ͷ���кͿ���
			continue;
		}
		QStringList AngleLine = line.split(QRegExp("[\\s,]+"), Qt::SkipEmptyParts);
		dAngle = AngleLine[0].toDouble();
		processedCount++;
	}
}

void InstanceWire::NomHeight(QStringList parts)
{
	int highid = parts[1].toInt();
	qDebug() << towerList->TP_Height.size() << "\t";
    NominalHeight*heigt=towerList->TP_Height.Find_Entity(highid);

	for (const auto& j : heigt->BodyList)
	{
		Part_Base* part = towerList->TP_Part.Find_Entity(j);
		tower->addPart(part);
	}

}

void InstanceWire::LegList()
{
	int processedCount = 0;
	while (processedCount < 1)
	{
		QString line = Stream1.readLine();
		if (line.startsWith("**") || line.isEmpty())
		{
			// ������ "**" ��ͷ���кͿ���
			continue;
		}
		QStringList legLine = line.split(QRegExp("[\\s,]+"), Qt::SkipEmptyParts);
		int LegId = legLine[0].toInt();//����A
		int LegIdB = legLine[1].toInt();//B
		int LegIdC = legLine[2].toInt();//C
		int LegIdD = legLine[3].toInt();//D

		//����A--��������
		Part_Base* part = towerList->TP_Part.Find_Entity(LegId);//A
		if (part == nullptr)
		{
			qDebug() << "������" << "\t";
		}
		for (int i = 0; i < part->m_Nodes.size(); i++)
		{
			part->m_Nodes[i].x *= -1;
			part->m_Nodes[i].y *= -1;
		}
		if (part != nullptr)
		{
			tower->addPart(part);
			//��ԭ
			for (int i = 0; i < part->m_Nodes.size(); i++)
			{
				part->m_Nodes[i].x *= -1;
				part->m_Nodes[i].y *= -1;
			}
		}
		//����B--�ڶ�����
		Part_Base* partB = towerList->TP_Part.Find_Entity(LegIdB);//B
		for (int i = 0; i < partB->m_Nodes.size(); i++)
		{
			partB->m_Nodes[i].x *= -1;
		}
		if (partB != nullptr)
		{
			tower->addPart(partB);
			for (int i = 0; i < partB->m_Nodes.size(); i++)
			{
				partB->m_Nodes[i].x *= -1;
			}
		}
		//����C--��һ����
		Part_Base* partC = towerList->TP_Part.Find_Entity(LegIdC);//C
		if (partC != nullptr)
		{
			tower->addPart(partC);
		}
		//����D--��������
		Part_Base* partD = towerList->TP_Part.Find_Entity(LegIdD);//D
		for (int i = 0; i < partD->m_Nodes.size(); i++)
		{
			partD->m_Nodes[i].y *= -1;
		}
		if (partD != nullptr)
		{
			tower->addPart(partD);
			for (int i = 0; i < partD->m_Nodes.size(); i++)
			{
				partD->m_Nodes[i].y *= -1;
			}
		}
		processedCount++;
	}
}
void InstanceWire::showGroup(TowerWireGroup* towerWireGroup)
{
	towerWireGroup->Show_VTKnode(m_pInterFace->m_Renderer);
	towerWireGroup->Show_VTKbeam(m_pInterFace->m_Renderer);
	towerWireGroup->Show_VTKtruss(m_pInterFace->m_Renderer);
	m_pInterFace->m_Renderer->ResetCamera();
}
