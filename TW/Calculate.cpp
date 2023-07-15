//#include "Calculate.h"
//#include"InterFace.h"
//Calculate::Calculate(InterFace* InterFace, QWidget *parent)
//	: QDialog(parent)
//{
//	ui.setupUi(this);
//	m_pInterFace = InterFace;
//	CreateTreeWight();
//}
//
//Calculate::~Calculate()
//{}
//
//void Calculate::CreateTreeWight()
//{
//	int towerSize = m_pInterFace->TP.size();
//	int towerWireGroupSize= m_pInterFace->TWG.size();
//	int instanceSize = m_pInterFace->TP.size() + m_pInterFace->TWG.size();
//	cout << "instanceSize" << instanceSize << "\n";
//	QStringList headertext;
//	headertext << "所有实例";
//	ui.tableWidget->setColumnCount(headertext.count());
//	ui.tableWidget->setRowCount(instanceSize);
//	ui.tableWidget->setHorizontalHeaderLabels(headertext);
//	ui.tableWidget->verticalHeader()->setVisible(false);
//	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//
//	for (int i = 0; i < instanceSize; i++)
//	{
//		if (i < towerSize)
//		{
//			ui.tableWidget->setItem(i, 0, new QTableWidgetItem("杆塔实例 " + QString::number(i + 1)));
//			ui.tableWidget->item(i, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//		}
//		else
//		{
//			ui.tableWidget->setItem(i, 0, new QTableWidgetItem("杆线实例 " + QString::number(i + 1- towerSize)));
//			ui.tableWidget->item(i, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//		}
//	}
//
//	////支持多选模式，比如Ctrl、Shift、 Ctrl+A、鼠标左键按下滑动多选
//	//ui.tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
//	//设置单行选择
//	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
//}
