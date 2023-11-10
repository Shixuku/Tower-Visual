#include "Create_Constraint.h"
#include"InterFace.h"

Create_Constraint::Create_Constraint(Instance* instance, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_pInterFace = dynamic_cast<InterFace*>(parent);
	Q_ASSERT(m_pInterFace != nullptr);
	m_pInstance = instance;
	//按钮响应事件
	connect(ui.OK_Btn, &QPushButton::clicked, this, &Create_Constraint::Draw_Con1);
	connect(ui.Cancel_Btn, &QPushButton::clicked, this, &Create_Constraint::close);
	connect(m_pInterFace, &InterFace::Msg_Select_Nodes, this, &Create_Constraint::Insert_Data);
	this->setWindowTitle("创建约束");
}

Create_Constraint::~Create_Constraint()
{}

void Create_Constraint::Draw_Con1()
{
	QStringList allIds = ui.line_idNode->text().split(" ", Qt::SkipEmptyParts);
	// 使用所有显示的ID，执行相应的操作
	for (const QString& id : allIds)
	{
		int nodeId = id.toInt();//节点编号
		int conSIze = Con_Nodes.size();//先取到size
		// 获取ID对应的坐标信息
		if (idMap.contains(nodeId))
		{
			int index = idMap[nodeId];
			const QVector<double>& coordinates = nodeCoordinates[index];
			double x = coordinates[0];
			double y = coordinates[1];
			double z = coordinates[2];
			Con_Nodes.push_back(Node(1,x, y, z, 0));
		}
		if (ui.radioButton->isChecked())//铰接
		{
			for (int i = conSIze; i < Con_Nodes.size(); i++)
			{
				m_pInstance->Draw_hinge_joint(Con_Nodes[i].x, Con_Nodes[i].y, Con_Nodes[i].z, m_pInterFace->m_Renderer);
				for (int j = 0; j < 3; j++)
				{
					int id = m_pInstance->m_Constraint.size() + 1;//约束编号
					m_pInstance->m_Constraint.push_back(ParameterConstraint(id, nodeId, j));
				}
				
			}

		}
		else if (ui.radioButton_2->isChecked())//固定约束
		{

			for (int i = conSIze; i < Con_Nodes.size(); i++)
			{
				m_pInstance->Draw_fixed_Constrained(Con_Nodes[i].x, Con_Nodes[i].y, Con_Nodes[i].z, m_pInterFace->m_CurrentRenderer);
				for (int j = 0; j < 6; j++)
				{
					int id = m_pInstance->m_Constraint.size() + 25;//约束编号
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
			nodeCoordinates[index] = coordinates; // 存储坐标信息
			idMap[idNode] = index; // 存储ID与索引的映射关系

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
			ui.line_idNode->setText(existingText.trimmed()); // 追加所有新的id到lineEdit
		}
		else
		{
			if (existingIds.size() > 1)
			{
				QString clickedId = existingIds.last(); // 获取最后一个点击的节点ID
				existingText = clickedId; // 只保留最后一个点击的节点ID
			}
			ui.line_idNode->setText(existingText);
		}
	}

}


