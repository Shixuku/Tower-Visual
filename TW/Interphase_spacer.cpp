#include "Interphase_spacer.h"
#include"InterFace.h"
#include"TowerPart_Insulator.h"
Interphase_spacer::Interphase_spacer(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_pInterFace = dynamic_cast<InterFace*>(parent);
	Q_ASSERT(m_pInterFace != nullptr);
	connect(ui.btn_ok, &QPushButton::clicked, this, &Interphase_spacer::accept);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &Interphase_spacer::reject);
	connect(m_pInterFace, &InterFace::Msg_Select_Nodes, this, &Interphase_spacer::Get_PickedNode);
}

Interphase_spacer::~Interphase_spacer()
{}


void Interphase_spacer::Get_PickedNode()
{
	std::list<Node*> Nodes;
	m_pInterFace->Get_SelectedNode(Nodes);
	int k = 0;
	for (auto pNode : Nodes)
	{
		if (k == 0)
		{
			int idNode = pNode->m_idNode;
			ui.line_p1->setText(QString::number(idNode));
			++k;
			x1 = pNode->x;
			y1 = pNode->y;
			z1 = pNode->z;
			cout << x1 << " " << y1 << " " << z1 << "\n";
		}
		else if (k == 1)
		{
			int idNode = pNode->m_idNode;
			ui.line_p2->setText(QString::number(idNode));
			++k;
			x2 = pNode->x;
			y2 = pNode->y;
			z2 = pNode->z;
			cout << x2 << " " << y2 << " " << z2 << "\n";
		}
	}
}

void Interphase_spacer::Get_Data(TowerPart_Insulator* TP_insulator)
{
	TP_insulator->m_H = ui.line_H->text().toDouble();
	TP_insulator->m_type = ui.combo_type->currentIndex();
	TP_insulator->m_splits = ui.combo_count->currentText().toInt();
}


