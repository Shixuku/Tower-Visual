#include "Wire_InterFace.h"
#include"InterFace.h"


Wire_InterFace::Wire_InterFace(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_pInterFace = dynamic_cast<InterFace*>(parent);
	Q_ASSERT(m_pInterFace != nullptr);
	Initialize();
	connect(m_pInterFace, &InterFace::Msg_Select_Nodes, this, &Wire_InterFace::Insert_Data);
	connect(ui.Pts_OK, &QPushButton::clicked, this, [=]() {

		int a = ui.N_Pts->text().toInt();
		Insert_Point_Infor(a);
		});

	
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
