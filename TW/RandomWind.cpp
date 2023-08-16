#include "RandomWind.h"
#include <memory>
#include"InterFace.h"
#include"Wind.h"
#include"Instance.h"
#include <qmessagebox.h>
#include <qdialog.h>

#pragma execution_character_set("utf-8")

RandomWind::RandomWind( Wind* m_wind, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	wd = m_wind;
	initChart();
	Initialize();
	//connect(m_pInterFace, &InterFace::Msg_Select_Nodes, this, &RandomWind::InputPoints);

	connect(ui.ok_btn, &QPushButton::clicked, [=]() {	if (ui.NodeV_com->count() > 0) {
		return; // ���ComboBox�Ѿ���������������µ���
	}
	if (wd->alf == 0)
	{
		QMessageBox::warning(this, "��ʾ", "�����ϸ�����ѡ�����ֲڳ̶ȣ�");
		return;
	}
	ComputeEleCoordinate(); CreatEleGather(); SimulateWind(); CreatCombobox();   });
	void (QComboBox:: * activated)(int) = &QComboBox::activated;
	connect(ui.NodeV_com, activated, this, &RandomWind::ShowPic);
	ui.pushButton_3->setToolTip("�غ���0.005 - 0.02����������0.03 - 0.1�������ﲻ��Ľ���0.2 - 0.4; ���������2.0 - 3.0");

	connect(ui.btn_fileName, &QPushButton::clicked, this, &RandomWind::setFileName);
}

RandomWind::~RandomWind()
{
	//if (m_line) delete m_line;
	//if (fw) delete fw;
	//if (m_chartView != nullptr)
	//{
	//	delete m_chartView;
	//}
	//if (m_chart != nullptr)
	//{
	//	delete m_chart;
	//}
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
	//ui.lineEdit_points->setText(str);
}

void RandomWind::SimulateWind()
{
	//if (ui.lineEdit_points->text() == 0) return;//û������
	if (ui.lineEdit_segment->text() == 0)return;//û�и��ֶ�ȡ
	//ȡ����
	z0 = ui.z0_edi->text().toDouble();
	v10 = ui.V10_edi->text().toDouble();
	N = ui.N_edi->text().toInt();
	M = ui.M_edi->text().toInt();
	w_up = ui.CutOff_edi->text().toDouble();
	T = ui.Time_edi->text().toDouble();

	//for (auto pNode : pointsSelected)
	//{
	//	Node_Base n(pNode->m_idNode, pNode->x, pNode->y, pNode->z);
	//	//Node_Base n(1, 10, 10, 10);
	//	points.push_back(n);
	//}
	CreatSegment();
	//
	//if (fw) delete fw;  //��ֹ�ڴ�й¶
	////fw = new Force_Wind;
	//
	//fw = new Force_Wind(1.29, wd->alf, z0, v10, N, M, w_up, T);
	//fw->set_FilePath(QString("D:/"));
	//fw->input_node(points);
	//fw->simulate();
	
	
	if (fw) delete fw;  //��ֹ�ڴ�й¶
	//fw = new Force_Wind;
	
	fw = new Force_Wind(1.29, wd->alf, z0, v10, N, M, w_up, T);
	QString filename = ui.lineEdit_fileName->text();
	fw->set_FileName(filename);

	ParameterStableWind pwind = ParameterStableWind(wd->id, wd->AnalysisStep, wd->angle, filename, wd->m_pInstance->Splits, wd->m_pInstance->LandformsType, wd->WindCoefficient);
	wd->m_pInstance->m_StableWind.push_back(pwind);

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
	ui.V10_edi->setText("15");
	ui.N_edi->setText("3000");
	ui.CutOff_edi->setText("6.283");
	ui.Time_edi->setText("600");
	ui.M_edi->setText("6000");

}

void RandomWind::CreatSegment()
{
	//wd->m_pInstance
	//R_pcreatWire = wd->m_pcreatWire;
	//R_pcreatWire->m_Nodes;
	NumOfWindzones = ui.lineEdit_segment->text().toInt();
	//����
	double Lx = wd->m_pInstance->m_Nodes[1].x - wd->m_pInstance->m_Nodes[0].x;
	double Ly = wd->m_pInstance->m_Nodes[1].y - wd->m_pInstance->m_Nodes[0].y;
	double x1 = Lx / (2 * NumOfWindzones);
	double y1 = Ly / (2 * NumOfWindzones);
	for (int i = 0; i < NumOfWindzones; i++)
	{
		double xi = x1 + 2 * i * x1;
		double yi = y1 + 2 * i * y1;
		double closestDistance = std::numeric_limits<double>::max();
		Node closestNode;
		for (const auto& node : wd->m_pInstance->m_Nodes)
		{
			double distance = std::sqrt((node.x - xi) * (node.x - xi) + (node.y - yi) * (node.y - yi));
			if (distance < closestDistance)
			{
				closestDistance = distance;
				closestNode = node;
			}
		}
		// ��ȡ���������� x��y �� z ֵ
		double closestX = closestNode.x;
		double closestY = closestNode.y;
		//double closestZ = closestNode.z;
		Node_Base n(1, closestX, closestY, 10);
		points.push_back(n);
	}
}

