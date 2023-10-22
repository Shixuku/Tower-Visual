#include "InstanceWire.h"
#include"InterFace.h"
#include "NominalHeight.h"
#include"HangList.h"
#include <QRegExp>
#pragma execution_character_set("utf-8")
void InstanceWire::GetAllInstanceTXT(string path, vector<string>& files)
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

void InstanceWire::ReadInstanceWire(InterFace* InterFace)
{
	m_pInterFace = InterFace;
	vector<string> fileNames;
	string path("../InstanceTXT"); 	//自己选择目录测试
	GetAllInstanceTXT(path, fileNames);
	//for (const auto& ph : fileNames) {
	//	std::cout << ph << "\n";
	//}
	//qDebug() << "fileNames.size()！" << fileNames.size() << "\t";
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
					QStringList parts = qLine.mid(1).split(QRegExp("[\\s,]+"), Qt::SkipEmptyParts);
					keyword = parts[0].trimmed();
					if (keyword.compare("Line", Qt::CaseInsensitive) == 0)
					{
						QString lineName = parts[1].trimmed();
						Ground= parts[2].toInt();
						Conductor= parts[3].toInt();
						Spilt = parts[4].toInt();
						TowerSize = parts[5].toInt();
						
						for (const auto& j : m_pInterFace->TWG)
						{
							if (j.second->m_Name == lineName)
							{
								towerWire = j.second;
								CreatTowerWierGroupItem(lineName);
								break;
							}
						}
						QString filePath = "../HangPointTXT/" + lineName + ".txt";
						Qf.setFileName(filePath);  // 创建 QFile 对象并指定文件路径

						if (Qf.open(QIODevice::WriteOnly | QIODevice::Text)) {
							qDebug() << "文件打开成功！";
							Stream.setDevice(&Qf);
							
						}
						else {
							qDebug() << "文件打开失败！";
							// 文件打开失败，处理错误
						}
					}
					else if (keyword.compare("Section", Qt::CaseInsensitive) == 0)
					{
						//Stream << qLine << "\n";
						SectionList(parts);
						
					}
					else if (keyword.compare("Instance", Qt::CaseInsensitive) == 0)
					{
						tower = new Tower();
						tower->m_id= parts[1].toInt();
						QString TowerName = parts[2];
						Line = parts[3].toInt();
						if (Line == 1)
						{
							LineSegment1= Tension;
						}
						else if (Line == 0)
						{
							LineSegment1 = Tension;
							LineSegment2 = Tension+1;
						}
						for (const auto& j : towerWire->TWG_TP)
						{
							qDebug() << j.second->m_Name << "  " << TowerName << "\n";
							if (j.second->m_Name == TowerName)
							{
								towerList = j.second;
								break;
							}
						}
						if (towerList == nullptr)
						{
							qDebug() << "y有问题" << "\t";
						}
						if (Line == 0)
						{
							Tension++;
						}
						instanceList(parts);
					}
				}
			}
			WriteHangList();
			Qf.close();
		}
		Tension = 0;//耐张段数
		Line = 0;//直线塔或者耐张塔
		TowerSize = 0;
		Ground = 0;//地线数量
		Conductor = 0;//导线
		Spilt = 0;
		LineSegment1 = 0;
		LineSegment2 = 0;
	}

	//showGroup(towerWire);
}

void InstanceWire::SectionList(QStringList parts)
{
	int SectionID = parts[1].toInt();
	int processedCount = 0;// 记录已处理的数量
	QRegExp regex(".*");
	QString keyword;
	QString Ten = "*Tension";
	Stream << Ten<<"," << SectionID << "\n";
	while (processedCount < SectionID)
	{
		QString line = Stream1.readLine();
		if (line.startsWith("**") || line.isEmpty())
		{
			// 跳过以 "**" 开头的行和空行
			continue;
		}
		// 解析数据
		

		else if (line.startsWith("*"))
		{
			QStringList parts = line.mid(1).split(",", Qt::SkipEmptyParts);
			keyword = parts[0].trimmed();
			if (keyword.compare("Tension", Qt::CaseInsensitive) == 0)
			{
				Stream << line << "\n";
			}
			else if (keyword.compare("Conductor_Line", Qt::CaseInsensitive) == 0)
			{
				Stream << line << "\n";
			}
			else if (keyword.compare("Ground_Line", Qt::CaseInsensitive) == 0)
			{
				Stream << line << "\n";
				GroungLineList(parts);
				processedCount++;
			}
		}
	}
}

void InstanceWire::GroungLineList(QStringList parts)
{
	int GroundLineID = parts[1].toInt();
	int processedCount = 0;// 记录已处理的呼高数量
	while (processedCount < GroundLineID)
	{
		QString line = Stream1.readLine();
		if (line.startsWith("**") || line.isEmpty())
		{
			// 跳过以 "**" 开头的行和空行
			continue;
		}
		Stream << line << "\n";
		processedCount++;
	}
}

