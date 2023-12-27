#include "resultVisualize.h"
#include"InterFace.h"
#include <vtkTextProperty.h>
#include<vtkLookupTable.h>
resultVisualize::resultVisualize(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	pCAE = dynamic_cast<InterFace*>(parent);
	assert(pCAE != nullptr);

	updateTimer = new QTimer(this);

	//自动调整放大因子
	ui.lineEdit->setText("1");
	connect(ui.radioButton, &QRadioButton::clicked, this, &resultVisualize::autoFactor);
	connect(ui.radioButton, &QRadioButton::clicked, ui.lineEdit, &QLineEdit::setDisabled);
	//更新动画
	connect(updateTimer, &QTimer::timeout, this, &resultVisualize::update);
	//播放
	connect(ui.play_btn, &QPushButton::clicked, this, &resultVisualize::start);
	//暂停
	connect(ui.stop_btn, &QPushButton::clicked, this, &resultVisualize::stop);
	//终止
	connect(ui.quit_btn, &QPushButton::clicked, this, &resultVisualize::quit);

	ui.label_frame->setText(QString::number(m_frames));//帧标签
	//设置速度滑块
	ui.speed_Slider->setRange(1, 60);
	ui.speed_Slider->setValue(30);
	m_speed = 30;
	connect(ui.speed_Slider, &QSlider::valueChanged, this, &resultVisualize::set_fps);
	//显示原始actor
	ui.checkBox_showOriginal->setChecked(false);
	connect(ui.checkBox_showOriginal, &QCheckBox::clicked, this, &resultVisualize::showOriginalActor);
	//循环播放
	ui.checkBox_loop->setChecked(true);
	set_loopPlay(true);
	connect(ui.checkBox_loop, &QCheckBox::clicked, this, &resultVisualize::set_loopPlay);
	//动画结束
	connect(this, &resultVisualize::animationFinished, [=]() {	m_frames = 0; });

	//云图combox
	QStringList qstrlist;
	qstrlist << "Ux" << "Uy" << "Uz" << "U" << "UR1" << "UR2" << "UR3" << "N";
	ui.comboBox_nephogram->addItems(qstrlist);//nephogram--"云图"
	void (QComboBox:: * MycurrentIndexChanged)(int) = &QComboBox::currentIndexChanged;
	connect(ui.comboBox_nephogram, MycurrentIndexChanged, this, &resultVisualize::setNephogramType);

	connect(ui.comboBox_step, MycurrentIndexChanged, this, &resultVisualize::setCurrentStep);

	lookupTable = vtkSmartPointer<vtkLookupTable>::New();
	lookupTable->SetHueRange(0.667, 0);
	lookupTable->SetNumberOfColors(16);
	lookupTable->Build();
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
		if (max_x < i.x) max_x = i.x;
		if (min_x > i.x) min_x = i.x;
		if (max_y < i.y) max_y = i.y;
		if (min_y > i.y) min_y = i.y;
		if (max_z < i.z) max_z = i.z;
		if (min_z > i.z) min_z = i.z;
	}
	if (boundary < (max_x - min_x)) boundary = max_x - min_x;
	if (boundary < (max_y - min_y)) boundary = max_y - min_y;
	if (boundary < (max_z - min_z)) boundary = max_z - min_z;
}

