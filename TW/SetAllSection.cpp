#include "SetAllSection.h"
QSet<int> selectedRows;
#include<QMessageBox.h>
SetAllSection::SetAllSection(InterFace* InterFace, Part_Base* Part, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_pInterFace = InterFace;
	m_Part = Part;
	Q_ASSERT(m_pInterFace != nullptr);

	connect(ui.btnOK, &QPushButton::clicked, this, [=]()
		{Assgin(Part); });
	table_Section_Lists();
	ui.label_type_2->setText("a(m)：");
	connect(ui.tianjia, &QPushButton::clicked, this, &SetAllSection::Get_Data);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &SetAllSection::reject);
	ui.line_name->setText("Section-" + QString::number(m_pInterFace->Ms.size() + 1));
	ui.LEdit_a->setText("0.200");
	ui.LEdit_b->setText("0.008");
}

SetAllSection::~SetAllSection()
{}



void SetAllSection::Assgin(Part_Base* Part)
{
	int BeamSize = Part->m_Elements_beams.size();
	int TrussSize = Part->m_Elements_Trusses.size();
	for (auto i : Part->m_Elements_beams)
	{
		int ipt1 = i.m_idNode[0] - 1; //id
		int ipt2 = i.m_idNode[1] - 1;
		double x1 = Part->m_Nodes[ipt1].x; double y1 = Part->m_Nodes[ipt1].y; double z1 = Part->m_Nodes[ipt1].z;
		double x2 = Part->m_Nodes[ipt2].x; double y2 = Part->m_Nodes[ipt2].y; double z2 = Part->m_Nodes[ipt2].z;
		double Vx[3];
		Vx[0] = x2 - x1;
		Vx[1] = y2 - y1;
		Vx[2] = z2 - z1;
		double m_L = sqrt(Vx[0] * Vx[0] + Vx[1] * Vx[1] + Vx[2] * Vx[2]);
		int row = ui.tableWidget->rowCount();
		for (int j = 0; j < row; j++)
		{
			double Setm_L = ui.tableWidget->item(j, 3)->text().toDouble();//找到这个截面的长度
			if (abs(Setm_L - m_L) < 1e-1)
			{
				int id = Part->pSection.size()+1;
				double a = ui.tableWidget->item(j, 1)->text().toDouble();
				double b = ui.tableWidget->item(j, 2)->text().toDouble();
				QString QClassSe = ui.tableWidget->item(j, 0)->text();//截面类型
				int ClassSe = -1;
				if (QClassSe == "L型") { ClassSe = 0; }
				else if(QClassSe == "圆环型"){ ClassSe = 1; }

				int ClassM = 0;
				QString QClassM = ui.tableWidget->item(j, 4)->text();//材料类型
				if (QClassM == "Q235") { ClassM = 1; }
				else if (QClassM == "Q345") { ClassM = 2; }
				else if (QClassM == "Q420") { ClassM = 3; }

				Part->pSection.push_back(id);
				QString name = ui.line_name->text();
				Section* section = new Section(name, a, b, id, ClassSe, ClassM);
				m_pInterFace->Ms.Add_Entity(section);
				QSet<int>group_row;
				group_row.insert(i.sectionID);
				Part->AssginSectionGroup(group_row, id);
				m_pInterFace->ShowSubstaceActor(Part);
			}
		}
	}
	this->accept();

}

void SetAllSection::Get_Data()
{
	//添加判断
	if (ui.LEdit_a->text().isEmpty())
	{
		QMessageBox::information(this, "Tips", "请输入a的值！"); return;
	}
	else if (ui.LEdit_b->text().isEmpty())
	{
		QMessageBox::information(this, "Tips", "请输入t的值！"); return;
	}
	else if (ui.mL_line->text().isEmpty())
	{
		QMessageBox::information(this, "Tips", "请输入杆件长度！"); return;
	}
	else
	{
		QStringList str;
		str << ui.line_name->text() << ui.comboBox_4->currentText() << ui.LEdit_a->text() << ui.LEdit_b->text() << ui.mL_line->text() << ui.comboBox_3->currentText();
		int row = ui.tableWidget->rowCount();
		ui.tableWidget->setRowCount(row + 1);
		for (int i = 0; i < 6; i++)
		{
			ui.tableWidget->setItem(row, i, new QTableWidgetItem(str[i]));
			ui.tableWidget->item(row, i)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//表格文字设置居中
		}
	}

}

void SetAllSection::table_Section_Lists()
{
	headertext << "截面名称" << "截面类型" << "a" << "b"  << "长度" << "材料";
	ui.tableWidget->setColumnCount(headertext.count());
	ui.tableWidget->setRowCount(0);//首先默认0行
	ui.tableWidget->setHorizontalHeaderLabels(headertext);
	ui.tableWidget->verticalHeader()->setVisible(false);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

}
