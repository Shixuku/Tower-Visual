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
	QStringList headertext;//表头
	headertext << "名称" << "节点数量" << "单元数量" << "说明";
	ui.tableWidget->setColumnCount(headertext.count());
	ui.tableWidget->setHorizontalHeaderLabels(headertext);
	ui.tableWidget->verticalHeader()->setVisible(false);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

	int T_tower = m_InterFace->ui.treeWidget->topLevelItem(3)->childCount();//塔实例数量
	
	ui.tableWidget->setRowCount(T_tower);//设置行数

	QVector<Tower*> ve_tower;//!
	m_InterFace->TP.Get_Array(ve_tower, true);//!

	for (int i = 0; i < T_tower; i++)
	{
		Tower* tower = ve_tower[i];//!
		//设置名称//设置节点单元数量
		int E_num = tower->m_Elements_beams.size() + tower->m_Elements_Trusses.size();
		QString name = m_InterFace->ui.treeWidget->topLevelItem(3)->child(i)->text(0);
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(name));
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(tower->m_Nodes.size())));
		ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(E_num)));
		ui.tableWidget->setItem(i, 3, new QTableWidgetItem("暂无"));
		for (int j = 0; j < 4; j++)//表头;
		{
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}

}

void Manage_InsData::Modify_Data()
{
	int Index = ui.tableWidget->currentRow();//获取表格所在行数--currentRow()从0开始
	if (ui.tableWidget->item(Index, 1) == nullptr || ui.tableWidget->item(Index, 1)->text().isEmpty())
	{
		QMessageBox::information(this, "Tips", "请选择要修改的塔腿部件！");
	}
	else
	{
		Tower_Assembly* T_As = m_InterFace->tower_assembles[Index];
		int ret = T_As->exec();
		if (ret == QDialog::Accepted)
		{
			//新增
			QTreeWidgetItem* parent = m_InterFace->ui.treeWidget->topLevelItem(3);
			QTreeWidgetItem* childItem = m_InterFace->ui.treeWidget->topLevelItem(3)->child(Index);
			Tower* tw = new Tower;
			for (auto& i : T_As->m_ArryLeg)//塔腿
			{
				tw->addPart(m_InterFace->TP_leg.Find_Entity(i));
			}
			for (auto& i : T_As->m_ArryBody)//塔身段
			{
				tw->addPart(m_InterFace->TP_body.Find_Entity(i));
			}
			for (auto& i : T_As->m_ArryHead)//塔头
			{
				tw->addPart(m_InterFace->TP_CrossArm.Find_Entity(i));
			}
			tw->Check();
			childItem->setText(0, T_As->Get_name());
			tw->Item = childItem;
			tw->m_id = Index + 1;

			double x = 0; double y = 0; double z = 0; double angle = 0;
			T_As->Get_Movedata(x, y, z, angle);//得到平移旋转角度
			tw->rotation(angle);//旋转
			tw->move(x, y, z);//平移

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

			//设置界面数据变化
			ui.tableWidget->setItem(Index, 0, new QTableWidgetItem(T_As->Get_name()));
			//设置节点单元数量
			ui.tableWidget->setItem(Index, 1, new QTableWidgetItem(QString::number(tw->m_Nodes.size())));
			int E_num = tw->m_Elements_beams.size() + tw->m_Elements_Trusses.size();
			ui.tableWidget->setItem(Index, 2, new QTableWidgetItem(QString::number(E_num)));
			ui.tableWidget->setItem(Index, 3, new QTableWidgetItem("暂无"));
			for (int j = 0; j < 4; j++)//表头;
			{
				ui.tableWidget->item(Index, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			}
			m_InterFace->ResetCamera();
		}
	}
}

void Manage_InsData::Delete_Data()
{
	 int Index = ui.tableWidget->currentRow();//获取表格所在行数--currentRow()从0开始
	if (ui.tableWidget->item(Index, 1) == nullptr || ui.tableWidget->item(Index, 1)->text().isEmpty())
	{
		//提示
		QMessageBox::information(this, "Tips", "请选择要删除的实例！");
	}
	else
	{
		QString name = ui.tableWidget->item(Index, 0)->text();
		//确认是否删除
		QMessageBox::StandardButton btn;
		btn = QMessageBox::question(this, "提示", "确定要删除 " + name + " 的数据吗？",
			QMessageBox::Yes | QMessageBox::No);
		if (btn == QMessageBox::Yes && m_InterFace->TP.Delete_Entity(Index + 1))
		{
			ui.tableWidget->removeRow(Index);//删掉表格中的那一行
			//删掉m_InterFace子节点
			QTreeWidgetItem* childItem = m_InterFace->ui.treeWidget->topLevelItem(3)->child(Index);
			delete(childItem);
			//需要删掉界面的指针
			std::vector<Tower_Assembly*>::iterator it = m_InterFace->tower_assembles.begin();
			it = m_InterFace->tower_assembles.begin() + Index;
			m_InterFace->tower_assembles.erase(it);
		}
	}
}

Manage_InsData::~Manage_InsData()
{}
