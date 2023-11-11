#include "CreateStrainWire.h"
#include <QFileDialog>
#include "InterFace.h"
#include"Section_L.h"

void CreateStrainWire::CreateInsulatorSus(int fenlie, map<int, vector<Insulator_Base>>& SusInfor)
{
	O_Id= GetSectionId("String_O", "D100");
	H_Id = GetSectionId("String_H", "D100");
	V_Id = GetSectionId("String_V", "D100");
	S_Id= GetSectionId("Spacer", "Spacer");

	for (const auto& sus : SusInfor)
	{
		int a = sus.first;
		const std::vector<Insulator_Base>& vectorData = sus.second;  // 获取对应的 vector
		NumberOfLine = std::numeric_limits<int>::min();
		for (const auto& a : vectorData)
		{
			if (a.m_line > NumberOfLine)
			{
				NumberOfLine = a.m_line;//线路数量
			}
		}
		wireNumberOfLine = NumberOfLine - 2;//线路数量
		wireQty = wireNumberOfLine;
		//确定向量
		double angle = 0;
		double x = vectorData[1].m_x - vectorData[0].m_x;
		double y = vectorData[1].m_y - vectorData[0].m_y;
		angle = ((atan2(x, y) * 180) / 3.1415926);
		if (abs(angle) < 1)
		{
			angle = 0;
		}
		angle = vtkMath::RadiansFromDegrees(angle);
		m_Angle.push_back(angle);
		row = vectorData.size() / NumberOfLine;//一个耐张段一条线路的挂点个数

		for (int j = 0; j < NumberOfLine; j++)//对线路数量进行循环
		{
			for (int i = 0; i < row; i++)
			{
				vector<int> nodeIds;
				vector<int> V_1_Ids;
				vector<int> V_2_Ids;
				vector<int> spacerIds;
				int index = j * row + i;
				if (vectorData[index].m_type != "G")//非地线
				{
					if (vectorData[index].H == 0 && vectorData[index].angle_1 == 0)//耐张点
					{
						Type_WO(index, a, vectorData, RealSus);
					}
					else//悬垂点带下面的间隔棒
					{
						if (vectorData[index].angle_1 == 0)//悬垂
						{
							if (fenlie == 1)//单导线
							{
								Type_WH_1(index, a, vectorData, nodeIds, RealSus);
							}
							else if (fenlie == 4)//四分裂
							{
								Type_WH_4(index, a, angle, vectorData, nodeIds, spacerIds, RealSus);
							}
							else if (fenlie == 6)//四分裂
							{
								Type_WH_6(index, a, angle, vectorData, nodeIds, spacerIds, RealSus);
							}
						}
						else//V型
						{
							if (fenlie == 1)
							{
								Type_WV_1(index, a, angle, vectorData, V_1_Ids, V_2_Ids, spacerIds, RealSus);
							}
							else if (fenlie == 4)//单导线
							{
								Type_WV_4(index, a, angle, vectorData, V_1_Ids, V_2_Ids, spacerIds, RealSus);
							}
						}
					}
				}
				else//地线
				{
					if (vectorData[index].H == 0)
					{	
						Type_GO(index, a, vectorData, G_RealSus);
					}
					else
					{
						Type_G(index, a, vectorData, nodeIds, G_RealSus);
					}
				}
				if (nodeIds.size() != 0)
				{
					CreateWireInsulator(m_Elements_beams, Truss_elementsID, nodeIds, H_Id);
					//CreateXuanChuiInsulator(m_Elements_Trusses, Truss_elementsID, nodeIds, H_Id);
					InsulatorId.push_back(Truss_elementsID);
				}
				if (V_1_Ids.size() != 0)
				{
					CreateWireInsulator(m_Elements_beams, Truss_elementsID, V_1_Ids, V_Id);
					V_InsulatorId.push_back(Truss_elementsID);
					CreateWireInsulator(m_Elements_beams, Truss_elementsID, V_2_Ids, V_Id);
					V_InsulatorId.push_back(Truss_elementsID);
				}
				CreatSpacer(m_Elements_beams, S_Id, spacerIds);
			}

		}
	}
}

