#include "resultVisualize.h"
#include"InterFace.h"
resultVisualize::resultVisualize(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	pCAE = dynamic_cast<InterFace*>(parent);
	assert(pCAE != nullptr);

	updateTimer = new QTimer(this);

	//�Զ������Ŵ�����
	ui.lineEdit->setText("1");
	connect(ui.radioButton, &QRadioButton::clicked, this, &resultVisualize::autoFactor);
	connect(ui.radioButton, &QRadioButton::clicked, ui.lineEdit, &QLineEdit::setDisabled);
	//���¶���
	connect(updateTimer, &QTimer::timeout, this, &resultVisualize::update);
	//����
	connect(ui.play_btn, &QPushButton::clicked, this, &resultVisualize::start);
	//��ͣ
	connect(ui.stop_btn, &QPushButton::clicked, this, &resultVisualize::stop);
	//��ֹ
	connect(ui.quit_btn, &QPushButton::clicked, this, &resultVisualize::quit);

	ui.label_frame->setText(QString::number(m_frames));//֡��ǩ
	//�����ٶȻ���
	ui.speed_Slider->setRange(1, 30);
	ui.speed_Slider->setValue(30);
	m_speed = 30;
	connect(ui.speed_Slider, &QSlider::valueChanged, this, &resultVisualize::set_fps);
	//��ʾԭʼactor
	ui.checkBox_showOriginal->setChecked(true);
	connect(ui.checkBox_showOriginal, &QCheckBox::clicked, this, &resultVisualize::showOriginalActor);
	//ѭ������
	ui.checkBox_loop->setChecked(true);
	set_loopPlay(true);
	connect(ui.checkBox_loop, &QCheckBox::clicked, this, &resultVisualize::set_loopPlay);
	//��������
	connect(this, &resultVisualize::animationFinished, [=]() {	m_frames = 0; });

	//��ͼcombox
	QStringList qstrlist;
	qstrlist << "U1" << "U2" << "U3" << "S11";
	ui.comboBox_nephogram->addItems(qstrlist);
	void (QComboBox:: * MycurrentIndexChanged)(int) = &QComboBox::currentIndexChanged;
	connect(ui.comboBox_nephogram, MycurrentIndexChanged, this, &resultVisualize::setNephogramType);

	connect(ui.comboBox_step, MycurrentIndexChanged, this, &resultVisualize::setCurrentStep);
}

resultVisualize::~resultVisualize()
{}

void resultVisualize::set_fps(int fps)
{
	m_speed = fps;
}

void resultVisualize::showOriginalActor(bool flag)
{
	if (m_ins->Node_actor) m_ins->Node_actor->SetVisibility(flag);
	if (m_ins->m_BeamActor) m_ins->m_BeamActor->SetVisibility(flag);
	if (m_ins->m_TrussActor) m_ins->m_TrussActor->SetVisibility(flag);
	pCAE->m_renderWindow->Render();
}


void resultVisualize::getBoundary()
{
	double max_x = 0;
	double min_x = 0;
	double max_y = 0;
	double min_y = 0;
	double max_z = 0;
	double min_z = 0;
	for (auto& i : m_nodes)
	{
		if (max_x < i->m_x) max_x = i->m_x;
		if (min_x > i->m_x) min_x = i->m_x;

		if (max_y < i->m_y) max_y = i->m_y;
		if (min_y > i->m_y) min_y = i->m_y;

		if (max_z < i->m_z) max_z = i->m_z;
		if (min_z > i->m_z) min_z = i->m_z;
	}
	if (boundary < (max_x - min_x)) boundary = max_x - min_x;
	if (boundary < (max_y - min_y)) boundary = max_y - min_y;
	if (boundary < (max_z - min_z)) boundary = max_z - min_z;
}

void resultVisualize::update()
{
	if (m_frames >= outputData->size())
	{
		if (loopPlay) emit animationFinished();
		return;
	}
	double ampFactor = ui.lineEdit->text().toDouble();//�Ŵ�����

	vtkIdType pointsNum = m_nodes.size();

	Outputter* iframe = &outputData->at(m_frames);
	//�޸ĵ������
	for (auto& i : iframe->nodeData)
	{
		double* p = m_originalPoints->GetPoint(i.first - 1);
		m_points->SetPoint(i.first - 1, p[0] + ampFactor * i.second.displaymentX, p[1] + ampFactor * i.second.displaymentY, p[2] + ampFactor * i.second.displaymentZ);

		//�����ͼ����
		switch (currentType)
		{
		case static_cast<int>(nephogramType::DisplaymentX):
			scalars->SetValue(i.first - 1, i.second.displaymentX);
			break;
		case static_cast<int>(nephogramType::DisplaymentY):
			scalars->SetValue(i.first - 1, i.second.displaymentY);
			break;
		case static_cast<int>(nephogramType::DisplaymentZ):
			scalars->SetValue(i.first - 1, i.second.displaymentZ);
			break;
		case static_cast<int>(nephogramType::StressN):
			scalars->SetValue(i.first - 1, i.second.stressN);
			break;
		default:
			qDebug() << "δ֪��ͼ���ͣ�";
			break;
		}

	}
	m_frames++;//֡++
	ui.label_frame->setText(QString::number(m_frames));//֡
	//ui.label_time->setText(QString::number(m_frames*0.05));//֡

	//����vtkActor��vtkPolyData������
	m_points->Modified();
	//scalars->Modified();

	pCAE->m_renderWindow->Render();
}

