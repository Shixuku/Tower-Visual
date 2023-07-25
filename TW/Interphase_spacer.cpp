#include "Interphase_spacer.h"
#include"InterFace.h"
#include"TowerPart_Insulator.h"
#include<QMessageBox.h>
Interphase_spacer::Interphase_spacer(TowerData_CrossArm* CrossArm, TowerWireGroup* TowerWireGroup,QWidget* parent): QDialog(parent)
{
	ui.setupUi(this);
	m_pInterFace = dynamic_cast<InterFace*>(parent);
	Arm = CrossArm;
	towerWireGroup = TowerWireGroup;
	Q_ASSERT(m_pInterFace != nullptr);
	TP_insulator = new TowerPart_Insulator;
	
	connect(ui.btn_cancle, &QPushButton::clicked, this, [=]()
		{
			delete(TP_insulator);
			this->reject();
		});
	connect(m_pInterFace, &InterFace::Msg_Select_Nodes, this, &Interphase_spacer::Get_Nodes);

	connect(ui.btn_ok, &QPushButton::clicked, this, [=]()
		{
			if (Arm != nullptr)
			{
				Get_Data();

				ui.lineEdit->clear();
				ui.line_p1->clear();
				ui.line_p2->clear();
				Arm->addInsulator(TP_insulator);
				this->accept();
			}
			else if (Arm == nullptr)
			{
				Create_Nodes();
				Get_Data();
				ui.lineEdit->clear();
				ui.line_p1->clear();
				ui.line_p2->clear();
				this->accept();
			}
			
		});
	if (Arm != nullptr)
	{
	    ui.stackedWidget->setCurrentIndex(0);//设置默认page
	    connect(ui.rdb_I, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(0); });
	    connect(ui.rdb_V, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(1); });
	    connect(ui.radioButton, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(2); });
	}
	else if (Arm == nullptr)
	{
		ui.stackedWidget->setCurrentIndex(3);//设置默认page
		connect(ui.rdb_I, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(3); });
		connect(ui.rdb_V, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(4); });
		connect(ui.radioButton, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(5); });
	}
	ui.lineEdit_WireLogo->setText("1");
	ui.lineEdit_5->setText("30");
}

Interphase_spacer::~Interphase_spacer()
{
}

void Interphase_spacer::Get_Nodes()
{
	//取点编号
	std::list<Node*> Nodes;
	m_pInterFace->Get_SelectedNode(Nodes);
	int k = 0;
	for (auto pNode : Nodes)
	{
		int ret = ui.stackedWidget->currentIndex();
		if (ret == 0)
		{
			//悬垂的点
			int idNode = pNode->m_idNode;
			ui.lineEdit->setText(QString::number(idNode));
			TP_insulator->m_node = pNode;
		}
		else if (ret == 1)
		{
			if (k == 0)
			{
				//V型的点
				int idNode = pNode->m_idNode;
				ui.line_p1->setText(QString::number(idNode));
				++k;
				TP_insulator->m_node1 = pNode;
			}
			else if (k == 1)
			{
				int idNode = pNode->m_idNode;
				ui.line_p2->setText(QString::number(idNode));
				++k;
				TP_insulator->m_node2 = pNode;
			}
		}
		else if (ret == 2)
		{
			int idNode = pNode->m_idNode;
			int logo=ui.lineEdit_WireLogo->text().toDouble();
			ui.lineEdit_2->setText(QString::number(idNode));
			Arm->SuspensionNode.push_back(idNode);
			Arm->WireLoge.push_back(logo);
		}
	}
}

