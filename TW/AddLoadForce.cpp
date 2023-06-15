#include "AddLoadForce.h"
#include"InterFace.h"
AddLoadForce::AddLoadForce(Tower* tower, QWidget* parent): QDialog(parent)
{
	ui.setupUi(this);
	m_pInterFace = dynamic_cast<InterFace*>(parent);
	Q_ASSERT(m_pInterFace != nullptr);
	m_tower = tower;
	connect(ui.btn_cancle, &QPushButton::clicked, this, &AddLoadForce::reject);
	connect(m_pInterFace, &InterFace::Msg_Select_Nodes, this, &AddLoadForce::Get_Nodes);
	connect(ui.btn_ok, &QPushButton::clicked, this, [=]()
		{
			Get_Data();
			m_tower->Show_Load(m_pInterFace->m_Renderer_2);
			m_pInterFace->ShowLoadactor(m_tower);
			this->accept();
		});
}

AddLoadForce::~AddLoadForce()
{}

void AddLoadForce::Get_Nodes()
{
	std::list<Node*> Nodes;
	m_pInterFace->Get_SelectedNode(Nodes);

	QString existingText = ui.lineEdit->text();
	QString newText = existingText;

	for (auto pNode : Nodes)
	{
		int idNode = pNode->m_idNode;
		QString nodeText = QString::number(idNode);

		// 检查节点是否已经存在于文本中
		if (!existingText.contains(nodeText))
		{
			if (!newText.isEmpty()) 
			{
				newText += ";"; // 添加一个空格分隔符
			}
			newText += nodeText;
		}
	}

	ui.lineEdit->setText(newText);
}
void AddLoadForce::Get_Data()
{
	QString lineEditText = ui.lineEdit->text();
	QStringList nodeTextList = lineEditText.split(";", QString::SkipEmptyParts);

	QList<int> idNodes;
	for (const QString& nodeText : nodeTextList) {
		int idNode = nodeText.toInt();
		idNodes.append(idNode);
	}
	for (int i = 0; i < idNodes.size(); i++)
	{

		double i_x = ui.lineEdit_2->text().toDouble();
		if (i_x != 0)
		{
			int id = m_tower->Load.size() + 1;
			int idNode = idNodes[i];
			int direction = 0;
			m_tower->Load.push_back(LoadForce(id, idNode, direction, i_x, 0,1));
		}
		double i_y = ui.lineEdit_3->text().toDouble();
		if (i_y != 0)
		{
			int id = m_tower->Load.size() + 1;
			int idNode = idNodes[i];
			int direction = 1;
			m_tower->Load.push_back(LoadForce(id, idNode, direction, i_y, 0, 1));
		}
		double i_z = ui.lineEdit_4->text().toDouble();
		if (i_z != 0)
		{
			int id = m_tower->Load.size() + 1;
			int idNode = idNodes[i];
			int direction = 1;
			m_tower->Load.push_back(LoadForce(id, idNode, direction, i_z, 0, 1));
		}
	}

}
