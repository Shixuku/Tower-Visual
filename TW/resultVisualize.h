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
#include"Outputter_ice.h"

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

	std::vector<Node> m_nodes;//包含位移的点

	QTimer* updateTimer;
	double m_dt = 1000;//ms

	vtkSmartPointer<vtkActor> m_vtkNodes;
	vtkSmartPointer<vtkActor> m_vtklines;
	vtkSmartPointer<vtkPoints> m_originalPoints;//原始数据
	vtkSmartPointer<vtkPoints> m_points;

	vtkSmartPointer<vtkDoubleArray> scalars;//设置云图颜色
	vtkSmartPointer<vtkScalarBarActor> scalarBar;//云图块actor
	vtkSmartPointer<vtkLookupTable> lookupTable;

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

	void addData(std::list<std::vector<double>>& nodes, Instance* ins);//界面的数据
	//void addData_ice(std::list<std::vector<double>>& nodes, Instance* ins);//界面的数据
	void addActorData();//添加界面的actor
	void removeActor();

	void setNephogramType(int iTpye);
	void setCurrentStep(int idStep);
signals:
	void animationFinished();//动画结束

public slots:
	void showOriginalActor(bool flag);//显示原始actor

private:
	Ui::resultVisualizeClass ui;

	Outputter* m_Outputter = nullptr;
	//Outputter_ice* m_Outputter_ice = nullptr;

	void getBoundary();
	double boundary = 0; //模型边界大小

	DataType currentType;
	DataType_ice currentType_ice;
};