void CreateStrainWire::CreateRealSus()
{
	int* node = nullptr;
	int* ap_node = nullptr;
	for (const auto& entry : RealSus)
	{
		int key = entry.first;  // 获取 int 键
		const std::vector<Insulator_Base>& vectorData = entry.second;  // 获取对应的 vector
		vector<Node>m_TempNodes;
		for (int i = 0; i < vectorData.size(); i++)
		{
			m_TempNodes.push_back(Node(1, vectorData[i].m_x, vectorData[i].m_y, vectorData[i].m_z, 0, 2));
		}
		double rou = 0;
		double stress = 0;
		double k = 0;
		for (int i = 0; i < Property.size(); i++)
		{
			if (Property[i].idTension == key && Property[i].isGround == false)
			{
				rou = Property[i].w_rou * 9.8 / (Property[i].w_area * 1000000);
				stress = Property[i].w_f * 1000 / (Property[i].w_area * 1000000);
				k = rou / stress;
				for (int i = 0; i < wireNumberOfLine; i++)
				{
					CreateStrainTempWireNode(key, 2, i + 1, k, m_TempNodes, Property);
				}
			}
		
		}
		

		//确定向量 
		double angle = 0;
		int num = vectorData.size();
		/*double x = vectorData[num - 1].m_x - vectorData[num - 2].m_x;
		double y = vectorData[num - 1].m_y - vectorData[num - 2].m_y;
		angle = ((atan2(x, y) * 180) / 3.1415926);
		angle = vtkMath::RadiansFromDegrees(angle);*/
		angle = m_Angle[key-1];
		int susnum = num * fenlie;//
		node = new int[susnum];
		//ap_node = new int[num];
		int index = 0; // 节点索引
		//int ap_index = 0;
		double offsetX = 0; double offsetY = 0; double offsetZ = 0;
		for (int i = 0; i < num; i++)//
		{
			for (int j = 0; j < fenlie; j++)
			{
				if (fenlie == 1)
				{
					offsetX = 0.0;
					offsetZ = 0.0;
				}
				else if (fenlie == 4)
				{
					double offsets[4][3] = { {0.225 * cos(angle),-0.225 * sin(angle), 0.225}, {0.225 * cos(angle),-0.225 * sin(angle), -0.225} ,
					{-0.225 * cos(angle),0.225 * sin(angle), -0.225}, {-0.225 * cos(angle),0.225 * sin(angle), 0.225} };
					offsetX = offsets[j][0];
					offsetY = offsets[j][1];
					offsetZ = offsets[j][2];
				}
				else if (fenlie == 6)
				{
					double offsets[6][3] = { {0.2 * cos(angle),-0.2 * sin(angle), 0.35}, {0.4 * cos(angle),-0.4 * sin(angle), 0} ,
					{0.2 * cos(angle),-0.2 * sin(angle), -0.35}, {-0.2 * cos(angle),0.2 * sin(angle), -0.35},
						{-0.4 * cos(angle),0.4 * sin(angle), 0},{-0.2 * cos(angle),0.2 * sin(angle), 0.35} };
					offsetX = offsets[j][0];
					offsetY = offsets[j][1];
					offsetZ = offsets[j][2];
				}
				// 创建节点
				if (index < susnum)
				{
					node[index] = Creat_Node(m_Str_realSus[key - 1][i].x + offsetX, m_Str_realSus[key - 1][i].y + offsetY, m_Str_realSus[key - 1][i].z + offsetZ, 0, 2);
					SaveSus({ node[index] }); // 放入悬挂点
					//生成耐张段
				}
				else
				{
					// 处理索引超出范围的情况，例如抛出异常或打印错误信息
					// 并确保不会继续写入超出范围的节点
					break;
				}

				index++;
			}

		}
		//生成耐张段
		int NzSize = m_Str_realSus[key - 1].size();//一个耐张段的悬挂点个数
		int OneSusSize = NzSize / wireNumberOfLine;//一条线路的的悬挂点个数
		vector<Element_Beam>TempTruss;
		double x1 = 0; double y1 = 0; double z1 = 0;
		double x2 = 0; double y2 = 0; double z2 = 0;
		for (int i = 0; i < wireNumberOfLine; i++)
		{
			node[1] = Creat_Node(m_Str_realSus[key - 1][i* OneSusSize].x + offsetX, m_Str_realSus[key - 1][i * OneSusSize].y + offsetY, m_Str_realSus[key - 1][i * OneSusSize].z + offsetZ, 0, 2);
			node[2] = Creat_Node(m_Str_realSus[key - 1][(i + 1) * OneSusSize - 1 ].x + offsetX, m_Str_realSus[key - 1][(i + 1) * OneSusSize - 1].y + offsetY, m_Str_realSus[key - 1][(i + 1) * OneSusSize - 1].z + offsetZ, 0, 2);
			vector<int> start_ids;
			vector<int> end_ids;
			for (int i = 0; i < fenlie; i++)
			{
				start_ids.push_back(node[1] + i - fenlie + 1);
				end_ids.push_back(node[2] + i - fenlie + 1);
			}
			x1 = EndPoint[key][i * 2].x;
			y1 = EndPoint[key][i * 2].y;
			z1 = EndPoint[key][i * 2].z;
			x2 = EndPoint[key][i * 2 + 1].x;
			y2 = EndPoint[key][i * 2 + 1].y;
			z2 = EndPoint[key][i * 2 + 1].z;
			CreatSpacer(m_Elements_beams, S_Id,start_ids);
			CreatSpacer(m_Elements_beams, S_Id, end_ids);
			CreateStrainLine(m_Elements_beams, x1, y1, z1, start_ids, O_Id);
			CreateStrainLine(m_Elements_beams, x2, y2, z2, end_ids, O_Id);
		}

		 //在使用完后别忘了释放内存
		delete[] node;
	
	}
}

