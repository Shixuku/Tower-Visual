#include "ChooseNode.h"
#include"resultVisualize.h"
#include"Instance.h"
#include <cmath>
#include <iostream>

#pragma execution_character_set("utf-8")

ChooseNode::ChooseNode(resultVisualize* res, QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_res = res;
    m_ins = m_res->m_ins;

    SetComBox();
    //connect(ui.comboBoxABC, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeWireNode));
    connect(ui.comboBoxABC, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ChooseNode::ChangeWireNode);
	connect(ui.btn_ok, &QPushButton::clicked, this, &ChooseNode::ShowGragh);
}

ChooseNode::~ChooseNode()
{
    if (m_chart) delete m_chart;
    if (m_line) delete m_line;
    if (m_chartView) delete m_chartView;
}

void ChooseNode::SetComBox()
{
    for (const auto& i : m_ins->WireMidId)//ȡ�����е��ߵ��е�
    {
        int key = i.first; // ��ȡ��
        QString name;
        if (key == 0)
        {
            name = "1A";
        }
        else if (key == 1)
        {
            name = "1B";
        }
        else if (key == 2)
        {
            name = "1C";
        }
        else if (key == 3)
        {
            name = "2A";
        }
        else if (key == 4)
        {
            name = "2B";
        }
        else if (key == 5)
        {
            name = "2C";
        }
        ui.comboBoxABC->addItem(name);
    }
    for (const auto& i : m_ins->GroundMidId)//ȡ�����е��ߵ��е�
    {
        int key = i.first; // ��ȡ��
        QString name;
        if (key == 0)
        {
            name = "Ground1";
        }
        else if (key == 1)
        {
            name = "Ground2";
        }
        ui.comboBoxABC->addItem(name);
    }
    int sizeNode = m_ins->WireMidId[0].size();
    for (int i = 0; i < sizeNode; i++)
    {
        
        ui.comboBox123->addItem(QString::number(i+1)+" "+ QString::number(m_ins->WireMidId[0][i]));
    }
    emit ui.comboBoxABC->currentIndexChanged(0);//����һ��item��ΪĬ��
    emit ui.comboBox123->currentIndexChanged(0);//����һ��item��ΪĬ��
}

