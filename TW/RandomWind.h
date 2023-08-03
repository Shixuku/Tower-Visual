#pragma once

#include <QDialog>
#include "ui_RandomWind.h"

#include"Node_Base.h"
#include"Node.h"
#include<set>
#include"TowerWireGroup.h"
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include"Ele_Location.h"

QT_CHARTS_USE_NAMESPACE

class InterFace;
class Force_Wind;
class Wind;

class RandomWind : public QDialog
{
	Q_OBJECT

		friend class Wind;
public:
	RandomWind(Wind* m_wind, QWidget *parent = nullptr);
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
	TowerWireGroup* R_pcreatWire = nullptr;
	//m_pair(1.29), alf(0.16), z0(0.03), v10(30) int N, int M, double w_up, double T
	double z0;//�ر�ֲڳ���
	double v10;//ʮ�׸�10����ƽ���ٶ�
	int N;//Ƶ�ʵȷ���
	int M;//M>=2*M
	double w_up;//��Ƶ��ֹ��
	double T;//ģ��ʱ��
	int NumOfWindzones;//��������
	void initChart();
	//void set_Axis();
	void Plot(QLineSeries* lineData);

	void InputPoints();
	void SimulateWind();
	void CreatCombobox();
	void Initialize();
	void CreatSegment();
	void CreatEleGather();
	void ComputeEleCoordinate();//����ÿ����Ԫ�ĵ�Ч����
	std::vector<Ele_Location>m_EleLocation;
	std::map<int, int>m_EleWind;

public slots:
	void ShowPic(int index);//��ʾ����������ͼ

private:
	Ui::RandomWindClass ui;

	Force_Wind* fw = nullptr;
};