void CreateStrainWire::CreateStrainWireInfor(vector<WireProperties> pro)
{
	for (const auto& entry : m_Str_realSus)
	{
		const std::vector<Node>& vectorData = entry.second;  // 获取对应的 vector
		int key = entry.first + 1;  // 获取 int 键
		double rou = 0;
		double stress=0;
		double k =0;
		double area = 0;
		double unitMass = 0;
		for (int i = 0; i < pro.size(); i++)
		{
			if (pro[i].idTension == key&& pro[i].isGround==false)
			{
				rou = pro[i].w_rou * 9.8 / (pro[i].w_area * 1000000);
				stress = pro[i].w_f * 1000 / (pro[i].w_area * 1000000);
				area = pro[i].w_area * 1000000;
				unitMass = pro[i].w_rou;
				k = rou / stress;
				//double k = 0.0007629;
				int n = 100;
				int N = 100;
				int num = vectorData.size() / (wireNumberOfLine)-1;
				LengthId += num;
				Length.push_back(LengthId);
				double angle = 0;
				angle = m_Angle[key - 1];
				/*int max = vectorData.size() - 1;
				double x = vectorData[max].x - vectorData[max - 1].x;
				double y = vectorData[max].y - vectorData[max - 1].y;
				angle = ((atan2(x, y) * 180) / 3.1415926);
				angle = vtkMath::RadiansFromDegrees(angle);*/
				for (int j = 0; j < wireNumberOfLine; j++)
				{
					int* node = new int[num * (n + 1) * fenlie];
					int* ap_node = new int[num * (n + 1)];
					for (int i = 0 + vectorData.size() / (wireNumberOfLine)*j; i < vectorData.size() / (wireNumberOfLine) * (j + 1) - 1; i++)
					{
						double lxi = sqrt((vectorData[i].x - vectorData[i + 1].x) * (vectorData[i].x - vectorData[i + 1].x) + (vectorData[i].y - vectorData[i + 1].y) * (vectorData[i].y - vectorData[i + 1].y));//档距
						double wireL = sqrt((vectorData[i].x - vectorData[i + 1].x) * (vectorData[i].x - vectorData[i + 1].x) +
							(vectorData[i].y - vectorData[i + 1].y) * (vectorData[i].y - vectorData[i + 1].y) +
							(vectorData[i].z - vectorData[i + 1].z) * (vectorData[i].z - vectorData[i + 1].z));//档距
						double  hi = vectorData[i + 1].z - vectorData[i].z;//高差
						double Li = (2 / k) * sinh(k * lxi / 2); //线长
						double li = vectorData[i + 1].x - vectorData[i].x; //两点x之差
						double mi = vectorData[i + 1].y - vectorData[i].y; //两点y之差
						double nni = sqrt(li * li + mi * mi); //二维中两点之差
						double dxi = li / N;
						double dyi = mi / N;
						double dzi = nni / N;
						double y0i = vectorData[i].z + 1 / k * ((1 - cosh((k) * (nni / 2)) * sqrt(1 + (hi / Li) * (hi / Li)))) + hi / 2;
						double force = 0;//每个点对应的轴力
						double wireGravity = wireL * unitMass * 9.8 * fenlie;
						WireGravity.push_back(wireGravity);
						if (fenlie == 1)
						{
							for (int m = 0; m < N + 1; m++)
							{
								double Zi = m * dzi;
								double x = m * dxi + vectorData[i].x;
								double y = m * dyi + vectorData[i].y;
								double z = ((1. / k) * (hi / Li)) * (sinh(k * lxi / 2) + sinh(k * (2 * Zi - lxi) / 2)) - ((2 / k) * sinh(k * Zi / 2) *
									sinh(k * (lxi - Zi) / 2)) * sqrt(1 + (hi / Li) * (hi / Li)) + vectorData[i].z;
								force = area * (stress + rou * (z - y0i));
								node[(i - vectorData.size() / wireQty * j) * (n + 1) + m] = Creat_Node(x, y, z, force, 2);
							}
						}
						else if (fenlie == 2)
						{
							for (int m = 0; m < N + 1; m++)
							{
								double Zi = m * dzi;
								double x1 = m * dxi + vectorData[i].x + 0.225;
								double x2 = m * dxi + vectorData[i].x - 0.225;
								double y = m * dyi + vectorData[i].y;
								double z = ((1. / k) * (hi / Li)) * (sinh(k * lxi / 2) + sinh(k * (2 * Zi - lxi) / 2)) - ((2 / k) * sinh(k * Zi / 2) *
									sinh(k * (lxi - Zi) / 2)) * sqrt(1 + (hi / Li) * (hi / Li)) + vectorData[i].z;
								force = area * (stress + rou * (z - y0i));
								force = area * (stress + rou * (z - y0i));
								node[(i - vectorData.size() / wireQty * j) * (n + 1) + m] = Creat_Node(x1, y, z, force, 2);
								node[(i - vectorData.size() / wireQty * j) * (n + 1) + num * (N + 1) + m] = Creat_Node(x2, y, z, force, 2);
							}
						}
						else if (fenlie == 4)
						{
							for (int m = 0; m < N + 1; m++)
							{
								double Zi = m * dzi;
								double x1 = m * dxi + vectorData[i].x + 0.225 * cos(angle);
								double x2 = m * dxi + vectorData[i].x - 0.225 * cos(angle);
								double y1 = m * dyi + vectorData[i].y + 0.225 * sin(angle);
								double y2 = m * dyi + vectorData[i].y - 0.225 * sin(angle);
								double z = ((1. / k) * (hi / Li)) * (sinh(k * lxi / 2) + sinh(k * (2 * Zi - lxi) / 2)) - ((2 / k) * sinh(k * Zi / 2) *
									sinh(k * (lxi - Zi) / 2)) * sqrt(1 + (hi / Li) * (hi / Li)) + vectorData[i].z;
								double z1 = z + 0.225;
								double z2 = z - 0.225;
								force = area * (stress + rou * (z1 - y0i));
								node[(i - vectorData.size() / wireQty * j) * (n + 1) + m] = Creat_Node(x1, y2, z1, force, 2);
								node[(i - vectorData.size() / wireQty * j) * (n + 1) + num * (N + 1) + m] = Creat_Node(x1, y2, z2, force, 2);
								node[(i - vectorData.size() / wireQty * j) * (n + 1) + 2 * num * (N + 1) + m] = Creat_Node(x2, y1, z2, force, 2);
								node[(i - vectorData.size() / wireQty * j) * (n + 1) + 3 * num * (N + 1) + m] = Creat_Node(x2, y1, z1, force, 2);
							}


						}
						else if (fenlie == 6)
						{
							for (int m = 0; m < N + 1; m++)
							{
								double Zi = m * dzi;
								double x1 = m * dxi + vectorData[i].x + 0.2 * cos(angle);
								double x2 = m * dxi + vectorData[i].x - 0.2 * cos(angle);
								double x3 = m * dxi + vectorData[i].x + 0.4 * cos(angle);
								double x4 = m * dxi + vectorData[i].x - 0.4 * cos(angle);
								double y1 = m * dyi + vectorData[i].y + 0.2 * sin(angle);
								double y2 = m * dyi + vectorData[i].y - 0.2 * sin(angle);
								double y3 = m * dyi + vectorData[i].y + 0.4 * sin(angle);
								double y4 = m * dyi + vectorData[i].y - 0.4 * sin(angle);
								double z = ((1. / k) * (hi / Li)) * (sinh(k * lxi / 2) + sinh(k * (2 * Zi - lxi) / 2)) - ((2 / k) * sinh(k * Zi / 2) *
									sinh(k * (lxi - Zi) / 2)) * sqrt(1 + (hi / Li) * (hi / Li)) + vectorData[i].z;
								double z1 = z + 0.35;
								double z2 = z - 0.35;
								force = area * (stress + rou * (z1 - y0i));
								node[(i - vectorData.size() / wireQty * j) * (n + 1) + m] = Creat_Node(x1, y2, z1, force, 2);
								node[(i - vectorData.size() / wireQty * j) * (n + 1) + num * (N + 1) + m] = Creat_Node(x3, y4, z, force, 2);
								node[(i - vectorData.size() / wireQty * j) * (n + 1) + 2 * num * (N + 1) + m] = Creat_Node(x1, y2, z2, force, 2);
								node[(i - vectorData.size() / wireQty * j) * (n + 1) + 3 * num * (N + 1) + m] = Creat_Node(x2, y1, z2, force, 2);
								node[(i - vectorData.size() / wireQty * j) * (n + 1) + 4 * num * (N + 1) + m] = Creat_Node(x4, y3, z, force, 2);
								node[(i - vectorData.size() / wireQty * j) * (n + 1) + 5 * num * (N + 1) + m] = Creat_Node(x2, y1, z1, force, 2);
							}
						}
					}


					if (fenlie == 1)
					{
						for (int m = 0; m < num; m++)
						{
							std::vector<int> nodeIds1;
				
							for (int i = 0; i < (N + 1); i++)
							{
								nodeIds1.push_back(node[i + m * (N + 1)]);
							}
							if (key == 1)
							{
								CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds1, pro[i].MaId, "G-" + QString::number(m + 1));
							}
							else
							{
								CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds1, pro[i].MaId, "G-" + QString::number(m + 1 + Length[key - 2]));
							}
							
						}
					}
					else if (fenlie == 4)
					{
						for (int m = 0; m < num; m++)
						{
							std::vector<int> nodeIds1;
							std::vector<int> nodeIds2;
							std::vector<int> nodeIds3;
							std::vector<int> nodeIds4;
							for (int i = 0; i <  (N + 1); i++)
							{
								nodeIds1.push_back(node[i + m * (N + 1)]);
								nodeIds2.push_back(node[num * (N + 1) + i + m * (N + 1)]);
								nodeIds3.push_back(node[2 * num * (N + 1) + i + m * (N + 1)]);
								nodeIds4.push_back(node[3 * num * (N + 1) + i + m * (N + 1)]);
							}
							if (key == 1)
							{
								CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds1, pro[i].MaId, "L-" + QString::number(m + 1));
								CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds2, pro[i].MaId, "L-" + QString::number(m + 1));
								CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds3, pro[i].MaId, "L-" + QString::number(m + 1));
								CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds4, pro[i].MaId, "L-" + QString::number(m + 1));
							}
							else
							{
								CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds1, pro[i].MaId, "L-" + QString::number(m + 1 + Length[key - 2]));
								CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds2, pro[i].MaId, "L-" + QString::number(m + 1 + Length[key - 2]));
								CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds3, pro[i].MaId, "L-" + QString::number(m + 1 + Length[key - 2]));
								CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds4, pro[i].MaId, "L-" + QString::number(m + 1 + Length[key - 2]));
							}
							
						}
						
					}
					else if (fenlie == 6)
					{
						for (int m = 0; m < num; m++)
						{
							std::vector<int> nodeIds1;
							std::vector<int> nodeIds2;
							std::vector<int> nodeIds3;
							std::vector<int> nodeIds4;
							std::vector<int> nodeIds5;
							std::vector<int> nodeIds6;
							for (int i = 0; i < (N + 1); i++)
							{
								nodeIds1.push_back(node[i + m * (N + 1)]);
								nodeIds2.push_back(node[num * (N + 1) + i + m * (N + 1)]);
								nodeIds3.push_back(node[2 * num * (N + 1) + i + m * (N + 1)]);
								nodeIds4.push_back(node[3 * num * (N + 1) + i + m * (N + 1)]);
								nodeIds5.push_back(node[4 * num * (N + 1) + i + m * (N + 1)]);
								nodeIds6.push_back(node[5 * num * (N + 1) + i + m * (N + 1)]);
							}

							CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds1, pro[i].MaId, "L-" + QString::number(m + 1));
							CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds2, pro[i].MaId, "L-" + QString::number(m + 1));
							CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds3, pro[i].MaId, "L-" + QString::number(m + 1));
							CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds4, pro[i].MaId, "L-" + QString::number(m + 1));
							CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds5, pro[i].MaId, "L-" + QString::number(m + 1));
							CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds6, pro[i].MaId, "L-" + QString::number(m + 1));
						}
					}
					delete[]node;
					delete[]ap_node;
				}
			}
		}
		
		for (int i = 0; i < m_Elements_Trusses.size(); i++)
		{
			m_Elements_Trusses[i].AxialForce = (m_Nodes[m_Elements_Trusses[i].m_idNode[0] - 1].F + m_Nodes[m_Elements_Trusses[i].m_idNode[1] - 1].F) / 2;
		}
	}

}

