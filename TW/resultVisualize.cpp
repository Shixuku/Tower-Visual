#include "resultVisualize.h"
#include"InterFace.h"
#include <vtkTextProperty.h>
#include <vtkLookupTable.h>
#include <QDebug>

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
	//���¶���
	connect(updateTimer, &QTimer::timeout, this, &resultVisualize::update);
	//����
	connect(ui.play_btn, &QPushButton::clicked, this, &resultVisualize::start);
	//��ͣ
	connect(ui.stop_btn, &QPushButton::clicked, this, &resultVisualize::stop);

	ui.label_frame->setText(QString::number(m_frames));//֡��ǩ
	//�����ٶȻ���
	ui.speed_Slider->setRange(1, 60);
	ui.speed_Slider->setValue(30);
	m_speed = 30;
	connect(ui.speed_Slider, &QSlider::valueChanged, this, &resultVisualize::set_fps);
	//��ʾԭʼactor
	ui.checkBox_showOriginal->setChecked(true);
	connect(ui.checkBox_showOriginal, &QCheckBox::clicked, this, &resultVisualize::showOriginalActor);
	//��ʾPoints
	ui.checkBox_showPoints->setChecked(true);
	connect(ui.checkBox_showPoints, &QCheckBox::clicked, this, &resultVisualize::showPointsActor);
	//ѭ������
	ui.checkBox_loop->setChecked(true);
	set_loopPlay(true);
	connect(ui.checkBox_loop, &QCheckBox::clicked, this, &resultVisualize::set_loopPlay);
	//��������
	connect(this, &resultVisualize::animationFinished, [=]() {	m_frames = 0; });

	//��ͼcombox
	QStringList qstrlist;
	qstrlist << "Ux" << "Uy" << "Uz" << "U" << "URx" << "URy" << "URz" << "N" << "My" << "Mz" << "Mises";
	ui.comboBox_nephogram->addItems(qstrlist);
	void (QComboBox:: * MycurrentIndexChanged)(int) = &QComboBox::currentIndexChanged;
	connect(ui.comboBox_nephogram, MycurrentIndexChanged, this, &resultVisualize::setNephogramType);
	connect(ui.comboBox_step, MycurrentIndexChanged, this, &resultVisualize::setCurrentStep);

	//lookupTable
	lookupTable = vtkSmartPointer<vtkLookupTable>::New();
	//lookupTable->SetTableRange(0, 1); //���ò��ұ��е�ֵ��Χ ��������ɫӳ��
	lookupTable->SetRange(0, 1); //�������ݷ�Χ�����Զ���Ӧ��ɫӳ��
	lookupTable->SetNumberOfColors(16);//������ɫӳ����е���ɫ����
	lookupTable->SetHueRange(0.667, 0); //����ɫ�෶Χ
	lookupTable->Build();

	scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
	scalarBar->SetLookupTable(lookupTable);
	scalarBar->SetNumberOfLabels(16);
	scalarBar->SetVisibility(0);

	// ������ɫ���Ŀ��
	scalarBar->SetWidth(0.15);
	// ������ɫ���ĸ߶�
	scalarBar->SetHeight(0.8);
	// ������ɫ������Ⱦ�����е�λ��
	scalarBar->SetPosition(0.85, 0.1); // (x, y) ���꣬��Χ�� 0 �� 1
	// ��ȡ�����ı�����
	vtkTextProperty* titleTextProperty = scalarBar->GetTitleTextProperty();
	// ���ñ��������С
	titleTextProperty->SetFontSize(10);
	// ���ñ���������ʽ����������Ϊ����
	titleTextProperty->SetBold(0);
	// ���ñ�������б��
	titleTextProperty->SetItalic(0);
	// ���ñ���������ɫ
	titleTextProperty->SetColor(1.0, 1.0, 1.0); // ����Ϊ��ɫ
}

resultVisualize::~resultVisualize()
{}

