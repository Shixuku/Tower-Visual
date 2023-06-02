#include "Wire_InterFace.h"
#include"InterFace.h"
#include"Senior.h"

Wire_InterFace::Wire_InterFace(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_pInterFace = dynamic_cast<InterFace*>(parent);
	Q_ASSERT(m_pInterFace != nullptr);
	Initialize();
	connect(m_pInterFace, &InterFace::Msg_Select_Nodes, this, &Wire_InterFace::Insert_Data);
	connect(ui.Pts_OK, &QPushButton::clicked, this, [=]() {int a = ui.N_Pts->text().toInt();Insert_Point_Infor(a);});
	connect(ui.Senior_Btn, &QPushButton::clicked, this, [=]() {Show_Senior(); });
	connect(ui.Btn_ensure, &QPushButton::clicked, this, [=]() {Insert_Pts_to_vector();   OK(); this->close();  });
	connect(ui.Btn_cancel, &QPushButton::clicked, this, [=]() { this->close();  });
}

Wire_InterFace::~Wire_InterFace()
{}

void Wire_InterFace::Insert_Point_Infor(int N)
{
	ui.Section_Lists->clear();
	ui.Section_Lists->setHorizontalHeaderLabels(headertext);
	ui.Section_Lists->setRowCount(N);//默认N行
	for (int i = 0; i < N; i++)
	{
		if (i == 0)
		{
			QComboBox* Point_Type = new QComboBox;
			Point_Type->addItem("耐张端点");
			Point_Type->addItem("端点");
			ui.Section_Lists->setCellWidget(i, 4, (QWidget*)Point_Type);
		}
		else if (i == N - 1)
		{
			//最后一行
			QComboBox* Point_Type = new QComboBox;
			Point_Type->addItem("耐张端点");
			Point_Type->addItem("端点");
			ui.Section_Lists->setCellWidget(i, 4, (QWidget*)Point_Type);
		}
		else
		{
			ui.Section_Lists->setItem(i, 4, new QTableWidgetItem("悬挂点"));
		}
	}
}

void Wire_InterFace::Insert_Pts_to_vector()
{
	//端点部分
	QString msg;
	QString msg_end;
	int rowNum = ui.Section_Lists->rowCount(); //取出点的类型 
	QWidget* widget = ui.Section_Lists->cellWidget(0, 4);
	QWidget* widget_end = ui.Section_Lists->cellWidget(rowNum - 1, 4);
	QComboBox* combox = (QComboBox*)widget;
	QComboBox* combox1 = (QComboBox*)widget_end;
	msg = combox->currentText();
	msg_end = combox1->currentText();
	if (msg == "端点")
	{
		Extreme_Point.push_back(0);
	}
	else if (msg == "耐张端点")
	{
		Extreme_Point.push_back(1);
	}
	if (msg_end == "端点")
	{
		Extreme_Point.push_back(0);
	}
	else if (msg_end == "耐张端点")
	{
		Extreme_Point.push_back(1);
	}
	//中间无耐张点
	double xi, yi, zi;
	//表格点
	for (int i = 0; i < rowNum; i++)
	{
		xi = ui.Section_Lists->item(i, 1)->text().toDouble()*1000;
		yi = ui.Section_Lists->item(i, 2)->text().toDouble()*1000;
		zi = ui.Section_Lists->item(i, 3)->text().toDouble()*1000;
		xgd.push_back(Node(xi, yi, zi));
	}
}