void InstanceWire::instanceList(QStringList parts)
{
	int instanceID = parts[1].toInt();
	int processedCount = 0;// 记录已处理的呼高数量
	QString keyword;
	int circulate = 5;

	while (processedCount < circulate)
	{
		QString line = Stream1.readLine();
		if (line.startsWith("**") || line.isEmpty())
		{
			// 跳过以 "**" 开头的行和空行
			continue;
		}
		// 解析数据
		else if (line.startsWith("*"))
		{
			QStringList parts = line.mid(1).split(",", Qt::SkipEmptyParts);
			keyword = parts[0].trimmed();
			if (keyword.compare("TowerPosition", Qt::CaseInsensitive) == 0)
			{
				//TowerPosition(parts);
				QStringList xyzLine = line.split(QRegExp("[\\s,]+"), Qt::SkipEmptyParts);
				x = xyzLine[1].toDouble();
				y = xyzLine[2].toDouble();
				z = xyzLine[3].toDouble();
			}
			else if (keyword.compare("Angle", Qt::CaseInsensitive) == 0)
			{
				//Angle(parts);
				QStringList AngleLine = line.split(QRegExp("[\\s,]+"), Qt::SkipEmptyParts);
				dAngle = AngleLine[1].toDouble();
			}
			else if (keyword.compare("Body", Qt::CaseInsensitive) == 0)
			{
				NomHeight(parts);
			}
			else if (keyword.compare("Leg", Qt::CaseInsensitive) == 0)
			{
				LegList(parts);
			}
			else if (keyword.compare("String", Qt::CaseInsensitive) == 0)
			{
				StringList(parts);
			}
			processedCount++;
		}

	}
	if (tower != nullptr)
	{

		qDebug() << x << "   " << y << "   " << z << "\t";
		towerWire->AddTowerToGroup(tower, instanceID, x, y, z, dAngle);
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
			// 跳过以 "**" 开头的行和空行
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
			// 跳过以 "**" 开头的行和空行
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

void InstanceWire::LegList(QStringList parts)
{
	int LegId = parts[1].toInt();//这是A
	int LegIdB = parts[2].toInt();//B
	int LegIdC = parts[3].toInt();//C
	int LegIdD = parts[4].toInt();//D
	//处理A--第三象限
	Part_Base* part = towerList->TP_Part.Find_Entity(LegId);//A
	if (part == nullptr)
	{
		qDebug() << "有问题" << "\t";
	}
	for (int i = 0; i < part->m_Nodes.size(); i++)
	{
		part->m_Nodes[i].x *= -1;
		part->m_Nodes[i].y *= -1;
	}
	if (part != nullptr)
	{
		tower->addPart(part);
		//还原
		for (int i = 0; i < part->m_Nodes.size(); i++)
		{
			part->m_Nodes[i].x *= -1;
			part->m_Nodes[i].y *= -1;
		}
	}
	//处理B--第二象限
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
	//处理C--第一象限
	Part_Base* partC = towerList->TP_Part.Find_Entity(LegIdC);//C
	if (partC != nullptr)
	{
		tower->addPart(partC);
	}
	//处理D--第四象限
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
}
void InstanceWire::StringList(QStringList parts)
{
	int StringCount = parts[1].toInt();
	int processedCount = 0;// 记录已处理的部件数量
	while (processedCount < StringCount)
	{
		QString StringLine = Stream1.readLine();
		if (StringLine.startsWith("**") || StringLine.isEmpty())
		{
			// 跳过说明行和空行
			continue;
		}
		QStringList StringParts = StringLine.split(QRegExp("[\\s,]+"), Qt::SkipEmptyParts);
		int StringNumber = StringParts[0].toInt();
		QString WireLog = StringParts[1];
		for (auto& j : tower->m_HangList)
		{
			if (j.WireLoge == WireLog)
			{
				if (j.StringClass == "V")
				{
					double angle1 = StringParts[2].toDouble();
					double angle2 = StringParts[3].toDouble();
					j.Angle.push_back(angle1);
					j.Angle.push_back(angle2);
					j.Material = StringParts[4];
					j.Section = StringParts[5];
					j.LineSegment = LineSegment1;
				}
				else
				{
					double Length = StringParts[2].toDouble();
					j.length = Length;
					j.Material = StringParts[3];
					j.Section = StringParts[4];
					if (j.StringClass == "OS")
					{
						j.LineSegment = LineSegment1;
					}
					else if (j.StringClass == "OB")
					{
						j.LineSegment = LineSegment2;
					}
					else if(j.StringClass == "H"|| j.StringClass == "V")
					{
						j.LineSegment = LineSegment1;
					}
				}
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

void InstanceWire::CreatTowerWierGroupItem(QString lineName)
{
	QTreeWidgetItem* towerWierGroupItem = new QTreeWidgetItem(m_pInterFace->creat_towerwire_instance);
	QString str = QString::number(m_pInterFace->creat_towerwire_instance->childCount());     //塔线组的item
	towerWierGroupItem->setText(0, QString(lineName));
	//放入每个杆塔
	QTreeWidgetItem* GroupTower = new QTreeWidgetItem(towerWierGroupItem);
	GroupTower->setText(0, QString("杆塔"));
	for (int i = 0; i < TowerSize; i++)
	{
		QTreeWidgetItem* GroupTowerItem = new QTreeWidgetItem(GroupTower);
		QString str = QString::number(i+1);     //str转字符
		GroupTowerItem->setText(0, QString("杆塔编号" + str));
	}
	towerWire->Item = towerWierGroupItem;
	QTreeWidgetItem* Wire_modeling = new QTreeWidgetItem(towerWierGroupItem);
	Wire_modeling->setText(0, QString("导线建模"));

	QTreeWidgetItem* calculate = new QTreeWidgetItem(towerWierGroupItem);
	calculate->setText(0, QString("计算文件"));

	QTreeWidgetItem* Loads = new QTreeWidgetItem(towerWierGroupItem);
	Loads->setText(0, QString("施加荷载"));
}

void InstanceWire::WriteHangList()
{
	QString xx = "*Sus" ;
	towerWire->VectorToMap();
	int m_HangListSize = towerWire->m_HangList.size();
	int LostPointSize = (Ground + Conductor) * 2;
	Stream << xx << "," << m_HangListSize - LostPointSize <<","<<Spilt << "\n";
	for (int i = 0; i < towerWire->m_HangList.size(); i++)
	{
		if (towerWire->m_HangList[i].StringClass == "V")
		{
			int id1 = towerWire->m_HangList[i].NodeId[0];
			int id2 = towerWire->m_HangList[i].NodeId[1];
			Stream << towerWire->m_HangList[i].StringClass << "  " << towerWire->m_HangList[i].WireLoge << "  "<< towerWire->m_HangList[i].LineSegment <<"  " << towerWire->m_HangList[i] .Angle[0]
				<<"  "<< towerWire->m_HangList[i].Angle[1] <<"  " << towerWire->NodeData[id1].x << "  " << towerWire->NodeData[id1].y << "  " <<
				towerWire->NodeData[id1].z << "  " << towerWire->NodeData[id2].x << "  " << towerWire->NodeData[id2].y << "  " <<
				towerWire->NodeData[id2].z << "\n";
		}
		else if (towerWire->m_HangList[i].StringClass == "OS")
		{
			if (i <= LostPointSize)
			{
				continue;
			}
			else
			{
				QString StringClass = "O";
				if (towerWire->m_HangList[i].WireLoge == "1" || towerWire->m_HangList[i].WireLoge == "2")
				{
					StringClass = "G";
				}
				int id1 = towerWire->m_HangList[i].NodeId[0];
				Stream << StringClass << "  " << towerWire->m_HangList[i].WireLoge << "  " << towerWire->m_HangList[i].length <<"  " << towerWire->m_HangList[i].LineSegment << "  "
					<< towerWire->NodeData[id1].x << "  " << towerWire->NodeData[id1].y << "  " <<
					towerWire->NodeData[id1].z << "\n";
			}
		}
		else if (towerWire->m_HangList[i].StringClass == "OB")
		{
			if (i >= m_HangListSize - LostPointSize)
			{
				continue;
			}
			else
			{
				QString StringClass = "O";
				if (towerWire->m_HangList[i].WireLoge == "1" || towerWire->m_HangList[i].WireLoge == "2")
				{
					StringClass = "G";
				}

				int id1 = towerWire->m_HangList[i].NodeId[0];
				Stream << StringClass << "  " << towerWire->m_HangList[i].WireLoge << "  " << towerWire->m_HangList[i].length << "  " 
					<< towerWire->m_HangList[i].LineSegment << "  "
					<< towerWire->NodeData[id1].x << "  " << towerWire->NodeData[id1].y << "  " <<
					towerWire->NodeData[id1].z << "\n";
			}
		}
		else if(towerWire->m_HangList[i].StringClass == "H")
		{
			QString StringClass = "H";
			if (towerWire->m_HangList[i].WireLoge == "1" || towerWire->m_HangList[i].WireLoge == "2")
			{
				StringClass = "G";
			}
			int id1 = towerWire->m_HangList[i].NodeId[0];
			Stream  << StringClass << "  " << towerWire->m_HangList[i].WireLoge << "  "<< towerWire->m_HangList[i].length <<"  "<<
					towerWire->m_HangList[i].LineSegment <<"  " << towerWire->NodeData[id1].x << "  " << towerWire->NodeData[id1].y << "  " <<
					towerWire->NodeData[id1].z << "\n";
		}
		
	}
}
