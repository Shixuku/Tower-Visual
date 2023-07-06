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
	table_Section_Lists();//设置截面表格
	Add_created_section();//添加已经创建的截面

	//点击组数，会显示高亮（在原来的基础上新建一个actor）
	table_Group_Lists(Part);
	connect(ui.Group_Lists, &QTableWidget::itemClicked, this, [=]()
		{highlight_actor(Part);
	m_pInterFace->m_renderWindow->Render(); });

	//指派截面
	connect(ui.btn_assign, &QPushButton::clicked, this, [=]()
		{assigh_section(Part); });
	linesactor = vtkSmartPointer<vtkActor>::New();

	//显示创建截面对话框
	connect(ui.btn_add, &QPushButton::clicked, this, [=](){
	int ret = ma_ab2->exec();
	if (ret == QDialog::Accepted)
	{
		int ClassSection = 0;//取得该截面类型的参数
		ClassSection = ma_ab2->ClassSection;
		string NameSection = ma_ab2->name;
		double ia = ma_ab2->a;
		double ib = ma_ab2->b;
		int iM = ma_ab2->ClassMa;

		int id = m_pInterFace->Ms.size() + 1;//从1开始排序

		int irow = ui.Section_Lists->rowCount();
		ui.Section_Lists->setRowCount(irow + 1);
		QString QName = QString::fromStdString(NameSection);;
		ui.Section_Lists->setItem(irow, 0, new QTableWidgetItem(QName));
		ui.Section_Lists->item(irow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString s;
		if (ClassSection == 0)
		{
			s = "L型";
		}
		else
		{
			s = "圆环";
		}
		ui.Section_Lists->setItem(irow, 1, new QTableWidgetItem(s));
		ui.Section_Lists->item(irow, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString i_a = QString::number(ia);
		ui.Section_Lists->setItem(irow, 2, new QTableWidgetItem(i_a));
		ui.Section_Lists->item(irow, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString i_b = QString::number(ib);
		ui.Section_Lists->setItem(irow, 3, new QTableWidgetItem(i_b));
		ui.Section_Lists->item(irow, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString i_M = QString::number(iM);
		ui.Section_Lists->setItem(irow, 4, new QTableWidgetItem(i_M));
		ui.Section_Lists->item(irow, 4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString i_id = QString::number(id);
		ui.Section_Lists->setItem(irow, 5, new QTableWidgetItem(i_id));
		ui.Section_Lists->item(irow, 5)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	}});
	//添加截面
	//connect(ma_ab2, SIGNAL(SendData(QStringList&)), this, SLOT(GetData(QStringList&)));
	connect(ma_ab2, SIGNAL(SendData(QStringList&)), m_pInterFace, SLOT(GetData(QStringList&)));
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

//设置截面列表
void Assign_Section::table_Section_Lists()
{
	headertext << "名称" << "形状" << "a" << "b" << "材料编号" << "编号";
	ui.Section_Lists->setColumnCount(headertext.count());
	ui.Section_Lists->setRowCount(0);//首先默认0行
	ui.Section_Lists->setHorizontalHeaderLabels(headertext);
	ui.Section_Lists->verticalHeader()->setVisible(false);
	ui.Section_Lists->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

//将创建好的截面数据放入截面表
void Assign_Section::Add_created_section()
{
	for (auto& i: m_pInterFace->Ms)
	{
		Section* pSection = i.second;  //取出截面指针
		int irow = ui.Section_Lists->rowCount();
		ui.Section_Lists->setRowCount(irow + 1);

		QString QName = QString::fromStdString(pSection->Name);
		ui.Section_Lists->setItem(irow, 0, new QTableWidgetItem(QName));
		ui.Section_Lists->item(irow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString s;
		if (pSection->ClassSe == 0)
		{
			s = "L型";
		}
		else
		{
			s = "圆环";
		}
		ui.Section_Lists->setItem(irow, 1, new QTableWidgetItem(s));
		ui.Section_Lists->item(irow, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString ia = QString::number(pSection->a);
		ui.Section_Lists->setItem(irow, 2, new QTableWidgetItem(ia));
		ui.Section_Lists->item(irow, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString ib = QString::number(pSection->b);
		ui.Section_Lists->setItem(irow, 3, new QTableWidgetItem(ib));
		ui.Section_Lists->item(irow, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString iM = QString::number(pSection->ClassM);
		ui.Section_Lists->setItem(irow, 4, new QTableWidgetItem(iM));
		ui.Section_Lists->item(irow, 4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString id = QString::number(pSection->m_id);
		ui.Section_Lists->setItem(irow, 5, new QTableWidgetItem(id));
		ui.Section_Lists->item(irow, 5)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	}
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

	//确认是否删除
	QMessageBox::StandardButton btn;
	btn = QMessageBox::question(this, "提示", "确定要删除第 " + QString::number(section_rowIndex + 1) + " 组截面数据吗？",
		QMessageBox::Yes | QMessageBox::No);
	if (btn == QMessageBox::Yes) 
	{
		if (section_rowIndex != -1)
		{
			ui.Section_Lists->removeRow(section_rowIndex);
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
	int section_row = ui.Section_Lists->currentRow();//取得截面信息

	int id = ui.Section_Lists->item(section_row, 5)->text().toInt();//找到这个截面的编号

	int group_row = ui.Group_Lists->currentRow() + 1;//取得单元分组信息
	QList<QTableWidgetItem*> selectedItems = ui.Group_Lists->selectedItems();

	// 存储选中行的索引
	QSet<int> selectedRows;

	// 提取行号
	for (const auto* item : selectedItems) {
		int row = item->row()+1;
		selectedRows.insert(row);
	}

	Part->AssginSectionGroup(selectedRows, id);
	m_pInterFace->SubstaceActor(Part);

}

//点“x”关闭事件
void Assign_Section::closeEvent(QCloseEvent* e)
{
	m_pInterFace->m_Renderer->RemoveActor(linesactor);
}



