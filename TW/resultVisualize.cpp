#include "resultVisualize.h"
#include"InterFace.h"
#include <vtkTextProperty.h>

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
	ui.speed_Slider->setRange(1, 60);
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
	qstrlist << "U1" << "U2" << "U3" << "UR1" << "UR2" << "UR3" << "N" << "M2" << "M3" << "Mises";
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
	if (m_frames >= m_Outputter->dataSet.size())
	{
		if (loopPlay) emit animationFinished();
		return;
	}
	double ampFactor = ui.lineEdit->text().toDouble();//�Ŵ�����

	vtkIdType pointsNum = m_nodes.size();

	DataFrame* iframe = m_Outputter->dataSet[m_frames];
	//�޸ĵ������
	for (auto& i : iframe->nodeDatas)
	{
		int pointIndex = i.first - 1;
		NodeData& node = i.second;
		double* p = m_originalPoints->GetPoint(pointIndex);
		m_points->SetPoint(pointIndex, p[0] + ampFactor * node.displaymentZ, p[1] + ampFactor * node.displaymentX, p[2] + ampFactor * node.displaymentY);

		//�����ͼ����
		switch (currentType)
		{
		case DataType::U1:
			scalars->SetValue(pointIndex, node.displaymentZ);
			break;
		case DataType::U2:
			scalars->SetValue(pointIndex, node.displaymentX);
			break;
		case DataType::U3:
			scalars->SetValue(pointIndex, node.displaymentY);
			break;
		case DataType::UR1:
			scalars->SetValue(pointIndex, node.rotationZ);
			break;
		case DataType::UR2:
			scalars->SetValue(pointIndex, node.rotationX);
			break;
		case DataType::UR3:
			scalars->SetValue(pointIndex, node.rotationY);
			break;
		case DataType::N:
			scalars->SetValue(pointIndex, node.stressN);
			break;
		case DataType::M2:
			scalars->SetValue(pointIndex, node.stressM2);
			break;
		case DataType::M3:
			scalars->SetValue(pointIndex, node.stressM3);
			break;
		case DataType::Mises:
			scalars->SetValue(pointIndex, node.mises);
			break;
		default:
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

	double max_dx = m_Outputter->getBoundaryDisplaymentX()[1];
	if (max_disp < max_dx) max_disp = max_dx;

	double max_dy = m_Outputter->getBoundaryDisplaymentY()[1];
	if (max_disp < max_dy) max_disp = max_dy;

	double max_dz = m_Outputter->getBoundaryDisplaymentZ()[1];
	if (max_disp < max_dz) max_disp = max_dz;

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
	int stepnum = ins->s->getTotalSteps();
	//������boundary
	for (int i = 1; i <= stepnum; ++i)
	{
		ins->s->get_outputter(i).FindBoundary();
	}

	if (stepnum == ui.comboBox_step->count()) return;

	getBoundary();
	//������combox
	for (int i = 1; i <= stepnum; ++i)
	{
		ui.comboBox_step->addItem(QString("Step-") + QString::number(i));
	}
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
	scalarBar->SetNumberOfLabels(16);
	scalarBar->SetMaximumNumberOfColors(8);

	//scalarBar->SetWidth(0.07);  // ���ñ������Ŀ��Ϊ0.1���ٷֱȣ��������Ⱦ���ڵĿ�ȣ�
	//scalarBar->SetHeight(0.8); // ���ñ������ĸ߶�Ϊ0.8���ٷֱȣ��������Ⱦ���ڵĸ߶ȣ�

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
	currentType = static_cast<DataType>(iTpye);

	//qDebug() << "��ͼ���ͣ�" << currentType;

	//setBoundary
	vector<double> boundary(2);

	switch (currentType)
	{
	case DataType::U1:
		boundary = m_Outputter->getBoundaryDisplaymentZ();
		scalarBar->SetTitle("U1");
		break;
	case DataType::U2:
		boundary = m_Outputter->getBoundaryDisplaymentX();
		scalarBar->SetTitle("U2");
		break;
	case DataType::U3:
		boundary = m_Outputter->getBoundaryDisplaymentY();
		scalarBar->SetTitle("U3");
		break;
	case DataType::UR1:
		boundary = m_Outputter->getBoundaryRotationZ();
		scalarBar->SetTitle("UR1");
		break;
	case DataType::UR2:
		boundary = m_Outputter->getBoundaryRotationX();
		scalarBar->SetTitle("UR2");
		break;
	case DataType::UR3:
		boundary = m_Outputter->getBoundaryRotationY();
		scalarBar->SetTitle("UR3");
		break;
	case DataType::N:
		boundary = m_Outputter->getBoundaryStressN();
		scalarBar->SetTitle("StressN");
		break;
	case DataType::M2:
		boundary = m_Outputter->getBoundaryStressM2();
		scalarBar->SetTitle("StressM2");
		break;
	case DataType::M3:
		boundary = m_Outputter->getBoundaryStressM3();
		scalarBar->SetTitle("StressM3");
		break;
	case DataType::Mises:
		boundary = m_Outputter->getBoundaryMises();
		scalarBar->SetTitle("Mises");
		break;
	default:
		qDebug() << "δ֪��ͼ���ͣ�";
		break;
	}
	qDebug() << "range of value:" << boundary[0] << " " << boundary[1];

	m_vtklines->GetMapper()->SetScalarRange(boundary[0], boundary[1]);
}

void resultVisualize::setCurrentStep(int idStep)
{
	idStep += 1;

	m_Outputter = &(m_ins->s->get_outputter(idStep));

	qDebug() << "current step:" << m_Outputter->m_idStep;

	//��ʼ��ΪU1
	ui.comboBox_nephogram->setCurrentIndex(0);
	vector<double>boundary = m_Outputter->getBoundaryDisplaymentX();
	scalarBar->SetTitle("U1");
	qDebug() << "range of value:" << boundary[0] << " " << boundary[1];

	m_vtklines->GetMapper()->SetScalarRange(boundary[0], boundary[1]);
}