void resultVisualize::Init()
{
	m_frames = 0;
	int steps = ui.comboBox_step->count();
	for (int i = 0; i < steps; ++i) {
		ui.comboBox_step->setCurrentIndex(i);
	}
	ui.comboBox_nephogram->setCurrentIndex(0);
	ui.comboBox_nephogram->setCurrentIndex(1);

	if (ui.checkBox_showPoints->isChecked()) {
		FocusedModel->showAnimationPointsActor(true);
	}
	else {
		FocusedModel->showAnimationPointsActor(false);
	}

	if (ui.checkBox_showOriginal->isChecked()) {
		FocusedModel->showOriginalActor(true);
	}
	else {
		FocusedModel->showOriginalActor(false);
	}
}

void resultVisualize::set_fps(int fps)
{
	m_speed = fps;
}

void resultVisualize::showOriginalActor(bool flag)
{
	if (FocusedModel == nullptr) return;

	updateTimer->stop();

	FocusedModel->showOriginalActor(flag);
	pCAE->m_renderWindow->Render();

	updateTimer->start();
}

void resultVisualize::showPointsActor(bool flag)
{//�����еĵ�
	if (FocusedModel == nullptr) return;

	updateTimer->stop();

	FocusedModel->showAnimationPointsActor(flag);
	pCAE->m_renderWindow->Render();

	updateTimer->start();
}

void resultVisualize::showLinesActor(bool flag)
{
	if (FocusedModel == nullptr) return;

	FocusedModel->showAnimationLinesActor(flag);
	pCAE->m_renderWindow->Render();
}

void resultVisualize::updateBoundary()//���±߽�
{
	int step = ui.comboBox_step->count();
	//������boundary
	for (int i = 1; i <= step; ++i)
	{
		FocusedModel->s->get_outputter(i).FindBoundary();
	}
}

void resultVisualize::getBoundary()
{
	if (FocusedModel == nullptr) return;

	double bounds[6];
	FocusedModel->Node_actor->GetBounds(bounds);
	double boundary = FocusedModel->boundary;
	for (int i = 0; i < 3; ++i)
	{
		boundary = std::max(bounds[i * 2 + 1] - bounds[i * 2], boundary);
	}
	FocusedModel->boundary = boundary;
}

void resultVisualize::setCurrentModel(Instance* Model)
{
	FocusedModel = Model;
}

void resultVisualize::closeEvent(QCloseEvent* event)
{
	updateTimer->stop();
	m_frames = 0;
	removeActor();

	pCAE->m_renderWindow->Render();
	QDialog::closeEvent(event);
}

void resultVisualize::show()
{
	FocusedModel->addGeometryData();//�������������(copy)

	//������combox
	SetStepCombox();

	//setBoundary
	getBoundary();

	FocusedModel->m_vtklines->GetMapper()->SetLookupTable(lookupTable);
	updateBoundary();

	pCAE->m_Renderer->AddActor(scalarBar);
	scalarBar->SetVisibility(1);
	Init();
	pCAE->m_renderWindow->Render();
	pCAE->m_Renderer->ResetCamera();
	// ���ø����show()��������ʾ�Ի���
	QDialog::show();
}

void resultVisualize::update()
{
	int iType = ui.comboBox_nephogram->currentIndex();
	int step = ui.comboBox_step->currentIndex() + 1;

	double ampFator = ui.lineEdit->text().toDouble();
	ampFator = std::max(1., ampFator);

	if (!FocusedModel->update(loopPlay, step, m_frames, iType, ampFator))
	{//����ʧ��
		updateTimer->stop();
	}

	m_frames++;
	ui.label_frame->setText(QString::number(m_frames));//֡
	pCAE->m_renderWindow->Render();
}