void RandomWind::ShowPic(int index)
{

	/*QLineSeries* qline = fw->getQLineSeries(index);
	Plot(qline);*/
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
	m_chart->addSeries(m_line);//���ϵ�е�QChart��

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

void RandomWind::CreatEleGather()
{
	//ins = std::make_unique<Instance>(); // ��ʼ��ָ��
	//wd->m_pInstance = wd->m_pcreatWire;
	// ���� x �� y ����Сֵ�����ֵ
	double xmin = std::numeric_limits<double>::max();
	double xmax = std::numeric_limits<double>::lowest();
	double ymin = std::numeric_limits<double>::max();
	double ymax = std::numeric_limits<double>::lowest();

	for (const auto& node : wd->m_pInstance->m_Nodes) {
		if (node.x < xmin) xmin = node.x;
		if (node.x > xmax) xmax = node.x;
		if (node.y < ymin) ymin = node.y;
		if (node.y > ymax) ymax = node.y;
	}

	// �ж� xmax-xmin �� ymax-ymin �ĸ���ֵ����
	bool sortByX = (xmax - xmin) > (ymax - ymin);

	// �� m_Nodes �еĵ㰴�� x �� y ������������
	std::sort(wd->m_pInstance->m_Nodes.begin(), wd->m_pInstance->m_Nodes.end(), [sortByX](const Node& a, const Node& b) {
		return sortByX ? (a.x < b.x) : (a.y < b.y);
		});
	int NumSegments = ui.lineEdit_segment->text().toInt();
	if (sortByX)//����x����
	{
		for (int i = 0; i < NumSegments; i++)
		{

		}
	}
	else if (!sortByX)//����y����
	{
		// ����ÿ������� y ֵ��Χ
		double yRange = (ymax - ymin) / NumSegments;
		// ��ȡÿ������ı߽� y ֵ
		for (int i = 0; i < NumSegments; i++)
		{
			double segmentMinY = ymin + i * yRange;
			double segmentMaxY = ymin + (i + 1) * yRange;
			for (int j = 0; j < m_EleLocation.size(); j++)
			{
				const Ele_Location& ele = m_EleLocation[j]; // ��ȡ m_EleLocation �еĵ�ǰԪ��
				if (ele.m_y >= segmentMinY && ele.m_y <= segmentMaxY)
				{
					wd->m_pInstance->m_EleWind[i + 1].push_back(ele.m_id);
				}
			}
		}
	}

}

void RandomWind::ComputeEleCoordinate()
{
	//wd->m_pInstance = wd->m_pcreatWire;
	int numTruss = wd->m_pInstance->m_Elements_Trusses.size();
	int numBeam = wd->m_pInstance->m_Elements_beams.size();
	int totalNum = numTruss + numBeam;
	m_EleLocation.resize(totalNum);
	for (int i = 0; i < numTruss; i++)
	{
		const auto& ele = wd->m_pInstance->m_Elements_Trusses[i];
		int ipt1 = ele.m_idNode[0] - 1; // id
		int ipt2 = ele.m_idNode[1] - 1;
		const auto& node1 = wd->m_pInstance->m_Nodes[ipt1];
		const auto& node2 = wd->m_pInstance->m_Nodes[ipt2];
		int id = ele.m_idElement;
		double x = (node2.x + node1.x) / 2;
		double y = (node2.y + node1.y) / 2;
		double z = (node2.z + node1.z) / 2;
		m_EleLocation[i].m_id = id;
		m_EleLocation[i].m_x = x;
		m_EleLocation[i].m_y = y;
		m_EleLocation[i].m_z = z;
	}
	for (int i = numTruss; i < totalNum; i++)
	{
		const auto& ele = wd->m_pInstance->m_Elements_beams[i - numTruss];
		int ipt1 = ele.m_idNode[0] - 1; // id
		int ipt2 = ele.m_idNode[1] - 1;
		const auto& node1 = wd->m_pInstance->m_Nodes[ipt1];
		const auto& node2 = wd->m_pInstance->m_Nodes[ipt2];
		int id = ele.m_idElement;
		double x = (node2.x + node1.x) / 2;
		double y = (node2.y + node1.y) / 2;
		double z = (node2.z + node1.z) / 2;
		m_EleLocation[i].m_id = id;
		m_EleLocation[i].m_x = x;
		m_EleLocation[i].m_y = y;
		m_EleLocation[i].m_z = z;
	}
}

void RandomWind::setFileName()
{
	QString fileName = QFileDialog::getSaveFileName(this,"����", "/", "txtfile(*.txt);;All file(*.*)");
	ui.lineEdit_fileName->setText(fileName);
	
}