void resultVisualize::start()
{
	if (m_vtkNodes == nullptr) return;
	assert(outputData);

	int msec = m_dt / m_speed;
	qDebug() << "current time interval: " << msec << "ms";
	updateTimer->setInterval(msec);
	updateTimer->start();

	qDebug() << "play";
}

void resultVisualize::stop()
{
	updateTimer->stop();

	qDebug() << "stop";
}

void resultVisualize::quit()
{
	updateTimer->stop();
	m_frames = 0;

	removeActor();
	pCAE->m_renderWindow->Render();

	qDebug() << "quit";
}

void resultVisualize::autoFactor(bool flag)
{
	if (!flag) return;

	double max_disp = 0;
	for (auto& i : *outputData)
	{
		double max_dx = i.getBoundaryDisplaymentX()[1];
		if (max_disp < max_dx) max_disp = max_dx;

		double max_dy = i.getBoundaryDisplaymentY()[1];
		if (max_disp < max_dy) max_disp = max_dy;

		double max_dz = i.getBoundaryDisplaymentZ()[1];
		if (max_disp < max_dz) max_disp = max_dz;
	}

	double ampFator = boundary / max_disp / 10;

	ui.lineEdit->setText(QString::number(ampFator));
}

void resultVisualize::addData(vector<Node_Base*>node, Instance* ins)
{//ˢ�����ݣ����������ݱ仯��
	m_ins = ins;
	assert(m_ins);

	for (auto& i : node)
	{
		m_nodes.push_back(i);
	}
	addActorData();//�������������(copy)

	//��ӷ������������
	int numStep = 2;//����������
	for (int i = 1; i <= numStep; ++i)
	{
		stepData.insert(std::make_pair(i, ins->s->get_outputter(i)));
	}
	
	for (auto& i : stepData)
	{
		for (auto& iframe: i.second)
		{
			iframe.SortData();//����ֵ
		}
	}
	getBoundary();

	//������combox
	for (auto& i : stepData)
	{
		ui.comboBox_step->addItem(QString("Step-") + QString::number(i.first));
	}

	setCurrentStep(0);
	pCAE->m_Renderer->ResetCamera();
}

void resultVisualize::addActorData()
{
	if (m_vtkNodes != nullptr)
	{
		removeActor();
	}
	m_originalPoints = vtkSmartPointer<vtkPoints>::New();//ԭʼ����
	m_points = vtkSmartPointer<vtkPoints>::New();//������ʾ������

	vtkSmartPointer<vtkPoints> points = m_ins->m_pts;//�������е�����
	vtkSmartPointer<vtkCellArray> lines = m_ins->m_lines;//�������е�����

	m_originalPoints->ShallowCopy(points);//����ͬһ������
	m_points->DeepCopy(points);//����һ���µ�����

	vtkSmartPointer<vtkPolyData> nodesPolyData = vtkSmartPointer<vtkPolyData>::New();
	nodesPolyData->SetPoints(m_points);

	vtkNew<vtkVertexGlyphFilter> VertexFilter;
	VertexFilter->SetInputData(nodesPolyData);
	VertexFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(VertexFilter->GetOutputPort());
	m_vtkNodes = vtkSmartPointer<vtkActor>::New();
	m_vtkNodes->SetMapper(mapper);
	m_vtkNodes->GetProperty()->SetColor(0, 0, 0);
	m_vtkNodes->GetProperty()->SetPointSize(3);

	//Ӧ����Ⱦ
	int numPts = points->GetNumberOfPoints();
	scalars = vtkSmartPointer<vtkDoubleArray>::New();
	scalars->SetNumberOfComponents(1);
	scalars->SetNumberOfValues(numPts);
	for (int i = 0; i < numPts; ++i)
	{
		scalars->SetValue(i, 0);
	}

	//create the visualized line
	vtkSmartPointer<vtkPolyData> linesPolyData = vtkSmartPointer<vtkPolyData>::New();
	//������(pt)�������һϵ�е㹹�����ݼ�(vtkDataSet)�ļ��νṹ
	linesPolyData->SetPoints(m_points);
	//�����ݵ�������ʽ(lines)�γɵĵ�Ԫ����(cell Data)�������ݼ����˽ṹ
	linesPolyData->SetLines(lines);
	linesPolyData->GetPointData()->SetScalars(scalars);

	vtkSmartPointer<vtkPolyDataMapper> linesmapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	linesmapper->SetInputData(linesPolyData);
	linesmapper->ScalarVisibilityOn();
	linesmapper->SetScalarModeToUsePointData();
	//color_mapper->SetColorModeToMapScalars();
	linesmapper->SetScalarRange(0, 1);

	m_vtklines = vtkSmartPointer<vtkActor>::New();
	m_vtklines->SetMapper(linesmapper);
	m_vtklines->GetProperty()->SetColor(0, 0, 0);

	scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
	scalarBar->SetLookupTable(linesmapper->GetLookupTable());
	scalarBar->SetTitle("U1");
	scalarBar->SetNumberOfLabels(20);

	pCAE->m_Renderer->AddActor(m_vtkNodes);
	pCAE->m_Renderer->AddActor(m_vtklines);
	pCAE->m_Renderer->AddActor(scalarBar);
	pCAE->m_renderWindow->Render();
}

