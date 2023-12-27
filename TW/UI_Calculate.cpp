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
	QStringList headertext;//��ͷ
	headertext << "����" << "�����ļ�" ;
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
	m_str = QFileDialog::getOpenFileName(this, "��", "/", "textfile(*.txt);;All file(*.*)");
	if (m_str == nullptr)
	{
		std::cout << "open failed\n";
		return;
	}
	int lastSeparatorIndex = m_str.lastIndexOf('/');
	if (lastSeparatorIndex == -1)	lastSeparatorIndex = m_str.lastIndexOf('\\');
	// ��ȡ�ļ�������
	m_fileName = m_str.mid(lastSeparatorIndex + 1);
	//����ʵ��
	TowerWireGroup* towerWireGroup = new TowerWireGroup;
	towerWireGroup->m_id = m_InterFace->TWG.size() + 1;
	//��������
	towerWireGroup->m_name = "����ģ�� " + QString::number(m_InterFace->TWG.size() + 1);
	towerWireGroup->m_filename = m_str;//�ļ���
	m_InterFace->TWG.Add_Entity(towerWireGroup);
	m_ins = (Instance*)towerWireGroup;
	if (m_ins->s) delete m_ins->s;//�ͷ�֮ǰ�Ķ���
	m_ins->s = GetStructure();//����structure�����
	m_ins->s->set_ID(m_ins->m_id);
	m_ins->s->AddToStructures();

	m_ins->s->Input_Standard(m_str);

	m_ins->s->set_Name(m_ins->m_name);

	//���������ӵ�����
	m_ins->s->transToNodeBase(m_Nodes);
	m_ins->s->transToElementBase(m_LineElements);

	//����ϵת��
	m_ins->s->TransCoordinateSystem();
	emit msg_CreateModel();

	//������Ч����
	m_ins->s->DataFilter();

	m_ins->s->setElementType(S_InterFace::Beam_CR);

	update();

	clock_t start, end;//��ʱ
	start = clock();
	m_ins->s->execute_Stepping();//����
	end = clock();
	int Totalit = m_ins->s->getTotalIterations();
	double rtime = (end - start);
	qDebug() << "�����ܺ�ʱ�� " << rtime << " ms";
	m_InterFace->ui.textEdit->setText("���������");

}

//void Instance_Calculate::btn_CaculateModelIce()//���µ�³�Ѳ��Ķ�̬���ӿ�
//{
//	m_InterFace->m_Renderer->RemoveAllViewProps();
//	m_str = QFileDialog::getOpenFileName(this, "��", "/", "textfile(*.txt);;All file(*.*)");
//	if (m_str == nullptr)
//	{
//		std::cout << "open failed\n";
//		return;
//	}
//	int lastSeparatorIndex = m_str.lastIndexOf('/');
//	if (lastSeparatorIndex == -1)	lastSeparatorIndex = m_str.lastIndexOf('\\');
//	// ��ȡ�ļ�������
//	m_fileName = m_str.mid(lastSeparatorIndex + 1);
//	//����ʵ��
//	TowerWireGroup* towerWireGroup = new TowerWireGroup;
//	towerWireGroup->m_id = m_InterFace->TWG.size() + 1;
//	//��������
//	towerWireGroup->m_name = "����ģ�� " + QString::number(m_InterFace->TWG.size() + 1);
//	towerWireGroup->m_filename = m_str;//�ļ���
//	m_InterFace->TWG.Add_Entity(towerWireGroup);
//	m_ins = (Instance*)towerWireGroup;
//	if (m_ins->s_ice) delete m_ins->s_ice;//�ͷ�֮ǰ�Ķ���
//	m_ins->s_ice = GetStructure_ice();//����structure�����
//
//	m_ins->s_ice->set_id(m_ins->m_id);
//	m_ins->s_ice->AddToStructures();
//
//	m_ins->s_ice->Input_FemData(m_str);
//	m_ins->s_ice->set_Name(m_ins->m_name);
//	//CreateActor_ice();////���������ӵ�����
//
//	update();
//	clock_t start, end;//��ʱ
//	start = clock();
//	m_ins->s_ice->Solve();//����
//	//�������
//	end = clock();
//	double rtime = (end - start);
//	qDebug() << "�����ܺ�ʱ�� " << rtime << " ms";
//	m_InterFace->ui.textEdit->setText("���������");
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
		if (m_ins->s) delete m_ins->s;//�ͷ�֮ǰ�Ķ���
		m_ins->s = GetStructure();//����structure�����
		m_ins->s->set_ID(m_ins->m_id);
		m_ins->s->AddToStructures();

		m_ins->s->Input_Standard(str);

		m_ins->s->set_Name(m_ins->m_name);

		//����ϵת��
		m_ins->s->TransCoordinateSystem();

		//������Ч����
		m_ins->s->DataFilter();

		m_ins->s->setElementType(S_InterFace::Beam_CR);
	}
	else
	{
		cout << "str = nullptr!" << "\n";
		return;
	}
	QString FilePath("./");//�������ļ�·��
	clock_t start, end;//��ʱ
	start = clock();
	m_ins->s->execute_Stepping();//����
	end = clock();
	int Totalit = m_ins->s->getTotalIterations();
	double rtime = (end - start);
	qDebug() << "�����ܺ�ʱ�� " << rtime << " ms";
	//�������������
	m_InterFace->ui.textEdit->setText("���������");
}

void Instance_Calculate::visual()
{
	int index = ui.tableWidget->currentRow();
	m_InterFace->ui.textEdit->clear();
	if (index < 0)
	{
		QMessageBox::information(this, "Tips", "��ѡ��Ҫ��ʾ�ķ���㣡"); return;
	}
	else
	{
		if (!display)
		{
			display = new resultVisualize(m_InterFace);
		}
		this->accept();//��ԭ���Ľ���ر�
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
	int T_tower = m_InterFace->TP.size();//��ʵ������
	int T_wire = m_InterFace->TWG.size();//��+����ʵ������

	ui.tableWidget->setRowCount(T_tower + T_wire);//��������

	QVector<Tower*> ve_tower;
	m_InterFace->TP.Get_Array(ve_tower, true);
	QVector<TowerWireGroup*> ve_wire;
	m_InterFace->TWG.Get_Array(ve_wire, true);

	for (int i = 0; i < T_tower; i++)
	{
		Tower* tower = ve_tower[i];
		list_Instance.push_back(tower);
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(tower->m_name));//����
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(tower->m_filename));//�����ļ�
		for (int j = 0; j < 2; j++)//��ͷ;
		{
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}
		
	for (int i = 0; i < T_wire; i++)
	{
		TowerWireGroup* wire = ve_wire[i];//!
		list_Instance.push_back(wire);
		ui.tableWidget->setItem(T_tower + i, 0, new QTableWidgetItem(wire->m_name));//����
		ui.tableWidget->setItem(T_tower + i, 1, new QTableWidgetItem(wire->m_filename));//�����ļ�
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