void ChooseNode::ShowGragh()
{
    Outputter* outputter = m_res->getCurrentOutputter();
    getNode();

    // ����ʱ�䲽���Ͳ���������
    double timeStep = 0; // ÿ0.1�����һ��
    Eigen::MatrixXd displacement = outputter->outputNodeDisplaycement(IdNode, timeStep);
    //displacement ��ʽ size(n, 3)
    // ÿ��һ������ ��Ӧ xyz
    //std::cout << "dt: " << dt << "\n";
    //std::cout << displacement << "\n";
   
    int numPoints = displacement.rows();   // �ܹ�����100����

    // ����z��������
    std::vector<double> zCoordinates;
    for (int i = 0; i < displacement.rows(); ++i) {
        double x = displacement(i, 0);
        double y = displacement(i, 1);
        double z = displacement(i, 2);
        double magnitude = sqrt(x * x+y * y+ z * z);
        zCoordinates.push_back(magnitude);
    }

    QLineSeries* line = new QLineSeries;
    line->setColor(QColor(0, 100, 255));
    line->setName("����1");
    line->setVisible(true);
    for (int i = 0; i < numPoints; ++i)
    {
        line->append(QPointF(i * timeStep, zCoordinates[i]));
    }
    Plot(line);

    //// ���������
    //vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    //vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
    //for (int i = 0; i < numPoints; ++i)
    //{
    //    points->InsertNextPoint(i * timeStep, zCoordinates[i], 0);
    //    if (i > 0)
    //    {
    //        vtkIdType line[2] = { i - 1, i };
    //        lines->InsertNextCell(2, line);
    //    }
    //}
    //// ����PolyData����
    //vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    //polyData->SetPoints(points);
    //polyData->SetLines(lines);

    //// ����PolyDataMapper
    //vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    //mapper->SetInputData(polyData);

    //// ����Actor
    //vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    //actor->SetMapper(mapper);
    //actor->GetProperty()->SetColor(0.0, 0.0, 1.0); // ����������ɫΪ��ɫ
    //actor->GetProperty()->SetLineWidth(2.0);       // �����������

    //// ������Ⱦ��
    //vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    //renderer->SetBackground(1.0, 1.0, 1.0); // ���ñ�����ɫΪ��ɫ
    //renderer->AddActor(actor);

    //// ����X��
    //vtkSmartPointer<vtkAxisActor2D> xAxis = vtkSmartPointer<vtkAxisActor2D>::New();
    //xAxis->GetPositionCoordinate()->SetCoordinateSystemToWorld();
    //xAxis->GetPositionCoordinate()->SetValue(0, 0, 0);
    //xAxis->GetPosition2Coordinate()->SetCoordinateSystemToWorld();
    //xAxis->GetPosition2Coordinate()->SetValue(100, 0, 0);
    //xAxis->SetTitle("Time (s)");
    //xAxis->GetTitleTextProperty()->SetColor(1.0, 1.0, 1.0);

    //// ����X���ǩ
    //vtkTextProperty* xLabelProp = xAxis->GetLabelTextProperty();
    //xLabelProp->SetColor(0, 0, 0);
    //xLabelProp->SetFontSize(10); // ���������С
    //xAxis->SetNumberOfLabels(2); // ֻ��ʾ������ǩ

    //// ��������ᵽ��Ⱦ��
    //renderer->AddActor(xAxis);

    //// ����Y��
    //vtkSmartPointer<vtkAxisActor2D> yAxis = vtkSmartPointer<vtkAxisActor2D>::New();
    //yAxis->GetPositionCoordinate()->SetCoordinateSystemToWorld();
    //yAxis->GetPositionCoordinate()->SetValue(0, 0, 0);
    //yAxis->GetPosition2Coordinate()->SetCoordinateSystemToWorld();
    //yAxis->GetPosition2Coordinate()->SetValue(0, 100, 0); // ����Y�᷶Χ
    //yAxis->SetTitle("Z");
    //yAxis->GetTitleTextProperty()->SetColor(1.0, 1.0, 1.0);

    //// ����Y���ǩ
    //vtkTextProperty* yLabelProp = yAxis->GetLabelTextProperty();
    //yLabelProp->SetColor(0, 0, 0);
    //yLabelProp->SetFontSize(10); // ���������С
    //yAxis->SetNumberOfLabels(2); // ֻ��ʾ������ǩ

    //// ��������ᵽ��Ⱦ��
    //renderer->AddActor(yAxis);


    //// ȷ��������귶Χ
    //double minX = std::numeric_limits<double>::max();
    //double maxX = std::numeric_limits<double>::lowest();
    //double minY = std::numeric_limits<double>::max();
    //double maxY = std::numeric_limits<double>::lowest();
    //for (vtkIdType i = 0; i < numPoints; ++i)
    //{
    //    double point[3];
    //    points->GetPoint(i, point);
    //    minX = std::min(minX, point[0]);
    //    maxX = std::max(maxX, point[0]);
    //    minY = std::min(minY, point[1]);
    //    maxY = std::max(maxY, point[1]);
    //}
    //for (vtkIdType i = 0; i < numPoints; ++i)
    //{
    //    double point[3];
    //    points->GetPoint(i, point);
    //    point[0] *= 100.0 / (numPoints * timeStep);
    //    point[1] *= 100.0 / (*std::max_element(zCoordinates.begin(), zCoordinates.end()));
    //    points->SetPoint(i, point);
    //}
    //points->Modified();

    //xAxis->SetRange(minX, maxX);
    //yAxis->SetRange(minY, maxY);
    //// ������Ⱦ����
    //vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    //renderWindow->AddRenderer(renderer);

    //// ����������
    //vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    //renderWindow->SetInteractor(interactor);

    //// ��Ⱦ����
    //renderWindow->Render();

    //// ��ʼ�¼�ѭ��
    //interactor->Start();
}

