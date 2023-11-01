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

	//qDebug() << "fileNames.size()！"<< fileNames.size()<<"\t";
	for (int i = 0; i < fileNames.size(); i++)
	{
		str = QString::fromStdString(fileNames[i]);
		if (!str.isEmpty())
		{
			//qDebug() << str;
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
						QString lineName = parts[1].trimmed();//线路名称
						QString towerNumber = parts[2].trimmed();//塔类型
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
				int towerNumber = Partparts[2].toInt();//部件编号
				QString drawingNumber = Partparts[3].trimmed();//图纸名称
				Part->m_id = towerNumber;
				tower->TP_Part.Add_Entity(Part);
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
					//std::cout << "m_pInterFace->Ms.size() =  "<< m_pInterFace->Ms.size() << "\n";//==83个
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
		double NodeX = NodeParts[1].toDouble()/1000 ;
		double NodeY = NodeParts[2].toDouble() /1000;
		double NodeZ = NodeParts[3].toDouble()/1000 ;
		part->m_Nodes.push_back(Node(NodeNumber, NodeX, NodeY, NodeZ, 0, 0));
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
		QString Element_Beam3DMaterial = Element_Beam3DParts[6];//材料
		QString Element_Beam3DSection = Element_Beam3DParts[7];//截面
		double Direction[3] = { Element_Beam3DDirectionX,Element_Beam3DDirectionY,Element_Beam3DDirectionZ };
		int id = GetSectionId1(Element_Beam3DMaterial, Element_Beam3DSection);//截面编号
		part->m_Elements_beams.push_back(Element_Beam(Element_Beam3DNumber, Element_Beam3DidNode1, Element_Beam3DidNode2, id, Direction,1));
		processedCount++;
	}
}

int WireWiring::GetSectionId1(QString MaterialName, QString SectionName)
{
	//1、找--对应的材料编号和截面编号
	int MaterialId = 0;
	for (const auto& j : m_pInterFace->ME_Material)
	{
		if (MaterialName == j.second->m_Name)//找材料编号
		{
			MaterialId = j.second->m_id;
			break;
		}
	}
	int SectionId = 0;
	double Iu = 0;
	double Iv = 0;
	double J = 0;
	double A = 0;
	for (const auto& j : m_pInterFace->ME_Section_L)
	{
		if (SectionName == j.second->m_Name)//找截面编号
		{
			SectionId = j.second->m_id;
		}
	}
	//找不到截面编号得情况-得自己算Iu, Iv, J, A，然后添加到ME_Section_L里面
	if (SectionId == 0)
	{
		QString currentSegment = "";
		QStringList segments; // 用于保存拆分后的部分
		for (int i = 0; i < SectionName.length(); i++)
		{
			QChar currentChar = SectionName.at(i);
			if (currentChar.isLetter())
			{
				if (!currentSegment.isEmpty())
				{
					segments.append(currentSegment); // 将拆分后的部分添加到 QStringList
					currentSegment = ""; // 重置 currentSegment
				}
				segments.append(currentChar); // 将字母字符添加到 QStringList
			}
			else if (currentChar.isDigit()) { currentSegment += currentChar; }
		}

		// 检查是否还有未添加的数字部分
		if (!currentSegment.isEmpty()) {
			segments.append(currentSegment);
		}
		SectionId = m_pInterFace->ME_Section_L.size() + 1;
		GetLIxyz(segments[1].toDouble()/1000.0, segments[3].toDouble()/1000.0, &Iu, &Iv, &J, &A);
		Section_L* section = new Section_L(SectionId, SectionName, Iu, Iv, J, A);
		m_pInterFace->ME_Section_L.Add_Entity(section);
	}
	// 2、 判断--Ms.size()=0的情况；
	int MsSize = m_pInterFace->Ms.size();

	if (MsSize == 0)
	{
		Iu = m_pInterFace->ME_Section_L.Find_Entity(SectionId)->Iu;
		Iv = m_pInterFace->ME_Section_L.Find_Entity(SectionId)->Iv;
		J = m_pInterFace->ME_Section_L.Find_Entity(SectionId)->J;
		A = m_pInterFace->ME_Section_L.Find_Entity(SectionId)->A;
		Section* section = new Section(MsSize + 1, SectionName, Iu, Iv, J, MaterialId, A);
		m_pInterFace->Ms.Add_Entity(section);//添加进去
		return 1;
	}
	// 3、截面size不为0的情况下，在原来的Ms里面没有找到相同的材料号或者截面名称
	else
	{
		int index = 0;//相当于一个标识符
		for (const auto& i : m_pInterFace->Ms)
		{
			//1、在已经添加的截面里面找得到已经有得截面名称或者材料编号
			if (i.second->m_Name == SectionName && i.second->ClassM == MaterialId)	
				index= i.second->m_id;
			if(index != 0)	return index;
		}
		if (index == 0)
		{
			Iu = m_pInterFace->ME_Section_L.Find_Entity(SectionId)->Iu;
			Iv = m_pInterFace->ME_Section_L.Find_Entity(SectionId)->Iv;
			J = m_pInterFace->ME_Section_L.Find_Entity(SectionId)->J;
			A = m_pInterFace->ME_Section_L.Find_Entity(SectionId)->A;
			Section* section = new Section(MsSize + 1, SectionName, Iu, Iv, J, MaterialId, A);
			m_pInterFace->Ms.Add_Entity(section);//添加进去
			return MsSize + 1;
		}
	}
	return -1;
}

void WireWiring::GetLIxyz(double a, double b, double* Iu, double* Iv, double* J, double* S)
{
	double a2 = a * a;
	double a3 = a2 * a;
	double b2 = b * b;
	double b3 = b2 * b;
	*Iu = 1. / 12 * b * (2 * a2 - 2 * a * b + b2) * (2 * a - b);
	*Iv = b * (2 * a3 * a - 4 * a3 * b + 8 * a2 * b2 - 6 * a * b3 + b3 * b) / (24 * a - 12 * b);


	double L2 = a, t2 = b;
	*J = pow(t2, 0.3e1) * (0.1e1 / 0.3e1 - 0.105e0 * t2 * (0.1e1 - pow(t2, 0.4e1) * pow(L2 - t2, -0.4e1) / 0.192e3) / (L2 - t2)) * (L2 - t2) +
		L2 * pow(t2, 0.3e1) * (0.1e1 / 0.3e1 - 0.21e0 * t2 * (0.1e1 - pow(t2, 0.4e1) * pow(L2, -0.4e1) / 0.12e2) / L2) + 0.7e-1 * pow(0.4e1 * t2 - 0.2e1 * sqrt(0.2e1) * sqrt(t2 * t2), 0.4e1);
	*S = 2 * a * b - b * b;
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
			QString StringClass1 = StringClass;//区分V1
			HangPoint* hangPoint1 = new HangPoint(id1, StringClass1, WireLoge, nodeid);
			tower->TP_Part.Find_Entity(FindPartid1)->TP_HangPoint.Add_Entity(hangPoint1);

			QString Partnumber2 = HangPointParts[5];
			int nodeid2 = HangPointParts[6].toInt();
			QStringList Partid2 = Partnumber2.split(QRegExp("[\\s-]+"), Qt::SkipEmptyParts);
			int FindPartid2 = Partid2[1].toInt();
			int id2 = tower->TP_Part.Find_Entity(FindPartid2)->TP_HangPoint.size() + 1;
			QString StringClass2 = StringClass ;//区分V2
			HangPoint* hangPoint2 = new HangPoint(id2, StringClass2, WireLoge, nodeid2);
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


