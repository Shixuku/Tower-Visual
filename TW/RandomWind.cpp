#include "RandomWind.h"

#include"InterFace.h"
#include"Wind.h"

#include <qmessagebox.h>


RandomWind::RandomWind(Wind* m_wind, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	wd = m_wind;

	initChart();
	Initialize();
	connect(m_pInterFace, &InterFace::Msg_Select_Nodes, this, &RandomWind::InputPoints);

	connect(ui.ok_btn, &QPushButton::clicked, [=]() {	if (ui.NodeV_com->count() > 0) {
		return; // 如果ComboBox已经包含项，则不再生成新的项
	}
	if (wd->alf == 0)
	{
		QMessageBox::warning(this, "提示", "请在上个界面选择地面粗糙程度！");
		return;
	}
	ComputeEleCoordinate(); CreatEleGather(); SimulateWind(); CreatCombobox(); CreatSegment();  });
	void (QComboBox:: * activated)(int) = &QComboBox::activated;
	connect(ui.NodeV_com, activated, this, &RandomWind::ShowPic);
	ui.pushButton_3->setToolTip("沿海区0.005 - 0.02；开阔场地0.03 - 0.1；建筑物不多的郊区0.2 - 0.4; 大城市中心2.0 - 3.0");
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
	//取点编号
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
	//if (ui.lineEdit_points->text() == 0) return;//没有数据
	if (ui.lineEdit_segment->text() == 0)return;//没有给分段取
	//取数据
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
	//if (fw) delete fw;  //防止内存泄露
	////fw = new Force_Wind;
	//
	//fw = new Force_Wind(1.29, wd->alf, z0, v10, N, M, w_up, T);
	//fw->set_FilePath(QString("D:/"));
	//fw->input_node(points);
	//fw->simulate();
	
}

