#include "Create_Constraint.h"
#include"InterFace.h"

Create_Constraint::Create_Constraint(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_pInterFace = dynamic_cast<InterFace*>(parent);
	Q_ASSERT(m_pInterFace != nullptr);
	//按钮响应事件
	connect(ui.OK_Btn, &QPushButton::clicked, this, [=]()
		{
			Draw_Con();
			this->accept();
		});

}

Create_Constraint::~Create_Constraint()
{}

void Create_Constraint::Draw_Con()
{
	
	if (ui.radioButton->isChecked())//铰接
	{

		for (int i = 0; i < m_pInterFace->Con_Nodes.size(); i++)
		{
			Con->Draw_hinge_joint(m_pInterFace->Con_Nodes[i].x, m_pInterFace->Con_Nodes[i].y, m_pInterFace->Con_Nodes[i].z, m_pInterFace->m_Renderer_2);
		}
	}
	else if (ui.radioButton_2->isChecked())//固定约束
	{
		
		for (int i = 0; i < m_pInterFace->Con_Nodes.size(); i++)
		{
			Con->Draw_fixed_Constrained(m_pInterFace->Con_Nodes[i].x, m_pInterFace->Con_Nodes[i].y, m_pInterFace->Con_Nodes[i].z, m_pInterFace->m_Renderer_2);
		}
		
	}
}


