#include "CreateSingleTower.h"
#include"Tower.h"
#include"InterFace.h"
#include"QMessageBox"
CreateSingleTower::CreateSingleTower(InterFace* m_pinterface, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	m_pInterface = m_pinterface;
	Initialize();
	connect(ui.comboBox_line, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeTowerType()));
	connect(ui.comboBox_towertype, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeTowerHugao()));
	connect(ui.btn_ok, &QPushButton::clicked, this, &CreateSingleTower::accept);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &CreateSingleTower::reject);
	connect(ui.btn_createTowerLeg, &QPushButton::clicked, this, &CreateSingleTower::CreateTowerLeg);
}

CreateSingleTower::~CreateSingleTower()
{}

void CreateSingleTower::Initialize()
{
	//��·����
	for (int i = 1; i < m_pInterface->TWG.size() + 1; i++)
	{
		if (m_pInterface->TWG.Find_Entity(i)->TWG_TP.size() != 0)
		{
			ui.comboBox_line->addItem(m_pInterface->TWG.Find_Entity(i)->m_Name);
			//������
			int towerSize = m_pInterface->TWG.Find_Entity(i)->TWG_TP.size();//ѡ��������·����������
			//������ӵ�item����
			for (int j = 0; j < towerSize; j++)
			{
				ui.comboBox_towertype->addItem(m_pInterface->TWG.Find_Entity(i)->TWG_TP.Find_Entity(j + 1)->m_Name);
			}
			//����
			int hugaoSize = m_pInterface->TWG.Find_Entity(i)->TWG_TP.Find_Entity(1)->TP_Height.size();//ѡ����������ĺ�������
			//��������ӵ�combobox����
			for (int j = 0; j < hugaoSize; j++)
			{
				QString hugaoID = QString::number(m_pInterface->TWG.Find_Entity(i)->TWG_TP.Find_Entity(1)->TP_Height.Find_Entity(j + 1)->m_id);
				QString hugaoHIGH = QString::number(m_pInterface->TWG.Find_Entity(i)->TWG_TP.Find_Entity(1)->TP_Height.Find_Entity(j + 1)->hight);
				ui.comboBox_hugao->addItem(hugaoID + " " + hugaoHIGH);
			}
		}	
	}
}

void CreateSingleTower::ChangeTowerType()
{
	ui.comboBox_towertype->blockSignals(true);
	ui.comboBox_towertype->clear();
	int LineID = ui.comboBox_line->currentIndex() + 1;//ȷ����·
	int towerSize = m_pInterface->TWG.Find_Entity(LineID)->TWG_TP.size();//ѡ��������·����������
	//������ӵ�item����
	for (int i = 0; i < towerSize; i++)
	{
		ui.comboBox_towertype->addItem(m_pInterface->TWG.Find_Entity(LineID)->TWG_TP.Find_Entity(i + 1)->m_Name);
	}
	ui.comboBox_towertype->blockSignals(false);
}


void CreateSingleTower::ChangeTowerHugao()
{
	ui.comboBox_hugao->blockSignals(true);
	ui.comboBox_hugao->clear();
	int LineID = ui.comboBox_line->currentIndex() + 1;//ȷ����·
	int TowerID = ui.comboBox_towertype->currentIndex() + 1;//ȷ��������
	int hugaoSize = m_pInterface->TWG.Find_Entity(LineID)->TWG_TP.Find_Entity(TowerID)->TP_Height.size();//ѡ����������ĺ�������
	//��������ӵ�combobox����
	for (int i = 0; i < hugaoSize; i++)
	{
		QString hugaoID = QString::number(m_pInterface->TWG.Find_Entity(LineID)->TWG_TP.Find_Entity(TowerID)->TP_Height.Find_Entity(i + 1)->m_id);
		QString hugaoHIGH = QString::number(m_pInterface->TWG.Find_Entity(LineID)->TWG_TP.Find_Entity(TowerID)->TP_Height.Find_Entity(i + 1)->hight);
		ui.comboBox_hugao->addItem(hugaoID + " " + hugaoHIGH);
	}
	ui.comboBox_hugao->blockSignals(false);
}