void ChooseNode::ChangeWireNode()
{
    ui.comboBox123->blockSignals(true);
    ui.comboBox123->clear();
    int i = ui.comboBoxABC->currentIndex();
    QString name = ui.comboBoxABC->itemText(i);
    qDebug() << "name:  " << name << "\n";
    if (name == "1A")
    {
        int sizeNode = m_ins->WireMidId[0].size();
        for (int i = 0; i < sizeNode; i++)
        {
            ui.comboBox123->addItem(QString::number(i + 1) + " " + QString::number(m_ins->WireMidId[0][i]));
            emit ui.comboBox123->currentIndexChanged(0);//����һ��item��ΪĬ��
        }
    }
    else if (name == "1B")
    {
        int sizeNode = m_ins->WireMidId[1].size();
        for (int i = 0; i < sizeNode; i++)
        {
            ui.comboBox123->addItem(QString::number(i + 1) + " " + QString::number(m_ins->WireMidId[1][i]));
            emit ui.comboBox123->currentIndexChanged(0);//����һ��item��ΪĬ��
        }
    }
    else if (name == "1C")
    {
        int sizeNode = m_ins->WireMidId[2].size();
        for (int i = 0; i < sizeNode; i++)
        {
            ui.comboBox123->addItem(QString::number(i + 1) + " " + QString::number(m_ins->WireMidId[2][i]));
            emit ui.comboBox123->currentIndexChanged(0);//����һ��item��ΪĬ��
        }
    }
    else if (name == "2A")
    {
        int sizeNode = m_ins->WireMidId[3].size();
        for (int i = 0; i < sizeNode; i++)
        {
            ui.comboBox123->addItem(QString::number(i + 1) + " " + QString::number(m_ins->WireMidId[3][i]));
            emit ui.comboBox123->currentIndexChanged(0);//����һ��item��ΪĬ��
        }
    }
    else if (name == "2B")
    {
        int sizeNode = m_ins->WireMidId[4].size();
        for (int i = 0; i < sizeNode; i++)
        {
            ui.comboBox123->addItem(QString::number(i + 1) + " " + QString::number(m_ins->WireMidId[4][i]));
            emit ui.comboBox123->currentIndexChanged(0);//����һ��item��ΪĬ��
        }
    }
    else if (name == "2C")
    {
        int sizeNode = m_ins->WireMidId[5].size();
        for (int i = 0; i < sizeNode; i++)
        {
            ui.comboBox123->addItem(QString::number(i + 1) + " " + QString::number(m_ins->WireMidId[5][i]));
            emit ui.comboBox123->currentIndexChanged(0);//����һ��item��ΪĬ��
        }
    }
    else if (name == "Ground1")
    {
        int sizeNode = m_ins->GroundMidId[0].size();
        for (int i = 0; i < sizeNode; i++)
        {
            ui.comboBox123->addItem(QString::number(i + 1) + " " + QString::number(m_ins->GroundMidId[0][i]));
            emit ui.comboBox123->currentIndexChanged(0);//����һ��item��ΪĬ��
        }
    }
    else if (name == "Ground2")
    {
        int sizeNode = m_ins->GroundMidId[1].size();
        for (int i = 0; i < sizeNode; i++)
        {
            ui.comboBox123->addItem(QString::number(i + 1) + " " + QString::number(m_ins->GroundMidId[1][i]));
            emit ui.comboBox123->currentIndexChanged(0);//����һ��item��ΪĬ��
        }
    }
    ui.comboBox123->blockSignals(false);
}

