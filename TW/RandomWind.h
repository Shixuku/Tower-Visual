#pragma once

#include <QDialog>
#include "ui_RandomWind.h"

#include"Node_Base.h"
#include"Node.h"
#include<set>

#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

class Force_Wind;
class InterFace;
class Wind;

class RandomWind : public QDialog
{
	Q_OBJECT

public:
	RandomWind(QWidget *parent = nullptr);
	~RandomWind();

	InterFace* m_pInterFace = nullptr;

	int Index = 0;
	std::vector<Node_Base> points;
	std::set<Node*> pointsSelected;
	QChart* m_chart = nullptr;
	QLineSeries* m_line = nullptr;
	QChartView* m_chartView = nullptr;
	QValueAxis* axisX = nullptr;
	QValueAxis* axisY = nullptr;
	Wind* wd = nullptr;
	//m_pair(1.29), alf(0.16), z0(0.03), v10(30) int N, int M, double w_up, double T
	double alf;//����ֲڳ̶�
	double z0;//�ر�ֲڳ���
	double v10;//ʮ�׸�10����ƽ���ٶ�
	int N;//Ƶ�ʵȷ���
	int M;//M>=2*M
	double w_up;//��Ƶ��ֹ��
	double T;//ģ��ʱ��

	void initChart();
	//void set_Axis();
	void Plot(QLineSeries* lineData);

	void InputPoints();
	void SimulateWind();
	void CreatCombobox();
	void Initialize();

public slots:
	void ShowPic(int index);//��ʾ����������ͼ

private:
	Ui::RandomWindClass ui;

	Force_Wind* fw = nullptr;
};
