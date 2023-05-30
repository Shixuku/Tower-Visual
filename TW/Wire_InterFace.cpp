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
	ui.Section_Lists->setRowCount(N);//Ĭ��N��
	for (int i = 0; i < N; i++)
	{
		if (i == 0)
		{
			QComboBox* Point_Type = new QComboBox;
			Point_Type->addItem("���Ŷ˵�");
			Point_Type->addItem("�˵�");
			ui.Section_Lists->setCellWidget(i, 4, (QWidget*)Point_Type);
		}
		else if (i == N - 1)
		{
			//���һ��
			QComboBox* Point_Type = new QComboBox;
			Point_Type->addItem("���Ŷ˵�");
			Point_Type->addItem("�˵�");
			ui.Section_Lists->setCellWidget(i, 4, (QWidget*)Point_Type);
		}
		else
		{
			ui.Section_Lists->setItem(i, 4, new QTableWidgetItem("���ҵ�"));
		}
	}
}

void Wire_InterFace::Insert_Pts_to_vector()
{
	//�˵㲿��
	QString msg;
	QString msg_end;
	int rowNum = ui.Section_Lists->rowCount(); //ȡ��������� 
	QWidget* widget = ui.Section_Lists->cellWidget(0, 4);
	QWidget* widget_end = ui.Section_Lists->cellWidget(rowNum - 1, 4);
	QComboBox* combox = (QComboBox*)widget;
	QComboBox* combox1 = (QComboBox*)widget_end;
	msg = combox->currentText();
	msg_end = combox1->currentText();
	if (msg == "�˵�")
	{
		Extreme_Point.push_back(0);
	}
	else if (msg == "���Ŷ˵�")
	{
		Extreme_Point.push_back(1);
	}
	if (msg_end == "�˵�")
	{
		Extreme_Point.push_back(0);
	}
	else if (msg_end == "���Ŷ˵�")
	{
		Extreme_Point.push_back(1);
	}
	//�м������ŵ�
	double xi, yi, zi;
	//����
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
	headertext << "�ҵ���" << "X(m)" << "Y(m)" << "Z(m)" << "������";
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
	//ȷ��������
	double v_x = xgd_Real[p].x - xgd_List[p - 1].x;
	double v_y = xgd_Real[p].y - xgd_List[p - 1].y;
	double angle = ((atan2(v_x, v_y) * 180) / 3.1415926);
	angle = vtkMath::RadiansFromDegrees(angle);
	Creat_WireData wire_temp(xgd_Real, rou, yingli, N, Node_Temp); //zuo���ѵĻ�����
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
			xgd_1.push_back(Node(xgd_Real[i].x - 200 * cos(angle), xgd_Real[i].y + 200 * sin(angle), xgd_Real[i].z + 350));
			xgd_2.push_back(Node(xgd_Real[i].x + 200 * cos(angle), xgd_Real[i].y - 200 * sin(angle), xgd_Real[i].z + 350));
			xgd_3.push_back(Node(xgd_Real[i].x + 400 * cos(angle), xgd_Real[i].y - 400 * sin(angle), xgd_Real[i].z));
			xgd_4.push_back(Node(xgd_Real[i].x + 200 * cos(angle), xgd_Real[i].y - 200 * sin(angle), xgd_Real[i].z - 350));
			xgd_5.push_back(Node(xgd_Real[i].x - 200 * cos(angle), xgd_Real[i].y + 200 * sin(angle), xgd_Real[i].z - 350));
			xgd_6.push_back(Node(xgd_Real[i].x - 400 * cos(angle), xgd_Real[i].y + 400 * sin(angle), xgd_Real[i].z));
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
	//������Ԫ
	for (int i = 0; i < fenlie; i++)
	{
		cd->CreatWire_ele((i + 1) + p * N * i + num_Node, p * N * (i + 1) + (i + 1) + num_Node, m_Elements_Wire);
	}

	//���Ŵ�����
	if (direction == 1)//������
	{
		//�����Ŷ��Ǹ������
		vector<int> ids;
		for (int i = 0; i < fenlie; i++)
		{
			ids.push_back(p * N * i + (i + 1) + num_Node);

		}
		cd->Creat_Spacer(m_Elements_Spacer, ids);//�������Ԫ
		cd->Creat_Strain_Strand(xgd_List, 0, ids, m_Nodes, m_Elements_Strain);
	}
	else if (direction == 2)
	{
		//�����Ŷ��Ǹ������
		vector<int> ids;
		for (int i = 0; i < fenlie; i++)
		{
			ids.push_back(p * N * (i + 1) + (i + 1) + num_Node);
		}
		cd->Creat_Spacer(m_Elements_Spacer, ids);//�������Ԫ
		int num = xgd_List.size() - 1;
		cd->Creat_Strain_Strand(xgd_List, num, ids, m_Nodes, m_Elements_Strain);
	}
	else if (direction == 3)
	{
		//�����Ŷ��Ǹ������
		vector<int> ids_1;
		vector<int> ids_2;
		for (int i = 0; i < fenlie; i++)
		{
			ids_1.push_back(p * N * i + (i + 1) + num_Node);
			ids_2.push_back(p * N * (i + 1) + (i + 1) + num_Node);

		}
		int num = xgd_List.size() - 1;
		cd->Creat_Spacer(m_Elements_Spacer, ids_1);//�������Ԫ
		cd->Creat_Spacer(m_Elements_Spacer, ids_2);//�������Ԫ
		cd->Creat_Strain_Strand(xgd_List, 0, ids_1, m_Nodes, m_Elements_Strain);
		cd->Creat_Strain_Strand(xgd_List, num, ids_2, m_Nodes, m_Elements_Strain);
	}
}

