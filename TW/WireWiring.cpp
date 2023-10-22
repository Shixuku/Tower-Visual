#include "WireWiring.h"
#include"InterFace.h"
#include"Material.h"
#include"Section_L.h"
#include"Section.h"
#include"NominalHeight.h"
#include"HangPoint.h"
#include"TowerList.h"
void WireWiring::GetAllPartTXT(string path, vector<string>& files)
{
	intptr_t hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,递归查找
			//如果不是,把文件绝对路径存入vector中
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

void WireWiring::ReadWireWiring(InterFace* InterFace)
{
	m_pInterFace = InterFace;
	vector<string> fileNames;
	string path("../PartTXT"); 	//自己选择目录测试
	GetAllPartTXT(path, fileNames);
	//for (const auto& ph : fileNames) {
	//	std::cout << ph << "\n";
	//}
	qDebug() << "fileNames.size()！"<< fileNames.size()<<"\t";
	for (int i = 0; i < fileNames.size(); i++)
	{
		str = QString::fromStdString(fileNames[i]);
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
					if (keyword.compare("Line", Qt::CaseInsensitive) == 0)
					{
						QString lineName = parts[1].trimmed();
						QString towerNumber = parts[2].trimmed();
						bool isExist = 0;
						for (const auto& j : m_pInterFace->TWG)
						{
							if (j.second->m_Name == lineName)
							{
								towerWire = j.second;
								isExist = 1;
							}
						}
						if (isExist == false)
						{
							towerWire = new TowerWireGroup();
							towerWire->m_Name = lineName;
							int id = m_pInterFace->TWG.size() + 1;
							towerWire->m_id = id;
							m_pInterFace->TWG.Add_Entity(towerWire);
						}
						tower = new TowerList();
						tower->m_Name = towerNumber;
						int id = towerWire->TWG_TP.size() + 1;
						tower->m_id = id;
						towerWire->TWG_TP.Add_Entity(tower);
					}
					else if (keyword.compare("Parts", Qt::CaseInsensitive) == 0)
					{
						PartList(parts);
					}
					else if (keyword.compare("High", Qt::CaseInsensitive) == 0)
					{
						HighList(parts);
					}
					else if (keyword.compare("HangPoint", Qt::CaseInsensitive) == 0)
					{
						HangPointList(parts);
					}
				}
			}

		}
	}
	for (const auto& j : m_pInterFace->TWG)
	{
		qDebug() << j.second->m_Name << "\t";
		for (const auto& i : j.second->TWG_TP)
		{
			qDebug() << i.second->m_Name << "  " << i.second->TP_Height.size() << "\t";
			for (const auto& l : i.second->TP_Height)
			{
				for (int i = 0; i < l.second->BodyList.size(); i++)
				{
					qDebug() << l.second->BodyList[i] << "\t";
				}
			}
		}
	}
}

void WireWiring::PartList(QStringList parts)
{
	int PartsNumber = parts[1].toInt();//部件总数
	int processedCount = 0;// 记录已处理的部件数量
	QString keyword;//记录部件的关键字
	Part_Base* Part = nullptr;
	while (processedCount < PartsNumber && !Stream1.atEnd())
	{
		QString qLine = Stream1.readLine();

		if (qLine.startsWith("**") || qLine.isEmpty())
		{
			// 跳过说明行和空行
			continue;
		}
		else if (qLine.startsWith("*"))
		{

			QStringList Partparts = qLine.mid(1).split(",", Qt::SkipEmptyParts);
			keyword = Partparts[0].trimmed();
			if (keyword.compare("Part", Qt::CaseInsensitive) == 0)
			{
				Part = new Part_Base();
				QString lineName = Partparts[1].trimmed();//部件类型
				int towerNumber = Partparts[2].toInt();
				QString drawingNumber = Partparts[3].trimmed();//图纸名称
				//QString LinkPart = Partparts[4].trimmed();//上接部件号
				Part->m_id = towerNumber;
				tower->TP_Part.Add_Entity(Part);
				//qDebug() << "tower->TP_Part" << tower->TP_Part.size()<<"\t";
			}
			else if (Part != nullptr)
			{
				if (keyword.compare("Node", Qt::CaseInsensitive) == 0)
				{
					NodeList(Part, Partparts);
				}
				else if (keyword.compare("Element_Beam3D", Qt::CaseInsensitive) == 0)
				{
					Element_Beam3DList(Part, Partparts);
					processedCount++;
				}
			}

		}
	}
}

