#include "SetAllSection.h"

SetAllSection::SetAllSection(InterFace* InterFace,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_pInterFace = InterFace;

	Q_ASSERT(m_pInterFace != nullptr);

	Create_combobox();
	ShowComboVTK();
	table_Section_Lists();
	connect(ui.tianjia, &QPushButton::clicked, this, &SetAllSection::Get_Data);


}

SetAllSection::~SetAllSection()
{}

void SetAllSection::Create_combobox()
{
	TowerSize = m_pInterFace->ui.treeWidget->topLevelItem(1)->childCount();//塔腿数量


	for (int i = 0; i < TowerSize; i++)
	{
		ui.comboBox->addItem("塔实例" + QString::number(i + 1));
	}

}

void SetAllSection::ShowComboVTK()
{

	T_legs = m_pInterFace->ui.treeWidget->topLevelItem(0)->childCount();//塔腿数量
	T_bodys = m_pInterFace->ui.treeWidget->topLevelItem(1)->childCount();//塔身数量
	T_heads = m_pInterFace->ui.treeWidget->topLevelItem(2)->childCount();//塔头数量
	int Index = ui.comboBox->currentIndex();

	id_Part = Index + 1;
	m_pInterFace->Show_Tower(m_pInterFace->TP.Find_Entity(id_Part));

	connect(ui.btnOK, &QPushButton::clicked, this, [=]()
		{Assgin(m_pInterFace->TP.Find_Entity(id_Part)); });

}

void SetAllSection::Assgin(Tower* tower)
{
	int size = tower->m_Elements_beams.size();
	cout << size << "\n";
	for (auto i : tower->m_Elements_beams)
	{
		int ipt1 = i.m_idNode[0] - 1; //id
		int ipt2 = i.m_idNode[1] - 1;
		double x1 = tower->m_Nodes[ipt1].x; double y1 = tower->m_Nodes[ipt1].y; double z1 = tower->m_Nodes[ipt1].z;
		double x2 = tower->m_Nodes[ipt2].x; double y2 = tower->m_Nodes[ipt2].y; double z2 = tower->m_Nodes[ipt2].z;
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
				int ID = i.m_idElement;
				double a= ui.tableWidget->item(j, 1)->text().toDouble();
				double b = ui.tableWidget->item(j, 2)->text().toDouble();
				int ClassSe= ui.tableWidget->item(j, 0)->text().toInt();
				tower->Show_Beam(ID, ClassSe, a, b);
			}
		}
	}
	m_pInterFace->Show_Tower(tower);

}

void SetAllSection::Get_Data()
{
	QStringList str;
	str << ui.lineEdit->text() << ui.a_Line->text() << ui.b_line->text() << ui.mL_line->text();
	int row = ui.tableWidget->rowCount();
	ui.tableWidget->setRowCount(row + 1);
	for (int i = 0; i < 4; i++) 
	{
		ui.tableWidget->setItem(row, i, new QTableWidgetItem(str[i]));
	}
}

void SetAllSection::table_Section_Lists()
{
	headertext << "形状" << "a" << "b" << "长度";
	ui.tableWidget->setColumnCount(headertext.count());
	ui.tableWidget->setRowCount(0);//首先默认0行
	ui.tableWidget->setHorizontalHeaderLabels(headertext);
	ui.tableWidget->verticalHeader()->setVisible(false);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}
