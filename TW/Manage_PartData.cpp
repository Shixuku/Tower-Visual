#include "Manage_PartData.h"
#pragma execution_character_set("utf-8")
#include<QMessageBox.h>
Manage_PartData::Manage_PartData(InterFace* InterFace, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_InterFace = InterFace;
	Set_headertext();
	connect(ui.btn_ok, &QPushButton::clicked, this, &Manage_PartData::accept);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &Manage_PartData::reject);
	connect(ui.btn_modify, &QPushButton::clicked, this, &Manage_PartData::Modify_Data);
	connect(ui.btn_delete, &QPushButton::clicked, this, &Manage_PartData::Delete_Data);

}

Manage_PartData::~Manage_PartData()
{}

void Manage_PartData::Set_headertext()
{
	QStringList headertext;//��ͷ
	headertext << "����" << "�ڵ�����" << "��Ԫ����" << "˵��";
	//����
	ui.tableWidget->setColumnCount(headertext.count());
	ui.tableWidget->setHorizontalHeaderLabels(headertext);
	ui.tableWidget->verticalHeader()->setVisible(false);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
	LLI T_legs = m_InterFace->ui.treeWidget->topLevelItem(0)->child(0)->childCount();//��������
	ui.tableWidget->setRowCount(T_legs);//��������

	QVector<TowerPart_leg*> ve_leg;//!
	m_InterFace->TP_leg.Get_Array(ve_leg, true);//!

	for (int i = 0; i < T_legs; i++)
	{
		TowerPart_leg* leg = ve_leg[i];//!

		QString name = m_InterFace->ui.treeWidget->topLevelItem(0)->child(0)->child(i)->text(0);
		int E_num = leg->m_Elements_beams.size() + leg->m_Elements_Trusses.size();
		//�������ơ��ڵ㵥Ԫ����
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(name));
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(leg->m_Nodes.size())));
		ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(E_num)));
		ui.tableWidget->setItem(i, 3, new QTableWidgetItem("����"));
		for (int j = 0; j < 4; j++)//��ͷ;
		{
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}

	//����
	ui.tableWidget_2->setColumnCount(headertext.count());
	ui.tableWidget_2->setHorizontalHeaderLabels(headertext);
	ui.tableWidget_2->verticalHeader()->setVisible(false);
	ui.tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_2->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
	LLI T_bodys = m_InterFace->ui.treeWidget->topLevelItem(0)->child(1)->childCount();//��������;
	ui.tableWidget_2->setRowCount(T_bodys);//��������

	QVector<TowerPart_body*> ve_body;//!
	m_InterFace->TP_body.Get_Array(ve_body, true);//!

	for (int i = 0; i < T_bodys; i++)
	{
		TowerPart_body* body = ve_body[i];//
		QString name = m_InterFace->ui.treeWidget->topLevelItem(0)->child(1)->child(i)->text(0);
		int E_num = body->m_Elements_beams.size() + body->m_Elements_Trusses.size();
		//���ýڵ㵥Ԫ����
		ui.tableWidget_2->setItem(i, 0, new QTableWidgetItem(name));
		ui.tableWidget_2->setItem(i, 1, new QTableWidgetItem(QString::number(body->m_Nodes.size())));
		ui.tableWidget_2->setItem(i, 2, new QTableWidgetItem(QString::number(E_num)));
		ui.tableWidget_2->setItem(i, 3, new QTableWidgetItem("����"));
		for (int j = 0; j < 4; j++)//��ͷ;
		{
			ui.tableWidget_2->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}

	//��ͷ
	ui.tableWidget_3->setColumnCount(headertext.count());
	ui.tableWidget_3->setHorizontalHeaderLabels(headertext);
	ui.tableWidget_3->verticalHeader()->setVisible(false);
	ui.tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_3->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
	LLI T_heads = m_InterFace->ui.treeWidget->topLevelItem(0)->child(2)->childCount();//��������;
	ui.tableWidget_3->setRowCount(T_heads);//��ͷ����

	QVector<TowerPart_CrossArm*> ve_head;//!
	m_InterFace->TP_CrossArm.Get_Array(ve_head, true);//!

	for (int i = 0; i < T_heads; i++)
	{
		//��������//���ýڵ㵥Ԫ����
		TowerPart_CrossArm* arm = ve_head[i];//!
		QString name = m_InterFace->ui.treeWidget->topLevelItem(0)->child(2)->child(i)->text(0);
		int E_num = arm->m_Elements_beams.size() + arm->m_Elements_Trusses.size();

		ui.tableWidget_3->setItem(i, 0, new QTableWidgetItem(name));
		ui.tableWidget_3->setItem(i, 1, new QTableWidgetItem(QString::number(arm->m_Nodes.size())));
		ui.tableWidget_3->setItem(i, 2, new QTableWidgetItem(QString::number(E_num)));
		ui.tableWidget_3->setItem(i, 3, new QTableWidgetItem("����"));
		for (int j = 0; j < 4; j++)//��ͷ;
		{
			ui.tableWidget_3->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}
}

void Manage_PartData::Modify_Data()
{	
	int tab_Index = ui.tabWidget->currentIndex();
	int Index = 0;
	switch (tab_Index)
	{
	case 0://����
		Index = ui.tableWidget->currentRow();
		if (ui.tableWidget->item(Index, 1) == nullptr || ui.tableWidget->item(Index, 1)->text().isEmpty())
		{
			QMessageBox::information(this, "Tips", "��ѡ��Ҫ�޸ĵ����Ȳ�����");
		}
		else
		{
			T_Foot* T_foot = m_InterFace->t_foots[Index];
			int ret = T_foot->exec();
			if (ret == QDialog::Accepted)
			{
				//����
				QTreeWidgetItem* parent = m_InterFace->ui.treeWidget->topLevelItem(0)->child(0);
				QTreeWidgetItem* childItem = m_InterFace->ui.treeWidget->topLevelItem(0)->child(0)->child(Index);
				TowerPart_leg* t = new TowerPart_leg;
				T_foot->Get_Data(*t);
				t->Create_Mesh();
				t->m_id = Index + 1;
				childItem->setText(0, t->m_Name);//����
				t->Item = childItem;
				m_InterFace->HiddeAllPart();
				t->Show_VTKnode(m_InterFace->m_Renderer);
				t->Show_VTKtruss(m_InterFace->m_Renderer);
				t->Show_VTKbeam(m_InterFace->m_Renderer);
				
				m_InterFace->TP_leg.Add_Entity(t);

				//���ý������ݱ仯
				ui.tableWidget->setItem(Index, 0, new QTableWidgetItem(t->m_Name));
				//���ýڵ㵥Ԫ����
				ui.tableWidget->setItem(Index, 1, new QTableWidgetItem(QString::number(t->m_Nodes.size())));
				int E_num = t->m_Elements_beams.size() + t->m_Elements_Trusses.size();
				ui.tableWidget->setItem(Index, 2, new QTableWidgetItem(QString::number(E_num)));
				ui.tableWidget->setItem(Index, 3, new QTableWidgetItem("����"));
				for (int j = 0; j < 4; j++)//��ͷ;
				{
					ui.tableWidget->item(Index, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
				}
				m_InterFace->m_Renderer->ResetCamera();
			}
		}
		break;
	case 1://����
		Index = ui.tableWidget_2->currentRow();
		if (ui.tableWidget_2->item(Index, 1) == nullptr || ui.tableWidget_2->item(Index, 1)->text().isEmpty())
		{
			QMessageBox::information(this, "Tips", "��ѡ��Ҫ�޸ĵ���������");
		}
		else
		{
			T_Body* T_body = m_InterFace->t_bodys[Index];
			int ret = T_body->exec();
			if (ret == QDialog::Accepted )
			{
				//����
				QTreeWidgetItem* parent = m_InterFace->ui.treeWidget->topLevelItem(0)->child(1);
				QTreeWidgetItem* childItem = m_InterFace->ui.treeWidget->topLevelItem(0)->child(1)->child(Index);
				TowerPart_body* t = new TowerPart_body;
				T_body->Get_Data(t);
				t->Create_Mesh();
				t->m_id = Index + 1;
				childItem->setText(0, t->m_name);//����
				t->Item = childItem;
				m_InterFace->HiddeAllPart();
				t->Show_VTKtruss(m_InterFace->m_Renderer);
				t->Show_VTKbeam(m_InterFace->m_Renderer);
				t->Show_VTKnode(m_InterFace->m_Renderer);
				m_InterFace->TP_body.Add_Entity(t);

				//���ý������ݱ仯
				ui.tableWidget_2->setItem(Index, 0, new QTableWidgetItem(t->m_name));
				//���ýڵ㵥Ԫ����
				ui.tableWidget_2->setItem(Index, 1, new QTableWidgetItem(QString::number(t->m_Nodes.size())));
				int E_num = t->m_Elements_beams.size() + t->m_Elements_Trusses.size();
				ui.tableWidget_2->setItem(Index, 2, new QTableWidgetItem(QString::number(E_num)));
				ui.tableWidget_2->setItem(Index, 3, new QTableWidgetItem("����"));
				for (int j = 0; j < 4; j++)//��ͷ;
				{
					ui.tableWidget_2->item(Index, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
				}
				m_InterFace->m_Renderer->ResetCamera();
			}
		}
		break;
	case 2://��ͷ
		Index = ui.tableWidget_3->currentRow();
		if (ui.tableWidget_3->item(Index, 1) == nullptr || ui.tableWidget_3->item(Index, 1)->text().isEmpty())
		{
			QMessageBox::information(this, "Tips", "��ѡ��Ҫ�޸ĵ���ͷ������");
		}
		else
		{
			T_CrossArm* T_crossArm = m_InterFace->t_crossarms[Index];
			int ret = T_crossArm->exec();
			if (ret == QDialog::Accepted)
			{
				//����
				QTreeWidgetItem* parent = m_InterFace->ui.treeWidget->topLevelItem(0)->child(2);
				QTreeWidgetItem* childItem = m_InterFace->ui.treeWidget->topLevelItem(0)->child(2)->child(Index);
				TowerPart_CrossArm* t = new TowerPart_CrossArm;
				T_crossArm->Get_Data(*t);
				t->Create_Mesh();
				t->m_id = Index + 1;
				childItem->setText(0, t->m_name);//����
				t->Item = childItem;
				m_InterFace->HiddeAllPart();
				t->Show_VTKtruss(m_InterFace->m_Renderer);
				t->Show_VTKbeam(m_InterFace->m_Renderer);
				t->Show_VTKnode(m_InterFace->m_Renderer);
				m_InterFace->TP_CrossArm.Add_Entity(t);

				//���ý������ݱ仯
				ui.tableWidget_3->setItem(Index, 0, new QTableWidgetItem(t->m_name));
				//���ýڵ㵥Ԫ����
				ui.tableWidget_3->setItem(Index, 1, new QTableWidgetItem(QString::number(t->m_Nodes.size())));
				int E_num = t->m_Elements_beams.size() + t->m_Elements_Trusses.size();
				ui.tableWidget_3->setItem(Index, 2, new QTableWidgetItem(QString::number(E_num)));
				ui.tableWidget_3->setItem(Index, 3, new QTableWidgetItem("����"));
				for (int j = 0; j < 4; j++)//��ͷ;
				{
					ui.tableWidget_3->item(Index, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
				}
				m_InterFace->m_Renderer->ResetCamera();
			}

		}
		break;
	default:
		break;
	}
	
}

void Manage_PartData::Delete_Data()
{
	int tab_Index = ui.tabWidget->currentIndex();
	int Index = 0;
	switch (tab_Index)
	{
	case 0://����
		Index = ui.tableWidget->currentRow();//��ȡ�����������--currentRow()��0��ʼ
		if (ui.tableWidget->item(Index, 1) == nullptr || ui.tableWidget->item(Index, 1)->text().isEmpty())
		{
			//��ʾ
			QMessageBox::information(this, "Tips", "��ѡ��Ҫɾ�������Ȳ�����");
		}
		else
		{
			QString name = ui.tableWidget->item(Index, 0)->text();
			//ȷ���Ƿ�ɾ��
			QMessageBox::StandardButton btn;
			btn = QMessageBox::question(this, "��ʾ", "ȷ��Ҫɾ�� " + name + " ��������",
				QMessageBox::Yes | QMessageBox::No);
			if (btn == QMessageBox::Yes && m_InterFace->TP_leg.Delete_Entity(Index + 1))
			{
				ui.tableWidget->removeRow(Index);//ɾ������е���һ��
				//ɾ��m_InterFace�ӽڵ�
				QTreeWidgetItem* childItem = m_InterFace->ui.treeWidget->topLevelItem(0)->child(0)->child(Index);
				delete(childItem);
				//��Ҫɾ�������ָ��
				std::vector<T_Foot*>::iterator it = m_InterFace->t_foots.begin();
				it = m_InterFace->t_foots.begin() + Index;
				m_InterFace->t_foots.erase(it);
			}
		}
		break;
	case 1://����
		Index = ui.tableWidget_2->currentRow();//��ȡ�����������--currentRow()��0��ʼ
		if (ui.tableWidget_2->item(Index, 1) == nullptr || ui.tableWidget_2->item(Index, 1)->text().isEmpty())
		{
			//��ʾ�ȴ�������
			QMessageBox::information(this, "Tips", "��ѡ��Ҫɾ������������");
		}
		else
		{
			QString name = ui.tableWidget_2->item(Index, 0)->text();
			//ȷ���Ƿ�ɾ��
			QMessageBox::StandardButton btn;
			btn = QMessageBox::question(this, "��ʾ", "ȷ��Ҫɾ�� " + name + " ��������",
				QMessageBox::Yes | QMessageBox::No);
			if (btn == QMessageBox::Yes && m_InterFace->TP_body.Delete_Entity(Index + 1))
			{
				ui.tableWidget_2->removeRow(Index);//ɾ������е���һ��
				//ɾ��m_InterFace�ӽڵ�
				QTreeWidgetItem* childItem = m_InterFace->ui.treeWidget->topLevelItem(0)->child(1)->child(Index);
				delete(childItem);
				//��Ҫɾ�������ָ��
				std::vector<T_Body*>::iterator it = m_InterFace->t_bodys.begin();
				it = m_InterFace->t_bodys.begin() + Index;
				m_InterFace->t_bodys.erase(it);
			}
		}
		break;
	case 2://��ͷ
		Index = ui.tableWidget_3->currentRow();//��ȡ�����������--currentRow()��0��ʼ
		if (ui.tableWidget_3->item(Index, 1) == nullptr || ui.tableWidget_3->item(Index, 1)->text().isEmpty())
		{
			//��ʾ�ȴ�������
			QMessageBox::information(this, "Tips", "��ѡ��Ҫɾ������ͷ������");
		}
		else
		{
			QString name = ui.tableWidget_3->item(Index, 0)->text();
			//ȷ���Ƿ�ɾ��
			QMessageBox::StandardButton btn;
			btn = QMessageBox::question(this, "��ʾ", "ȷ��Ҫɾ�� " + name + " ��������",
				QMessageBox::Yes | QMessageBox::No);
			if (btn == QMessageBox::Yes && m_InterFace->TP_CrossArm.Delete_Entity(Index + 1))
			{
				ui.tableWidget_3->removeRow(Index);//ɾ������е���һ��
				//ɾ��m_InterFace�ӽڵ�
				QTreeWidgetItem* childItem = m_InterFace->ui.treeWidget->topLevelItem(0)->child(2)->child(Index);
				delete(childItem);
				//��Ҫɾ�������ָ��
				std::vector<T_CrossArm*>::iterator it = m_InterFace->t_crossarms.begin();
				it = m_InterFace->t_crossarms.begin() + Index;
				m_InterFace->t_crossarms.erase(it);
			}
		}
		break;
	default:
		break;
	}

}
