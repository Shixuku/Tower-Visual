#include "UI_Calculate.h"
#include"TowerWireGroup.h"
#include<QMessageBox.h>
Instance_Calculate::Instance_Calculate(InterFace* InterFace, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_InterFace = InterFace;
	Set_headertext();
	connect(ui.btn_Wind, &QPushButton::clicked, this, &Instance_Calculate::on_btk_ok_clicked);
	connect(ui.btn_showWind, &QPushButton::clicked, this, &Instance_Calculate::visual);
	connect(ui.btn_InputWind, &QPushButton::clicked, this, &Instance_Calculate::on_btn_import_clicked);
	connect(this, &Instance_Calculate::msg_CreateModel, this, &Instance_Calculate::CreateActor);

}

void Instance_Calculate::Set_headertext()
{
	QStringList headertext;//表头
	headertext << "名称" << "计算文件" ;
	ui.tableWidget->setColumnCount(headertext.count());
	ui.tableWidget->setHorizontalHeaderLabels(headertext);
	ui.tableWidget->verticalHeader()->setVisible(false);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

}

Instance_Calculate::~Instance_Calculate()
{}

void Instance_Calculate::on_btn_import_clicked()
{
	m_InterFace->m_Renderer->RemoveAllViewProps();
	m_str = QFileDialog::getOpenFileName(this, "打开", "/", "textfile(*.txt);;All file(*.*)");
	if (m_str == nullptr)
	{
		std::cout << "open failed\n";
		return;
	}
	int lastSeparatorIndex = m_str.lastIndexOf('/');
	if (lastSeparatorIndex == -1)	lastSeparatorIndex = m_str.lastIndexOf('\\');
	// 提取文件名部分
	m_fileName = m_str.mid(lastSeparatorIndex + 1);
	//创建实例
	TowerWireGroup* towerWireGroup = new TowerWireGroup;
	towerWireGroup->m_id = m_InterFace->TWG.size() + 1;
	//加入名称
	towerWireGroup->m_name = "导入模型 " + QString::number(m_InterFace->TWG.size() + 1);
	towerWireGroup->m_filename = m_str;//文件名
	m_InterFace->TWG.Add_Entity(towerWireGroup);
	m_ins = (Instance*)towerWireGroup;
	if (m_ins->s) delete m_ins->s;//释放之前的对象
	m_ins->s = GetStructure();//生成structure类对象
	m_ins->s->set_ID(m_ins->m_id);
	m_ins->s->AddToStructures();

	m_ins->s->Input_Standard(m_str);

	m_ins->s->set_Name(m_ins->m_name);

	//将点和线添加到界面
	m_ins->s->transToNodeBase(m_Nodes);
	m_ins->s->transToElementBase(m_LineElements);

	//坐标系转换
	m_ins->s->TransCoordinateSystem();
	emit msg_CreateModel();

	//过滤无效数据
	m_ins->s->DataFilter();

	m_ins->s->setElementType(S_InterFace::Beam_CR);

	update();

	clock_t start, end;//计时
	start = clock();
	m_ins->s->execute_Stepping();//计算
	end = clock();
	int Totalit = m_ins->s->getTotalIterations();
	double rtime = (end - start);
	qDebug() << "计算总耗时： " << rtime << " ms";
	m_InterFace->ui.textEdit->setText("计算结束！");

}

//void Instance_Calculate::btn_CaculateModelIce()//更新的鲁佳帛的动态链接库
//{
//	m_InterFace->m_Renderer->RemoveAllViewProps();
//	m_str = QFileDialog::getOpenFileName(this, "打开", "/", "textfile(*.txt);;All file(*.*)");
//	if (m_str == nullptr)
//	{
//		std::cout << "open failed\n";
//		return;
//	}
//	int lastSeparatorIndex = m_str.lastIndexOf('/');
//	if (lastSeparatorIndex == -1)	lastSeparatorIndex = m_str.lastIndexOf('\\');
//	// 提取文件名部分
//	m_fileName = m_str.mid(lastSeparatorIndex + 1);
//	//创建实例
//	TowerWireGroup* towerWireGroup = new TowerWireGroup;
//	towerWireGroup->m_id = m_InterFace->TWG.size() + 1;
//	//加入名称
//	towerWireGroup->m_name = "导入模型 " + QString::number(m_InterFace->TWG.size() + 1);
//	towerWireGroup->m_filename = m_str;//文件名
//	m_InterFace->TWG.Add_Entity(towerWireGroup);
//	m_ins = (Instance*)towerWireGroup;
//	if (m_ins->s_ice) delete m_ins->s_ice;//释放之前的对象
//	m_ins->s_ice = GetStructure_ice();//生成structure类对象
//
//	m_ins->s_ice->set_id(m_ins->m_id);
//	m_ins->s_ice->AddToStructures();
//
//	m_ins->s_ice->Input_FemData(m_str);
//	m_ins->s_ice->set_Name(m_ins->m_name);
//	//CreateActor_ice();////将点和线添加到界面
//
//	update();
//	clock_t start, end;//计时
//	start = clock();
//	m_ins->s_ice->Solve();//计算
//	//获得数据
//	end = clock();
//	double rtime = (end - start);
//	qDebug() << "计算总耗时： " << rtime << " ms";
//	m_InterFace->ui.textEdit->setText("计算结束！");
//}

void Instance_Calculate::showEvent(QShowEvent* event)
{
	update();

	QWidget::showEvent(event);

}