void CreateStrainWire::CreateStrainString(int id)
{


	//int NumofNz = Test_a.size();//耐张段的个数
	//vector<int> ids;
	//vector<Element_Beam>TempTruss;
	//vector<Element_Beam>ApTempTruss;
	//double x1 = 0; double y1 = 0; double z1 = 0;
	//double x2 = 0; double y2 = 0; double z2 = 0;
	//int SusSize = row;//一个耐张段一条线路的挂点个数
	//for (int m = 0; m < NumofNz; m++)//对耐张段进行循环
	//{
	//	for (int j = 0; j < (wireNumberOfLine); j++)//对线路循环
	//	{
	//		vector<int> start_ids;
	//		vector<int> end_ids;
	//		if (fenlie == 1)
	//		{

	//		}
	//		else
	//		{
	//			int Initial = NumofNz * NumberOfLine * 2 + xuanchui.size() * (fenlie + 2) + V.size() * (fenlie + 3) + G_xuanchui.size() * 2;
	//			for (int i = 0; i < fenlie; i++)
	//			{
	//				start_ids.push_back(Initial + fenlie * 2 * j + i + 1 + 2 * fenlie * m * wireNumberOfLine);
	//				end_ids.push_back(Initial + fenlie * 2 * j + i + 1 + fenlie + 2 * fenlie * m * wireNumberOfLine);
	//			}
	//			x1 = EndPoint[m + 1][j * 2].x;
	//			y1 = EndPoint[m + 1][j * 2].y;
	//			z1 = EndPoint[m + 1][j * 2].z;
	//			x2 = EndPoint[m + 1][j * 2 + 1].x;
	//			y2 = EndPoint[m + 1][j * 2 + 1].y;
	//			z2 = EndPoint[m + 1][j * 2 + 1].z;

	//		}
	//		CreatSpacer(m_Elements_beams, start_ids);
	//		CreatSpacer(m_Elements_beams, end_ids);
	//		CreateStrainLine(TempTruss, x1, y1, z1, start_ids);
	//		CreateStrainLine(TempTruss, x2, y2, z2, end_ids);
	//	}
	//}
	//for (int i = 0; i < TempTruss.size(); i++)
	//{
	//	TempTruss[i].ClassSectionID = WireSectionId;
	//	TempTruss[i].AxialForce = 6000;
	//	TempTruss[i].MaterialID = 4;
	//}
	//m_Elements_beams.insert(m_Elements_beams.end(), TempTruss.begin(), TempTruss.end());
}

