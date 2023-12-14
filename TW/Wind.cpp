#include "Wind.h"
#include"InterFace.h"
#include"WindList.h"
#include <cmath>
#include <QMessageBox>
#include"RandomWind.h"
#include<math.h>
#include<Eigen/Dense>
#include"Creat_Loads.h"
#include"Instance.h"
#include"qstring.h"

Wind::Wind(Creat_Loads* creat_loads, QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);
	m_pCreat_loads = creat_loads;
	m_pInstance = m_pCreat_loads->m_instance;//调用父类的tower
	m_pcreatWire = m_pCreat_loads->m_pInterFace->TWG.Find_Entity(1);
	Initialize();
	//CreateCombobox();
	connect(ui.rad_t, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(0); });
	connect(ui.rad_f, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(1); });
	connect(ui.rad_stab, &QRadioButton::clicked, this, [=]() {ui.stackedWidget_2->setCurrentIndex(0); });
	connect(ui.rad_ran, &QRadioButton::clicked, this, [=]() {ui.stackedWidget_2->setCurrentIndex(1); });

	connect(ui.rad_A, &QRadioButton::clicked, [=]() {alf = 0.12;  m_pInstance->LandformsType = "A";  });
	connect(ui.rad_B, &QRadioButton::clicked, [=]() {alf = 0.16;  m_pInstance->LandformsType = "B"; });
	connect(ui.rad_C, &QRadioButton::clicked, [=]() {alf = 0.20;  m_pInstance->LandformsType = "C"; });
	connect(ui.rad_D, &QRadioButton::clicked, [=]() {alf = 0.30;  m_pInstance->LandformsType = "D";  });

	ShowObject();
	void (QComboBox:: * intChanged)(int) = &QComboBox::currentIndexChanged;
	//connect(ui.object_com, intChanged, this, &Wind::ShowObject);
	connect(ui.sure_btn_2, &QPushButton::clicked, this, &Wind::ui_Speed);
	//connect(ui.count_btn, &QPushButton::clicked, this, [=]() {m_pcreatWire->CreateOutPut(); this->close(); });
	connect(ui.sure_btn, &QPushButton::clicked, this, [=]() {Get_ui_Data();this->close(); });

	
}

Wind::~Wind()
{}

void Wind::Initialize()
{
	ui.ice_edi->setText("5");
	ui.wind_edi->setText("10");
	ui.direction_edi->setText("0");
	ui.StepSize_edi->setText("0.3");
	ui.Step_edi->setText("100");
	//分析步的combobox
	int size = m_pCreat_loads->m_pInterFace->ME_AnalysisSteps.size();
	for (int i = 1; i < size + 1; i++)
	{
		ui.comboBox->addItem(m_pCreat_loads->m_pInterFace->ME_AnalysisSteps.Find_Entity(i)->m_Name);
	}
}

void Wind::CreateCombobox()
{
	/*ExampleNum =  m_pInterFace->ui.treeWidget->topLevelItem(2)->childCount();
	
	for (int i = 0; i < ExampleNum; i++)
	{
		ui.object_com->addItem("导线实例" + QString::number(i + 1));
	
	}*/
}

void Wind::ShowObject()
{
	//int Index = ui.object_com->currentIndex();
	//if (Index == -1)return;
	//if (Index < ExampleNum)
	//{
	//	id_Part = Index + 1;
	//	m_Renderer->RemoveAllViewProps();  // 清空当前显示的所有actor
	
	//	if (m_pcreatWire != nullptr)
	//	{
	//		m_pcreatWire->Show_VTKnode(m_Renderer);
	//		m_pcreatWire->Show_VTKbeam(m_Renderer);
	//		m_pcreatWire->Show_VTKtruss(m_Renderer);
	//	}
	//	
	//}
	//m_Renderer->ResetCamera();
	//ui.widget_5->update();
}