void resultVisualize::removeActor()
{
	pCAE->m_Renderer->RemoveActor(m_vtkNodes);
	pCAE->m_Renderer->RemoveActor(m_vtklines);
	pCAE->m_Renderer->RemoveActor(scalarBar);

	m_vtkNodes = nullptr;
	m_vtklines = nullptr;
}

void resultVisualize::setNephogramType(int iTpye)
{
	currentType = iTpye;

	qDebug() << "��ͼ���ͣ�" << currentType;

	//setBoundary
	vector<double> boundary(2);

	switch (currentType)
	{
	case static_cast<int>(nephogramType::DisplaymentX):

		boundary = outputData->at(0).getBoundaryDisplaymentX();
		for (auto& i : *outputData)
		{
			vector<double> boundaryX = i.getBoundaryDisplaymentX();
			if (boundary[0] < boundaryX[0]) boundary[0] = boundaryX[0];
			if (boundary[1] > boundaryX[1]) boundary[1] = boundaryX[1];
		}
		scalarBar->SetTitle("U1");
		break;
	case static_cast<int>(nephogramType::DisplaymentY):

		boundary = outputData->at(0).getBoundaryDisplaymentY();
		for (auto& i : *outputData)
		{
			vector<double> boundaryY = i.getBoundaryDisplaymentY();
			if (boundary[0] < boundaryY[0]) boundary[0] = boundaryY[0];
			if (boundary[1] > boundaryY[1]) boundary[1] = boundaryY[1];
		}
		scalarBar->SetTitle("U2");
		break;
	case static_cast<int>(nephogramType::DisplaymentZ):

		boundary = outputData->at(0).getBoundaryDisplaymentZ();
		for (auto& i : *outputData)
		{
			vector<double> boundaryZ = i.getBoundaryDisplaymentZ();
			if (boundary[0] < boundaryZ[0]) boundary[0] = boundaryZ[0];
			if (boundary[1] > boundaryZ[1]) boundary[1] = boundaryZ[1];
		}
		scalarBar->SetTitle("U3");
		break;
	case static_cast<int>(nephogramType::StressN):

		boundary = outputData->at(0).getBoundaryStressN();
		for (auto& i : *outputData)
		{
			vector<double> boundaryN = i.getBoundaryStressN();
			if (boundary[0] < boundaryN[0]) boundary[0] = boundaryN[0];
			if (boundary[1] > boundaryN[1]) boundary[1] = boundaryN[1];
		}
		scalarBar->SetTitle("Stress");
		break;
	default:
		qDebug() << "δ֪��ͼ���ͣ�";
		break;
	}
	qDebug() << "range of value:" << boundary[0] << " " << boundary[1];

	if (boundary[0] > boundary[1]) std::swap(boundary[0], boundary[1]);

	m_vtklines->GetMapper()->SetScalarRange(boundary[0], boundary[1]);
}

void resultVisualize::setCurrentStep(int idStep)
{
	idStep += 1;
	qDebug() << "current step:" << idStep;
	outputData = &stepData.find(idStep)->second;

	//��ʼ��ΪU1
	//ui.comboBox_nephogram->setCurrentIndex(0);
	setNephogramType(0);
}
