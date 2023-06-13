#include "Interphase_spacer.h"
#include"InterFace.h"
#include"TowerPart_Insulator.h"
Interphase_spacer::Interphase_spacer(QWidget* parent): QDialog(parent)
{
	ui.setupUi(this);
	m_pInterFace = dynamic_cast<InterFace*>(parent);
	Q_ASSERT(m_pInterFace != nullptr);

	connect(ui.btn_cancle, &QPushButton::clicked, this, &Interphase_spacer::reject);
	connect(m_pInterFace, &InterFace::Msg_Select_Nodes, this, &Interphase_spacer::Get_Nodes);

	connect(ui.btn_ok, &QPushButton::clicked, this, [=]()
		{
			Get_Data();
			TP_insulator->Create_Mesh();
			TP_insulator->Show_VTKnode(m_pInterFace->m_Renderer_2);
			TP_insulator->Show_VTKtruss(m_pInterFace->m_Renderer_2);
			TP_insulator->Show_VTKbeam(m_pInterFace->m_Renderer_2);
			this->accept();
		});
	ui.stackedWidget->setCurrentIndex(0);//设置默认page
	connect(ui.rdb_I, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(0); });
	connect(ui.rdb_V, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(1); });
}

Interphase_spacer::~Interphase_spacer()
{}

void Interphase_spacer::Get_Nodes()
{
	//取点编号
	TP_insulator = new TowerPart_Insulator;
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
			//cout << TP_insulator->m_node->x << "\n";
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

	}


}

void Interphase_spacer::Get_Data()
{
	TP_insulator->m_H = ui.line_H->text().toDouble();
	TP_insulator->m_splits = ui.combo_count->currentText().toInt();
	TP_insulator->m_type = ui.stackedWidget->currentIndex() + 1;
}


