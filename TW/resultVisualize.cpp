#include "resultVisualize.h"
#include"InterFace.h"
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
	ui.speed_Slider->setRange(1, 30);
	ui.speed_Slider->setValue(30);
	m_speed = 30;
	connect(ui.speed_Slider, &QSlider::valueChanged, this, &resultVisualize::set_fps);
	//显示原始actor
	ui.checkBox_showOriginal->setChecked(true);
	connect(ui.checkBox_showOriginal, &QCheckBox::clicked, this, &resultVisualize::showOriginalActor);
	//循环播放
	ui.checkBox_loop->setChecked(true);
	set_loopPlay(true);
	connect(ui.checkBox_loop, &QCheckBox::clicked, this, &resultVisualize::set_loopPlay);
	//动画结束
	connect(this, &resultVisualize::animationFinished, [=]() {	m_frames = 0; });
}

resultVisualize::~resultVisualize()
{}

void resultVisualize::set_fps(int fps)
{
	m_speed = fps;
}

void resultVisualize::showOriginalActor(bool flag)
{
	m_ins->Node_actor->SetVisibility(flag);
	m_ins->m_BeamActor->SetVisibility(flag);
	m_ins->m_TrussActor->SetVisibility(flag);
	pCAE->m_renderWindow->Render();
}

void resultVisualize::update()
{
	if (m_frames >= p_nodes[0]->Get_DisplaymentXData().size())
	{
		if (loopPlay) emit animationFinished();
		return;
	}
	double ampFactor = ui.lineEdit->text().toDouble();//放大因子

	//vtkIdType pointsNum = m_points->GetNumberOfPoints();
	vtkIdType pointsNum = p_nodes.size();
	//修改点数据中的坐标
	for (auto& i : p_nodes)
	{
		std::vector<double>yData = i->Get_DisplaymentXData();
		std::vector<double>zData = i->Get_DisplaymentYData();
		std::vector<double>xData = i->Get_DisplaymentZData();
		double* p = m_originalPoints->GetPoint(i->m_id - 1);
		m_points->SetPoint(i->m_id - 1, p[0] + ampFactor * xData[m_frames], p[1] + ampFactor * yData[m_frames], p[2] + ampFactor * zData[m_frames]);

	}
	m_frames++;//帧++
	ui.label_frame->setText(QString::number(m_frames));//帧

	//更新vtkActor和vtkPolyData的数据
	m_points->Modified();
	//m_vtkNodes->GetMapper()->Update();
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
	double max_x = 0;
	double min_x = 0;
	double max_y = 0;
	double min_y = 0;
	double max_z = 0;
	double min_z = 0;
	double max_disp = 0;
	for (auto& i : p_nodes)
	{
		if (max_x < i->m_x) max_x = i->m_x;
		if (min_x > i->m_x) min_x = i->m_x;

		if (max_y < i->m_y) max_y = i->m_y;
		if (min_y > i->m_y) min_y = i->m_y;

		if (max_z < i->m_z) max_z = i->m_z;
		if (min_z > i->m_z) min_z = i->m_z;

		vector<double> dux = i->Get_DisplaymentXData();
		vector<double> duy = i->Get_DisplaymentYData();
		vector<double> duz = i->Get_DisplaymentZData();
		double dX = abs(dux[dux.size() - 1]);
		double dY = abs(duy[duy.size() - 1]);
		double dZ = abs(duz[duz.size() - 1]);
		if (max_disp < dX) max_disp = dX;
		if (max_disp < dY) max_disp = dY;
		if (max_disp < dZ) max_disp = dZ;
	}
	double bd = 0;
	if (bd < (max_x - min_x)) bd = max_x - min_x;
	if (bd < (max_y - min_y)) bd = max_y - min_y;
	if (bd < (max_z - min_z)) bd = max_z - min_z;

	double ampFator = bd / max_disp / 10;

	ui.lineEdit->setText(QString::number(ampFator));
}

void resultVisualize::addData(vector<Node_Base*>node, Instance* ins)
{//刷新数据（主界面数据变化后）
	m_ins = ins;
	assert(m_ins);

	for (auto& i : node)
	{
		p_nodes.push_back(i);
	}
	addActorData();//添加主界面数据(copy)

	pCAE->m_Renderer->ResetCamera();
}

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
	m_vtkNodes->GetProperty()->SetColor(1, 0, 0);
	m_vtkNodes->GetProperty()->SetPointSize(3);

	//create the visualized line
	vtkSmartPointer<vtkPolyData> linesPolyData = vtkSmartPointer<vtkPolyData>::New();
	//点数据(pt)所定义的一系列点构成数据集(vtkDataSet)的几何结构
	linesPolyData->SetPoints(m_points);
	//点数据的连接形式(lines)形成的单元数据(cell Data)构成数据集拓扑结构
	linesPolyData->SetLines(lines);

	vtkSmartPointer<vtkPolyDataMapper> linesmapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	linesmapper->SetInputData(linesPolyData);

	m_vtklines = vtkSmartPointer<vtkActor>::New();
	m_vtklines->SetMapper(linesmapper);
	m_vtklines->GetProperty()->SetColor(1, 0, 0);

	pCAE->m_Renderer->AddActor(m_vtkNodes);
	pCAE->m_Renderer->AddActor(m_vtklines);
	pCAE->m_renderWindow->Render();
}

void resultVisualize::removeActor()
{
	pCAE->m_Renderer->RemoveActor(m_vtkNodes);
	pCAE->m_Renderer->RemoveActor(m_vtklines);

	m_vtkNodes = nullptr;
	m_vtklines = nullptr;
}