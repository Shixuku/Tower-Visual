#include "Assign_Section.h"
#pragma execution_character_set("utf-8")
#include"InterFace.h"
#include<QMessageBox.h>
#include<QTextCodec>
#include<QStringLiteral>
#include<QList>
#include<QSet>

Assign_Section::Assign_Section(InterFace* InterFace, Part_Base*Part, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	m_pInterFace = InterFace;
	m_Part = Part;
	Q_ASSERT(m_pInterFace != nullptr);

	ma_ab2 = new Set_Section(m_pInterFace);
	Set_headertext();

	//点击组数，会显示高亮（在原来的基础上新建一个actor）
	table_Group_Lists(Part);
	connect(ui.Group_Lists, &QTableWidget::itemClicked, this, [=](){
		highlight_actor(Part);
		m_pInterFace->m_renderWindow->Render(); 
		});

	//指派截面
	connect(ui.btn_assign, &QPushButton::clicked, this, [=]()
		{assigh_section(Part); });
	linesactor = vtkSmartPointer<vtkActor>::New();

	//显示创建截面对话框
	connect(ui.btn_add, &QPushButton::clicked, this, &Assign_Section::BtnAddSection);

	//删除已添加的截面
	connect(ui.btn_delete, &QPushButton::clicked, this, &Assign_Section::table_Section_Lists_delete);

	//点击完成按钮，会删除高亮的actor
	connect(ui.btn_finish, &QPushButton::clicked, this, [=]()
		{
			m_pInterFace->m_Renderer->RemoveActor(linesactor);
			this->reject();
		});

}

Assign_Section::~Assign_Section()
{

}

