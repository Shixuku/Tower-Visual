#include "UI_Calculate.h"
#include"TowerWireGroup.h"

Instance_Calculate::Instance_Calculate(InterFace* InterFace, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_InterFace = InterFace;
	Set_headertext();
	connect(ui.btn_ok, &QPushButton::clicked, this, &Instance_Calculate::on_btk_ok_clicked);
	connect(ui.btn_show, &QPushButton::clicked, this, &Instance_Calculate::visual);

	connect(ui.ptn_import, &QPushButton::clicked, this, &Instance_Calculate::on_btn_import_clicked);
	connect(this, &Instance_Calculate::msg_CreateModel, this, &Instance_Calculate::CreateActor);

}

void Instance_Calculate::Set_headertext()
{
	QStringList headertext;//表头
	headertext << "名称" << "计算文件" << "说明";
	ui.tableWidget->setColumnCount(headertext.count());
	ui.tableWidget->setHorizontalHeaderLabels(headertext);
	ui.tableWidget->verticalHeader()->setVisible(false);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

	update();
}

Instance_Calculate::~Instance_Calculate()
{}

void Instance_Calculate::on_btn_import_clicked()
{
	QString str = QFileDialog::getOpenFileName(this, "打开", "/", "textfile(*.txt);;All file(*.*)");
	if (str == nullptr)
	{
		std::cout << "open failed\n";
		return;
	}
	qDebug() << str;

	//创建实例
	TowerWireGroup* towerWireGroup = new TowerWireGroup;
	//towerWireGroup->Item = item;
	towerWireGroup->m_id = m_InterFace->TWG.size() + 1;
	m_InterFace->TWG.Add_Entity(towerWireGroup);

	m_ins = (Instance*)towerWireGroup;

	if (m_ins->s) delete m_ins->s;//释放之前的对象
	m_ins->s = GetStructure();//生成structure类对象
	m_ins->s->Input_Standard(str);

	//将点和线添加到界面
	m_ins->s->transToNodeBase(m_Nodes);
	m_ins->s->transToElementBase(m_LineElements);


	//坐标系转换
	m_ins->s->TransCoordinateSystem();
	emit msg_CreateModel();

	//过滤无效数据
	m_ins->s->DataFilter();

	m_ins->s->setElementType(S_InterFace::Beam_CR);

	QString FilePath("./");//计算结果文件路径
	//ins->s->set_outInfo(FilePath, towercaculate->IdNode);//这一步需要，更新范理的第二个参数
	clock_t start, end;//计时
	start = clock();
	m_ins->s->execute_Stepping();//计算
	end = clock();
	int Totalit = m_ins->s->getTotalIterations();
	//qDebug() << "总迭代次数： " << Totalit;
	double rtime = (end - start);
	qDebug() << "计算总耗时： " << rtime << " ms";

	update();
}
void Instance_Calculate::on_btk_ok_clicked()
{
	int index = ui.tableWidget->currentRow();
	Instance* ins = nullptr;
	ins = list_Instance[index];
	if (!ins) return;
	QString str = ins->m_name;
	if (str != nullptr)
	{
		qDebug() << str;
		if (ins->s) delete ins->s;//释放之前的对象
		ins->s = GetStructure();//生成structure类对象
		ins->s->Input_Standard(str);

		//坐标系转换
		ins->s->TransCoordinateSystem();

		////将点和线添加到界面
		//ins->s->transToNodeBase(m_Nodes);
		//ins->s->transToElementBase(m_LineElements);

		//过滤无效数据
		ins->s->DataFilter();

		ins->s->setElementType(S_InterFace::Beam_CR);
	}
	else
	{
		cout << "str = nullptr!" << "\n";
		return;
	}
	QString FilePath("./");//计算结果文件路径
	//ins->s->set_outInfo(FilePath, towercaculate->IdNode);//这一步需要，更新范理的第二个参数
	clock_t start, end;//计时
	start = clock();
	ins->s->execute_Stepping();//计算
	end = clock();
	int Totalit = ins->s->getTotalIterations();
	//qDebug() << "总迭代次数： " << Totalit;
	double rtime = (end - start);
	qDebug() << "计算总耗时： " << rtime << " ms";
}

void Instance_Calculate::visual()
{
	if (!display)
	{
		display = new resultVisualize(m_InterFace);
	}
	display->show();
	int index = ui.tableWidget->currentRow();
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
}

void Instance_Calculate::update()
{
	int T_tower = m_InterFace->TP.size();//塔实例数量
	int T_wire = m_InterFace->TWG.size();//线实例数量

	ui.tableWidget->setRowCount(T_tower + T_wire);//设置行数

	QVector<Tower*> ve_tower;//!
	m_InterFace->TP.Get_Array(ve_tower, true);//!

	QVector<TowerWireGroup*> ve_wire;//!
	m_InterFace->TWG.Get_Array(ve_wire, true);//!

	for (int i = 0; i < T_tower; i++)
	{
		Tower* tower = ve_tower[i];//!
		list_Instance.push_back(tower);

		//设置名称
		QString name = tower->m_name;
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem("InstanceTower"));
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(name));
		for (int j = 0; j < 2; j++)//表头;
		{
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}

	for (int i = 0; i < T_wire; i++)
	{
		TowerWireGroup* wire = ve_wire[i];//!
		list_Instance.push_back(wire);

		//设置名称
		QString name = wire->m_name;
		ui.tableWidget->setItem(T_tower + i, 0, new QTableWidgetItem("InstanceWire"));
		ui.tableWidget->setItem(T_tower + i, 1, new QTableWidgetItem(name));
		for (int j = 0; j < 2; j++)//表头;
		{
			ui.tableWidget->item(T_tower + i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}

}

void Instance_Calculate::CreateActor()
{
	//vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();
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
		//		vertices->InsertNextCell(1, pid);	
	}

	PolyData->SetPoints(m_ins->m_pts);
	//	PolyData->SetVerts(vertices);

	vtkNew<vtkVertexGlyphFilter> VertexFilter;
	VertexFilter->SetInputData(PolyData);
	VertexFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(VertexFilter->GetOutputPort());
	//	mapper->SetInputData(PolyData);
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
	//点数据(pts)所定义的一系列点构成数据集(vtkDataSet)的几何结构
	linesPolyData->SetPoints(m_ins->m_pts);
	//点数据的连接形式(lines)形成的单元数据(cell Data)构成数据集拓扑结构
	linesPolyData->SetLines(m_ins->m_lines);
	vtkSmartPointer<vtkPolyDataMapper> linesmapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	linesmapper->SetInputData(linesPolyData);

	//vtkSmartPointer<vtkActor> linesactor = vtkSmartPointer<vtkActor>::New();
	m_ins->m_TrussActor = vtkSmartPointer<vtkActor>::New();
	m_ins->m_TrussActor->SetMapper(linesmapper);
	m_ins->m_TrussActor->GetProperty()->SetColor(0, 0, 0);
	m_InterFace->m_Renderer->AddActor(m_ins->m_TrussActor);
	m_InterFace->m_Renderer->ResetCamera();
	m_InterFace->m_renderWindow->Render();
}