void Wind::ui_Speed()
{
	Get_ui_Data();
	if (ui.wind_com->currentIndex() == 0)
	{
		if (w_l == nullptr)
		{
			w_l = new WindList(this);
			w_l->show();
		}
		else
		{
			w_l->show();
		}
	
	}
	else if (ui.wind_com->currentIndex() == 1)
	{
		if (ran == nullptr)
		{
			ran = new RandomWind(this);
			ran->show();
		}
		else
		{
			ran->show();
		}
	}
	
}

void Wind::ReadStableData()
{
	//QStringList headerLabels;
	//headerLabels << "单元编号" << "单元长度(m)" << "风压系数"<<"风载荷(N)";
	//ui.tableWidget->setColumnCount(headerLabels.count());
	//ui.tableWidget->setHorizontalHeaderLabels(headerLabels);
	//ui.tableWidget->verticalHeader()->setVisible(false);
	//ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	//ui.tableWidget->setRowCount(num_ele);  // 默认N行
	//for (int i = 0; i < num_ele; i++)
	//{
	//	ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(m_pcreatWire->m_Elements_Trusses[i].m_idElement)));
	//	ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number((L_ele[i]))));
	//	ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(Uz_ele[i])));
	//	ui.tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(Wx_values[i] * 1e3)));
	//	// 设置单元格为只读
	//	for (int j = 0; j < ui.tableWidget->columnCount(); j++)
	//	{
	//		QTableWidgetItem* item = ui.tableWidget->item(i, j);
	//		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
	//	}
	//}
}

void Wind::ReadRandomData()
{
	/*QStringList headerLabels;
	headerLabels << "t(s)" << "风速(m/s)";
	ui.tableWidget->setColumnCount(headerLabels.count());
	ui.tableWidget->setHorizontalHeaderLabels(headerLabels);
	ui.tableWidget->verticalHeader()->setVisible(false);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	
*/
}

void Wind::BtnOk()
{
	if (m_pcreatWire == nullptr)
	{
		QMessageBox::warning(this, "提示", "请创建分析对象！");
		return;
	}

	num_node = m_pcreatWire->m_Nodes.size();
	num_ele = m_pcreatWire->m_Elements_Trusses.size();
	bool groundSelected = false;

	QList<QRadioButton*> radioButtons = { ui.rad_A, ui.rad_B, ui.rad_C, ui.rad_D };

	for (QRadioButton* radioButton : radioButtons)
	{
		if (radioButton->isChecked())
		{
			groundSelected = true;
			break;
		}
	}

	if (!groundSelected)
	{
		QMessageBox::warning(this, "提示", "请先选择地面粗糙程度！");
		return;
	}
	if (ui.rad_stab->isChecked())
	{
		CountElePara();
		CountWindForce();
		ReadStableData();
	}
	else if (ui.rad_ran->isChecked())
	{
		int num = ui.Step_edi->text().toInt();
		double StepSize = ui.StepSize_edi->text().toDouble();
		for (int j = 0; j < ran->NumOfWindzones; j++)
		{
			double t = 0;
			for (int i = 0; i < num; i++)
			{
				t += StepSize;
				//WindSpeedInterpolation(j, t);

			}
		}
		
	}
}

void Wind::Get_ui_Data()
{
	 id = m_pInstance->m_StableWind.size() + 1;
	//分析步
	 AnalysisStep = ui.comboBox->currentIndex() + 1;
	 v = ui.wind_edi->text().toDouble();
	 angle = ui.direction_edi->text().toInt();
	
	//cout << "xxxx" << "  " << ui.stackedWidget_2->currentIndex() << "\n";
	if (ui.stackedWidget_2->currentIndex() == 0)
	{
		m_pInstance->TypeWind = 0;
	}
	else if(ui.stackedWidget_2->currentIndex() == 1)
	{
		m_pInstance->TypeWind = 1;
	}
	double d = 2 * sqrt(m_pInstance->areaWire / 3.1415926);
	if (ui.rad_t->isChecked() || d < 17)
	{
		miusc = 1.2;
	}
	else if (d >= 17)
	{
		miusc = 1.1;
	}
	if (ui.ice_edi->text().toDouble() <= 5)
	{
		B = 1.1;
	}
	else if (ui.ice_edi->text().toDouble() > 5)
	{
		B = 1.2;
	}
	else
	{
		B = 1.1;
	}
	WindCoefficient = miusc * B;
}

