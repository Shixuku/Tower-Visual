#include "Interphase_spacer.h"
#include"InterFace.h"
#include"TowerPart_Insulator.h"
#include<QMessageBox.h>
Interphase_spacer::Interphase_spacer(TowerData_CrossArm* CrossArm,QWidget* parent): QDialog(parent)
{
	ui.setupUi(this);
	m_pInterFace = dynamic_cast<InterFace*>(parent);
	Arm = CrossArm;
	Q_ASSERT(m_pInterFace != nullptr);
	TP_insulator = new TowerPart_Insulator;
	
	connect(ui.btn_cancle, &QPushButton::clicked, this, &Interphase_spacer::reject);
	connect(m_pInterFace, &InterFace::Msg_Select_Nodes, this, &Interphase_spacer::Get_Nodes);

	connect(ui.btn_ok, &QPushButton::clicked, this, [=]()
		{
			Get_Data();
			
			ui.lineEdit->clear();
			ui.line_p1->clear();
			ui.line_p2->clear();
		});
	ui.stackedWidget->setCurrentIndex(0);//����Ĭ��page
	connect(ui.rdb_I, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(0); });
	connect(ui.rdb_V, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(1); });

}

Interphase_spacer::~Interphase_spacer()
{
}

void Interphase_spacer::Get_Nodes()
{
	//ȡ����
	std::list<Node*> Nodes;
	m_pInterFace->Get_SelectedNode(Nodes);
	int k = 0;
	for (auto pNode : Nodes)
	{
		int ret = ui.stackedWidget->currentIndex();
		if (ret == 0)
		{
			//�����ĵ�
			int idNode = pNode->m_idNode;
			ui.lineEdit->setText(QString::number(idNode));
			TP_insulator->m_node = pNode;
		}
		else if (ret == 1)
		{
			if (k == 0)
			{
				//V�͵ĵ�
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
	}

}

void Interphase_spacer::Get_Data()
{
	int index = ui.stackedWidget->currentIndex();
	//ѡ������������ûѡ������
	if (index == 0 && TP_insulator->m_node == nullptr)
	{
		QMessageBox::information(this, "Tips", "��ѡ��������Ե�ӹҵ㣡");
	}
	//ѡ��V�ʹ����ǵ�ûѡ������
	else if (index == 1 && (TP_insulator->m_node1 == nullptr || TP_insulator->m_node2 == nullptr))
	{
		QMessageBox::information(this, "Tips", "��ѡ��V�;�Ե�ӹҵ㣡");
	}
	else
	{
		TP_insulator->m_H = ui.line_H->text().toDouble();
		TP_insulator->m_splits = ui.combo_count->currentText().toInt();
		TP_insulator->m_type = index + 1;
		if (TP_insulator->m_type == 1)
		{

			//����ͷ�ĵ����ѭ��
			for (auto& i : Arm->m_Nodes)
			{
				if (abs(TP_insulator->m_node->x - i.x) < 1e-1 && abs(TP_insulator->m_node->y - i.y) < 1e-1 &&
					abs(TP_insulator->m_node->z - i.z) < 1e-1)
				{
					//ȡ����
					TP_insulator->m_W1 = Arm->m_bodyButtomL;//ȡ�����
					TP_insulator->m_W2 = Arm->m_c2Wb;//ȡ�ᵣ��
					TP_insulator->m_L2 = TP_insulator->m_W1 / 2 + Arm->Get_SumL();//ȡ�ᵣ��
				}

			}
		}
		TP_insulator->ArmId = Arm->m_id;
		TP_insulator->Create_Mesh();
		TP_insulator->Show_VTKnode(m_pInterFace->m_Renderer);
		TP_insulator->Show_VTKtruss(m_pInterFace->m_Renderer);
		TP_insulator->Show_VTKbeam(m_pInterFace->m_Renderer);
		m_pInterFace->towerPartInsulator.Add_Entity(TP_insulator);
	}
}