void resultVisualize::update()
{
	if (m_Outputter != nullptr)
	{
		if (m_frames >= m_Outputter->dataSet.size())
		{
			if (loopPlay) emit animationFinished();
			return;
		}
		double ampFactor = ui.lineEdit->text().toDouble();//放大因子

		vtkIdType pointsNum = m_nodes.size();

		DataFrame* iframe = m_Outputter->dataSet[m_frames];
		//修改点的坐标
		for (auto& i : iframe->nodeDatas)
		{
			int pointIndex = i.first - 1;
			NodeData& node = i.second;
			double* p = m_originalPoints->GetPoint(pointIndex);
			m_points->SetPoint(pointIndex, p[0] + ampFactor * node.displaymentZ, p[1] + ampFactor * node.displaymentX, p[2] + ampFactor * node.displaymentY);

			//添加云图数据
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
			case DataType::MagnitudeU:
				scalars->SetValue(pointIndex, node.displayment);
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
			default:
				break;
			}

		}
	}
	//if (m_Outputter_ice != nullptr)
	//{
	//	if (m_frames >= m_Outputter_ice->dataSet.size())
	//	{
	//		if (loopPlay) emit animationFinished();
	//		return;
	//	}
	//	double ampFactor = ui.lineEdit->text().toDouble();//放大因子

	//	vtkIdType pointsNum = m_nodes.size();

	//	DataFrame_ice* iframe = m_Outputter_ice->dataSet[m_frames];
	//	//修改点的坐标
	//	for (auto& i : iframe->nodeDatas)
	//	{
	//		int pointIndex = i.first - 1;
	//		NodeData_ice& node = i.second;
	//		double* p = m_originalPoints->GetPoint(pointIndex);
	//		m_points->SetPoint(pointIndex, p[0] + ampFactor * node.displaymentZ, p[1] + ampFactor * node.displaymentX, p[2] + ampFactor * node.displaymentY);

	//		//添加云图数据
	//		switch (currentType_ice)
	//		{
	//		case DataType_ice::U1:
	//			scalars->SetValue(pointIndex, node.displaymentZ);
	//			break;
	//		case DataType_ice::U2:
	//			scalars->SetValue(pointIndex, node.displaymentX);
	//			break;
	//		case DataType_ice::U3:
	//			scalars->SetValue(pointIndex, node.displaymentY);
	//			break;
	//		case DataType_ice::UR1:
	//			scalars->SetValue(pointIndex, node.rotationZ);
	//			break;
	//		case DataType_ice::UR2:
	//			scalars->SetValue(pointIndex, node.rotationX);
	//			break;
	//		case DataType_ice::UR3:
	//			scalars->SetValue(pointIndex, node.rotationY);
	//			break;
	//		case DataType_ice::N:
	//			scalars->SetValue(pointIndex, node.stressN);
	//			break;
	//		case DataType_ice::M2:
	//			scalars->SetValue(pointIndex, node.stressM2);
	//			break;
	//		case DataType_ice::M3:
	//			scalars->SetValue(pointIndex, node.stressM3);
	//			break;
	//		default:
	//			break;
	//		}

	//	}
	//}
	m_frames++;//帧++
	ui.label_frame->setText(QString::number(m_frames));//帧
	m_points->Modified();
	pCAE->m_renderWindow->Render();
}

void resultVisualize::start()
{
	if (m_vtkNodes == nullptr) return;

	int msec = m_dt / m_speed;
	//qDebug() << "current time interval: " << msec << "ms";
	updateTimer->setInterval(msec);
	updateTimer->start();

	//qDebug() << "play";
}

void resultVisualize::stop()
{
	updateTimer->stop();

	//qDebug() << "stop";
}

void resultVisualize::quit()
{
	updateTimer->stop();
	m_frames = 0;

	removeActor();
	pCAE->m_renderWindow->Render();
	this->close();
	//qDebug() << "quit";
}

void resultVisualize::autoFactor(bool flag)
{
	if (m_Outputter != nullptr)
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
	/*if (m_Outputter_ice != nullptr)
	{
		if (!flag) return;

		double max_disp = 0;

		double max_dx = m_Outputter_ice->getBoundaryDisplaymentX()[1];
		if (max_disp < max_dx) max_disp = max_dx;

		double max_dy = m_Outputter_ice->getBoundaryDisplaymentY()[1];
		if (max_disp < max_dy) max_disp = max_dy;

		double max_dz = m_Outputter_ice->getBoundaryDisplaymentZ()[1];
		if (max_disp < max_dz) max_disp = max_dz;

		double ampFator = boundary / max_disp / 10;
		ui.lineEdit->setText(QString::number(ampFator));
	}*/

}

