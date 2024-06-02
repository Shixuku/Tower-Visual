#pragma once
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkAxisActor2D.h>
#include <vtkTextProperty.h>
#include <vtkCamera.h>
#include <vector>
#include <QWidget>
#include "ui_ChooseNode.h"
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

class resultVisualize;
class Instance;
class ChooseNode : public QWidget
{
	Q_OBJECT

public:
	ChooseNode(resultVisualize*res,QWidget *parent = nullptr);
	~ChooseNode();
	void SetComBox();
	resultVisualize* m_res = nullptr;
	Instance* m_ins = nullptr;
	void ShowGragh();
	void ChangeWireNode();
	int IdNode;
	void getNode();

	QChart* m_chart = nullptr;
	QLineSeries* m_line = nullptr;
	QChartView* m_chartView = nullptr;

	void initChart();
	void set_Axis();
	void Plot(QLineSeries* lineData);
private:
	Ui::ChooseNodeClass ui;
};