void Wire_InterFace::Initialize()
{
	headertext << "挂点编号" << "X(m)" << "Y(m)" << "Z(m)" << "点类型";
	ui.Section_Lists->setColumnCount(headertext.count());
	ui.Section_Lists->setHorizontalHeaderLabels(headertext);
	ui.Section_Lists->verticalHeader()->setVisible(false);
	ui.Section_Lists->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void Wire_InterFace::Insert_Data()
{
	std::list<Node*> Nodes;
	m_pInterFace->Get_SelectedNode(Nodes);
	int k = 0;
	for (auto pNode : Nodes)
	{
		int idNode = pNode->m_idNode;
		ui.Section_Lists->setItem(k, 0, new QTableWidgetItem(QString::number(idNode)));
		ui.Section_Lists->setItem(k, 1, new QTableWidgetItem(QString::number(pNode->x/1000)));
		ui.Section_Lists->setItem(k, 2, new QTableWidgetItem(QString::number(pNode->y/1000)));
		ui.Section_Lists->setItem(k, 3, new QTableWidgetItem(QString::number(pNode->z/1000)));
		++k;

	}
}

void Wire_InterFace::Line_Segment(vector<Node> xgd_List, vector<Node> xgd_Real, int direction)
{
	int num_Node = m_Nodes.size();
	int p = xgd_Real.size() - 1;
	//确定的向量
	double v_x = xgd_Real[p].x - xgd_Real[p - 1].x;
	double v_y = xgd_Real[p].y - xgd_Real[p - 1].y;
	double angle = ((atan2(v_x, v_y) * 180) / 3.1415926);
	angle = vtkMath::RadiansFromDegrees(angle);
	Creat_WireData wire_temp(xgd_Real, rou, yingli, N, Node_Temp); //zuo分裂的基础点
	if (fenlie == 1)
	{
		vector<Node> xgd_1;
		for (int i = 0; i < xgd_Real.size(); i++)
		{
			xgd_1.push_back(Node(xgd_Real[i].x, xgd_Real[i].y, xgd_Real[i].z));

		};
		Creat_WireData wire_1(xgd_1, rou, yingli, N, m_Nodes);
		//cd->CreatWire_ele( 1 + p , p  * N *  + 1, m_Elements_Wire);
	}
	else if (fenlie == 2)
	{
		vector<Node> xgd_1;
		vector<Node> xgd_2;
		for (int i = 0; i < xgd_Real.size(); i++)
		{
			xgd_1.push_back(Node(xgd_Real[i].x + 225 * cos(angle), xgd_Real[i].y - 225 * sin(angle), xgd_Real[i].z));
			xgd_2.push_back(Node(xgd_Real[i].x - 225 * cos(angle), xgd_Real[i].y + 225 * sin(angle), xgd_Real[i].z));

		};
		Creat_WireData wire_1(xgd_1, rou, yingli, N, m_Nodes);
		Creat_WireData wire_2(xgd_2, rou, yingli, N, m_Nodes);
	}
	else if (fenlie == 4)
	{
		vector<Node> xgd_1;
		vector<Node> xgd_2;
		vector<Node> xgd_3;
		vector<Node> xgd_4;
		for (int i = 0; i < xgd_Real.size(); i++)
		{
			xgd_1.push_back(Node(xgd_Real[i].x + 225 * cos(angle), xgd_Real[i].y - 225 * sin(angle), xgd_Real[i].z + 225));
			xgd_2.push_back(Node(xgd_Real[i].x - 225 * cos(angle), xgd_Real[i].y + 225 * sin(angle), xgd_Real[i].z + 225));
			xgd_3.push_back(Node(xgd_Real[i].x - 225 * cos(angle), xgd_Real[i].y + 225 * sin(angle), xgd_Real[i].z - 225));
			xgd_4.push_back(Node(xgd_Real[i].x + 225 * cos(angle), xgd_Real[i].y - 225 * sin(angle), xgd_Real[i].z - 225));

		};
		Creat_WireData wire_1(xgd_1, rou, yingli, N, m_Nodes);
		Creat_WireData wire_2(xgd_2, rou, yingli, N, m_Nodes);
		Creat_WireData wire_3(xgd_3, rou, yingli, N, m_Nodes);
		Creat_WireData wire_4(xgd_4, rou, yingli, N, m_Nodes);
	}
	else if (fenlie == 6)
	{
		vector<Node> xgd_1;
		vector<Node> xgd_2;
		vector<Node> xgd_3;
		vector<Node> xgd_4;
		vector<Node> xgd_5;
		vector<Node> xgd_6;
		for (int i = 0; i < xgd_Real.size(); i++)
		{
			xgd_1.push_back(Node(xgd_Real[i].x - 200 * cos(angle), xgd_Real[i].y + 200 * sin(angle), xgd_Real[i].z + 346));
			xgd_2.push_back(Node(xgd_Real[i].x + 200 * cos(angle), xgd_Real[i].y - 200 * sin(angle), xgd_Real[i].z + 346));
			xgd_3.push_back(Node(xgd_Real[i].x + 346 * cos(angle), xgd_Real[i].y - 346 * sin(angle), xgd_Real[i].z));
			xgd_4.push_back(Node(xgd_Real[i].x + 200 * cos(angle), xgd_Real[i].y - 200 * sin(angle), xgd_Real[i].z - 346));
			xgd_5.push_back(Node(xgd_Real[i].x - 200 * cos(angle), xgd_Real[i].y + 200 * sin(angle), xgd_Real[i].z - 346));
			xgd_6.push_back(Node(xgd_Real[i].x - 346 * cos(angle), xgd_Real[i].y + 346 * sin(angle), xgd_Real[i].z));
		};
		Creat_WireData wire_1(xgd_1, rou, yingli, N, m_Nodes);
		Creat_WireData wire_2(xgd_2, rou, yingli, N, m_Nodes);
		Creat_WireData wire_3(xgd_3, rou, yingli, N, m_Nodes);
		Creat_WireData wire_4(xgd_4, rou, yingli, N, m_Nodes);
		Creat_WireData wire_5(xgd_5, rou, yingli, N, m_Nodes);
		Creat_WireData wire_6(xgd_6, rou, yingli, N, m_Nodes);
	}
	else if (fenlie == 8)
	{
		vector<Node> xgd_1;
		vector<Node> xgd_2;
		vector<Node> xgd_3;
		vector<Node> xgd_4;
		vector<Node> xgd_5;
		vector<Node> xgd_6;
		vector<Node> xgd_7;
		vector<Node> xgd_8;
		for (int i = 0; i < xgd_Real.size(); i++)
		{
			xgd_1.push_back(Node(xgd_Real[i].x - 200 * cos(angle), xgd_Real[i].y + 200 * sin(angle), xgd_Real[i].z + 480));
			xgd_2.push_back(Node(xgd_Real[i].x + 200 * cos(angle), xgd_Real[i].y - 200 * sin(angle), xgd_Real[i].z + 480));
			xgd_3.push_back(Node(xgd_Real[i].x + 480 * cos(angle), xgd_Real[i].y - 480 * sin(angle), xgd_Real[i].z + 200));
			xgd_4.push_back(Node(xgd_Real[i].x + 480 * cos(angle), xgd_Real[i].y - 480 * sin(angle), xgd_Real[i].z - 200));
			xgd_5.push_back(Node(xgd_Real[i].x + 200 * cos(angle), xgd_Real[i].y - 200 * sin(angle), xgd_Real[i].z - 480));
			xgd_6.push_back(Node(xgd_Real[i].x - 200 * cos(angle), xgd_Real[i].y + 200 * sin(angle), xgd_Real[i].z - 480));
			xgd_7.push_back(Node(xgd_Real[i].x - 480 * cos(angle), xgd_Real[i].y + 480 * sin(angle), xgd_Real[i].z - 200));
			xgd_8.push_back(Node(xgd_Real[i].x - 480 * cos(angle), xgd_Real[i].y + 480 * sin(angle), xgd_Real[i].z + 200));
		};
		Creat_WireData wire_1(xgd_1, rou, yingli, N, m_Nodes);
		Creat_WireData wire_2(xgd_2, rou, yingli, N, m_Nodes);
		Creat_WireData wire_3(xgd_3, rou, yingli, N, m_Nodes);
		Creat_WireData wire_4(xgd_4, rou, yingli, N, m_Nodes);
		Creat_WireData wire_5(xgd_5, rou, yingli, N, m_Nodes);
		Creat_WireData wire_6(xgd_6, rou, yingli, N, m_Nodes);
		Creat_WireData wire_7(xgd_7, rou, yingli, N, m_Nodes);
		Creat_WireData wire_8(xgd_8, rou, yingli, N, m_Nodes);
	}
	//创建单元
	for (int i = 0; i < fenlie; i++)
	{
		cd->CreatWire_ele((i + 1) + p * N * i + num_Node, p * N * (i + 1) + (i + 1) + num_Node, m_Elements_Wire);
	}
	//耐张串部分
	if (direction == 1)//左耐张
	{
		//找耐张段那个间隔棒
		vector<int> ids;
		for (int i = 0; i < fenlie; i++)
		{
			ids.push_back(p * N * i + (i + 1) + num_Node);

		}
		if (fenlie != 1)
		{
			cd->Creat_Spacer(m_Elements_Spacer, ids);//间隔棒单元
		}
		cd->Creat_Strain_Strand(xgd_List, 0, ids, m_Nodes, m_Elements_Strain);
	}
	else if (direction == 2)
	{
		//找耐张段那个间隔棒
		vector<int> ids;
		for (int i = 0; i < fenlie; i++)
		{
			ids.push_back(p * N * (i + 1) + (i + 1) + num_Node);
		}
		if (fenlie != 1)
		{
			cd->Creat_Spacer(m_Elements_Spacer, ids);//间隔棒单元
		}
		int num = xgd_List.size() - 1;
		cd->Creat_Strain_Strand(xgd_List, num, ids, m_Nodes, m_Elements_Strain);
	}
	else if (direction == 3)
	{
		//找耐张段那个间隔棒
		vector<int> ids_1;
		vector<int> ids_2;
		for (int i = 0; i < fenlie; i++)
		{
			ids_1.push_back(p * N * i + (i + 1) + num_Node);
			ids_2.push_back(p * N * (i + 1) + (i + 1) + num_Node);

		}
		int num = xgd_List.size() - 1;
		if (fenlie != 1)
		{
			cd->Creat_Spacer(m_Elements_Spacer, ids_1);//间隔棒单元
			cd->Creat_Spacer(m_Elements_Spacer, ids_2);//间隔棒单元
		}
		cd->Creat_Strain_Strand(xgd_List, 0, ids_1, m_Nodes, m_Elements_Strain);
		cd->Creat_Strain_Strand(xgd_List, num, ids_2, m_Nodes, m_Elements_Strain);
	}
	
}

void Wire_InterFace::Show_Senior()
{
	sen = new Senior(this);
	sen->N_Pts = ui.N_Pts->text().toInt();
	Qt::WindowFlags flags = sen->windowFlags();
	sen->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);
	sen->show();
}