double Wind::CountUz(double h)
{
	double uz = 0.0;
	if (ui.rad_A->isChecked())
	{
		uz = 1.284 * pow(h / 10, 0.24);
		if (uz < 1.09)
		{
			uz = 1.09;
		}
	}
	else if (ui.rad_B->isChecked())
	{
		uz = 1.000 * pow(h / 10, 0.30);
		if (uz < 1.00)
		{
			uz = 1.00;
		}
	}
	else if (ui.rad_C->isChecked())
	{
		uz = 0.544 * pow(h / 10, 0.44);
		if (uz < 0.65)
		{
			uz = 0.65;
		}
	}
	else if (ui.rad_D->isChecked())
	{
		uz = 0.262 * pow(h / 10, 0.60);
		if (uz < 0.51)
		{
			uz = 0.51;
		}
	}

	return uz;
}

void Wind::CountElePara()
{
	// 算每个节点的风压系数
	for (int k = 0; k < num_node; k++)
	{
		const auto& node = m_pcreatWire->m_Nodes[k];
		double uz = CountUz((node.z + 14000) / 1000);
		Uz_node.push_back(uz);
	}

	// 计算每个单元的长度和风压系数
	for (int i = 0; i < num_ele; i++)
	{
		const auto& ele = m_pcreatWire->m_Elements_Trusses[i];
		int ipt1 = ele.m_idNode[0] - 1; // id
		int ipt2 = ele.m_idNode[1] - 1;
		const auto& node1 = m_pcreatWire->m_Nodes[ipt1];
		const auto& node2 = m_pcreatWire->m_Nodes[ipt2];

		double Vx[3];
		Vx[0] = (node2.x - node1.x) ;
		Vx[1] = (node2.y - node1.y) ;
		Vx[2] = (node2.z - node1.z) ;

		double m_L = sqrt(Vx[0] * Vx[0] + Vx[1] * Vx[1] + Vx[2] * Vx[2]);
		L_ele.push_back(m_L);

		double uz_ele = (Uz_node[ipt1] + Uz_node[ipt2]) / 2;
		Uz_ele.push_back(uz_ele);
	}
}


std::vector<double> Wind::CountWindForce()
{
	double v = ui.wind_edi->text().toDouble();
	//m_pcreatWire->s
	double W0 = v * v / 1600;
	double aerfa = 1.00;
	double beitaC = 1.0;
	double Usc = 1.1;
	double d = 555;
	double B = 1.0;
	double theta = 90;
	//差Uz和Lp
	for (int i = 0; i < num_ele; i++)
	{
		double Wx = W0 * aerfa * Uz_ele[i] * Usc * beitaC * d * L_ele[i] * B;
		Wx_values.push_back(Wx);
	}
	return Wx_values;
}

//double Wind::WindSpeedInterpolation(int pointIndex, double time)
//{
//	double T = 0, Dt = 0;
//	Eigen::MatrixXd velocitiys;
//	Eigen::VectorXd v;
//	if (ran->fw)
//	{
//		velocitiys = ran->fw->getVelocityMat();
//		v = velocitiys.col(pointIndex);//第i点的风速
//
//		Dt = 0.5;
//		//double Dt = ran->fw->getDt();
//		T = ran->T;
//	}
//	else
//	{
//		QMessageBox::warning(this, "提示", "请创建随机风！");
//	}
//	double re_v = 0;
//	
//	int start = time / Dt;//第几次开始
//
//	if ((time >= (start * Dt)) && (time <= ((start + 1) * Dt)))
//	{
//		re_v = v[start] + (v[start + 1] - v[start]) * (time - Dt * start) / Dt;
//		cout << pointIndex + 1 << "  " << "(" << v[start] << "," << v[start + 1] << "): " << re_v << "\n";
//	}
//	return re_v;
//}