//设置单元组列表
void Assign_Section::table_Group_Lists(Part_Base* Part)
{
	QStringList headertext;
	int BeamSize= Part->id_BeamSection;
	int TrussSize = Part->id_TrussSection;
	int rowcount = BeamSize+ TrussSize;
	headertext << "杆件组列表";
	ui.Group_Lists->setColumnCount(headertext.count());
	ui.Group_Lists->setRowCount(rowcount);
	ui.Group_Lists->setHorizontalHeaderLabels(headertext);
	ui.Group_Lists->verticalHeader()->setVisible(false);
	ui.Group_Lists->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	for (int i = 0; i < rowcount; i++)
	{
		ui.Group_Lists->setItem(i, 0, new QTableWidgetItem("第 "+ QString::number(i + 1) + " 组"));
		ui.Group_Lists->item(i, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		ui.Group_Lists->item(i, 0)->setFlags(ui.Group_Lists->item(i, 0)->flags() & (~Qt::ItemIsEditable));  // 设置只读属性
	}

	//支持多选模式，比如Ctrl、Shift、 Ctrl+A、鼠标左键按下滑动多选
	ui.Group_Lists->setSelectionMode(QAbstractItemView::ExtendedSelection);
	//设置单行选择
	ui.Group_Lists->setSelectionBehavior(QAbstractItemView::SelectRows);
}


//功能——删除截面
void Assign_Section::table_Section_Lists_delete()
{
	int section_rowIndex = ui.Section_Lists->currentRow();
	if (ui.Section_Lists->rowCount() == 0)
	{
		QMessageBox::information(this, "Tips", "当前没有截面！"); return;
	}
	else if (section_rowIndex < 0)
	{
		QMessageBox::information(this, "Tips", "请选择要删除的截面！"); return;
	}
	else
	{
		//确认是否删除
		QMessageBox::StandardButton btn;
		btn = QMessageBox::question(this, "提示", "确定要删除第 " + QString::number(section_rowIndex + 1) + " 组截面数据吗？",
			QMessageBox::Yes | QMessageBox::No);
		if (btn == QMessageBox::Yes)
		{
			if (section_rowIndex != -1)
			{
				int id = ui.Section_Lists->item(section_rowIndex, 0)->text().toInt();
				ui.Section_Lists->removeRow(section_rowIndex);
				
				cout << id << "\n";
				m_pInterFace->Ms.Delete_Entity(id);//删除m_pInterFace里面的截面数据
				//m_pInterFace->Ms.Delete_Entity(section_rowIndex + 1);//删除m_pInterFace里面的截面数据
			}
		}

	}
}

//功能——高亮单元组
void Assign_Section::highlight_actor(Part_Base* Part)
{
	list<Element_Beam*>B_Elements; //高亮
	list<Element_Truss*>T_Elements; //高亮
	int group_rowIndex = ui.Group_Lists->currentRow()+1;

	for (auto& i : Part->m_Elements_beams)
	{
		if (i.sectionID == group_rowIndex)
		{
			B_Elements.push_back(&i);
		}
		
	}
	for (auto& i : Part->m_Elements_Trusses)
	{
		if (i.sectionID == group_rowIndex- Part->id_BeamSection)
		{
			T_Elements.push_back(&i);
		}

	}
	vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkPoints> linepts = vtkSmartPointer<vtkPoints>::New();
	vector<Node> m_Nodes = Part->m_Nodes;
	if (B_Elements.size() != 0)
	{
		for (auto& i : B_Elements)
		{
			int ip1 = i->m_idNode[0] - 1;
			int ip2 = i->m_idNode[1] - 1;

			double x = m_Nodes[ip1].x;
			double y = m_Nodes[ip1].y;
			double z = m_Nodes[ip1].z;

			double x2 = m_Nodes[ip2].x;
			double y2 = m_Nodes[ip2].y;
			double z2 = m_Nodes[ip2].z;

			vtkIdType pid[2];
			pid[0] = linepts->InsertNextPoint(x, y, z);
			pid[1] = linepts->InsertNextPoint(x2, y2, z2);

			vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
			line->GetPointIds()->SetId(0, pid[0]);
			line->GetPointIds()->SetId(1, pid[1]);
			lines->InsertNextCell(line);
		}
	}
	else 
	{
		for (auto& i : T_Elements)
		{
			int ip1 = i->m_idNode[0] - 1;
			int ip2 = i->m_idNode[1] - 1;

			double x = m_Nodes[ip1].x;
			double y = m_Nodes[ip1].y;
			double z = m_Nodes[ip1].z;

			double x2 = m_Nodes[ip2].x;
			double y2 = m_Nodes[ip2].y;
			double z2 = m_Nodes[ip2].z;

			vtkIdType pid[2];
			pid[0] = linepts->InsertNextPoint(x, y, z);
			pid[1] = linepts->InsertNextPoint(x2, y2, z2);

			vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
			line->GetPointIds()->SetId(0, pid[0]);
			line->GetPointIds()->SetId(1, pid[1]);
			lines->InsertNextCell(line);
		}
	}

	vtkSmartPointer<vtkPolyData> linesPolyData = vtkSmartPointer<vtkPolyData>::New();
	linesPolyData->SetPoints(linepts);
	linesPolyData->SetLines(lines);
	vtkSmartPointer<vtkPolyDataMapper> linesmapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	linesmapper->SetInputData(linesPolyData);

	if (linesactor != nullptr)
	{
		m_pInterFace->m_Renderer->RemoveActor(linesactor);
	}
	
	linesactor->SetMapper(linesmapper);
	linesactor->GetProperty()->SetColor(1, 0, 0);
	linesactor->GetProperty()->SetLineWidth(3);
	m_pInterFace->m_Renderer->AddActor(linesactor);

}

//功能——指派
void Assign_Section::assigh_section(Part_Base* Part)
{
	int section_row = ui.Section_Lists->currentRow();//取得截面信息,如果没有表格，就等于-1
	int group_row = ui.Group_Lists->currentRow() + 1;//取得单元分组信息
	//首先判断有无截面
	if (ui.Section_Lists->rowCount() == 0)
	{
		QMessageBox::information(this, "Tips", "请先创建截面！"); return;
	}
	//然后判断截面信息有没有被选中
	else if (section_row < 0)
	{
		QMessageBox::information(this, "Tips", "请选择截面！"); return;
	}
	//判断杆件组信息有没有选中
	else if (group_row < 1)
	{
		QMessageBox::information(this, "Tips", "请选择杆件组！"); return;
	}
	else
	{
		//int id = ui.Section_Lists->item(section_row, 5)->text().toInt();//找到这个截面的编号
		int id = ui.Section_Lists->item(section_row, 0)->text().toInt();//修改过后
		

		QList<QTableWidgetItem*> selectedItems = ui.Group_Lists->selectedItems();

		// 存储选中行的索引
		QSet<int> selectedRows;
		// 提取行号
		for (const auto* item : selectedItems) {
			int row = item->row() + 1;
			selectedRows.insert(row);
		}
		Part->AssginSectionGroup(selectedRows, id);
		m_pInterFace->ShowSubstaceActor(Part);
	}

}

void Assign_Section::BtnAddSection()
{
	int ret = ma_ab2->exec();
	if (ret == QDialog::Accepted)
	{
		int id = m_pInterFace->Ms.size();//从1开始排序
		int irow = ui.Section_Lists->rowCount();
		ui.Section_Lists->setRowCount(irow + 1);
		Section* pSection = m_pInterFace->Ms.Find_Entity(id);  //取出截面指针

		ui.Section_Lists->setItem(irow, 0, new QTableWidgetItem(QString::number(pSection->m_id)));//编号
		ui.Section_Lists->setItem(irow, 1, new QTableWidgetItem(pSection->m_Name));//名称
		QString Material;
		if (pSection->ClassM == 1)
		{
			Material = "Q235";
		}
		else if (pSection->ClassM == 2)
		{
			Material = "Q345";
		}
		else if (pSection->ClassM == 3)
		{
			Material = "Q420";
		}
		QString type;
		if (pSection->ClassSe == 0)//L型
		{
			type = "L";
		}
		else if (pSection->ClassSe == 1)
		{
			//type = "\u03A6";//大写fai--显示不出来！
			type = "D";
		}
		QString data = QString::number((pSection->a) * 1000) + "X" + QString::number((pSection->b) * 1000);
		ui.Section_Lists->setItem(irow, 2, new QTableWidgetItem(Material + type + data));
		for (int j = 0; j < 3; j++)//表头;
		{
			ui.Section_Lists->item(irow, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}
}

void Assign_Section::Set_headertext()
{
	QStringList headertext;//表头
	headertext << "编号" << "名称" << "类型";
	ui.Section_Lists->setColumnCount(headertext.count());
	ui.Section_Lists->setHorizontalHeaderLabels(headertext);
	ui.Section_Lists->verticalHeader()->setVisible(false);
	ui.Section_Lists->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.Section_Lists->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

	int rowcount = m_pInterFace->Ms.size();
	ui.Section_Lists->setRowCount(rowcount);
	QVector<Section*> ve_section;
	m_pInterFace->Ms.Get_Array(ve_section, true);

	for (int i = 0; i < rowcount; i++)
	{
		Section* section = ve_section[i];
		ui.Section_Lists->setItem(i, 0, new QTableWidgetItem(QString::number(section->m_id)));//编号
		ui.Section_Lists->setItem(i, 1, new QTableWidgetItem(section->m_Name));//名称
		QString Material;
		if (section->ClassM == 1)
		{
			Material = "Q235";
		}
		else if (section->ClassM == 2)
		{
			Material = "Q345";
		}
		else if (section->ClassM == 3)
		{
			Material = "Q420";
		}
		QString type;
		if (section->ClassSe == 0)//L型
		{
			type = "L";
		}
		else if (section->ClassSe == 1)
		{
			//type = "\u03A6";//大写fai--显示不出来！
			type = "D";
		}
		QString data = QString::number((section->a) * 1000) + "X" + QString::number((section->b) * 1000);
		ui.Section_Lists->setItem(i, 2, new QTableWidgetItem(Material + type + data));
		for (int j = 0; j < 3; j++)//表头;
		{
			ui.Section_Lists->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}

}
//点“x”关闭事件
void Assign_Section::closeEvent(QCloseEvent* e)
{
	m_pInterFace->m_Renderer->RemoveActor(linesactor);
}



