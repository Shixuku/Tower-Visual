#include "ConcentrateForce.h"
#include"InterFace.h"
#include"test_pic_focusforce.h"
#include"Creat_Loads.h"
#include <QMessageBox>
#pragma execution_character_set("utf-8")

ConcentrateForce::ConcentrateForce(Creat_Loads* creat_loads, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_pCreat_loads = creat_loads;
	initialization();
	//按钮响应事件
	connect(ui.btn_ok, &QPushButton::clicked, this, [=]()
		{
			QString data = ui.lineEdit->text();
			if (data.isEmpty()) {  // 如果内容为空，弹出消息框提示用户
				QMessageBox::information(this, "Tips", "请选择节点！");
			}
			else
			{
				draw();
			}
		});
	connect(ui.btn_cancle, &QPushButton::clicked, this, &ConcentrateForce::reject);
	connect(m_pCreat_loads->m_InterFace, &InterFace::Msg_Select_Nodes, this, &ConcentrateForce::Get_Nodes);

}

ConcentrateForce::~ConcentrateForce()
{}
void ConcentrateForce::initialization()
{
	ff = new test_pic_focusforce;
	ui.label_name->setText("名称：" + m_pCreat_loads->m_str);//显示父窗口的名称
	this->setWindowFlags(Qt::WindowStaysOnTopHint);//设置窗口在最上层
}

void ConcentrateForce::Get_Nodes()
{
	set<Node*> Nodes;
	m_pCreat_loads->m_InterFace->Get_SelectedNode(Nodes);
	QString nodeInfo;
	QString existingText = ui.lineEdit->text();  // 获取lineEdit中已有的文本内容
	QStringList existingIds = existingText.split(" ", Qt::SkipEmptyParts);  // 将已有的ID分割为字符串列表
	for (auto pNode : Nodes)
	{
		int idNode = pNode->m_idNode;
		QString idStr = QString("%1").arg(idNode);

		// 检查lineEdit中是否已包含当前id
		if (!existingIds.contains(idStr))
		{
			if (!nodeInfo.isEmpty())
				nodeInfo += " ";

			nodeInfo += idStr;
			existingIds.append(idStr);
		}
		m_ffNodes.push_back(pNode);
	}

	// 仅在有新的id时才将其添加到lineEdit
	if (!nodeInfo.isEmpty())
	{
		if (QApplication::keyboardModifiers() == Qt::ShiftModifier)
		{
			// 如果按住Shift键，则追加所有新的id到lineEdit
			ui.lineEdit->setText(existingText.trimmed() + " " + nodeInfo.trimmed());
		}
		else
		{
			// 如果没有按住Shift键，则仅追加第一个新的id到lineEdit
			ui.lineEdit->setText(existingText.trimmed() + " " + nodeInfo.trimmed().split(" ", Qt::SkipEmptyParts).first());
			// 更新lineEdit中的文本
			ui.lineEdit->setText(nodeInfo);
		}
	}

}

void ConcentrateForce::draw()
{
	QString data = ui.lineEdit->text();  // 读取QLineEdit控件的值
	QStringList dataList = data.split(" ");  // 使用逗号分隔符将字符串分割成多个子字符串

	// 遍历子字符串并输出
	for (auto i : m_ffNodes)
	{
		int id;
		for (int j = 0; j < dataList.size(); j++)
		{
			id = dataList.at(j).toInt();
			if (id == i->m_idNode)
			{
				ff->m_x = ui.line_x->text().toInt();
				ff->m_y = ui.line_y->text().toInt();
				ff->m_z = ui.line_z->text().toInt();
				Node* n = i;
				ff->m_node = n;
				ff->creat(m_pCreat_loads->m_InterFace->m_Renderer_2);

			}
		}
	}
	this->accept();
	m_ffNodes.clear();
	m_pCreat_loads->m_InterFace->concentrateforce.push_back(this);
	m_pCreat_loads->m_InterFace->Close_Point();

}