void Wire_InterFace::Creat_Distance(vector<Node> xgd_real, vector<int> num, vector<int>& l_Spacer, vector<int>& d_Spacer)
{
	int c = sen->choose;//0:无 1:公式 1:指派
	int si;//每档均分为几段 每段的长度
	int ssi;//均次档距
	int d_l;//每个间隔棒到左端点的距离
	int dangwei;//间隔棒所在的档位

	if (c == 1)//公式
	{
		if (sen->choose_way == 0)//等间距
		{
			for (int i = 0; i < xgd_real.size() - 1; i++)
			{
				double li = sqrt(pow(xgd_real[i + 1].x - xgd_real[i].x, 2) + pow(xgd_real[i + 1].y - xgd_real[i].y, 2));//档距
				si = li / (num[i] + 1); //每档均分为几段 每段的长度
				for (int j = 1; j < num[i] + 1; j++)
				{
					d_l = si * j;
					dangwei = i + 1;
					l_Spacer.push_back(d_l);
					d_Spacer.push_back(dangwei);
				}
			}
		}
		else if (sen->choose_way == 1)//不等间距
		{
			for (int i = 0; i < xgd_real.size() - 1; i++)
			{
				double li = sqrt(pow(xgd_real[i + 1].x - xgd_real[i].x, 2) + pow(xgd_real[i + 1].y - xgd_real[i].y, 2));//档距
				ssi = li / num[i]; //(均次档距)
				if (num[i] == 1)
				{
					int s1 = 0.6 * ssi;
					dangwei = i + 1;
					l_Spacer.push_back(s1);
					d_Spacer.push_back(dangwei);
				}
				else if (num[i] == 2)
				{
					int s1 = 0.6 * ssi;
					int s2 = 1.6 * ssi;
					l_Spacer.push_back(s1);
					l_Spacer.push_back(s2);
					for (int z = 0; z < 2; z++)
					{
						dangwei = i + 1;
						d_Spacer.push_back(dangwei);
					}

				}
				else if (num[i] == 3)
				{
					int s1 = 0.65 * ssi;
					int s2 = 1.7 * ssi;
					int s3 = 2.5 * ssi;
					l_Spacer.push_back(s1);
					l_Spacer.push_back(s2);
					l_Spacer.push_back(s3);
					for (int z = 0; z < 3; z++)
					{
						dangwei = i + 1;
						d_Spacer.push_back(dangwei);
					}

				}
				else if (num[i] == 4)
				{
					int s1 = 0.6 * ssi;
					int s2 = 1.6 * ssi;
					int s3 = 2.45 * ssi;
					int s4 = 3.45 * ssi;
					l_Spacer.push_back(s1);
					l_Spacer.push_back(s2);
					l_Spacer.push_back(s3);
					l_Spacer.push_back(s4);
					for (int z = 0; z < 4; z++)
					{
						dangwei = i + 1;
						d_Spacer.push_back(dangwei);
					}
				}
				else if (num[i] == 5)
				{
					int s1 = 0.6 * ssi;
					int s2 = 1.6 * ssi;
					int s3 = 2.4 * ssi;
					int s4 = 3.45 * ssi;
					int s5 = 4.45 * ssi;
					l_Spacer.push_back(s1);
					l_Spacer.push_back(s2);
					l_Spacer.push_back(s3);
					l_Spacer.push_back(s4);
					l_Spacer.push_back(s5);
					for (int z = 0; z < 5; z++)
					{
						dangwei = i + 1;
						d_Spacer.push_back(dangwei);
					}
				}
				else if (num[i] > 5)
				{
					int n = num[i];
					//判断奇偶
					if (n % 2 == 0)//偶数 
					{
						int s1 = 0.6 * ssi;
						int s2 = 1.6 * ssi;
						l_Spacer.push_back(s1);
						l_Spacer.push_back(s2);
						for (int m = 0; m < (n - 4) / 2; m++)
						{
							int sm = 1.6 * ssi + 2 * (m + 1) * ssi;
							int ssm = 0.5 * ssi + 2 * (m + 1) * ssi;
							l_Spacer.push_back(sm);
							l_Spacer.push_back(ssm);

						}
						int sn1 = (n - 0.55) * ssi;
						int sn2 = (n - 1.55) * ssi;
						l_Spacer.push_back(sn1);
						l_Spacer.push_back(sn2);
						for (int z = 0; z < n; z++)
						{
							dangwei = i + 1;
							d_Spacer.push_back(dangwei);
						}
					}
					else
					{
						int s1 = 0.6 * ssi;
						int s2 = 1.6 * ssi;
						l_Spacer.push_back(s1);
						l_Spacer.push_back(s2);
						for (int m = 0; m < (n - 4) / 2; m++)
						{
							int sm = 1.6 * ssi + 2 * (m + 1) * ssi;
							int ssm = 0.5 * ssi + 2 * (m + 1) * ssi;
							l_Spacer.push_back(sm);
							l_Spacer.push_back(ssm);

						}
						int sn1 = (n - 0.55) * ssi;
						int sn2 = (n - 1.55) * ssi;
						int sn3 = (n - 2.40) * ssi;
						l_Spacer.push_back(sn1);
						l_Spacer.push_back(sn2);
						l_Spacer.push_back(sn3);
						for (int z = 0; z < n; z++)
						{
							dangwei = i + 1;
							d_Spacer.push_back(dangwei);
						}
					}
				}
			}
		}
	}
}

