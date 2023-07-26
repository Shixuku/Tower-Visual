#include "SetAllSection.h"
QSet<int> selectedRows;
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
	connect(ui.tianjia, &QPushButton::clicked, this, &SetAllSection::Get_Data);


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
				int ClassSe = ui.tableWidget->item(j, 0)->text().toInt();
				int ClassM= ui.tableWidget->item(j,4)->text().toInt();
				//Part->pSection.push_back(Section(a, b, id, ClassSe, ClassM));
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

}

void SetAllSection::Get_Data()
{
	QStringList str;
	str << ui.lineEdit->text() << ui.a_Line->text() << ui.b_line->text() << ui.mL_line->text()<<ui.lineEdit->text();
	int row = ui.tableWidget->rowCount();
	ui.tableWidget->setRowCount(row + 1);
	for (int i = 0; i < 5; i++) 
	{
		ui.tableWidget->setItem(row, i, new QTableWidgetItem(str[i]));
	}
}

void SetAllSection::table_Section_Lists()
{
	headertext << "形状" << "a" << "b"  << "长度" << "材料";
	ui.tableWidget->setColumnCount(headertext.count());
	ui.tableWidget->setRowCount(0);//首先默认0行
	ui.tableWidget->setHorizontalHeaderLabels(headertext);
	ui.tableWidget->verticalHeader()->setVisible(false);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}
