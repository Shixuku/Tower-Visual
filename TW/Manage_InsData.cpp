#include "Manage_InsData.h"
#pragma execution_character_set("utf-8")
#include<QMessageBox.h>
Manage_InsData::Manage_InsData(InterFace* InterFace, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_InterFace = InterFace;
	Set_headertext();
	connect(ui.btn_ok, &QPushButton::clicked, this, &Manage_InsData::accept);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &Manage_InsData::reject);
	connect(ui.btn_modify, &QPushButton::clicked, this, &Manage_InsData::Modify_Data);
	connect(ui.btn_delete, &QPushButton::clicked, this, &Manage_InsData::Delete_Data);
}

void Manage_InsData::Set_headertext()
{
	QStringList headertext;//��ͷ
	headertext << "����" << "�ڵ�����" << "��Ԫ����" << "˵��";
	ui.tableWidget->setColumnCount(headertext.count());
	ui.tableWidget->setHorizontalHeaderLabels(headertext);
	ui.tableWidget->verticalHeader()->setVisible(false);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

	int T_tower = m_InterFace->ui.treeWidget->topLevelItem(3)->childCount();//��ʵ������
	
	ui.tableWidget->setRowCount(T_tower);//��������

	QVector<Tower*> ve_tower;//!
	m_InterFace->TP.Get_Array(ve_tower, true);//!

	for (int i = 0; i < T_tower; i++)
	{
		Tower* tower = ve_tower[i];//!
		//��������//���ýڵ㵥Ԫ����
		int E_num = tower->m_Elements_beams.size() + tower->m_Elements_Trusses.size();
		QString name = m_InterFace->ui.treeWidget->topLevelItem(3)->child(i)->text(0);
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(name));
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(tower->m_Nodes.size())));
		ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(E_num)));
		ui.tableWidget->setItem(i, 3, new QTableWidgetItem("����"));
		for (int j = 0; j < 4; j++)//��ͷ;
		{
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}

}

void Manage_InsData::Modify_Data()
{
	int Index = ui.tableWidget->currentRow();//��ȡ�����������--currentRow()��0��ʼ
	if (ui.tableWidget->item(Index, 1) == nullptr || ui.tableWidget->item(Index, 1)->text().isEmpty())
	{
		QMessageBox::information(this, "Tips", "��ѡ��Ҫ�޸ĵ����Ȳ�����");
	}
	else
	{
		Tower_Assembly* T_As = m_InterFace->tower_assembles[Index];
		int ret = T_As->exec();
		if (ret == QDialog::Accepted)
		{
			//����
			QTreeWidgetItem* parent = m_InterFace->ui.treeWidget->topLevelItem(3);
			QTreeWidgetItem* childItem = m_InterFace->ui.treeWidget->topLevelItem(3)->child(Index);
			Tower* tw = new Tower;
			for (auto& i : T_As->m_ArryLeg)//����
			{
				tw->addPart(m_InterFace->TP_leg.Find_Entity(i));
			}
			for (auto& i : T_As->m_ArryBody)//�����
			{
				tw->addPart(m_InterFace->TP_body.Find_Entity(i));
			}
			for (auto& i : T_As->m_ArryHead)//��ͷ
			{
				tw->addPart(m_InterFace->TP_CrossArm.Find_Entity(i));
			}
			tw->Check();
			childItem->setText(0, T_As->Get_name());
			tw->Item = childItem;
			tw->m_id = Index + 1;

			double x = 0; double y = 0; double z = 0; double angle = 0;
			T_As->Get_Movedata(x, y, z, angle);//�õ�ƽ����ת�Ƕ�
			tw->rotation(angle);//��ת
			tw->move(x, y, z);//ƽ��

			tw->Show_VTKtruss(m_InterFace->m_Renderer);
			tw->Show_VTKbeam(m_InterFace->m_Renderer);
			tw->Show_VTKnode(m_InterFace->m_Renderer);

			m_InterFace->TP.Add_Entity(tw);

			m_InterFace->HiddeAll();
			for (auto& i : m_InterFace->TP)
			{
				if (i.second != nullptr)
				{
					i.second->m_BeamActor->VisibilityOn();
					i.second->m_TrussActor->VisibilityOn();
					i.second->Node_actor->VisibilityOn();
				}
			}

			//���ý������ݱ仯
			ui.tableWidget->setItem(Index, 0, new QTableWidgetItem(T_As->Get_name()));
			//���ýڵ㵥Ԫ����
			ui.tableWidget->setItem(Index, 1, new QTableWidgetItem(QString::number(tw->m_Nodes.size())));
			int E_num = tw->m_Elements_beams.size() + tw->m_Elements_Trusses.size();
			ui.tableWidget->setItem(Index, 2, new QTableWidgetItem(QString::number(E_num)));
			ui.tableWidget->setItem(Index, 3, new QTableWidgetItem("����"));
			for (int j = 0; j < 4; j++)//��ͷ;
			{
				ui.tableWidget->item(Index, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			}
			m_InterFace->ResetCamera();
		}
	}
}

void Manage_InsData::Delete_Data()
{
	 int Index = ui.tableWidget->currentRow();//��ȡ�����������--currentRow()��0��ʼ
	if (ui.tableWidget->item(Index, 1) == nullptr || ui.tableWidget->item(Index, 1)->text().isEmpty())
	{
		//��ʾ
		QMessageBox::information(this, "Tips", "��ѡ��Ҫɾ����ʵ����");
	}
	else
	{
		QString name = ui.tableWidget->item(Index, 0)->text();
		//ȷ���Ƿ�ɾ��
		QMessageBox::StandardButton btn;
		btn = QMessageBox::question(this, "��ʾ", "ȷ��Ҫɾ�� " + name + " ��������",
			QMessageBox::Yes | QMessageBox::No);
		if (btn == QMessageBox::Yes && m_InterFace->TP.Delete_Entity(Index + 1))
		{
			ui.tableWidget->removeRow(Index);//ɾ������е���һ��
			//ɾ��m_InterFace�ӽڵ�
			QTreeWidgetItem* childItem = m_InterFace->ui.treeWidget->topLevelItem(3)->child(Index);
			delete(childItem);
			//��Ҫɾ�������ָ��
			std::vector<Tower_Assembly*>::iterator it = m_InterFace->tower_assembles.begin();
			it = m_InterFace->tower_assembles.begin() + Index;
			m_InterFace->tower_assembles.erase(it);
		}
	}
}

Manage_InsData::~Manage_InsData()
{}
