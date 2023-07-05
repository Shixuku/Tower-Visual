#include "Wire_InterFace.h"
#include"Senior.h"

Wire_InterFace::Wire_InterFace(TowerWireGroup* TowerWireGroup, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	towerWireGroup = TowerWireGroup;
	headertext << "序号" << "X(m)" << "Y(m)" << "Z(m)" << "点类型";
	ui.Section_Lists->setColumnCount(headertext.count());
	ui.Section_Lists->setHorizontalHeaderLabels(headertext);
	ui.Section_Lists->verticalHeader()->setVisible(false);
	ui.Section_Lists->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	connect(ui.Pts_OK, &QPushButton::clicked, this, [=]() 
		{	
			int a = 0;
			if (towerWireGroup != nullptr)
			{
				a = towerWireGroup->SuspensionNode.size();
			}
			else
			{
				a = ui.N_Pts->text().toInt();
			}
			SetTableWideget(a); 
		});
	connect(ui.Senior_Btn, &QPushButton::clicked, this, &Wire_InterFace::ui_Senior);
	connect(ui.Btn_ensure, &QPushButton::clicked, this, [=]() {SaveSusPoint(); this->accept(); });
	connect(ui.Btn_cancel, &QPushButton::clicked, this, &Wire_InterFace::reject);

}

Wire_InterFace::~Wire_InterFace()
{}

void Wire_InterFace::SetTableWideget(int row)
{
	ui.Section_Lists->clear();
	ui.Section_Lists->setHorizontalHeaderLabels(headertext);
	ui.Section_Lists->setRowCount(row);//默认N行
	for (int i = 0; i < row; i++)
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
		if (towerWireGroup != nullptr)
		{
			int id = towerWireGroup->SuspensionNode[i];
			ui.Section_Lists->setItem(i, 1, new QTableWidgetItem(QString::number(towerWireGroup->NodeData[id].x / 1e3)));
			ui.Section_Lists->setItem(i, 2, new QTableWidgetItem(QString::number(towerWireGroup->NodeData[id].y / 1e3)));
			ui.Section_Lists->setItem(i, 3, new QTableWidgetItem(QString::number(towerWireGroup->NodeData[id].z / 1e3)));
		}

	}
}

void Wire_InterFace::Initialize()
{
}

void Wire_InterFace::ui_Senior()
{
	if (sen == nullptr)
	{
		sen = new Senior(this);
		sen->exec();
	}
	else
	{
		sen->exec();
	}
	
}

void Wire_InterFace::Get_Data(WireData& wd)
{
	wd.N = N;
	wd.fenlie = ui.Divide_choose->currentText().toInt();
	wd.unitMass = unitMass;
	wd.area = area;
	wd.stress = stress;
	wd.strainL = strainL;
	wd.sag = sag;
	wd.WireSus = WireSus;
}

void Wire_InterFace::SaveSusPoint()
{
	int rowNum = ui.Section_Lists->rowCount(); //表格有多少行
	double xi, yi, zi;
	//表格点
	for (int i = 0; i < rowNum; i++)
	{
		xi = ui.Section_Lists->item(i, 1)->text().toDouble();
		yi = ui.Section_Lists->item(i, 2)->text().toDouble();
		zi = ui.Section_Lists->item(i, 3)->text().toDouble();
		WireSus.push_back(Node(xi, yi, zi));
	}
}