void CreateStrainWire::CreateGWire(vector<WireProperties> pro)
{
	if (G_RealSus.size() == 0)return;
	for (const auto& entry : G_RealSus)
	{
		int key = entry.first - 1;  // 获取 int 键
		const std::vector<Insulator_Base>& vectorData = entry.second;  // 获取对应的 vector
		vector<Node>m_TempNodes;
		for (int i = 0; i < vectorData.size(); i++)
		{
			m_TempNodes.push_back(Node(1, vectorData[i].m_x, vectorData[i].m_y, vectorData[i].m_z, 0, 2));
		}
		double rou;
		double stress;
		double k;
		double area;
		double unitMass = 0;
		int n = 100;
		int num = m_TempNodes.size() / 2 - 1;
	
		for (int j = 0; j < 2; j++)
		{
			for (int i = 0; i < pro.size(); i++)
			{
				if (pro[i].idTension == (key + 1) && pro[i].isGround == true)
				{
					rou = pro[i].w_rou * 9.8 / (pro[i].w_area * 1000000);
					stress = pro[i].w_f * 1000 / (pro[i].w_area * 1000000);
					k = rou / stress;
					area = pro[i].w_area * 1000000;
					unitMass = pro[i].w_rou;
					int* node = new int[num * (n + 1)];
					for (int i = 0 + m_TempNodes.size() / 2 * j; i < m_TempNodes.size() / 2 * (j + 1) - 1; i++)
					{
						double lxi = sqrt((m_TempNodes[i].x - m_TempNodes[i + 1].x) * (m_TempNodes[i].x - m_TempNodes[i + 1].x) + (m_TempNodes[i].y - m_TempNodes[i + 1].y) * (m_TempNodes[i].y - m_TempNodes[i + 1].y));//档距
						double wireL = sqrt((m_TempNodes[i].x - m_TempNodes[i + 1].x) * (m_TempNodes[i].x - m_TempNodes[i + 1].x) +
							(m_TempNodes[i].y - m_TempNodes[i + 1].y) * (m_TempNodes[i].y - m_TempNodes[i + 1].y) +
							(m_TempNodes[i].z - m_TempNodes[i + 1].z) * (m_TempNodes[i].z - m_TempNodes[i + 1].z));//档距
						double  hi = m_TempNodes[i + 1].z - m_TempNodes[i].z;//高差
						//double k = rou / stress;
						double Li = (2 / k) * sinh(k * lxi / 2); //线长
						double li = m_TempNodes[i + 1].x - m_TempNodes[i].x; //两点x之差
						double mi = m_TempNodes[i + 1].y - m_TempNodes[i].y; //两点y之差
						double nni = sqrt(li * li + mi * mi); //二维中两点之差
						double dxi = li / n;
						double dyi = mi / n;
						double dzi = nni / n;
						double y0i = m_TempNodes[i].z + 1 / k * ((1 - cosh((k) * (nni / 2)) * sqrt(1 + (hi / Li) * (hi / Li)))) + hi / 2;
						double force = 0;//每个点对应的轴力
						double wireGravity = wireL * unitMass * 9.8;
						WireGravity.push_back(wireGravity);
						for (int m = 0; m < n + 1; m++)
						{
							double Zi = m * dzi;
							double x = m * dxi + m_TempNodes[i].x;
							double y = m * dyi + m_TempNodes[i].y;
							double z = ((1. / k) * (hi / Li)) * (sinh(k * lxi / 2) + sinh(k * (2 * Zi - lxi) / 2)) - ((2 / k) * sinh(k * Zi / 2) *
								sinh(k * (lxi - Zi) / 2)) * sqrt(1 + (hi / Li) * (hi / Li)) + m_TempNodes[i].z;
							force = area * (stress + rou * (z - y0i));
							node[(i - m_TempNodes.size() / 2 * j) * (n + 1) + m] = Creat_Node(x, y, z, force, 2);
						}
					}
					for (int m = 0; m < num; m++)
					{
						std::vector<int> nodeIds1;

						for (int i = 0; i < (N + 1); i++)
						{
							nodeIds1.push_back(node[i + m * (N + 1)]);
						}
						if (key == 0)
						{
							CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds1, pro[i].MaId, "G-" + QString::number(m + 1));
						}
						else
						{
							CreatWireEle(m_Elements_Trusses, Truss_elementsID, nodeIds1, pro[i].MaId, "G-" + QString::number(m + 1 + Length[key - 1]));
						}
					}
					break;
				}
				
			}
		}
		for (int i = 0; i < m_Elements_Trusses.size(); i++)
		{
			m_Elements_Trusses[i].AxialForce = (m_Nodes[m_Elements_Trusses[i].m_idNode[0] - 1].F + m_Nodes[m_Elements_Trusses[i].m_idNode[1] - 1].F) / 2;
		}
		
	}
}