void ChooseNode::getNode()
{
    int i = ui.comboBoxABC->currentIndex();
    int nodeadss = ui.comboBox123->currentIndex();
    QString name = ui.comboBoxABC->itemText(i);
    if (name == "1A")
    {
        
        IdNode = m_ins->WireMidId[0][nodeadss];
    }
    else if (name == "1B")
    {
        IdNode = m_ins->WireMidId[1][nodeadss];
    }
    else if (name == "1C")
    {
        IdNode = m_ins->WireMidId[2][nodeadss];
    }
    else if (name == "2A")
    {
        IdNode = m_ins->WireMidId[3][nodeadss];
    }
    else if (name == "2B")
    {
        IdNode = m_ins->WireMidId[4][nodeadss];
    }
    else if (name == "2C")
    {
        IdNode = m_ins->WireMidId[5][nodeadss];
    }
    else if (name == "Ground1")
    {
        IdNode = m_ins->GroundMidId[0][nodeadss];
    }
    else if (name == "Ground2")
    {
        IdNode = m_ins->GroundMidId[1][nodeadss];
    }
}

void ChooseNode::initChart()
{
    if (m_chart != nullptr)
    {
        delete m_chart;
    }
    m_chart = new QChart;
    // ����һ��QChartView����ʾQChart
    QChartView* chartView = new QChartView(m_chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // ��QChartViewǶ�뵽frameChart��
    QVBoxLayout* layout = new QVBoxLayout(ui.frame); // ȷ��frameChart�Ķ���������frameChart
    layout->addWidget(chartView);

    m_chart->setTheme(QChart::ChartThemeLight);//���ð�ɫ����
    m_chart->setDropShadowEnabled(true);//������Ӱ    m_chart->setAutoFillBackground(true);  //���ñ����Զ����

    m_chart->setTitleBrush(QBrush(QColor(0, 0, 255)));//���ñ���Brush
    m_chart->setTitleFont(QFont("΢���ź�"));//���ñ�������
    m_chart->setTitle("����ͼ");

    //�޸�˵����ʽ
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);//�ײ�����
    m_chart->legend()->setBackgroundVisible(true);//���ñ����Ƿ����
    m_chart->legend()->setAutoFillBackground(true);//���ñ����Զ����
    m_chart->legend()->setColor(QColor(222, 233, 251));//������ɫ
    m_chart->legend()->setLabelColor(QColor(0, 100, 255));//���ñ�ǩ��ɫ
    m_chart->legend()->setMaximumHeight(50);
    m_chartView = new QChartView(m_chart, this);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->resize(600, 600);
    m_chartView->show();
}

void ChooseNode::set_Axis()
{
    ////����X���Y��
//QValueAxis* axisX = new QValueAxis;
//axisX->setRange(0, 150);    //Ĭ��������Ϊ��̬�����С��
//axisX->setLabelFormat("%ds");
//QValueAxis* axisY = new QValueAxis;
//axisY->setRange(0, 250);    //Ĭ��������Ϊ��̬�����С��
//axisY->setTitleText("valueֵ");
}

void ChooseNode::Plot(QLineSeries* lineData)
{
    if (m_line) delete m_line;
    m_line = lineData;

    initChart();
    m_chart->addSeries(m_line);//���ϵ�е�QChart��

    //����X���Y��
    QValueAxis* axisX = new QValueAxis;
    //axisX->setRange(0, 150);    //Ĭ��������Ϊ��̬�����С��
    axisX->setTitleText("t");
    //axisX->setLabelFormat("%d");
    QValueAxis* axisY = new QValueAxis;
    //axisY->setRange(0, 250);    //Ĭ��������Ϊ��̬�����С��
    axisY->setTitleText("value");
    m_chart->setAxisX(axisX, m_line);
    m_chart->setAxisY(axisY, m_line);
    //m_chart->createDefaultAxes();             //���ߴ���Ĭ����
}