void WireWiring::NodeList(Part_Base* part, QStringList parts)
{
	int NodeCount = parts[1].toInt();
	int processedCount = 0;// 记录已处理的部件数量
	while (processedCount < NodeCount)
	{
		QString NodeLine = Stream1.readLine();
		if (NodeLine.startsWith("**") || NodeLine.isEmpty())
		{
			// 跳过说明行和空行
			continue;
		}
		QStringList NodeParts = NodeLine.split(QRegExp("[\\s,]+"), Qt::SkipEmptyParts);
		int NodeNumber = NodeParts[0].toInt();
		double NodeX = NodeParts[1].toDouble() /100;
		double NodeY = NodeParts[2].toDouble() /100;
		double NodeZ = NodeParts[3].toDouble() /100;
		part->m_Nodes.push_back(Node(NodeNumber, NodeX, NodeY, NodeZ, 0));
		processedCount++;
	}
}

void WireWiring::Element_Beam3DList(Part_Base* part, QStringList parts)
{
	int Element_Beam3DCount = parts[1].toInt();
	int processedCount = 0;// 记录已处理的部件数量
	while (processedCount < Element_Beam3DCount)
	{
		QString Element_Beam3DLine = Stream1.readLine();
		if (Element_Beam3DLine.startsWith("**") || Element_Beam3DLine.isEmpty())
		{
			// 跳过说明行和空行
			continue;
		}
		QStringList Element_Beam3DParts = Element_Beam3DLine.split(QRegExp("[\\s,]+"), Qt::SkipEmptyParts);
		int Element_Beam3DNumber = Element_Beam3DParts[0].toInt();
		int Element_Beam3DidNode1 = Element_Beam3DParts[1].toDouble();
		int Element_Beam3DidNode2 = Element_Beam3DParts[2].toDouble();
		double Element_Beam3DDirectionX= Element_Beam3DParts[3].toDouble();
		double Element_Beam3DDirectionY = Element_Beam3DParts[4].toDouble();
		double Element_Beam3DDirectionZ = Element_Beam3DParts[5].toDouble();
		QString Element_Beam3DMaterial = Element_Beam3DParts[6];
		QString Element_Beam3DSection = Element_Beam3DParts[7];
		double Direction[3] = { Element_Beam3DDirectionX,Element_Beam3DDirectionY,Element_Beam3DDirectionZ };
		int id = GetSectionId(Element_Beam3DMaterial, Element_Beam3DSection);
		part->m_Elements_beams.push_back(Element_Beam(Element_Beam3DNumber, Element_Beam3DidNode1, Element_Beam3DidNode2, id, Direction,1));
		processedCount++;
	}
}

int WireWiring::GetSectionId(QString MaterialName, QString SectionName)
{
	int id = 0;
	int sectionSize = m_pInterFace->Ms.size();
	int MaterialId = 0;
	int SectionId = 0;
	double Iu=0;
	double Iv=0;
	double J=0;
	for (const auto& j : m_pInterFace->ME_Material)
	{
		if (j.second->m_Name == MaterialName)
		{

			MaterialId = j.second->m_id;
		}
	}
	for (const auto& j : m_pInterFace->ME_Section_L)
	{
		if (j.second->m_Name == SectionName)
		{
			SectionId = j.second->m_id;
			Iu = j.second->Iu;
			Iv = j.second->Iv;
			J = j.second->J;
		}
	}
	if (sectionSize == 0)
	{
		Section* section = new Section(1, SectionName, Iu, Iv, J, MaterialId);
		m_pInterFace->Ms.Add_Entity(section);
		id = 1;
	}
	else
	{
		for (const auto& j : m_pInterFace->Ms)
		{
			if (j.second->m_Name == SectionName)
			{
				if (j.second->ClassM = MaterialId)
				{
					id = j.second->m_id;
				}
			}
		}
		if (id == 0)
		{
			id = m_pInterFace->Ms.size() + 1;
			Section* section = new Section(id, SectionName, Iu, Iv, J, MaterialId);
			m_pInterFace->Ms.Add_Entity(section);
		}
	}
	return id;
}

