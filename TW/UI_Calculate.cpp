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

	connect(ui.btn_caculateIce, &QPushButton::clicked, this, &Instance_Calculate::on_btk_ok_clicked_ice);
	connect(ui.btn_displayIce, &QPushButton::clicked, this, &Instance_Calculate::visual_ice);
	connect(ui.btn_CaculateModelIce, &QPushButton::clicked, this, &Instance_Calculate::btn_CaculateModelIce);

}

void Instance_Calculate::Set_headertext()
{
	QStringList headertext;//��ͷ
	headertext << "����" << "�����ļ�" << "˵��";
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
	QString str = QFileDialog::getOpenFileName(this, "��", "/", "textfile(*.txt);;All file(*.*)");
	if (str == nullptr)
	{
		std::cout << "open failed\n";
		return;
	}
	qDebug() << str;

	//����ʵ��
	TowerWireGroup* towerWireGroup = new TowerWireGroup;
	towerWireGroup->m_id = m_InterFace->TWG.size() + 1;
	m_InterFace->TWG.Add_Entity(towerWireGroup);
	m_ins = (Instance*)towerWireGroup;
	if (m_ins->s) delete m_ins->s;//�ͷ�֮ǰ�Ķ���
	m_ins->s = GetStructure();//����structure�����
	m_ins->s->Input_Standard(str);

	//���������ӵ�����
	m_ins->s->transToNodeBase(m_Nodes);
	m_ins->s->transToElementBase(m_LineElements);

	//����ϵת��
	m_ins->s->TransCoordinateSystem();
	emit msg_CreateModel();

	//������Ч����
	m_ins->s->DataFilter();

	m_ins->s->setElementType(S_InterFace::Beam_CR);

	QString FilePath("./");//�������ļ�·��
	//ins->s->set_outInfo(FilePath, towercaculate->IdNode);//��һ����Ҫ�����·���ĵڶ�������
	clock_t start, end;//��ʱ
	start = clock();
	m_ins->s->execute_Stepping();//����
	end = clock();
	int Totalit = m_ins->s->getTotalIterations();
	//qDebug() << "�ܵ��������� " << Totalit;
	double rtime = (end - start);
	qDebug() << "�����ܺ�ʱ�� " << rtime << " ms";
	update();
}
void Instance_Calculate::btn_CaculateModelIce()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("ѡ�������ļ�"), "", tr("TXT(*.txt)"));
	if (fileName.isEmpty())
	{
		qDebug() << "�ļ�ѡ��·������";
		return;
	}
	//����ʵ��
	TowerWireGroup* towerWireGroup = new TowerWireGroup;
	towerWireGroup->m_id = m_InterFace->TWG.size() + 1;
	m_InterFace->TWG.Add_Entity(towerWireGroup);
	m_ins = (Instance*)towerWireGroup;
	if (m_ins->s_ice) delete m_ins->s_ice;//�ͷ�֮ǰ�Ķ���
	m_ins->s_ice = GetStructure_ice();//����structure�����
	m_ins->s_ice->Input_FemData(fileName);
	////���������ӵ�����
	CreateActor_ice();
	clock_t start, end;//��ʱ
	start = clock();
	m_ins->s_ice->Solve();//����
	//�������
	end = clock();
	double rtime = (end - start);
	qDebug() << "�����ܺ�ʱ�� " << rtime << " ms";
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
		if (ins->s) delete ins->s;//�ͷ�֮ǰ�Ķ���
		ins->s = GetStructure();//����structure�����
		ins->s->Input_Standard(str);
		//����ϵת��
		ins->s->TransCoordinateSystem();
		//������Ч����
		ins->s->DataFilter();
		ins->s->setElementType(S_InterFace::Beam_CR);
	}
	else
	{
		cout << "str = nullptr!" << "\n";
		return;
	}
	QString FilePath("./");//�������ļ�·��
	clock_t start, end;//��ʱ
	start = clock();
	ins->s->execute_Stepping();//����
	end = clock();
	int Totalit = ins->s->getTotalIterations();
	double rtime = (end - start);
	qDebug() << "�����ܺ�ʱ�� " << rtime << " ms";
}

