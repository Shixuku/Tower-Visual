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
	QStringList headertext;//表头
	headertext << "名称" << "节点数量" << "单元数量" << "说明";
	//塔腿
	ui.tableWidget->setColumnCount(headertext.count());
	ui.tableWidget->setHorizontalHeaderLabels(headertext);
	ui.tableWidget->verticalHeader()->setVisible(false);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
	LLI T_legs = m_InterFace->ui.treeWidget->topLevelItem(0)->child(0)->childCount();//塔腿数量
	ui.tableWidget->setRowCount(T_legs);//塔腿行数

	QVector<TowerPart_leg*> ve_leg;//!
	m_InterFace->TP_leg.Get_Array(ve_leg, true);//!

	for (int i = 0; i < T_legs; i++)
	{
		TowerPart_leg* leg = ve_leg[i];//!

		QString name = m_InterFace->ui.treeWidget->topLevelItem(0)->child(0)->child(i)->text(0);
		int E_num = leg->m_Elements_beams.size() + leg->m_Elements_Trusses.size();
		//设置名称、节点单元数量
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(name));
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(leg->m_Nodes.size())));
		ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(E_num)));
		ui.tableWidget->setItem(i, 3, new QTableWidgetItem("暂无"));
		for (int j = 0; j < 4; j++)//表头;
		{
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}

	//塔身
	ui.tableWidget_2->setColumnCount(headertext.count());
	ui.tableWidget_2->setHorizontalHeaderLabels(headertext);
	ui.tableWidget_2->verticalHeader()->setVisible(false);
	ui.tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_2->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
	LLI T_bodys = m_InterFace->ui.treeWidget->topLevelItem(0)->child(1)->childCount();//塔腿数量;
	ui.tableWidget_2->setRowCount(T_bodys);//塔身行数

	QVector<TowerPart_body*> ve_body;//!
	m_InterFace->TP_body.Get_Array(ve_body, true);//!

	for (int i = 0; i < T_bodys; i++)
	{
		TowerPart_body* body = ve_body[i];//
		QString name = m_InterFace->ui.treeWidget->topLevelItem(0)->child(1)->child(i)->text(0);
		int E_num = body->m_Elements_beams.size() + body->m_Elements_Trusses.size();
		//设置节点单元数量
		ui.tableWidget_2->setItem(i, 0, new QTableWidgetItem(name));
		ui.tableWidget_2->setItem(i, 1, new QTableWidgetItem(QString::number(body->m_Nodes.size())));
		ui.tableWidget_2->setItem(i, 2, new QTableWidgetItem(QString::number(E_num)));
		ui.tableWidget_2->setItem(i, 3, new QTableWidgetItem("暂无"));
		for (int j = 0; j < 4; j++)//表头;
		{
			ui.tableWidget_2->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}

	//塔头
	ui.tableWidget_3->setColumnCount(headertext.count());
	ui.tableWidget_3->setHorizontalHeaderLabels(headertext);
	ui.tableWidget_3->verticalHeader()->setVisible(false);
	ui.tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_3->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
	LLI T_heads = m_InterFace->ui.treeWidget->topLevelItem(0)->child(2)->childCount();//塔腿数量;
	ui.tableWidget_3->setRowCount(T_heads);//塔头行数

	QVector<TowerPart_CrossArm*> ve_head;//!
	m_InterFace->TP_CrossArm.Get_Array(ve_head, true);//!

	for (int i = 0; i < T_heads; i++)
	{
		//设置名称//设置节点单元数量
		TowerPart_CrossArm* arm = ve_head[i];//!
		QString name = m_InterFace->ui.treeWidget->topLevelItem(0)->child(2)->child(i)->text(0);
		int E_num = arm->m_Elements_beams.size() + arm->m_Elements_Trusses.size();

		ui.tableWidget_3->setItem(i, 0, new QTableWidgetItem(name));
		ui.tableWidget_3->setItem(i, 1, new QTableWidgetItem(QString::number(arm->m_Nodes.size())));
		ui.tableWidget_3->setItem(i, 2, new QTableWidgetItem(QString::number(E_num)));
		ui.tableWidget_3->setItem(i, 3, new QTableWidgetItem("暂无"));
		for (int j = 0; j < 4; j++)//表头;
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
	case 0://塔腿
		Index = ui.tableWidget->currentRow();
		if (ui.tableWidget->item(Index, 1) == nullptr || ui.tableWidget->item(Index, 1)->text().isEmpty())
		{
			QMessageBox::information(this, "Tips", "请选择要修改的塔腿部件！");
		}
		else
		{
			T_Foot* T_foot = m_InterFace->t_foots[Index];
			int ret = T_foot->exec();
			if (ret == QDialog::Accepted)
			{
				//新增
				QTreeWidgetItem* parent = m_InterFace->ui.treeWidget->topLevelItem(0)->child(0);
				QTreeWidgetItem* childItem = m_InterFace->ui.treeWidget->topLevelItem(0)->child(0)->child(Index);
				TowerPart_leg* t = new TowerPart_leg;
				T_foot->Get_Data(*t);
				t->Create_Mesh();
				t->m_id = Index + 1;
				childItem->setText(0, t->m_Name);//命名
				t->Item = childItem;
				m_InterFace->HiddeAllPart();
				t->Show_VTKnode(m_InterFace->m_Renderer);
				t->Show_VTKtruss(m_InterFace->m_Renderer);
				t->Show_VTKbeam(m_InterFace->m_Renderer);
				
				m_InterFace->TP_leg.Add_Entity(t);

				//设置界面数据变化
				ui.tableWidget->setItem(Index, 0, new QTableWidgetItem(t->m_Name));
				//设置节点单元数量
				ui.tableWidget->setItem(Index, 1, new QTableWidgetItem(QString::number(t->m_Nodes.size())));
				int E_num = t->m_Elements_beams.size() + t->m_Elements_Trusses.size();
				ui.tableWidget->setItem(Index, 2, new QTableWidgetItem(QString::number(E_num)));
				ui.tableWidget->setItem(Index, 3, new QTableWidgetItem("暂无"));
				for (int j = 0; j < 4; j++)//表头;
				{
					ui.tableWidget->item(Index, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
				}
				m_InterFace->m_Renderer->ResetCamera();
			}
		}
		break;
	case 1://塔身
		Index = ui.tableWidget_2->currentRow();
		if (ui.tableWidget_2->item(Index, 1) == nullptr || ui.tableWidget_2->item(Index, 1)->text().isEmpty())
		{
			QMessageBox::information(this, "Tips", "请选择要修改的塔身部件！");
		}
		else
		{
			T_Body* T_body = m_InterFace->t_bodys[Index];
			int ret = T_body->exec();
			if (ret == QDialog::Accepted )
			{
				//新增
				QTreeWidgetItem* parent = m_InterFace->ui.treeWidget->topLevelItem(0)->child(1);
				QTreeWidgetItem* childItem = m_InterFace->ui.treeWidget->topLevelItem(0)->child(1)->child(Index);
				TowerPart_body* t = new TowerPart_body;
				T_body->Get_Data(t);
				t->Create_Mesh();
				t->m_id = Index + 1;
				childItem->setText(0, t->m_name);//命名
				t->Item = childItem;
				m_InterFace->HiddeAllPart();
				t->Show_VTKtruss(m_InterFace->m_Renderer);
				t->Show_VTKbeam(m_InterFace->m_Renderer);
				t->Show_VTKnode(m_InterFace->m_Renderer);
				m_InterFace->TP_body.Add_Entity(t);

				//设置界面数据变化
				ui.tableWidget_2->setItem(Index, 0, new QTableWidgetItem(t->m_name));
				//设置节点单元数量
				ui.tableWidget_2->setItem(Index, 1, new QTableWidgetItem(QString::number(t->m_Nodes.size())));
				int E_num = t->m_Elements_beams.size() + t->m_Elements_Trusses.size();
				ui.tableWidget_2->setItem(Index, 2, new QTableWidgetItem(QString::number(E_num)));
				ui.tableWidget_2->setItem(Index, 3, new QTableWidgetItem("暂无"));
				for (int j = 0; j < 4; j++)//表头;
				{
					ui.tableWidget_2->item(Index, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
				}
				m_InterFace->m_Renderer->ResetCamera();
			}
		}
		break;
	case 2://塔头
		Index = ui.tableWidget_3->currentRow();
		if (ui.tableWidget_3->item(Index, 1) == nullptr || ui.tableWidget_3->item(Index, 1)->text().isEmpty())
		{
			QMessageBox::information(this, "Tips", "请选择要修改的塔头部件！");
		}
		else
		{
			T_CrossArm* T_crossArm = m_InterFace->t_crossarms[Index];
			int ret = T_crossArm->exec();
			if (ret == QDialog::Accepted)
			{
				//新增
				QTreeWidgetItem* parent = m_InterFace->ui.treeWidget->topLevelItem(0)->child(2);
				QTreeWidgetItem* childItem = m_InterFace->ui.treeWidget->topLevelItem(0)->child(2)->child(Index);
				TowerPart_CrossArm* t = new TowerPart_CrossArm;
				T_crossArm->Get_Data(*t);
				t->Create_Mesh();
				t->m_id = Index + 1;
				childItem->setText(0, t->m_name);//命名
				t->Item = childItem;
				m_InterFace->HiddeAllPart();
				t->Show_VTKtruss(m_InterFace->m_Renderer);
				t->Show_VTKbeam(m_InterFace->m_Renderer);
				t->Show_VTKnode(m_InterFace->m_Renderer);
				m_InterFace->TP_CrossArm.Add_Entity(t);

				//设置界面数据变化
				ui.tableWidget_3->setItem(Index, 0, new QTableWidgetItem(t->m_name));
				//设置节点单元数量
				ui.tableWidget_3->setItem(Index, 1, new QTableWidgetItem(QString::number(t->m_Nodes.size())));
				int E_num = t->m_Elements_beams.size() + t->m_Elements_Trusses.size();
				ui.tableWidget_3->setItem(Index, 2, new QTableWidgetItem(QString::number(E_num)));
				ui.tableWidget_3->setItem(Index, 3, new QTableWidgetItem("暂无"));
				for (int j = 0; j < 4; j++)//表头;
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
	case 0://塔腿
		Index = ui.tableWidget->currentRow();//获取表格所在行数--currentRow()从0开始
		if (ui.tableWidget->item(Index, 1) == nullptr || ui.tableWidget->item(Index, 1)->text().isEmpty())
		{
			//提示
			QMessageBox::information(this, "Tips", "请选择要删除的塔腿部件！");
		}
		else
		{
			QString name = ui.tableWidget->item(Index, 0)->text();
			//确认是否删除
			QMessageBox::StandardButton btn;
			btn = QMessageBox::question(this, "提示", "确定要删除 " + name + " 的数据吗？",
				QMessageBox::Yes | QMessageBox::No);
			if (btn == QMessageBox::Yes && m_InterFace->TP_leg.Delete_Entity(Index + 1))
			{
				ui.tableWidget->removeRow(Index);//删掉表格中的那一行
				//删掉m_InterFace子节点
				QTreeWidgetItem* childItem = m_InterFace->ui.treeWidget->topLevelItem(0)->child(0)->child(Index);
				delete(childItem);
				//需要删掉界面的指针
				std::vector<T_Foot*>::iterator it = m_InterFace->t_foots.begin();
				it = m_InterFace->t_foots.begin() + Index;
				m_InterFace->t_foots.erase(it);
			}
		}
		break;
	case 1://塔身
		Index = ui.tableWidget_2->currentRow();//获取表格所在行数--currentRow()从0开始
		if (ui.tableWidget_2->item(Index, 1) == nullptr || ui.tableWidget_2->item(Index, 1)->text().isEmpty())
		{
			//提示先创建截面
			QMessageBox::information(this, "Tips", "请选择要删除的塔身部件！");
		}
		else
		{
			QString name = ui.tableWidget_2->item(Index, 0)->text();
			//确认是否删除
			QMessageBox::StandardButton btn;
			btn = QMessageBox::question(this, "提示", "确定要删除 " + name + " 的数据吗？",
				QMessageBox::Yes | QMessageBox::No);
			if (btn == QMessageBox::Yes && m_InterFace->TP_body.Delete_Entity(Index + 1))
			{
				ui.tableWidget_2->removeRow(Index);//删掉表格中的那一行
				//删掉m_InterFace子节点
				QTreeWidgetItem* childItem = m_InterFace->ui.treeWidget->topLevelItem(0)->child(1)->child(Index);
				delete(childItem);
				//需要删掉界面的指针
				std::vector<T_Body*>::iterator it = m_InterFace->t_bodys.begin();
				it = m_InterFace->t_bodys.begin() + Index;
				m_InterFace->t_bodys.erase(it);
			}
		}
		break;
	case 2://塔头
		Index = ui.tableWidget_3->currentRow();//获取表格所在行数--currentRow()从0开始
		if (ui.tableWidget_3->item(Index, 1) == nullptr || ui.tableWidget_3->item(Index, 1)->text().isEmpty())
		{
			//提示先创建截面
			QMessageBox::information(this, "Tips", "请选择要删除的塔头部件！");
		}
		else
		{
			QString name = ui.tableWidget_3->item(Index, 0)->text();
			//确认是否删除
			QMessageBox::StandardButton btn;
			btn = QMessageBox::question(this, "提示", "确定要删除 " + name + " 的数据吗？",
				QMessageBox::Yes | QMessageBox::No);
			if (btn == QMessageBox::Yes && m_InterFace->TP_CrossArm.Delete_Entity(Index + 1))
			{
				ui.tableWidget_3->removeRow(Index);//删掉表格中的那一行
				//删掉m_InterFace子节点
				QTreeWidgetItem* childItem = m_InterFace->ui.treeWidget->topLevelItem(0)->child(2)->child(Index);
				delete(childItem);
				//需要删掉界面的指针
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
