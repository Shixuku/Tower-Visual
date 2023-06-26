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
	connect(m_pCreat_loads->m_InterFace, &InterFace::Msg_Select_Nodes, this, &ConcentrateForce::Get_Nodes);

}

ConcentrateForce::~ConcentrateForce()
{}
void ConcentrateForce::initialization()
{
	ff = new test_pic_focusforce;
	ui.label_name->setText("���ƣ�" + m_pCreat_loads->m_str);//��ʾ�����ڵ�����
	this->setWindowFlags(Qt::WindowStaysOnTopHint);//���ô��������ϲ�
}

void ConcentrateForce::Get_Nodes()
{
	set<Node*> Nodes;
	m_pCreat_loads->m_InterFace->Get_SelectedNode(Nodes);
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
		}
		m_ffNodes.push_back(pNode);
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