void CreateStrainWire::Create_Mesh()
{
	CreateInsulatorSus(fenlie , Test_a);
	CreateRealSus();
	CreateStrainWireInfor(Property);
	CreateGWire(Property);

	
	//StrainSpacerNum[1] = { 10,10,10,10,10,10,10,10,10,10,10,10,10 };
	//StrainSpacerNum[2] = { 10,10,10,10,10,10,10,10,10,10,10,10,10,10,10 };
	//StrainSpacerNum[3] = { 10,10,10 };

	//for (auto& sus : m_Str_realSus)
	//{
	//	int key = sus.first + 1;
	//	std::vector<Node>& vectorData = sus.second;  // 获取对应的 vector
	//	double k = 0;
	//	for (int i = 0; i < Property.size(); i++)
	//	{
	//		if (Property[i].idTension == key && Property[i].isGround == false)
	//		{
	//			double rou = Property[i].w_rou * 9.8 / (Property[i].w_area * 1000000);
	//			double stress = Property[i].w_f * 1000 / (Property[i].w_area * 1000000);
	//			k = rou / stress;
	//			for (int i = 0; i < wireQty; i++)
	//			{
	//				CreateStrainTempRealWireNode(key, i + 1, k, vectorData);
	//			}
	//		}

	//	}
	//}
}

void CreateStrainWire::Type_WO(int i, int StrainId, const vector<Insulator_Base>&  data, map<int, vector<Insulator_Base>>& RealSusInfor)
{
	int node1 = Creat_Node(data[i].m_x, data[i].m_y, data[i].m_z, 0, 1);
	SaveSus({ node1 }); // 放入悬挂点
	StrainAllRestraintNode.push_back(node1);
	RealSusInfor[StrainId].push_back(Insulator_Base("O", data[i].m_x, data[i].m_y, data[i].m_z, 0, data[i].m_line));//1是线路默认的 后面的改
	EndPoint[StrainId].push_back(Node(1, data[i].m_x, data[i].m_y, data[i].m_z, 0, 1));
}

void CreateStrainWire::Type_WH_1(int i, int StrainId, const vector<Insulator_Base>& data, vector<int>& Id, map<int, vector<Insulator_Base>>& RealSusInfor)
{
	int node1 = Creat_Node(data[i].m_x, data[i].m_y, data[i].m_z, 0, 1);//塔上悬挂点
	int node2 = Creat_Node(data[i].m_x, data[i].m_y, data[i].m_z - data[i].H, 0, 2);

	Id.push_back(node1);
	Id.push_back(node2);
	SaveSus({ node1 });
	SaveSus({ node2 });
	StrainAllRestraintNode.push_back(node1);
	RealSusInfor[StrainId].push_back(Insulator_Base("H",data[i].m_x, data[i].m_y, data[i].m_z - data[i].H, 0, data[i].m_line));
	xuanchui.push_back(1);
}

void CreateStrainWire::Type_WH_4(int i, int StrainId, double Angle, const vector<Insulator_Base>& data, vector<int>& Id, vector<int>& SpacerId, map<int, vector<Insulator_Base>>& RealSusInfor)
{
	int node1 = Creat_Node(data[i].m_x, data[i].m_y, data[i].m_z, 0, 1);//塔上悬挂点
	int node2 = Creat_Node(data[i].m_x, data[i].m_y, data[i].m_z - data[i].H, 0, 2);
	int node_1 = Creat_Node(data[i].m_x + 0.225 * cos(Angle), data[i].m_y - 0.225 * sin(Angle), data[i].m_z - data[i].H, 0, 2);
	int node_2 = Creat_Node(data[i].m_x + 0.225 * cos(Angle), data[i].m_y - 0.225 * sin(Angle), data[i].m_z - data[i].H - 0.45, 0, 2);
	int node_3 = Creat_Node(data[i].m_x - 0.225 * cos(Angle), data[i].m_y + 0.225 * sin(Angle), data[i].m_z - data[i].H - 0.45, 0, 2);
	int node_4 = Creat_Node(data[i].m_x - 0.225 * cos(Angle), data[i].m_y + 0.225 * sin(Angle), data[i].m_z - data[i].H, 0, 2);
	SaveSus({ node1 });
	SaveSus({ node1 });
	SaveSus({ node_1 });
	SaveSus({ node_2 });
	SaveSus({ node_3 });
	SaveSus({ node_4 });
	Id.push_back(node1);
	Id.push_back(node2);
	SpacerId.push_back(node_1);
	SpacerId.push_back(node_2);
	SpacerId.push_back(node_3);
	SpacerId.push_back(node_4);
	SpacerId.push_back(node2);
	RealSusInfor[StrainId].push_back(Insulator_Base("H",data[i].m_x, data[i].m_y, data[i].m_z - data[i].H - 0.225, 0, data[i].m_line));
	StrainAllRestraintNode.push_back(node1);
	xuanchui.push_back(1);
}

