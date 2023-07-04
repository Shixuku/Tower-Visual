#include "RandomWind.h"

#include"InterFace.h"
#include"Force_Wind.h"
#include"Wind.h"


RandomWind::RandomWind(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	m_pInterFace = dynamic_cast<InterFace*>(parent);
	Q_ASSERT(m_pInterFace != nullptr);
	wd = dynamic_cast<Wind*>(m_pInterFace->wd);
	Q_ASSERT(wd != nullptr);

	initChart();
	Initialize();
	connect(m_pInterFace, &InterFace::Msg_Select_Nodes, this, &RandomWind::InputPoints);

	connect(ui.ok_btn, &QPushButton::clicked, [=]() {	if (ui.NodeV_com->count() > 0) {
		return; // ���ComboBox�Ѿ���������������µ���
	}
	SimulateWind(); CreatCombobox(); });
	void (QComboBox:: * activated)(int) = &QComboBox::activated;
	connect(ui.NodeV_com, activated, this, &RandomWind::ShowPic);
	ui.pushButton_3->setToolTip("�غ���0.005 - 0.02����������0.03 - 0.1�������ﲻ��Ľ���0.2 - 0.4; ���������2.0 - 3.0");
}

RandomWind::~RandomWind()
{
	if (m_line) delete m_line;
	if (fw) delete fw;
	if (m_chartView != nullptr)
	{
		delete m_chartView;
	}
	if (m_chart != nullptr)
	{
		delete m_chart;
	}
}

void RandomWind::InputPoints()
{
	//ȡ����
	if (QApplication::keyboardModifiers() != Qt::ShiftModifier)
	{
		pointsSelected.clear();
	}
	m_pInterFace->Get_SelectedNode(pointsSelected);
	QString str;
	for (auto pNode : pointsSelected)
	{
		str.append(QString::number(pNode->m_idNode) + " ");
	}
	ui.lineEdit_points->setText(str);
}

void RandomWind::SimulateWind()
{
	if (ui.lineEdit_points->text() == 0) return;//û������

	//ȡ����
	z0 = ui.z0_edi->text().toDouble();
	v10 = ui.V10_edi->text().toDouble();
	N = ui.N_edi->text().toInt();
	M = ui.M_edi->text().toInt();
	w_up = ui.CutOff_edi->text().toDouble();
	T = ui.Time_edi->text().toDouble();

	for (auto pNode : pointsSelected)
	{
		Node_Base n(pNode->m_idNode, pNode->x, pNode->y, pNode->z);
		//Node_Base n(1, 10, 10, 10);
		points.push_back(n);
	}

	//
	if (fw) delete fw;  //��ֹ�ڴ�й¶
	//fw = new Force_Wind;
	fw = new Force_Wind(1.29, wd->alf, z0, v10, N, M, w_up, T);
	fw->set_FilePath(QString("D:/"));
	fw->input_node(points);
	fw->simulate();
	
}

void RandomWind::CreatCombobox()
{

	int num = points.size();
	for (int i = 0; i < num; i++)
	{
		ui.NodeV_com->addItem("����" + QString::number(i + 1));
	}
}

void RandomWind::Initialize()
{
	ui.z0_edi->setText("0.03");
	ui.V10_edi->setText("30");
	ui.N_edi->setText("3000");
	ui.CutOff_edi->setText("6.283");
	ui.Time_edi->setText("600");
	ui.M_edi->setText("6000");

}

void RandomWind::ShowPic(int index)
{

	QLineSeries* qline = fw->getQLineSeries(index);
	Plot(qline);
}

void RandomWind::initChart()
{
	if (m_chart == nullptr)
	{
		m_chart = new QChart();
		m_chart->setTheme(QChart::ChartThemeLight);//���ð�ɫ����
		m_chart->setDropShadowEnabled(true);//������Ӱ    m_chart->setAutoFillBackground(true);  //���ñ����Զ����

		m_chart->setTitleBrush(QBrush(QColor(0, 0, 255)));//���ñ���Brush
		m_chart->setTitleFont(QFont("΢���ź�"));//���ñ�������
		m_chart->setTitle("����ͼ");
		//�޸�˵����ʽ
		m_chart->legend()->setVisible(false);
		//m_chart->legend()->setAlignment(Qt::AlignBottom);//�ײ�����
		//m_chart->legend()->setBackgroundVisible(true);//���ñ����Ƿ����
		//m_chart->legend()->setAutoFillBackground(true);//���ñ����Զ����
		//m_chart->legend()->setColor(QColor(222, 233, 251));//������ɫ
		//m_chart->legend()->setLabelColor(QColor(0, 100, 255));//���ñ�ǩ��ɫ
		//m_chart->legend()->setMaximumHeight(50);
		m_chartView = new QChartView;
		m_chartView->setRenderHint(QPainter::Antialiasing);
		m_chartView->resize(600, 600);

		//����X���Y��
		axisX = new QValueAxis;
		axisX->setTitleText("t");
		//axisX->setLabelFormat("%d");
		axisY = new QValueAxis;
		axisY->setTitleText("value");
	}
}

void RandomWind::Plot(QLineSeries * lineData)
{
	if (m_line) delete m_line;
	m_line = lineData;
	m_chart->addSeries(m_line);//����ϵ�е�QChart��

	// �������ݶ�̬����������ķ�Χ�Ϳ̶�
	double minX = m_line->at(0).x();
	double maxX = m_line->at(m_line->count() - 1).x();
	double minY = std::numeric_limits<double>::max();
	double maxY = std::numeric_limits<double>::min();
	for (const QPointF& point : m_line->points()) {
		minY = std::min(minY, point.y());
		maxY = std::max(maxY, point.y());
	}
	axisX->setRange(minX, maxX);
	axisY->setRange(minY, maxY);

	m_chart->setAxisX(axisX, m_line);
	m_chart->setAxisY(axisY, m_line);
	//m_chart->createDefaultAxes();             //���ߴ���Ĭ����

	m_chartView->setChart(m_chart);
	m_chartView->show();
}
