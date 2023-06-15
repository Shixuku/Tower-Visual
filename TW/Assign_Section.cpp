#include "Assign_Section.h"
#pragma execution_character_set("utf-8")
#include"InterFace.h"
#include<QMessageBox.h>
#include<QTextCodec>
#include<QStringLiteral>
Assign_Section::Assign_Section(InterFace* InterFace, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);


	m_pInterFace = InterFace;
	
	Q_ASSERT(m_pInterFace != nullptr);

	Create_combobox();
	ma_ab2 = new Set_Section;
	table_Section_Lists();//设置截面表格
	Add_created_section();//添加已经创建的截面

	//connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ShowComboVTK()));
	void (QComboBox:: * intChanged)(int) = &QComboBox::currentIndexChanged;
	connect(ui.comboBox, intChanged, this, &Assign_Section::ShowComboVTK);

	linesactor = vtkSmartPointer<vtkActor>::New();

	//显示创建截面对话框
	connect(ui.btn_add, &QPushButton::clicked, this, [=](){ma_ab2->show();});
	//添加截面
	connect(ma_ab2, SIGNAL(SendData(QStringList&)), this, SLOT(GetData(QStringList&)));
	//删除已添加的截面
	connect(ui.btn_delete, &QPushButton::clicked, this, &Assign_Section::table_Section_Lists_delete);

	//点击组数，会显示高亮（在原来的基础上新建一个actor）
	//connect(ui.Group_Lists, &QTableWidget::itemClicked, this, &Assign_Section::highlight_actor);

	//指派截面
	//connect(ui.btn_assign, &QPushButton::clicked, this, &Assign_Section::assigh_section);

	//点击完成按钮，会删除高亮的actor
	connect(ui.btn_finish, &QPushButton::clicked, this, [=]()
		{
			m_pInterFace->m_Renderer->RemoveActor(linesactor);
			int size = m_pInterFace->TP_leg.Find_Entity(id_Part)->m_Elements_beams.size();
			for (int i = 0; i < size; i++)
			{
				cout << "梁的方向" << m_pInterFace->TP_leg.Find_Entity(id_Part)->m_Elements_beams[i].direction[0] << "  "
				<< m_pInterFace->TP_leg.Find_Entity(id_Part)->m_Elements_beams[i].direction[1] << "  "
				<< m_pInterFace->TP_leg.Find_Entity(id_Part)->m_Elements_beams[i].direction[2] << "\n";
			}	
			this->reject();
		});

}

Assign_Section::~Assign_Section()
{

}