void Instance_Calculate::on_btk_ok_clicked()
{
	int index = ui.tableWidget->currentRow();
	m_ins = list_Instance[index];
	if (!m_ins) return;
	QString str = m_ins->m_filename;
	if (str != nullptr)
	{
		qDebug() << str;
		if (m_ins->s) delete m_ins->s;//释放之前的对象
		m_ins->s = GetStructure();//生成structure类对象
		m_ins->s->set_ID(m_ins->m_id);
		m_ins->s->AddToStructures();

		m_ins->s->Input_Standard(str);

		m_ins->s->set_Name(m_ins->m_name);

		//坐标系转换
		m_ins->s->TransCoordinateSystem();

		//过滤无效数据
		m_ins->s->DataFilter();

		m_ins->s->setElementType(S_InterFace::Beam_CR);
	}
	else
	{
		cout << "str = nullptr!" << "\n";
		return;
	}
	QString FilePath("./");//计算结果文件路径
	clock_t start, end;//计时
	start = clock();
	m_ins->s->execute_Stepping();//计算
	end = clock();
	int Totalit = m_ins->s->getTotalIterations();
	double rtime = (end - start);
	qDebug() << "计算总耗时： " << rtime << " ms";
	//计算结束的提醒
	m_InterFace->ui.textEdit->setText("计算结束！");
}

void Instance_Calculate::visual()
{
	int index = ui.tableWidget->currentRow();
	m_InterFace->ui.textEdit->clear();
	if (index < 0)
	{
		QMessageBox::information(this, "Tips", "请选择要显示的风计算！"); return;
	}
	else
	{
		if (!display)
		{
			display = new resultVisualize(m_InterFace);
		}
		this->accept();//把原来的界面关闭
		display->show();
		Instance* ins = nullptr;
		ins = list_Instance[index];
		if (!ins) return;
		vector<Node_Base*> ptr_nodes = ins->s->GetNodes();
		std::list<std::vector<double>> nodes;
		for (auto& i : ptr_nodes)
		{
			std::vector<double> coor(3);
			coor = { i->m_x,i->m_y,i->m_z };
			nodes.push_back(coor);
		}
		display->addData(nodes, ins);
		display->start();
	}
}

void Instance_Calculate::update()
{
	int T_tower = m_InterFace->TP.size();//塔实例数量
	int T_wire = m_InterFace->TWG.size();//线+塔线实例数量

	ui.tableWidget->setRowCount(T_tower + T_wire);//设置行数

	QVector<Tower*> ve_tower;
	m_InterFace->TP.Get_Array(ve_tower, true);
	QVector<TowerWireGroup*> ve_wire;
	m_InterFace->TWG.Get_Array(ve_wire, true);

	for (int i = 0; i < T_tower; i++)
	{
		Tower* tower = ve_tower[i];
		list_Instance.push_back(tower);
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(tower->m_name));//名称
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(tower->m_filename));//计算文件
		for (int j = 0; j < 2; j++)//表头;
		{
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}
		
	for (int i = 0; i < T_wire; i++)
	{
		TowerWireGroup* wire = ve_wire[i];//!
		list_Instance.push_back(wire);
		ui.tableWidget->setItem(T_tower + i, 0, new QTableWidgetItem(wire->m_name));//名称
		ui.tableWidget->setItem(T_tower + i, 1, new QTableWidgetItem(wire->m_filename));//计算文件
		for (int j = 0; j < 2; j++)//表头;
		{
			ui.tableWidget->item(T_tower + i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}

}

void Instance_Calculate::CreateActor()
{
	vtkSmartPointer<vtkPolyData> PolyData = vtkSmartPointer<vtkPolyData>::New();
	m_ins->m_pts = vtkSmartPointer<vtkPoints>::New();
	for (const auto& i : m_Nodes)
	{
		const Node_Base* pNode = i.second;
		double x = pNode->m_x;
		double y = pNode->m_y;
		double z = pNode->m_z;
		vtkIdType pid = pNode->m_id;
		m_ins->m_pts->InsertPoint(pid - 1, x, y, z);	
	}

	PolyData->SetPoints(m_ins->m_pts);

	vtkNew<vtkVertexGlyphFilter> VertexFilter;
	VertexFilter->SetInputData(PolyData);
	VertexFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(VertexFilter->GetOutputPort());
	m_ins->Node_actor = vtkSmartPointer<vtkActor>::New();
	m_ins->Node_actor->SetMapper(mapper);
	m_ins->Node_actor->GetProperty()->SetColor(0, 0, 0);
	m_ins->Node_actor->GetProperty()->SetPointSize(3);
	m_InterFace->m_Renderer->AddActor(m_ins->Node_actor);
	m_InterFace->m_renderWindow->Render();

	//线
	m_ins->m_lines = vtkSmartPointer<vtkCellArray>::New();
	for (const auto& i : m_LineElements)
	{
		const Element_Base* pElement = i.second;

		vtkIdType id1 = pElement->m_idNode[0] - 1;
		vtkIdType id2 = pElement->m_idNode[1] - 1;

		vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();

		line->GetPointIds()->SetId(0, id1);
		line->GetPointIds()->SetId(1, id2);
		m_ins->m_lines->InsertNextCell(line);
	}
	//create the visualized line
	vtkSmartPointer<vtkPolyData> linesPolyData = vtkSmartPointer<vtkPolyData>::New();
	linesPolyData->SetPoints(m_ins->m_pts);
	linesPolyData->SetLines(m_ins->m_lines);
	vtkSmartPointer<vtkPolyDataMapper> linesmapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	linesmapper->SetInputData(linesPolyData);

	m_ins->m_TrussActor = vtkSmartPointer<vtkActor>::New();
	m_ins->m_TrussActor->SetMapper(linesmapper);
	m_ins->m_TrussActor->GetProperty()->SetColor(0, 0, 0);
	m_InterFace->m_Renderer->AddActor(m_ins->m_TrussActor);
	m_InterFace->m_Renderer->ResetCamera();
	m_InterFace->m_renderWindow->Render();
}