void Wire_InterFace::Creat_Spacer(vector<int> L, vector<int> num, vector<Node> xgd_Real)
{
	vector<int> ids;
	for (int i = 0; i < L.size(); i++)
	{
		ids = cd->Find_Spacer(fenlie, L[i], N,  xgd_Real, Node_Temp, num[i]);
		cd->Creat_Spacer(m_Elements_Spacer, ids);
	}
}

void Wire_InterFace::OK()
{
	double* p1 = new double[1];
	double* p2 = new double[1];
	double* p3 = new double[1];
	double* p4 = new double[1];
	double* p5 = new double[1];
	sen->Draw_Wire(p1, p2, p3, p4, p5);//N kg/m mm*mm MPa 
	Rou = p1[0] * 0.001;//单位质量
	yingli = p2[0];//应力
	N = p3[0] ; //导线分段数
	area = p4[0];//截面积
	nz_len = p5[0];//耐张串长度
	rou = Rou * 10 / area;
	K = rou / yingli;
	int fenlie_num = ui.Divide_choose->currentIndex();//分裂数
	if (fenlie_num == 0)//1
	{
		fenlie = 1;
	}
	else if (fenlie_num == 1)//2
	{
		fenlie = 2;
	}
	else if (fenlie_num == 2)//4
	{
		fenlie = 4;
	}
	else if (fenlie_num == 3)//6
	{
		fenlie = 6;
	}
	else if (fenlie_num == 4)//8
	{
		fenlie = 8;
	}
	if (Extreme_Point[0] == 0 && Extreme_Point[1] == 0)//端点均不为耐张
	{
		Line_Segment(xgd, xgd, 0);
		Creat_Distance(xgd, sen->Num_Spacer, L_Spacer, D_Spacer);
		Creat_Spacer(L_Spacer, D_Spacer, xgd);
	}
	else if (Extreme_Point[0] == 0 && Extreme_Point[1] == 1)//左端右耐张
	{

		Creat_WireData wire_base(xgd, rou, yingli, N, Node_Base);
		vector<Node>xgd_real;
		cd->Find_Real_XGD(Node_Base, nz_len, N, 1, xgd, xgd_real);
		Line_Segment(xgd, xgd_real, 2);
		Creat_Distance(xgd_real, sen->Num_Spacer, L_Spacer, D_Spacer);
		Creat_Spacer(L_Spacer, D_Spacer, xgd_real);
	}
	else if (Extreme_Point[0] == 1 && Extreme_Point[1] == 0)//左耐张右端
	{
		Creat_WireData wire_base(xgd, rou, yingli, N, Node_Base);
		vector<Node>xgd_real;
		cd->Find_Real_XGD(Node_Base, nz_len, N, 0, xgd, xgd_real);
		Line_Segment(xgd, xgd_real,  1);
		Creat_Distance(xgd_real, sen->Num_Spacer, L_Spacer, D_Spacer);
		Creat_Spacer(L_Spacer, D_Spacer, xgd_real);
	}
	else if (Extreme_Point[0] == 1 && Extreme_Point[1] == 1)//端点均为耐张
	{
		Creat_WireData wire_base(xgd, rou, yingli, N, Node_Base);
		vector<Node>xgd_real;
		cd->Find_Real_XGD(Node_Base, nz_len, N, 2, xgd, xgd_real);
		Line_Segment(xgd, xgd_real, 3);
		Creat_Distance(xgd_real, sen->Num_Spacer, L_Spacer, D_Spacer);
		Creat_Spacer(L_Spacer, D_Spacer, xgd_real);
	}
	m_Elements.insert(m_Elements.end(), m_Elements_Wire.begin(), m_Elements_Wire.end());
	m_Elements.insert(m_Elements.end(), m_Elements_Spacer.begin(), m_Elements_Spacer.end());
	m_Elements.insert(m_Elements.end(), m_Elements_Strain.begin(), m_Elements_Strain.end());
	for (int i = 0; i < m_Elements.size(); i++)
	{
		m_Elements[i].m_idElement = i + 1;//给单元赋编号
	}
	Draw_Wire d;
	vtkSmartPointer<vtkActor> m_LineActor;
	vtkSmartPointer<vtkActor> Node_actor;
	d.Show_Wire(m_Nodes, m_Elements, m_pInterFace->m_Renderer_2);
}




