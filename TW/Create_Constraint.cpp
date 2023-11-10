#include "Create_Constraint.h"
#include"InterFace.h"

Create_Constraint::Create_Constraint(Instance* instance, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_pInterFace = dynamic_cast<InterFace*>(parent);
	Q_ASSERT(m_pInterFace != nullptr);
	m_pInstance = instance;
	//��ť��Ӧ�¼�
	connect(ui.OK_Btn, &QPushButton::clicked, this, &Create_Constraint::Draw_Con1);
	connect(ui.Cancel_Btn, &QPushButton::clicked, this, &Create_Constraint::close);
	connect(m_pInterFace, &InterFace::Msg_Select_Nodes, this, &Create_Constraint::Insert_Data);
	this->setWindowTitle("����Լ��");
}

Create_Constraint::~Create_Constraint()
{}

void Create_Constraint::Draw_Con1()
{
	QStringList allIds = ui.line_idNode->text().split(" ", Qt::SkipEmptyParts);
	// ʹ��������ʾ��ID��ִ����Ӧ�Ĳ���
	for (const QString& id : allIds)
	{
		int nodeId = id.toInt();//�ڵ���
		int conSIze = Con_Nodes.size();//��ȡ��size
		// ��ȡID��Ӧ��������Ϣ
		if (idMap.contains(nodeId))
		{
			int index = idMap[nodeId];
			const QVector<double>& coordinates = nodeCoordinates[index];
			double x = coordinates[0];
			double y = coordinates[1];
			double z = coordinates[2];
			Con_Nodes.push_back(Node(1,x, y, z, 0));
		}
		if (ui.radioButton->isChecked())//�½�
		{
			for (int i = conSIze; i < Con_Nodes.size(); i++)
			{
				m_pInstance->Draw_hinge_joint(Con_Nodes[i].x, Con_Nodes[i].y, Con_Nodes[i].z, m_pInterFace->m_Renderer);
				for (int j = 0; j < 3; j++)
				{
					int id = m_pInstance->m_Constraint.size() + 1;//Լ�����
					m_pInstance->m_Constraint.push_back(ParameterConstraint(id, nodeId, j));
				}
				
			}

		}
		else if (ui.radioButton_2->isChecked())//�̶�Լ��
		{

			for (int i = conSIze; i < Con_Nodes.size(); i++)
			{
				m_pInstance->Draw_fixed_Constrained(Con_Nodes[i].x, Con_Nodes[i].y, Con_Nodes[i].z, m_pInterFace->m_CurrentRenderer);
				for (int j = 0; j < 6; j++)
				{
					int id = m_pInstance->m_Constraint.size() + 25;//Լ�����
					m_pInstance->m_Constraint.push_back(ParameterConstraint(id, nodeId, j));
				}
			}

		}
	}

	m_pInterFace->Close_Point();
	this->accept();
}

void Create_Constraint::Insert_Data()
{
	set<Node*> Nodes;
	m_pInterFace->Get_SelectedNode(Nodes);
	QString existingText = ui.line_idNode->text();
	QStringList existingIds = existingText.split(" ", Qt::SkipEmptyParts);

	for (auto pNode : Nodes)
	{
		int idNode = pNode->m_idNode;
		QString idStr = QString("%1").arg(idNode);

		if (!existingIds.contains(idStr))
		{
			QVector<double> coordinates = { pNode->x, pNode->y, pNode->z };
			nodeCoordinates[index] = coordinates; // �洢������Ϣ
			idMap[idNode] = index; // �洢ID��������ӳ���ϵ

			if (!existingText.isEmpty())
				existingText += " ";

			existingText += idStr;
			existingIds.append(idStr);

			index++;
		}
	}

	if (!existingText.isEmpty())
	{
		if (QApplication::keyboardModifiers() == Qt::ShiftModifier)
		{
			ui.line_idNode->setText(existingText.trimmed()); // ׷�������µ�id��lineEdit
		}
		else
		{
			if (existingIds.size() > 1)
			{
				QString clickedId = existingIds.last(); // ��ȡ���һ������Ľڵ�ID
				existingText = clickedId; // ֻ�������һ������Ľڵ�ID
			}
			ui.line_idNode->setText(existingText);
		}
	}

}