void resultVisualize::start()
{
	updateTimer->stop();
	m_frames = 0;

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

void resultVisualize::autoFactor(bool flag)
{
	if (!flag) return;

	if (FocusedModel == nullptr) return;

	double max_disp = 0;

	int step = ui.comboBox_step->currentIndex() + 1;
	Outputter* m_Outputter = &FocusedModel->s->get_outputter(step);

	//Boundary
	double max_dx = m_Outputter->getBoundaryDisplaymentX()[1];
	if (max_disp < max_dx) max_disp = max_dx;

	double max_dy = m_Outputter->getBoundaryDisplaymentY()[1];
	if (max_disp < max_dy) max_disp = max_dy;

	double max_dz = m_Outputter->getBoundaryDisplaymentZ()[1];
	if (max_disp < max_dz) max_disp = max_dz;

	double ampFator = FocusedModel->boundary / max_disp / 10;

	ui.lineEdit->setText(QString::number(ampFator));
}

void resultVisualize::removeActor()
{
	pCAE->m_Renderer->RemoveActor(scalarBar);
	FocusedModel->removeActor();
	pCAE->m_renderWindow->Render();
}

void resultVisualize::setNephogramType(int iTpye)
{
	updateTimer->stop();

	currentType = static_cast<DataType>(iTpye);

	//qDebug() << "��ͼ���ͣ�" << currentType;

	if (FocusedModel == nullptr) return;

	int step = ui.comboBox_step->currentIndex() + 1;
	Outputter* m_Outputter = &FocusedModel->s->get_outputter(step);

	//setBoundary
	vector<double> boundary(2);

	switch (currentType)
	{
	case DataType::U1:
		boundary = m_Outputter->getBoundaryDisplaymentZ();
		scalarBar->SetTitle("Ux         ");
		break;
	case DataType::U2:
		boundary = m_Outputter->getBoundaryDisplaymentX();
		scalarBar->SetTitle("Uy         ");
		break;
	case DataType::U3:
		boundary = m_Outputter->getBoundaryDisplaymentY();
		scalarBar->SetTitle("Uz         ");
		break;
	case DataType::MagnitudeU:
		boundary = m_Outputter->getBoundaryDisplayment();
		scalarBar->SetTitle("U          ");
		break;
	case DataType::UR1:
		boundary = m_Outputter->getBoundaryRotationZ();
		scalarBar->SetTitle("URx         ");
		break;
	case DataType::UR2:
		boundary = m_Outputter->getBoundaryRotationX();
		scalarBar->SetTitle("URy         ");
		break;
	case DataType::UR3:
		boundary = m_Outputter->getBoundaryRotationY();
		scalarBar->SetTitle("URz         ");
		break;
	case DataType::N:
		boundary = m_Outputter->getBoundaryStressN();
		scalarBar->SetTitle("StressN         ");
		break;
	case DataType::M2:
		boundary = m_Outputter->getBoundaryStressM2();
		scalarBar->SetTitle("StressMy         ");
		break;
	case DataType::M3:
		boundary = m_Outputter->getBoundaryStressM3();
		scalarBar->SetTitle("StressM2         ");
		break;
	case DataType::Mises:
		boundary = m_Outputter->getBoundaryMises();
		scalarBar->SetTitle("Mises         ");
		break;
	default:
		qDebug() << "δ֪��ͼ���ͣ�";
		break;
	}
	qDebug() << "range of value:" << boundary[0] << " " << boundary[1];
	if (boundary[0] > boundary[1])
	{
		std::swap(boundary[0], boundary[1]);
	}
	lookupTable->SetRange(boundary[0], boundary[1]);
	lookupTable->Build();

	FocusedModel->m_vtklines->GetMapper()->SetScalarRange(boundary[0], boundary[1]);
}

void resultVisualize::SetStepCombox()
{
	if (FocusedModel == nullptr)
	{
		return;
	}
	ui.comboBox_step->clear();
	int stepnum = FocusedModel->s->getTotalSteps();
	for (int i = 1; i <= stepnum; ++i)
	{
		ui.comboBox_step->addItem(QString("Step-") + QString::number(i));
	}
}

void resultVisualize::setCurrentStep(int index)
{
	updateTimer->stop();
	if (index < 0) return;

	index += 1;

	//m_Outputter = &(pCAE->s->get_outputter(index));

	qDebug() << "current step:" << index;
}