void WireWiring::HighList(QStringList parts)
{
	int HighCount = parts[1].toInt();
	for (int i = 0; i < HighCount; ++i)
	{
		int processedCount = 0;// 记录已处理的呼高数量
		NominalHeight* nominalHeight = new NominalHeight();
		// 读取和处理每个呼高组合的数据
		while (processedCount < 3)
		{
			QString line = Stream1.readLine();
			if (line.startsWith("**") || line.isEmpty())
			{
				// 跳过以 "**" 开头的行和空行
				continue;
			}
			
			// 解析数据
			if (processedCount == 0)
			{
				QStringList HighLine = line.split(QRegExp("[\\s,]+"), Qt::SkipEmptyParts);
				int highNumber = HighLine[0].toInt();
				double height = HighLine[1].toDouble();
				nominalHeight->m_id = highNumber;
				nominalHeight->hight = height;
				processedCount++;
			}
			else if (processedCount == 1)
			{
				QStringList PartLine = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
				QStringList partBody = PartLine.mid(0);
				for (int j = 0; j < partBody.size(); j++)
				{
					nominalHeight->BodyList.push_back(partBody[j].toInt());
				}
				processedCount++;
			}
			else if (processedCount == 2)
			{
				QStringList LegLine = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
				QStringList partLeg = LegLine.mid(0);
				for (int j = 0; j < partLeg.size(); j++)
				{
					nominalHeight->LegList.push_back(partLeg[j].toInt());
				}
				processedCount++;
				tower->TP_Height.Add_Entity(nominalHeight);
			}
		}
	}
}

void WireWiring::HangPointList(QStringList parts)
{
	int HangPointCount = parts[1].toInt();
	int processedCount = 0;
	while (processedCount < HangPointCount)
	{
		QString HangPointLine = Stream1.readLine();
		if (HangPointLine.startsWith("**") || HangPointLine.isEmpty())
		{
			// 跳过说明行和空行
			continue;
		}
		QStringList HangPointParts = HangPointLine.split(QRegExp("[\\s,]+"), Qt::SkipEmptyParts);
		int id = HangPointParts[0].toInt();
		QString StringClass = HangPointParts[1];
		QString WireLoge = HangPointParts[2];
		if (StringClass == "V")
		{
			QString Partnumber = HangPointParts[3];
			int nodeid = HangPointParts[4].toInt();
			QStringList Partid = Partnumber.split(QRegExp("[\\s-]+"), Qt::SkipEmptyParts);
			int FindPartid1 = Partid[1].toInt();
			int id1 = tower->TP_Part.Find_Entity(FindPartid1)->TP_HangPoint.size() + 1;
			HangPoint* hangPoint1 = new HangPoint(id1, StringClass, WireLoge, nodeid);
			tower->TP_Part.Find_Entity(FindPartid1)->TP_HangPoint.Add_Entity(hangPoint1);

			QString Partnumber2 = HangPointParts[5];
			int nodeid2 = HangPointParts[6].toInt();
			QStringList Partid2 = Partnumber2.split(QRegExp("[\\s-]+"), Qt::SkipEmptyParts);
			int FindPartid2 = Partid2[1].toInt();
			int id2 = tower->TP_Part.Find_Entity(FindPartid2)->TP_HangPoint.size() + 1;
			HangPoint* hangPoint2 = new HangPoint(id2, StringClass, WireLoge, nodeid2);
			tower->TP_Part.Find_Entity(FindPartid2)->TP_HangPoint.Add_Entity(hangPoint2);
		}
		else
		{
			QString Partnumber = HangPointParts[3];
			int nodeid = HangPointParts[4].toInt();
			QStringList Partid = Partnumber.split(QRegExp("[\\s-]+"), Qt::SkipEmptyParts);
			int FindPartid = Partid[1].toInt();
			int id1 = tower->TP_Part.Find_Entity(FindPartid)->TP_HangPoint.size() + 1;
			HangPoint* hangPoint = new HangPoint(id1, StringClass, WireLoge, nodeid);
			tower->TP_Part.Find_Entity(FindPartid)->TP_HangPoint.Add_Entity(hangPoint);
		} 
		processedCount++;
	}
}