void Wire_InterFace::Show_Senior()
{
	sen = new Senior(this);
	Qt::WindowFlags flags = sen->windowFlags();
	sen->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);
	sen->show();
}

void Wire_InterFace::OK()
{
	double* p1 = new double[1];
	double* p2 = new double[1];
	double* p3 = new double[1];
	double* p4 = new double[1];
	double* p5 = new double[1];
	sen->Draw_Wire(p1, p2, p3, p4, p5);//N kg/m mm*mm MPa 
	Rou = p1[0] * 0.001;//��λ����
	yingli = p2[0];//Ӧ��
	N = p3[0] ; //���߷ֶ���
	area = p4[0];//�����
	nz_len = p5[0];//���Ŵ�����
	rou = Rou * 10 / area;
	K = rou / yingli;
	int fenlie_num = ui.Divide_choose->currentIndex();//������
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
	if (Extreme_Point[0] == 0 && Extreme_Point[1] == 0)//�˵����Ϊ����
	{
		Line_Segment(xgd, xgd, 0);
		//Creat_Distance(xgd, sen->Num_Spacer, L_Spacer, D_Spacer);
		//Creat_Spacer(L_Spacer, D_Spacer, 0, xgd);
	}
	else if (Extreme_Point[0] == 0 && Extreme_Point[1] == 1)//���������
	{

		Creat_WireData wire_base(xgd, rou, yingli, N, Node_Base);
		vector<Node>xgd_real;
		cd->Find_Real_XGD(Node_Base, nz_len, N, 1, xgd, xgd_real);
		Line_Segment(xgd, xgd_real, 2);
		//Creat_Distance(xgd_real, sen->Num_Spacer, L_Spacer, D_Spacer);
		//Creat_Spacer(L_Spacer, D_Spacer, 0, xgd_real);
	}
	else if (Extreme_Point[0] == 1 && Extreme_Point[1] == 0)//�������Ҷ�
	{
		Creat_WireData wire_base(xgd, rou, yingli, N, Node_Base);
		vector<Node>xgd_real;
		cd->Find_Real_XGD(Node_Base, nz_len, N, 0, xgd, xgd_real);
		Line_Segment(xgd, xgd_real,  1);
		//Creat_Distance(xgd_real, sen->Num_Spacer, L_Spacer, D_Spacer);
		//Creat_Spacer(L_Spacer, D_Spacer, 0, xgd_real);
	}
	else if (Extreme_Point[0] == 1 && Extreme_Point[1] == 1)//�˵��Ϊ����
	{
		Creat_WireData wire_base(xgd, rou, yingli, N, Node_Base);
		vector<Node>xgd_real;
		cd->Find_Real_XGD(Node_Base, nz_len, N, 2, xgd, xgd_real);
		Line_Segment(xgd, xgd_real, 3);
		//Creat_Distance(xgd_real, sen->Num_Spacer, L_Spacer, D_Spacer);
		//Creat_Spacer(L_Spacer, D_Spacer, 0, xgd_real);
	}
	m_Elements.insert(m_Elements.end(), m_Elements_Wire.begin(), m_Elements_Wire.end());
	m_Elements.insert(m_Elements.end(), m_Elements_Spacer.begin(), m_Elements_Spacer.end());
	m_Elements.insert(m_Elements.end(), m_Elements_Strain.begin(), m_Elements_Strain.end());
	for (int i = 0; i < m_Elements.size(); i++)
	{
		m_Elements[i].m_idElement = i + 1;//����Ԫ�����
	}
	Draw_Wire d;
	vtkSmartPointer<vtkActor> m_LineActor;
	vtkSmartPointer<vtkActor> Node_actor;
	d.Show_Wire(m_Nodes, m_Elements, m_pInterFace->m_Renderer);
}




