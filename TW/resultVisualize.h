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
#include <vtkActor.h>
#include"Node_Base.h"

class InterFace;
class Instance;
class resultVisualize : public QDialog
{
	Q_OBJECT

public:
	resultVisualize(QWidget *parent = nullptr);
	~resultVisualize();
	InterFace* pCAE = nullptr;
	Instance* m_ins = nullptr;

	std::vector<Node_Base*> p_nodes;//包含位移的点

	QTimer* updateTimer;
	double m_dt = 1000;//ms

	vtkSmartPointer<vtkActor> m_vtkNodes;
	vtkSmartPointer<vtkActor> m_vtklines;
	vtkSmartPointer<vtkPoints> m_originalPoints;//原始数据
	vtkSmartPointer<vtkPoints> m_points;

	int m_speed = 0;//播放速度(帧/s)
	int m_frames = 0;//帧
	bool loopPlay = false;
	void set_loopPlay(bool flag) { loopPlay = flag; }

	void set_fps(int fps);
	void update();
	void start();
	void stop();
	void quit();
	void autoFactor(bool flag);

	void addData(vector<Node_Base*>node, Instance* ins);//界面的数据
	void addActorData();//添加界面的actor
	void removeActor();

signals:
	void animationFinished();//动画结束

public slots:
	void showOriginalActor(bool flag);//显示原始actor

private:
	Ui::resultVisualizeClass ui;
};