void CreateSingleTower::CreateTowerLeg()
{
	QStringList headertext, towerLeg;//��ͷ
	headertext << "����" << "����";
	towerLeg << "LegA" << "LegB" << "LegC" << "LegD";
	ui.tableWidget->setColumnCount(headertext.count());//��
	ui.tableWidget->setRowCount(4);//��
	ui.tableWidget->setHorizontalHeaderLabels(headertext);
	ui.tableWidget->verticalHeader()->setVisible(false);
	//�����б�ͷ
	for (int i = 0; i < towerLeg.count(); i++)
	{
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(towerLeg[i]));
		ui.tableWidget->item(i, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	}
	//ȡ����
	int LineID = ui.comboBox_line->currentIndex() + 1;//ȷ����·
	int TowerID = ui.comboBox_towertype->currentIndex() + 1;//ȷ��������
	int hugaoID = ui.comboBox_hugao->currentIndex() + 1;//ȷ������
	int legSize = m_pInterface->TWG.Find_Entity(LineID)->TWG_TP.Find_Entity(TowerID)->
		TP_Height.Find_Entity(hugaoID)->LegList.size();//ѡ����������ĺ�������

	QComboBox* AnalysisStepA = new QComboBox;
	QComboBox* AnalysisStepB = new QComboBox;
	QComboBox* AnalysisStepC = new QComboBox;
	QComboBox* AnalysisStepD = new QComboBox;

	for (int i = 0; i < legSize; i++)
	{
		AnalysisStepA->addItem(QString::number(m_pInterface->TWG.Find_Entity(LineID)->TWG_TP.Find_Entity(TowerID)->
			TP_Height.Find_Entity(hugaoID)->LegList[i]));
		AnalysisStepB->addItem(QString::number(m_pInterface->TWG.Find_Entity(LineID)->TWG_TP.Find_Entity(TowerID)->
			TP_Height.Find_Entity(hugaoID)->LegList[i]));
		AnalysisStepC->addItem(QString::number(m_pInterface->TWG.Find_Entity(LineID)->TWG_TP.Find_Entity(TowerID)->
			TP_Height.Find_Entity(hugaoID)->LegList[i]));
		AnalysisStepD->addItem(QString::number(m_pInterface->TWG.Find_Entity(LineID)->TWG_TP.Find_Entity(TowerID)->
			TP_Height.Find_Entity(hugaoID)->LegList[i]));
	}
	ui.tableWidget->setCellWidget(0, 1, (QWidget*)AnalysisStepA);
	ui.tableWidget->setCellWidget(1, 1, (QWidget*)AnalysisStepB);
	ui.tableWidget->setCellWidget(2, 1, (QWidget*)AnalysisStepC);
	ui.tableWidget->setCellWidget(3, 1, (QWidget*)AnalysisStepD);

	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
}

