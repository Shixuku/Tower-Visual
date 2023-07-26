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
	m_pTower = m_pCreat_loads->m_tower;//���ø����tower

	initialization();
	//��ť��Ӧ�¼�
	connect(ui.btn_ok, &QPushButton::clicked, this, [=]()
		{
			QString data = ui.lineEdit->text();
			if (data.isEmpty()) {  // �������Ϊ�գ�������Ϣ����ʾ�û�
				QMessageBox::information(this, "Tips", "��ѡ��ڵ㣡");
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
	ui.label_name->setText("���ƣ�" + m_pCreat_loads->m_str);//��ʾ�����ڵ�����
	this->setWindowFlags(Qt::WindowStaysOnTopHint);//���ô��������ϲ�
}

void ConcentrateForce::Get_Nodes()
{
	set<Node*> Nodes;
	m_pCreat_loads->m_pInterFace->Get_SelectedNode(Nodes);
	QString nodeInfo;
	QString existingText = ui.lineEdit->text();  // ��ȡlineEdit�����е��ı�����
	QStringList existingIds = existingText.split(" ", Qt::SkipEmptyParts);  // �����е�ID�ָ�Ϊ�ַ����б�
	for (auto pNode : Nodes)
	{
		int idNode = pNode->m_idNode;
		QString idStr = QString("%1").arg(idNode);

		// ���lineEdit���Ƿ��Ѱ�����ǰid
		if (!existingIds.contains(idStr))
		{
			if (!nodeInfo.isEmpty())
				nodeInfo += " ";

			nodeInfo += idStr;
			existingIds.append(idStr);
			m_ffNodes.push_back(pNode);//ֻҪ��ѡ�˶�������
		}
		
	}

	// �������µ�idʱ�Ž�����ӵ�lineEdit
	if (!nodeInfo.isEmpty())
	{
		if (QApplication::keyboardModifiers() == Qt::ShiftModifier)
		{
			// �����סShift������׷�������µ�id��lineEdit
			ui.lineEdit->setText(existingText.trimmed() + " " + nodeInfo.trimmed());
		}
		else
		{
			// ���û�а�סShift�������׷�ӵ�һ���µ�id��lineEdit
			ui.lineEdit->setText(existingText.trimmed() + " " + nodeInfo.trimmed().split(" ", Qt::SkipEmptyParts).first());
			// ����lineEdit�е��ı�
			ui.lineEdit->setText(nodeInfo);
		}
	}

}

void ConcentrateForce::draw()
{
	QString data = ui.lineEdit->text();  // ��ȡQLineEdit�ؼ���ֵ
	QStringList dataList = data.split(" ");  // ʹ�ö��ŷָ������ַ����ָ�ɶ�����ַ���

	// �������ַ��������
	for (auto i : m_ffNodes)//����ѡ���ĵ�
	{
		for (int j = 0; j < dataList.size(); j++)
		{
			int num = dataList.at(j).toInt();//dataList line�����е�
			if (num == i->m_idNode)
			{
				Node* n = i;
				//ȡ����-----��������Ҫ��
				double startTime = 0;//��ʼʱ�� 
				double endTime = 1;//����ʱ��
				//ȡx����
				double i_x = ui.line_x->text().toDouble();//��С
				if (i_x != 0)
				{
					int id = m_pTower->Load.size() + 1;//���������
					int idNode = i->m_idNode;//�ڵ���
					int direction = 0;//����
					m_pTower->Load.push_back(LoadForce(id, idNode, direction, i_x, startTime, endTime));
					//����������Ϊ�������汣������ʹ��
					LoadForce* loadForce = new LoadForce(id, idNode, direction, i_x, startTime, endTime);
					m_pCreat_loads->m_pInterFace->ME_LoadForce.Add_Entity(loadForce);

					//��
					if (i_x < 0)
					{
						m_pTower->DrawForceX(n, -1, m_pCreat_loads->m_pInterFace->m_Renderer_2);
					}
					if (i_x > 0)
					{
						m_pTower->DrawForceX(n, 1, m_pCreat_loads->m_pInterFace->m_Renderer_2);
					}
				}

				//ȡy����
				double i_y = ui.line_y->text().toDouble();
				if (i_y != 0)
				{
					int id = m_pTower->Load.size() + 1;
					int idNode = i->m_idNode;
					int direction = 1;
					m_pTower->Load.push_back(LoadForce(id, idNode, direction, i_y, startTime, endTime));
					//����������Ϊ�������汣������ʹ��
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

				//ȡz����
				double i_z = ui.line_z->text().toDouble();
				if (i_z != 0)
				{
					int id = m_pTower->Load.size() + 1;
					int idNode = i->m_idNode;
					int direction = 2;
					m_pTower->Load.push_back(LoadForce(id, idNode, direction, i_z, startTime, endTime));
					//����������Ϊ�������汣������ʹ��
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

	//���
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