void Interphase_spacer::Create_Nodes()
{
	int ret = ui.stackedWidget->currentIndex();
	if (ret == 3)//悬垂（还需修改）
	{
		double x = ui.lineEditSusNodeX->text().toDouble();
		double y = ui.lineEditSusNodeY->text().toDouble();
		double z = ui.lineEditSusNodeZ->text().toDouble();
		m_Nodes.push_back(new Node(0, x, y, z, 0));
		TP_insulator->m_node = m_Nodes[0];
	}
	else if (ret == 4)//V型（需修改）
	{
		double x1 = ui.lineEditSusNodeX1->text().toDouble();
		double y1 = ui.lineEditSusNodeY1->text().toDouble();
		double z1 = ui.lineEditSusNodeZ1->text().toDouble();
		m_Nodes.push_back(new Node(0, x1, y1, z1, 0));
		TP_insulator->m_node1 = m_Nodes[0];
		double x2 = ui.lineEditSusNodeX1->text().toDouble();
		double y2 = ui.lineEditSusNodeY1->text().toDouble();
		double z2 = ui.lineEditSusNodeZ1->text().toDouble();
		m_Nodes.push_back(new Node(0, x2, y2, z2, 0));
		TP_insulator->m_node2 = m_Nodes[1];
	}
	else if (ret == 5)//耐张
	{
		double x1 = ui.lineEdit_3->text().toDouble();
		double y1 = ui.lineEdit_4->text().toDouble();
		double z1 = ui.lineEdit_5->text().toDouble();
		int SuspensionNodeId = towerWireGroup->Creat_Node(x1, y1, z1);
		int wireLogo= ui.lineEdit_WireLogo->text().toDouble();
		towerWireGroup->susPoint.push_back(SuspensionNodeId);
		towerWireGroup->SuspensionNode.push_back(SuspensionNodeId);
		towerWireGroup->realSuspoint.push_back(SuspensionNodeId);
		towerWireGroup->WireLogo.push_back(wireLogo);
	}
}

void Interphase_spacer::Get_Data()
{
	int index = ui.stackedWidget->currentIndex();
	//选择悬垂串但是没选点的情况
	if (index == 0 && TP_insulator->m_node == nullptr)
	{
		QMessageBox::information(this, "Tips", "请选择悬垂绝缘子挂点！");
	}
	//选择V型串但是点没选完的情况
	else if (index == 1 && (TP_insulator->m_node1 == nullptr || TP_insulator->m_node2 == nullptr))
	{
		QMessageBox::information(this, "Tips", "请选择V型绝缘子挂点！");
	}
	else if (towerWireGroup == nullptr) 
	{
		TP_insulator->m_H = ui.line_H->text().toDouble();
		TP_insulator->m_splits = ui.combo_count->currentText().toInt();
		TP_insulator->m_type = index + 1;
		if (TP_insulator->m_type == 1)
		{
			if (Arm != nullptr)
			{
				//对塔头的点进行循环
				for (auto& i : Arm->m_Nodes)
				{
					if (abs(TP_insulator->m_node->x - i.x) < 1e-1 && abs(TP_insulator->m_node->y - i.y) < 1e-1 &&
						abs(TP_insulator->m_node->z - i.z) < 1e-1)
					{
						//取数据
						TP_insulator->m_W1 = Arm->m_bodyButtomL;//取塔身宽
						TP_insulator->m_W2 = Arm->m_c2Wb;//取横担宽
						TP_insulator->m_L2 = TP_insulator->m_W1 / 2 + Arm->Get_SumL();//取横担长
					}

				}
			}

		}
		TP_insulator->wireLogo = ui.lineEdit_WireLogo->text().toInt();
		TP_insulator->Create_Mesh();
		TP_insulator->Show_VTKnode(m_pInterFace->m_Renderer);
		TP_insulator->Show_VTKtruss(m_pInterFace->m_Renderer);
		TP_insulator->Show_VTKbeam(m_pInterFace->m_Renderer);
		m_pInterFace->towerPartInsulator.Add_Entity(TP_insulator);
	}
	else if (towerWireGroup != nullptr)
	{
		TP_insulator->m_H = ui.line_H->text().toDouble();
		TP_insulator->m_splits = ui.combo_count->currentText().toInt();
		TP_insulator->m_type = index + 1-3;
		if (TP_insulator->m_type == 1)
		{
			if (Arm == nullptr)
			{
				TP_insulator->m_W1 = 10;//取塔身宽
				TP_insulator->m_W2 = 10;//取横担宽
				TP_insulator->m_L2 = 10;//取横担长
			}
		}
		TP_insulator->wireLogo = ui.lineEdit_WireLogo->text().toInt();
		TP_insulator->Create_Mesh();
		towerWireGroup->addSpacer(TP_insulator);
		towerWireGroup->Show_VTKnode(m_pInterFace->m_Renderer_2);
		towerWireGroup->Show_VTKtruss(m_pInterFace->m_Renderer_2);
		towerWireGroup->Show_VTKbeam(m_pInterFace->m_Renderer_2);
		towerWireGroup->VectorToMap();
	}
}

TowerPart_Insulator* Interphase_spacer::ReturnInsulator()
{
	return TP_insulator;
}


