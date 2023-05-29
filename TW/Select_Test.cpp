#include "Select_Test.h"
#include"InterFace.h"
Select_Test::Select_Test(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_pInterFace = dynamic_cast<InterFace*>(parent);
	Q_ASSERT(m_pInterFace != nullptr);
	connect(m_pInterFace, &InterFace::Msg_Select_Nodes, this, &Select_Test::Get_PickedNode);
}

Select_Test::~Select_Test()
{}

void Select_Test::Get_PickedNode()
{
	std::list<Node*> Nodes;
	m_pInterFace->Get_SelectedNode(Nodes);
	int k = 0;
	//	std::list<Node*>::iterator it = Nodes.begin(); //迭代器能指向后续

	for (auto pNode : Nodes)
	{
		if (k == 0)
		{
			int idNode = pNode->m_idNode;
			ui.lineEdit_PT1->setText(QString::number(idNode));
			++k;
			x1 = pNode->x;
			y1 = pNode->y;
			z1 = pNode->z;
			cout << x1 << " " << y1 << " " << z1;
		}
		else if (k == 1)
		{
			int idNode = pNode->m_idNode;
			ui.lineEdit_PT2->setText(QString::number(idNode));
			++k;
			x2 = pNode->x;
			y2 = pNode->y;
			z2 = pNode->z;
			cout << x2 << " " << y2 << " " << z2 << "\n";
		}
	}
}

void Select_Test::Btn_OK()
{
}