void RandomWind::CreatCombobox()
{

	int num = points.size();
	for (int i = 0; i < num; i++)
	{
		ui.NodeV_com->addItem("风速" + QString::number(i + 1));
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

void RandomWind::CreatSegment()
{
	R_pcreatWire = wd->m_pcreatWire;
	//R_pcreatWire->m_Nodes;
	NumOfWindzones = ui.lineEdit_segment->text().toInt();
	//单档
	double Lx = R_pcreatWire->m_Nodes[1].x - R_pcreatWire->m_Nodes[0].x;
	double Ly = R_pcreatWire->m_Nodes[1].y - R_pcreatWire->m_Nodes[0].y;
	double x1 = Lx / (2 * NumOfWindzones);
	double y1 = Ly / (2 * NumOfWindzones);
	for (int i = 0; i < NumOfWindzones; i++)
	{
		double xi = x1 + 2 * i * x1;
		double yi = y1 + 2 * i * y1;
		double closestDistance = std::numeric_limits<double>::max();
		Node closestNode;
		for (const auto& node : R_pcreatWire->m_Nodes)
		{
			double distance = std::sqrt((node.x - xi) * (node.x - xi) + (node.y - yi) * (node.y - yi));
			if (distance < closestDistance)
			{
				closestDistance = distance;
				closestNode = node;
			}
		}
		// 获取距离最近点的 x、y 和 z 值
		double closestX = closestNode.x;
		double closestY = closestNode.y;
		double closestZ = closestNode.z;
		Node_Base n(1, closestX, closestY, closestZ);
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
		m_chart->setTheme(QChart::ChartThemeLight);//设置白色主题
		m_chart->setDropShadowEnabled(true);//背景阴影    m_chart->setAutoFillBackground(true);  //设置背景自动填充

		m_chart->setTitleBrush(QBrush(QColor(0, 0, 255)));//设置标题Brush
		m_chart->setTitleFont(QFont("微软雅黑"));//设置标题字体
		m_chart->setTitle("曲线图");
		//修改说明样式
		m_chart->legend()->setVisible(false);
		//m_chart->legend()->setAlignment(Qt::AlignBottom);//底部对齐
		//m_chart->legend()->setBackgroundVisible(true);//设置背景是否可视
		//m_chart->legend()->setAutoFillBackground(true);//设置背景自动填充
		//m_chart->legend()->setColor(QColor(222, 233, 251));//设置颜色
		//m_chart->legend()->setLabelColor(QColor(0, 100, 255));//设置标签颜色
		//m_chart->legend()->setMaximumHeight(50);
		m_chartView = new QChartView;
		m_chartView->setRenderHint(QPainter::Antialiasing);
		m_chartView->resize(600, 600);

		//创建X轴和Y轴
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
	m_chart->addSeries(m_line);//添加系列到QChart上

	// 根据数据动态计算坐标轴的范围和刻度
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
	//m_chart->createDefaultAxes();             //或者创建默认轴

	m_chartView->setChart(m_chart);
	m_chartView->show();
}

void RandomWind::CreatEleGather()
{
	R_pcreatWire = wd->m_pcreatWire;
	// 计算 x 和 y 的最小值和最大值
	double xmin = std::numeric_limits<double>::max();
	double xmax = std::numeric_limits<double>::lowest();
	double ymin = std::numeric_limits<double>::max();
	double ymax = std::numeric_limits<double>::lowest();

	for (const auto& node : R_pcreatWire->m_Nodes) {
		if (node.x < xmin) xmin = node.x;
		if (node.x > xmax) xmax = node.x;
		if (node.y < ymin) ymin = node.y;
		if (node.y > ymax) ymax = node.y;
	}

	// 判断 xmax-xmin 和 ymax-ymin 哪个差值更大
	bool sortByX = (xmax - xmin) > (ymax - ymin);

	// 对 m_Nodes 中的点按照 x 或 y 进行升序排列
	std::sort(R_pcreatWire->m_Nodes.begin(), R_pcreatWire->m_Nodes.end(), [sortByX](const Node& a, const Node& b) {
		return sortByX ? (a.x < b.x) : (a.y < b.y);
		});
	int NumSegments = ui.lineEdit_segment->text().toInt();
	if (sortByX)//按照x升序
	{
		for (int i = 0; i < NumSegments; i++)
		{

		}
	}
	else if (!sortByX)//按照y升序
	{
		// 计算每个区间的 y 值范围
		double yRange = (ymax - ymin) / NumSegments;
		// 获取每个区间的边界 y 值
		for (int i = 0; i < NumSegments; i++)
		{
			double segmentMinY = ymin + i * yRange;
			double segmentMaxY = ymin + (i + 1) * yRange;
			for (int j = 0; j < m_EleLocation.size(); j++)
			{
				const Ele_Location& ele = m_EleLocation[j]; // 获取 m_EleLocation 中的当前元素
				if (ele.m_y >= segmentMinY && ele.m_y <= segmentMaxY)
				{
					m_EleWind[i + 1] = ele.m_id;
					cout << "风区" << "  " << i + 1 << " 单元号" << "  " << ele.m_id << "\n";
				}
			}
		}
	}
	
}

void RandomWind::ComputeEleCoordinate()
{
	R_pcreatWire = wd->m_pcreatWire;
	int numTruss = R_pcreatWire->m_Elements_Trusses.size();
	int numBeam = R_pcreatWire->m_Elements_beams.size();
	int totalNum = numTruss + numBeam;
	m_EleLocation.resize(totalNum);
	for (int i = 0; i < numTruss; i++)
	{
		const auto& ele = R_pcreatWire->m_Elements_Trusses[i];
		int ipt1 = ele.m_idNode[0] - 1; // id
		int ipt2 = ele.m_idNode[1] - 1;
		const auto& node1 = R_pcreatWire->m_Nodes[ipt1];
		const auto& node2 = R_pcreatWire->m_Nodes[ipt2];
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
		const auto& ele = R_pcreatWire->m_Elements_beams[i];
		int ipt1 = ele.m_idNode[0] - 1; // id
		int ipt2 = ele.m_idNode[1] - 1;
		const auto& node1 = R_pcreatWire->m_Nodes[ipt1];
		const auto& node2 = R_pcreatWire->m_Nodes[ipt2];
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