void CreateStrainWire::Type_WH_6(int i, int StrainId, double Angle, const vector<Insulator_Base>& data, vector<int>& Id, vector<int>& SpacerId, map<int, vector<Insulator_Base>>& RealSusInfor)
{
	int node1 = Creat_Node(data[i].m_x, data[i].m_y, data[i].m_z, 0, 1);//塔上悬挂点
	int node2 = Creat_Node(data[i].m_x, data[i].m_y, data[i].m_z - data[i].H, 0, 2);
	int node_1 = Creat_Node(data[i].m_x + 0.2 * cos(Angle), data[i].m_y - 0.2 * sin(Angle), data[i].m_z - data[i].H, 0, 2);
	int node_2 = Creat_Node(data[i].m_x + 0.4 * cos(Angle), data[i].m_y - 0.4 * sin(Angle), data[i].m_z - data[i].H - 0.35, 0, 2);
	int node_3 = Creat_Node(data[i].m_x + 0.2 * cos(Angle), data[i].m_y - 0.2 * sin(Angle), data[i].m_z - data[i].H - 0.7, 0, 2);
	int node_4 = Creat_Node(data[i].m_x - 0.2 * cos(Angle), data[i].m_y + 0.2 * sin(Angle), data[i].m_z - data[i].H - 0.7, 0, 2);
	int node_5 = Creat_Node(data[i].m_x - 0.4 * cos(Angle), data[i].m_y + 0.4 * sin(Angle), data[i].m_z - data[i].H - 0.35, 0, 2);
	int node_6 = Creat_Node(data[i].m_x - 0.2 * cos(Angle), data[i].m_y + 0.2 * sin(Angle), data[i].m_z - data[i].H, 0, 2);
	SaveSus({ node1 });
	SaveSus({ node2 });
	SaveSus({ node_1 });
	SaveSus({ node_2 });
	SaveSus({ node_3 });
	SaveSus({ node_4 });
	SaveSus({ node_5 });
	SaveSus({ node_6 });
	Id.push_back(node1);
	Id.push_back(node2);
	SpacerId.push_back(node_1);
	SpacerId.push_back(node_2);
	SpacerId.push_back(node_3);
	SpacerId.push_back(node_4);
	SpacerId.push_back(node_5);
	SpacerId.push_back(node_6);
	SpacerId.push_back(node2);
	RealSusInfor[StrainId].push_back(Insulator_Base("H",data[i].m_x, data[i].m_y, data[i].m_z - data[i].H - 0.35, 0, data[i].m_line));
	StrainAllRestraintNode.push_back(node1);
	xuanchui.push_back(1);
}

void CreateStrainWire::Type_WV_1(int i, int StrainId, double Angle, const vector<Insulator_Base>& data, vector<int>& V_Id1, vector<int>& V_Id2, vector<int>& SpacerId, map<int, vector<Insulator_Base>>& RealSusInfor)
{
	double x1 = data[i].m_x1; double y1 = data[i].m_y1; double z1 = data[i].m_z1;
	double x2 = data[i].m_x2; double y2 = data[i].m_y2; double z2 = data[i].m_z2;
	double angle_1 = data[i].angle_1;
	double angle_2 = data[i].angle_2;

	angle_1 = vtkMath::RadiansFromDegrees(angle_1);
	angle_2 = vtkMath::RadiansFromDegrees(angle_2);
	//确定转角
	double V_angle = 0;
	double x = (x2 - x1);
	double y = (y2 - y1);
	V_angle = ((atan2(y, x) * 180) / 3.1415926);
	V_angle = vtkMath::RadiansFromDegrees(V_angle);
	double test = cos(V_angle);
	//塔上V串的两个悬挂点
	int node_T1 = Creat_Node(x1, y1, z1, 0, 1);//塔上悬挂点
	int node_T2 = Creat_Node(x2, y2, z2, 0, 1);
	double zc_part = (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) - (z2 - z1) * tan(angle_2)) / (tan(angle_1) + tan(angle_2));
	double xc_part = abs(zc_part) * tan(angle_1);
	double x_c = x1 + abs(xc_part) * cos(V_angle);
	double y_c = y1 - abs(xc_part) * sin(V_angle);
	double z_c = z1 - abs(zc_part);
	int node_V = Creat_Node(x_c, y_c, z_c, 0, 2);
	RealSusInfor[StrainId].push_back(Insulator_Base("V", x_c, y_c, z_c , 0, data[i].m_line));
	SaveSus({ node_T1 });
	SaveSus({ node_T2 });
	SaveSus({ node_V });
	V_Id1.push_back(node_T1);
	V_Id1.push_back(node_V);
	V_Id2.push_back(node_T2);
	V_Id2.push_back(node_V);
	StrainAllRestraintNode.push_back(node_T1);
	StrainAllRestraintNode.push_back(node_T2);
	V.push_back(1);
}