void Assign_Section::ShowComboVTK()
{
	
	T_legs = m_pInterFace->ui.treeWidget->topLevelItem(0)->childCount();//塔腿数量
	T_bodys = m_pInterFace->ui.treeWidget->topLevelItem(1)->childCount();//塔身数量
	T_heads = m_pInterFace->ui.treeWidget->topLevelItem(2)->childCount();//塔头数量
	int Index = ui.comboBox->currentIndex();
	if (Index < T_legs)
	{
		id_Part = Index + 1;
		//return m_pInterFace->TP_leg.Find_Entity(id_Part);
		m_pInterFace->Show_Part(m_pInterFace->TP_leg.Find_Entity(id_Part));
		table_Group_Lists(m_pInterFace->TP_leg.Find_Entity(id_Part));
		//点击组数，会显示高亮（在原来的基础上新建一个actor）
		connect(ui.Group_Lists, &QTableWidget::itemClicked, this, [=]()
			{highlight_actor(m_pInterFace->TP_leg.Find_Entity(id_Part));
		m_pInterFace->m_renderWindow->Render(); });

		//指派截面
		connect(ui.btn_assign, &QPushButton::clicked, this, [=]() 
			{assigh_section(m_pInterFace->TP_leg.Find_Entity(id_Part)); });

	}	
	else if (Index < T_legs+ T_bodys)
	{
		id_Part = Index + 1- T_legs;
		m_pInterFace->Show_Part(m_pInterFace->TP_body.Find_Entity(id_Part));
		table_Group_Lists(m_pInterFace->TP_body.Find_Entity(id_Part));
		connect(ui.Group_Lists, &QTableWidget::itemClicked, this, [=]() 
			{highlight_actor(m_pInterFace->TP_body.Find_Entity(id_Part)); });
		//指派截面
		connect(ui.btn_assign, &QPushButton::clicked, this, [=]()
			{assigh_section(m_pInterFace->TP_body.Find_Entity(id_Part)); });
	}
	else if (Index < T_legs + T_bodys+ T_heads)
	{
		id_Part = Index + 1 - T_legs- T_bodys;
		m_pInterFace->Show_Part(m_pInterFace->TP_CrossArm.Find_Entity(id_Part));
		connect(ui.Group_Lists, &QTableWidget::itemClicked, this, [=]() 
			{highlight_actor(m_pInterFace->TP_CrossArm.Find_Entity(id_Part)); });
		//指派截面
		connect(ui.btn_assign, &QPushButton::clicked, this, [=]()
			{assigh_section(m_pInterFace->TP_CrossArm.Find_Entity(id_Part)); });
	}


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
	for (int j = 0; j < m_pInterFace->MS.size(); j++)
	{
		int irow = ui.Section_Lists->rowCount();
		ui.Section_Lists->setRowCount(irow + 1);

		QString QName = QString::fromStdString(m_pInterFace->MS[j].Name);
		ui.Section_Lists->setItem(irow, 0, new QTableWidgetItem(QName));
		ui.Section_Lists->item(irow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString s;
		if (m_pInterFace->MS[j].ClassSe == 0)
		{
			s = "L型";
		}
		else
		{
			s = "圆环";
		}
		ui.Section_Lists->setItem(irow, 1, new QTableWidgetItem(s));
		ui.Section_Lists->item(irow, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString ia = QString::number(m_pInterFace->MS[j].a);
		ui.Section_Lists->setItem(irow, 2, new QTableWidgetItem(ia));
		ui.Section_Lists->item(irow, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString ib = QString::number(m_pInterFace->MS[j].b);
		ui.Section_Lists->setItem(irow, 3, new QTableWidgetItem(ib));
		ui.Section_Lists->item(irow, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString iM = QString::number(m_pInterFace->MS[j].ClassM);
		ui.Section_Lists->setItem(irow, 4, new QTableWidgetItem(iM));
		ui.Section_Lists->item(irow, 4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString id = QString::number(m_pInterFace->MS[j].m_id);
		ui.Section_Lists->setItem(irow, 5, new QTableWidgetItem(id));
		ui.Section_Lists->item(irow, 5)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	}
}

void Assign_Section::Create_combobox()
{
	T_legs = m_pInterFace->ui.treeWidget->topLevelItem(0)->child(0)->childCount();//塔腿数量
	T_bodys = m_pInterFace->ui.treeWidget->topLevelItem(0)->child(1)->childCount();//塔身数量
	T_heads = m_pInterFace->ui.treeWidget->topLevelItem(0)->child(2)->childCount();//塔头数量

	for (int i = 0; i < T_legs; i++)
	{
		ui.comboBox->addItem("塔腿部件" + QString::number(i + 1));
	}
	for (int i = 0; i < T_bodys; i++)
	{
		ui.comboBox->addItem("塔身部件" + QString::number(i + 1));
	}
	for (int i = 0; i < T_heads; i++)
	{
		ui.comboBox->addItem("塔头部件" + QString::number(i + 1));
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

//功能——添加截面
void Assign_Section::GetData(QStringList& sInfo)
{
	//添加行，并且将信息放进去
	int irow = ui.Section_Lists->rowCount();
	ui.Section_Lists->setRowCount(irow + 1);

	for (int i = 0; i < headertext.count(); i++)
	{
		QString str= sInfo[i];

		ui.Section_Lists->setItem(irow, i, new QTableWidgetItem(str));
		ui.Section_Lists->item(irow, i)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		qDebug() << str;
	}

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

	int id = ui.Section_Lists->item(section_row, 5)->text().toDouble();//找到这个截面的编号

	int group_row = ui.Group_Lists->currentRow() + 1;//取得单元分组信息

	Part->AssginSectionGroup(group_row, id);
	m_pInterFace->SubstaceActor(Part);

}

//点“x”关闭事件
void Assign_Section::closeEvent(QCloseEvent* e)
{
	m_pInterFace->m_Renderer->RemoveActor(linesactor);
}



