#pragma once

#include <QDialog>
#include "ui_resultVisualize.h"
#include "QTimer"
#include<vtkCellArray.h>
#include<vtkLine.h>
#include<vtkPolyData.h>
#include<vtkCellData.h>
#include<vtkPolyDataMapper.h>
#include<vtkMapper.h>
#include<vtkPointData.h>
#include<vtkProperty.h>
#include<vtkIdTypeArray.h>
#include<vtkVertexGlyphFilter.h>
#include <vtkSmartPointer.h>
#include <vtkDoubleArray.h>
#include <vtkScalarBarActor.h>
#include <map>
#include <vtkActor.h>
#include"Node.h"
#include"Outputter.h"

class InterFace;
class Instance;
class resultVisualize : public QDialog
{
	Q_OBJECT

public:
	resultVisualize(QWidget *parent = nullptr);
	~resultVisualize();
	InterFace* pCAE = nullptr;
	Instance* FocusedModel = nullptr; //��ǰ��ע��ģ��

	std::vector<Node> m_nodes;//����λ�Ƶĵ�

	QTimer* updateTimer;
	double m_dt = 1000;//ms

	vtkSmartPointer<vtkScalarBarActor> scalarBar;//��ͼ��actor
	vtkSmartPointer<vtkLookupTable> lookupTable;

	int m_speed = 0;//�����ٶ�(֡/s)
	int m_frames = 0;//֡
	bool loopPlay = false;
	void set_loopPlay(bool flag) { loopPlay = flag; }
	void Init();
	void set_fps(int fps);
	void update();
	void start();
	void stop();

	void show();

	void autoFactor(bool flag);

	void removeActor();

	void setNephogramType(int iTpye);
	void SetStepCombox();
	void setCurrentStep(int idStep);

	void setCurrentModel(Instance* Model);
protected:
	void updateBoundary();

	void getBoundary();

	void closeEvent(QCloseEvent* event) override;
signals:
	void animationFinished();//��������

public slots:
	void showOriginalActor(bool flag);//��ʾԭʼactor
	void showPointsActor(bool flag);
	void showLinesActor(bool flag);

private:
	Ui::resultVisualizeClass ui;
	DataType currentType;
};
