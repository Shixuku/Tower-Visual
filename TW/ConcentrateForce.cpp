#include "ConcentrateForce.h"
#include"InterFace.h"
#include"Creat_Loads.h"
#include <QMessageBox>
#include"Tower.h"
#pragma execution_character_set("utf-8")

ConcentrateForce::ConcentrateForce(Creat_Loads* creat_loads, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_pCreat_loads = creat_loads;
	m_pTower = m_pCreat_loads->m_tower;//调用父类的tower

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
	connect(m_pCreat_loads->m_pInterFace, &InterFace::Msg_Select_Nodes, this, &ConcentrateForce::Get_Nodes);

}

ConcentrateForce::~ConcentrateForce()
{

}
void ConcentrateForce::initialization()
{
	ui.label_name->setText("名称：" + m_pCreat_loads->m_str);//显示父窗口的名称
	this->setWindowFlags(Qt::WindowStaysOnTopHint);//设置窗口在最上层
}

void ConcentrateForce::Get_Nodes()
{
	set<Node*> Nodes;
	m_pCreat_loads->m_pInterFace->Get_SelectedNode(Nodes);
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
			m_ffNodes.push_back(pNode);//只要点选了都保存了
		}
		
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
	for (auto i : m_ffNodes)//所有选到的点
	{
		for (int j = 0; j < dataList.size(); j++)
		{
			int num = dataList.at(j).toInt();//dataList line里面有的
			if (num == i->m_idNode)
			{
				Node* n = i;
				//取数据-----分析步需要改
				double startTime = 0;//开始时间 
				double endTime = 1;//结束时间
				//取x方向
				double i_x = ui.line_x->text().toDouble();//大小
				if (i_x != 0)
				{
					int id = m_pTower->Load.size() + 1;//集中力编号
					int idNode = i->m_idNode;//节点编号
					int direction = 0;//方向
					m_pTower->Load.push_back(LoadForce(id, idNode, direction, i_x, startTime, endTime));
					//以下两行是为了主界面保存数据使用
					LoadForce* loadForce = new LoadForce(id, idNode, direction, i_x, startTime, endTime);
					m_pCreat_loads->m_pInterFace->ME_LoadForce.Add_Entity(loadForce);

					//画
					if (i_x < 0)
					{
						m_pTower->DrawForceX(n, -1, m_pCreat_loads->m_pInterFace->m_Renderer_2);
					}
					if (i_x > 0)
					{
						m_pTower->DrawForceX(n, 1, m_pCreat_loads->m_pInterFace->m_Renderer_2);
					}
				}

				//取y方向
				double i_y = ui.line_y->text().toDouble();
				if (i_y != 0)
				{
					int id = m_pTower->Load.size() + 1;
					int idNode = i->m_idNode;
					int direction = 1;
					m_pTower->Load.push_back(LoadForce(id, idNode, direction, i_y, startTime, endTime));
					//以下两行是为了主界面保存数据使用
					LoadForce* loadForce = new LoadForce(id, idNode, direction, i_x, startTime, endTime);
					m_pCreat_loads->m_pInterFace->ME_LoadForce.Add_Entity(loadForce);
					if (i_y < 0)
					{
						m_pTower->DrawForceY(n, -1, m_pCreat_loads->m_pInterFace->m_Renderer_2);
					}
					if (i_y > 0)
					{
						m_pTower->DrawForceY(n, 1, m_pCreat_loads->m_pInterFace->m_Renderer_2);
					}
				}

				//取z方向
				double i_z = ui.line_z->text().toDouble();
				if (i_z != 0)
				{
					int id = m_pTower->Load.size() + 1;
					int idNode = i->m_idNode;
					int direction = 2;
					m_pTower->Load.push_back(LoadForce(id, idNode, direction, i_z, startTime, endTime));
					//以下两行是为了主界面保存数据使用
					LoadForce* loadForce = new LoadForce(id, idNode, direction, i_x, startTime, endTime);
					m_pCreat_loads->m_pInterFace->ME_LoadForce.Add_Entity(loadForce);
					if (i_z < 0)
					{
						m_pTower->DrawForceZ(n, -1, m_pCreat_loads->m_pInterFace->m_Renderer_2);
					}
					if (i_z > 0)
					{
						m_pTower->DrawForceZ(n, 1, m_pCreat_loads->m_pInterFace->m_Renderer_2);
					}
				}
			}


		}
	}

	//输出
	for (int i = 0; i < m_pTower->Load.size(); i++)
	{
		cout << m_pTower->Load[i].id_force << " " << m_pTower->Load[i].id_node << "  " << m_pTower->Load[i].DirectionForce
			<< "  " << m_pTower->Load[i].Force << "  " << m_pTower->Load[i].StartTime << "  " << m_pTower->Load[i].EndTime
			<< "\n";
	}
	this->accept();
	m_ffNodes.clear();

	m_pCreat_loads->m_pInterFace->Close_Point();
}