void resultVisualize::addData(std::list<std::vector<double>>& nodes, Instance* ins)
{//刷新数据（主界面数据变化后）
	m_ins = ins;
	assert(m_ins);

	for (auto& i : nodes)
	{
		m_nodes.push_back(Node(0, i[0], i[1], i[2], 0.));
	}
	addActorData();//添加主界面数据(copy)

	//添加分析步结果数据
	int stepnum = ins->s->getTotalSteps();

	//分析步boundary
	for (int i = 1; i <= stepnum; ++i)
	{
		ins->s->get_outputter(i).FindBoundary();

	}
	if (stepnum != ui.comboBox_step->count())
	{
		getBoundary();
		//分析步combox
		for (int i = 1; i <= stepnum; ++i)
		{
			ui.comboBox_step->addItem(QString("Step-") + QString::number(i));
		}
	}

	vector<double> boundary(2);
	boundary = m_Outputter->getBoundaryDisplaymentZ();
	lookupTable->SetRange(boundary[0], boundary[1]);
	lookupTable->Build();
	m_vtklines->GetMapper()->SetScalarRange(boundary[0], boundary[1]);

	pCAE->m_Renderer->ResetCamera();
}

//void resultVisualize::addData_ice(std::list<std::vector<double>>& nodes, Instance* ins)
//{//刷新数据（主界面数据变化后）
//	m_ins = ins;
//	assert(m_ins);
//
//	for (auto& i : nodes)
//	{
//		m_nodes.push_back(Node(0., i[0], i[1], i[2], 0.));
//	}
//	addActorData();//添加主界面数据(copy)
//
//	getBoundary();
//	//分析步boundary
//	m_Outputter_ice = &m_ins->s_ice->get_outputter(1);
//	m_ins->s_ice->get_outputter(1).FindBoundary();
//	pCAE->m_Renderer->ResetCamera();
//}