void CreateSingleTower::BtnOK(Tower* tower)
{
	m_pInterface->m_Renderer->RemoveAllViewProps();
	tower->m_id = m_pInterface->TP.size() + 1;//�����
	int LineID = ui.comboBox_line->currentIndex() + 1;//ȷ����·
	int TowerID = ui.comboBox_towertype->currentIndex() + 1;//ȷ��������
	int hugaoID = ui.comboBox_hugao->currentIndex() + 1;//ȷ������

	//ѡ����
	for (const auto& j : m_pInterface->TWG.Find_Entity(LineID)->TWG_TP.Find_Entity(TowerID)
		->TP_Height.Find_Entity(hugaoID)->BodyList)
	{
		Part_Base* part = m_pInterface->TWG.Find_Entity(LineID)->TWG_TP.Find_Entity(TowerID)->TP_Part.Find_Entity(j);
		tower->addPart(part);
	}
	QWidget* widgetA = ui.tableWidget->cellWidget(0, 1);
	QWidget* widgetB = ui.tableWidget->cellWidget(1, 1);
	QWidget* widgetC = ui.tableWidget->cellWidget(2, 1);
	QWidget* widgetD = ui.tableWidget->cellWidget(3, 1);
	QComboBox* comboxA = (QComboBox*)widgetA;
	QComboBox* comboxB = (QComboBox*)widgetB;
	QComboBox* comboxC = (QComboBox*)widgetC;
	QComboBox* comboxD = (QComboBox*)widgetD;
	int legA = comboxA->currentText().toInt();
	int legB = comboxB->currentText().toInt();
	int legC = comboxC->currentText().toInt();
	int legD = comboxD->currentText().toInt();
	//����A--��������
	Part_Base* partA = m_pInterface->TWG.Find_Entity(LineID)->TWG_TP.Find_Entity(TowerID)->TP_Part.Find_Entity(legA);
	if (partA != nullptr)
	{
		vector<double> v_id;
		//������
		for (int i = 0; i < partA->m_Nodes.size(); i++)
		{
			partA->m_Nodes[i].x *= -1;
			partA->m_Nodes[i].y *= -1;
			v_id.push_back(partA->m_Nodes[i].z);
		}
		//���A����
		tower->addPart(partA);
		// ��partB��z��Сֵ
		auto min_itA = min_element(v_id.begin(), v_id.end());		double min_valueA = *min_itA;
		//����͵���--������tower����ȥ��
		for (const auto& i : tower->m_Nodes)
		{
			if (abs(min_valueA - i.z) < 1e-5)
			{
				int id = i.m_idNode;
				tower->RestraintNode.push_back(id);
			}

		}
		//��ԭ
		for (int i = 0; i < partA->m_Nodes.size(); i++)
		{
			partA->m_Nodes[i].x *= -1;
			partA->m_Nodes[i].y *= -1;
		}
	}

	//����B--�ڶ�����
	Part_Base* partB = m_pInterface->TWG.Find_Entity(LineID)->TWG_TP.Find_Entity(TowerID)->TP_Part.Find_Entity(legB);
	if (partB != nullptr)
	{
		vector<double> v_id;
		//������
		for (int i = 0; i < partB->m_Nodes.size(); i++)
		{
			partB->m_Nodes[i].x *= -1;
			v_id.push_back(partB->m_Nodes[i].z);
		}
		//���B����
		tower->addPart(partB);
		// ��partB��z��Сֵ
		auto min_itA = min_element(v_id.begin(), v_id.end());		double min_valueA = *min_itA;
		//����͵���--������tower����ȥ��
		for (const auto& i : tower->m_Nodes)
		{
			if (abs(min_valueA - i.z) < 1e-5 && i.x < 0 && i.y>0)
			{
				int id = i.m_idNode;
				// �ж��µ������Ƿ��Ѿ�������������
				auto it = std::find(tower->RestraintNode.begin(), tower->RestraintNode.end(), id);
				if (it == tower->RestraintNode.end())
				{
					// �����в����ڸ����ݣ��������
					tower->RestraintNode.push_back(id);
				}

			}
		}
		//��ԭ
		for (int i = 0; i < partB->m_Nodes.size(); i++)
		{
			partB->m_Nodes[i].x *= -1;
		}
	}

	//����C--��һ����
	Part_Base* partC = m_pInterface->TWG.Find_Entity(LineID)->TWG_TP.Find_Entity(TowerID)->TP_Part.Find_Entity(legC);
	if (partC != nullptr)
	{
		vector<double> v_id;
		//������
		for (int i = 0; i < partC->m_Nodes.size(); i++)
		{
			v_id.push_back(partC->m_Nodes[i].z);
		}
		//���C����
		tower->addPart(partC);
		// ��partB��z��Сֵ
		auto min_itA = min_element(v_id.begin(), v_id.end());		double min_valueA = *min_itA;
		//����͵���--������tower����ȥ��
		for (const auto& i : tower->m_Nodes)
		{
			if (abs(min_valueA - i.z) < 1e-5 && i.x > 0 && i.y > 0)
			{
				int id = i.m_idNode;
				// �ж��µ������Ƿ��Ѿ�������������
				auto it = std::find(tower->RestraintNode.begin(), tower->RestraintNode.end(), id);
				if (it == tower->RestraintNode.end())
				{
					// �����в����ڸ����ݣ��������
					tower->RestraintNode.push_back(id);
				}
			}
		}
	}

	//����D--��������
	Part_Base* partD = m_pInterface->TWG.Find_Entity(LineID)->TWG_TP.Find_Entity(TowerID)->TP_Part.Find_Entity(legD);
	if (partD != nullptr)
	{
		vector<double> v_id;
		//������
		for (int i = 0; i < partD->m_Nodes.size(); i++)
		{
			partD->m_Nodes[i].y *= -1;
			v_id.push_back(partD->m_Nodes[i].z);
		}
		//���D����
		tower->addPart(partD);
		auto min_itA = min_element(v_id.begin(), v_id.end());		double min_valueA = *min_itA;
		//����͵���--������tower����ȥ��
		for (const auto& i : tower->m_Nodes)
		{
			if (abs(min_valueA - i.z) < 1e-5 && i.y < 0 && i.x>0)
			{
				int id = i.m_idNode;
				// �ж��µ������Ƿ��Ѿ�������������
				auto it = std::find(tower->RestraintNode.begin(), tower->RestraintNode.end(), id);
				if (it == tower->RestraintNode.end())
				{
					// �����в����ڸ����ݣ��������
					tower->RestraintNode.push_back(id);
				}
			}
		}
		//��ԭ
		for (int i = 0; i < partD->m_Nodes.size(); i++)
		{
			partD->m_Nodes[i].y *= -1;
		}
	}
	
	tower->Show_VTKnode(m_pInterface->m_Renderer);
	tower->Show_VTKbeam(m_pInterface->m_Renderer);
	tower->showHangPoint(m_pInterface->m_Renderer);
	m_pInterface->m_Renderer->ResetCamera();
}