void Instance_Calculate::on_btk_ok_clicked_ice()
{
	int index = ui.tableWidget->currentRow();
	Instance* ins = nullptr;
	ins = list_Instance[index];
	if (!ins) return;
	QString str = ins->m_name;
	QString FilePath("./");//�������ļ�·��
	if (str != nullptr)
	{
		qDebug() << str;
		if (ins->s_ice) delete ins->s_ice;//�ͷ�֮ǰ�Ķ���

		ins->s_ice = GetStructure_ice();//����structure�����
		ins->s_ice->Input_FemData(str);
	}
	else
	{
		cout << "str = nullptr!" << "\n";
		return;
	}

	clock_t start, end;//��ʱ
	start = clock();
	ins->s_ice->Solve();//����

	//�������
	end = clock();
	double rtime = (end - start);
	qDebug() << "�����ܺ�ʱ�� " << rtime << " ms";
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

void Instance_Calculate::visual_ice()
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
	std::list<std::vector<double>> nodes= ins->s_ice->Get_node_ice();
	display->addData_ice(nodes, ins);
}

void Instance_Calculate::update()
{
	int T_tower = m_InterFace->TP.size();//��ʵ������
	int T_wire = m_InterFace->TWG.size();//��ʵ������

	ui.tableWidget->setRowCount(T_tower + T_wire);//��������

	QVector<Tower*> ve_tower;//!
	m_InterFace->TP.Get_Array(ve_tower, true);//!

	QVector<TowerWireGroup*> ve_wire;//!
	m_InterFace->TWG.Get_Array(ve_wire, true);//!

	for (int i = 0; i < T_tower; i++)
	{
		Tower* tower = ve_tower[i];//!
		list_Instance.push_back(tower);

		//��������
		QString name = tower->m_name;
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem("InstanceTower"));
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(name));
		for (int j = 0; j < 2; j++)//��ͷ;
		{
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}

	for (int i = 0; i < T_wire; i++)
	{
		TowerWireGroup* wire = ve_wire[i];//!
		list_Instance.push_back(wire);

		//��������
		QString name = wire->m_name;
		ui.tableWidget->setItem(T_tower + i, 0, new QTableWidgetItem("InstanceWire"));
		ui.tableWidget->setItem(T_tower + i, 1, new QTableWidgetItem(name));
		for (int j = 0; j < 2; j++)//��ͷ;
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

	//��
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

void Instance_Calculate::CreateActor_ice()
{
	vtkSmartPointer<vtkPolyData> PolyData = vtkSmartPointer<vtkPolyData>::New();
	m_ins->m_pts = vtkSmartPointer<vtkPoints>::New();
	std::list<std::vector<double>> nodes = m_ins->s_ice->Get_node_ice1();

	for (auto& i : nodes)
	{
		m_nodes.push_back(Node(i[0], i[1], i[2], i[3], 0.));
	}
	for (const auto& i : m_nodes)
	{
		double x = i.x;
		double y = i.y;
		double z = i.z;
		vtkIdType pid = i.m_idNode;
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

	// ��
	m_ins->m_lines = vtkSmartPointer<vtkCellArray>::New();
	std::list<std::vector<int>> truss = m_ins->s_ice->Get_truss_ice();

	for (auto& i : truss)
	{
		m_truss.push_back(Element_Truss(0., i[0], i[1], 0, 0.));
	}
	for (const auto& i : m_truss)
	{
		vtkIdType id1 = i.m_idNode[0] - 1;
		vtkIdType id2 = i.m_idNode[1] - 1;
		vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
		line->GetPointIds()->SetId(0, id1);
		line->GetPointIds()->SetId(1, id2);
		m_ins->m_lines->InsertNextCell(line);
	}
	std::list<std::vector<int>> beam = m_ins->s_ice->Get_beam_ice();

	for (auto& i : beam)
	{
		m_beam.push_back(Element_Truss(0., i[0], i[1], 0, 0.));
	}
	for (const auto& i : m_beam)
	{
		vtkIdType id1 = i.m_idNode[0] - 1;
		vtkIdType id2 = i.m_idNode[1] - 1;

		vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
		line->GetPointIds()->SetId(0, id1);
		line->GetPointIds()->SetId(1, id2);
		m_ins->m_lines->InsertNextCell(line);
	}
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