void resultVisualize::addActorData()
{
	if (m_vtkNodes != nullptr)
	{
		removeActor();
	}
	m_originalPoints = vtkSmartPointer<vtkPoints>::New();//原始数据
	m_points = vtkSmartPointer<vtkPoints>::New();//用于显示的数据

	vtkSmartPointer<vtkPoints> points = m_ins->m_pts;//主界面中的数据
	vtkSmartPointer<vtkCellArray> lines = m_ins->m_lines;//主界面中的数据

	m_originalPoints->ShallowCopy(points);//共用同一份数据
	m_points->DeepCopy(points);//复制一份新的数据

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
	m_vtkNodes->GetProperty()->SetPointSize(2);

	//应力渲染
	int numPts = points->GetNumberOfPoints();
	scalars = vtkSmartPointer<vtkDoubleArray>::New();
	scalars->SetNumberOfComponents(1);
	scalars->SetNumberOfValues(numPts);
	//for (int i = 0; i < numPts; ++i)
	//{
	//	scalars->SetValue(i, 0);
	//}

	vtkSmartPointer<vtkPolyData> linesPolyData = vtkSmartPointer<vtkPolyData>::New();
	linesPolyData->SetPoints(m_points);
	linesPolyData->SetLines(lines);
	linesPolyData->GetPointData()->SetScalars(scalars);

	vtkSmartPointer<vtkPolyDataMapper> linesmapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	linesmapper->SetInputData(linesPolyData);
	linesmapper->ScalarVisibilityOn();
	linesmapper->SetScalarModeToUsePointData();
	linesmapper->SetScalarRange(0, 1);

	m_vtklines = vtkSmartPointer<vtkActor>::New();
	m_vtklines->SetMapper(linesmapper);
	m_vtklines->GetProperty()->SetColor(0, 0, 0);

	scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
	scalarBar->SetLookupTable(lookupTable);
	//scalarBar->SetTitle("U1");
	scalarBar->SetNumberOfLabels(16);

	pCAE->m_Renderer->AddActor(m_vtkNodes);
	pCAE->m_Renderer->AddActor(m_vtklines);
	pCAE->m_Renderer->AddActor(scalarBar);

	if (m_ins->Node_actor) m_ins->Node_actor->SetVisibility(false);
	if (m_ins->m_BeamActor) m_ins->m_BeamActor->SetVisibility(false);
	if (m_ins->m_TrussActor)m_ins->m_TrussActor->SetVisibility(false);

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
	vector<double> boundary(2);
	if (m_Outputter != nullptr)
	{
		currentType = static_cast<DataType>(iTpye);

		switch (currentType)
		{
		case DataType::U1:
			boundary = m_Outputter->getBoundaryDisplaymentZ();
			//scalarBar->SetTitle("U1");
			break;
		case DataType::U2:
			boundary = m_Outputter->getBoundaryDisplaymentX();
			break;
		case DataType::U3:
			boundary = m_Outputter->getBoundaryDisplaymentY();
			break;
		case DataType::MagnitudeU:
			boundary = m_Outputter->getBoundaryDisplayment();
			break;
		case DataType::UR1:
			boundary = m_Outputter->getBoundaryRotationZ();
			break;
		case DataType::UR2:
			boundary = m_Outputter->getBoundaryRotationX();
			break;
		case DataType::UR3:
			boundary = m_Outputter->getBoundaryRotationY();
			break;
		case DataType::N:
			boundary = m_Outputter->getBoundaryStressN();
			break;
		default:
			qDebug() << "未知云图类型！";
			break;
		}
	}
	//if (m_Outputter_ice != nullptr)
	//{
	//	currentType_ice = static_cast<DataType_ice>(iTpye);
	//	switch (currentType_ice)
	//	{
	//	case DataType_ice::U1:
	//		boundary = m_Outputter_ice->getBoundaryDisplaymentZ();
	//		break;
	//	case DataType_ice::U2:
	//		boundary = m_Outputter_ice->getBoundaryDisplaymentX();
	//		break;
	//	case DataType_ice::U3:
	//		boundary = m_Outputter_ice->getBoundaryDisplaymentY();
	//		break;
	//	case DataType_ice::UR1:
	//		boundary = m_Outputter_ice->getBoundaryRotationZ();
	//		break;
	//	case DataType_ice::UR2:
	//		boundary = m_Outputter_ice->getBoundaryRotationX();
	//		break;
	//	case DataType_ice::UR3:
	//		boundary = m_Outputter_ice->getBoundaryRotationY();
	//		break;
	//	case DataType_ice::N:
	//		boundary = m_Outputter_ice->getBoundaryStressN();
	//		break;
	//	default:
	//		qDebug() << "未知云图类型！";
	//		break;
	//	}
	//	//qDebug() << "range of value:" << boundary[0] << " " << boundary[1];
	//}
	lookupTable->SetRange(boundary[0], boundary[1]);
	lookupTable->Build();
	m_vtklines->GetMapper()->SetScalarRange(boundary[0], boundary[1]);

}

void resultVisualize::setCurrentStep(int idStep)
{
	idStep += 1;
	if (m_ins->s != nullptr)
	{
		m_Outputter = &(m_ins->s->get_outputter(idStep));
		qDebug() << "current step:" << m_Outputter->m_idStep;
		ui.comboBox_nephogram->setCurrentIndex(0);
		vector<double>boundary = m_Outputter->getBoundaryDisplaymentX();
		//scalarBar->SetTitle("U1");
		qDebug() << "range of value:" << boundary[0] << " " << boundary[1];

		m_vtklines->GetMapper()->SetScalarRange(boundary[0], boundary[1]);
	}
	//if (m_ins->s_ice != nullptr)
	//{
	//	m_Outputter_ice = &(m_ins->s_ice->get_outputter(idStep));
	//	qDebug() << "current step:" << m_Outputter_ice->m_idStep;
	//	ui.comboBox_nephogram->setCurrentIndex(0);
	//	vector<double>boundary = m_Outputter_ice->getBoundaryDisplaymentX();
	//	//scalarBar->SetTitle("U1");
	//	qDebug() << "range of value:" << boundary[0] << " " << boundary[1];
	//	m_vtklines->GetMapper()->SetScalarRange(boundary[0], boundary[1]);
	//}	
}