void CreateStrainWire::Type_WV_4(int i, int StrainId, double Angle, const vector<Insulator_Base>& data, vector<int>& V_Id1, vector<int>& V_Id2, vector<int>& SpacerId, map<int, vector<Insulator_Base>>& RealSusInfor)
{
	double x1 = data[i].m_x1; double y1 = data[i].m_y1; double z1 = data[i].m_z1;
	double x2 = data[i].m_x2; double y2 = data[i].m_y2; double z2 = data[i].m_z2;
	double angle_1 = data[i].angle_1;
	double angle_2 = data[i].angle_2;

	angle_1 = vtkMath::RadiansFromDegrees(angle_1);
	angle_2 = vtkMath::RadiansFromDegrees(angle_2);
	//确定转角
	double V_angle = 0;
	double x = (x2 - x1);
	double y = (y2 - y1);
	V_angle = ((atan2(y, x) * 180) / 3.1415926);
	V_angle = vtkMath::RadiansFromDegrees(V_angle);
	double test = cos(V_angle);
	//塔上V串的两个悬挂点
	int node_T1 = Creat_Node(x1, y1, z1, 0, 1);//塔上悬挂点
	int node_T2 = Creat_Node(x2, y2, z2, 0, 1);
	double zc_part = (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) - (z2 - z1) * tan(angle_2)) / (tan(angle_1) + tan(angle_2));
	double xc_part = abs(zc_part) * tan(angle_1);
	double x_c = x1 + abs(xc_part) * cos(V_angle);
	double y_c = y1 - abs(xc_part) * sin(V_angle);
	double z_c = z1 - abs(zc_part);
	int node_V = Creat_Node(x_c, y_c, z_c, 0, 2);
	int node_1 = Creat_Node(x_c + 0.225 * cos(Angle), y_c - 0.225 * sin(Angle), z_c - data[i].H, 0, 2);
	int node_2 = Creat_Node(x_c + 0.225 * cos(Angle), y_c - 0.225 * sin(Angle), z_c - data[i].H - 0.45, 0, 2);
	int node_3 = Creat_Node(x_c - 0.225 * cos(Angle), y_c + 0.225 * sin(Angle), z_c - data[i].H - 0.45, 0, 2);
	int node_4 = Creat_Node(x_c - 0.225 * cos(Angle), y_c + 0.225 * sin(Angle), z_c - data[i].H, 0, 2);
	RealSusInfor[StrainId].push_back(Insulator_Base("V",x_c, y_c, z_c - 0.225, 0, data[i].m_line));
	SaveSus({ node_T1 });
	SaveSus({ node_T2 });
	SaveSus({ node_V });
	SaveSus({ node_1 });
	SaveSus({ node_2 });
	SaveSus({ node_3 });
	SaveSus({ node_4 });
	V_Id1.push_back(node_T1);
	V_Id1.push_back(node_V);
	V_Id2.push_back(node_T2);
	V_Id2.push_back(node_V);
	SpacerId.push_back(node_1);
	SpacerId.push_back(node_2);
	SpacerId.push_back(node_3);
	SpacerId.push_back(node_4);
	SpacerId.push_back(node_V);
	StrainAllRestraintNode.push_back(node_T1);
	StrainAllRestraintNode.push_back(node_T2);
	V.push_back(1);
}

void CreateStrainWire::Type_GO(int i, int StrainId, const vector<Insulator_Base>& data, map<int, vector<Insulator_Base>>& G_RealSusInfor)
{
	int node1 = Creat_Node(data[i].m_x, data[i].m_y, data[i].m_z, 0, 1);
	SaveSus({ node1 }); // 放入悬挂点
	StrainJointRestraintNode.push_back(node1);
	G_RealSusInfor[StrainId].push_back(Insulator_Base("G",data[i].m_x, data[i].m_y, data[i].m_z, 0, data[i].m_line));//1是线路默认的 后面的改
}

void CreateStrainWire::Type_G(int i, int StrainId, const vector<Insulator_Base>& data, vector<int>& Id, map<int, vector<Insulator_Base>>& G_RealSusInfor)
{
	int node1 = Creat_Node(data[i].m_x, data[i].m_y, data[i].m_z, 0, 1);//塔上悬挂点
	int node2 = Creat_Node(data[i].m_x, data[i].m_y, data[i].m_z - data[i].H, 0, 2);

	Id.push_back(node1);
	Id.push_back(node2);
	SaveSus({ node1 });
	SaveSus({ node2 });
	G_RealSusInfor[StrainId].push_back(Insulator_Base("G",data[i].m_x, data[i].m_y, data[i].m_z - data[i].H, 0, data[i].m_line));
	StrainAllRestraintNode.push_back(node1);
	G_xuanchui.push_back(1);
}

int CreateStrainWire::FindGroupIdNode(int idNode) const
{
	return wireToGroup[idNode - 1];
}

int CreateStrainWire::GetSectionId(QString MaterialName, QString SectionName)
{
	InterFace* m_pInterFace = Get_InterFace();
	int id = 0;
	int sectionSize = m_pInterFace->Ms.size();
	int MaterialId = 0;
	int SectionId = 0;
	double Iu = 0;
	double Iv = 0;
	double J = 0;
	double Area = 0;
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
			Area = j.second->A;
		}
	}
	if (sectionSize == 0)
	{
		Section* section = new Section(1, SectionName, Iu, Iv, J, MaterialId, Area);
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
			Section* section = new Section(id, SectionName, Iu, Iv, J, MaterialId, Area);
			m_pInterFace->Ms.Add_Entity(section);
		}
	}
	return id;;
}



