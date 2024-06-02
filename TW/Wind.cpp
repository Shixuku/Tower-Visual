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
	ui.ice_edi->setText("0.005");
	ui.wind_edi->setText("10");
//	ui.direction_edi->setText("0");
	ui.StepSize_edi->setText("0.3");
	ui.Step_edi->setText("100");
	//分析步的combobox
	int size = m_pCreat_loads->m_pInterFace->ME_AnalysisSteps.size();
	for (int i = 1; i < size + 1; i++)
	{
		ui.comboBox->addItem(m_pCreat_loads->m_pInterFace->ME_AnalysisSteps.Find_Entity(i)->m_Name);
	}
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



void Wind::Get_ui_Data()
{
	 id = m_pInstance->m_StableWind.size() + 1;
	//分析步
	 AnalysisStep = ui.comboBox->currentIndex() + 1;
	 v = ui.wind_edi->text().toDouble();
	 ice = ui.ice_edi->text().toDouble() ;
	// angle = ui.direction_edi->text().toInt();
	
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
	if (ui.rad_stab->isChecked())
	{
		if (ui.wudong_com->currentIndex() == 0)//稳定风风偏
		{
			ParameterStableWind pwind = ParameterStableWind(id, AnalysisStep, angle, v, m_pInstance->Splits, m_pInstance->LandformsType, WindCoefficient);
			m_pInstance->m_StableWind.push_back(pwind);
		}
		else if(ui.wudong_com->currentIndex() == 1)//舞动
		{
	
			ParameterGalloping pgallop = ParameterGalloping(id, AnalysisStep, v, ice);
			m_pInstance->m_GallopWind.push_back(pgallop);
		}
	}


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




