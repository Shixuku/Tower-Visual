#include "InterFace.h"
#include <vtkPropCollection.h>
#include <vtkProperty.h>
#include<QMessageBox.h>
#include"Manage_PartData.h"
#include"Manage_InsData.h"
#include"Manage_Loads.h"
#include <vtkPointData.h>//->GetArray("Address")
#include <vtkIdTypeArray.h>
#include"MouseInteractorHighLightActor.h"//点选点
#include"AreaSelected_InteractorStyle.h"//框选点
#include"InteractorStyle.h"//框选单元
#include"Interphase_spacer.h"
#include"T_Foot.h"
#include"T_Body.h"
#include"T_CrossArm.h"
#include"Tower_Assembly.h"
#include"Assign_Section.h"
#include"SetAllSection.h"
#include"Create_Constraint.h"
#include <QLineEdit>
#include"CreateAbaqusInp.h"
#include"ConcentrateForce.h"
#include"Creat_Loads.h"
#include"Wind.h"
#include"Wire_InterFace.h"
#include"CreatWire.h"
#include "resultVisualize.h"
#include"TowerCaculate.h"
#include<vtkAppendPolyData.h>
#include "UI_Calculate.h"
#include"ParameterIceElement.h"
#include"ui_AnalysisStep.h"
#include"Manage_Section.h"
#include"dll.h"
#include"InputMaterial.h"
#include"WireWiring.h"
InterFace::InterFace(QWidget* parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	Base::set_InterFace(this);

	layout = new QHBoxLayout(ui.widget);//生成布局,widget专门放生成的塔
	m_VtkWidget = new QVTKOpenGLNativeWidget();//生成控件

	m_renderWindow = vtkGenericOpenGLRenderWindow::New();
	m_VtkWidget->setRenderWindow(m_renderWindow);

	m_Renderer = vtkRenderer::New();
	m_renderWindow->AddRenderer(m_Renderer);//添加放部件的vtk

	this->setContextMenuPolicy(Qt::DefaultContextMenu);
	m_CurrentRenderer = nullptr;
	initMenu();
	//右边窗口
	SetupCentralWidget();
	//左边窗口
	TreeWidgetShow();
	//打开软件自动运行
	ReadMaterialTXT();
	ReadPartTXT();
	//默认打开的是part的vtk窗口
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &InterFace::onTreeitemClicked);
	connect(ui.treeWidget, &QTreeWidget::itemDoubleClicked, this, &InterFace::onTreeitemDoubleClicked);
	connect(ui.actionLeg, &QAction::triggered, this, &InterFace::ui_Foot);
	connect(ui.actionBody, &QAction::triggered, this, &InterFace::ui_Body);
	connect(ui.actionCrossArm, &QAction::triggered, this, [=]() 
		{
			if (ui.treeWidget->topLevelItem(1)->childCount() == 0)
			{
				QMessageBox::information(this, "Tips", "请先创建塔身！");
			}
			else { ui_CrossArm(); }
		});
	
	connect(ui.menuSave, &QAction::triggered, this, &InterFace::SaveFile);
	connect(ui.actionRead, &QAction::triggered, this, &InterFace::OpenFile);
	connect(ui.btn_part, &QPushButton::clicked, this, &InterFace::ui_Management_PartData);
	connect(ui.btn_ins, &QPushButton::clicked, this, &InterFace::ui_Management_InsData);
	connect(ui.btn_caculate, &QPushButton::clicked, this, &InterFace::Caculate);
	connect(ui.btn_display, &QPushButton::clicked, this, &InterFace::Display);

	// 创建坐标轴部件
	Axes = vtkAxesActor::New();
	widgetAxes = vtkOrientationMarkerWidget::New();
	widgetAxes->SetOrientationMarker(Axes);
	widgetAxes->SetInteractor(m_renderWindow->GetInteractor());
	widgetAxes->SetEnabled(1);
	widgetAxes->SetInteractive(1);
}

void InterFace::SetupCentralWidget()
{
	//更改背景颜色
	m_Renderer->SetBackground(1.0, 1.0, 1.0);              // 底部颜色值
	m_Renderer->SetBackground2(0.629, 0.8078, 0.92157);    // 顶部颜色值
	m_Renderer->SetGradientBackground(1);    

	m_Renderer->SetBackground(1.0, 1.0, 1.0);              // 底部颜色值
	m_Renderer->SetBackground2(0.629, 0.8078, 0.92157);    // 顶部颜色值
	m_Renderer->SetGradientBackground(1);

	m_Renderer->SetBackground(1.0, 1.0, 1.0);              // 底部颜色值
	m_Renderer->SetBackground2(0.629, 0.8078, 0.92157);    // 顶部颜色值
	m_Renderer->SetGradientBackground(1);                  // 开启渐变色背景设置

}
void InterFace::TreeWidgetShow()
{
	layout->addWidget(m_VtkWidget);//布局中加入控件
	ui.treeWidget->header()->setVisible(false);//设置表头不可见

	QTreeWidgetItem* creat_tower_part = new QTreeWidgetItem(ui.treeWidget);
	creat_tower_part->setText(0, QString("杆塔部件"));

	QTreeWidgetItem* part_leg = new QTreeWidgetItem(creat_tower_part);
	part_leg->setText(0, QString("塔腿部件"));

	QTreeWidgetItem* part_body = new QTreeWidgetItem(creat_tower_part);
	part_body->setText(0, QString("塔身部件"));

	QTreeWidgetItem* part_head = new QTreeWidgetItem(creat_tower_part);
	part_head->setText(0, QString("塔头部件"));

	QTreeWidgetItem* creat_tower_instance = new QTreeWidgetItem(ui.treeWidget);
	creat_tower_instance->setText(0, QString("杆塔实例"));

	QTreeWidgetItem* CreatWire = new QTreeWidgetItem(ui.treeWidget);
	CreatWire->setText(0, QString("单导实例"));

	creat_towerwire_instance = new QTreeWidgetItem(ui.treeWidget);
	creat_towerwire_instance->setText(0, QString("塔线组实例"));

	QTreeWidgetItem* AnalysisStep = new QTreeWidgetItem(ui.treeWidget);
	AnalysisStep->setText(0, QString("分析步"));

	QTreeWidgetItem* sectionMaterial = new QTreeWidgetItem(ui.treeWidget);
	sectionMaterial->setText(0, QString("截面材料"));

	QTreeWidgetItem* manageSection = new QTreeWidgetItem(sectionMaterial);
	manageSection->setText(0, QString("管理"));
	QTreeWidgetItem* saveSection = new QTreeWidgetItem(sectionMaterial);
	saveSection->setText(0, QString("保存"));
	QTreeWidgetItem* openSection = new QTreeWidgetItem(sectionMaterial);
	openSection->setText(0, QString("读取"));

}
void InterFace::ReadMaterialTXT()
{
	InputMaterial aa;
	aa.ReadMaterial(this);
}
void InterFace::ReadPartTXT()
{
	WireWiring aa;
	aa.ReadWireWiring(this);
}
//双击树item相应事件
void InterFace::onTreeitemDoubleClicked(QTreeWidgetItem* item)
{
	//杆塔部件
	if (item == ui.treeWidget->topLevelItem(0)->child(0))
	{//塔腿部件
		ui_Foot();
	}
	else if (item == ui.treeWidget->topLevelItem(0)->child(1))
	{//塔身部件
		ui_Body();
	}
	else if (item == ui.treeWidget->topLevelItem(0)->child(2))
	{//塔头部件
		if (ui.treeWidget->topLevelItem(0)->child(1)->childCount() == 0)
		{
			QMessageBox::information(this, "Tips", "请先创建塔身！");
		}
		else { ui_CrossArm(); }
	}
	else if (isChildOfPartSetSpacer(item))
	{//塔头下面绝缘子串
		ui_Interphase_spacer(item);
	}
	else if (isChildOfPartSetSection(item))
	{//赋予截面
		ui_SetSection(item);
	}
	else if (isChildOfPartSetAllSection(item))
	{//根据长度赋予截面
		ui_SetAllSection(item);
	}
	//杆塔实例
	else if (item == ui.treeWidget->topLevelItem(1))
	{
		ui_Tower();
	}
	else if (isChildOfTower(1,item, 0) || isChildOfSingleWire(item, 3)||isChildOfGroup(item, 3))
	{//施加载荷
		ui_CreatLoads(item);
	}
	else if (isChildOfTower(1,item, 1))
	{//施加约束
		ui_Constraint(item);
	}
	else if (isChildOfTower(1,item, 2)|| isChildOfGroup(item, 2)||isChildOfSingleWire(item,4))
	{//输出txt文件
		CreateOutPut(item);
	}
	else if (isChildOfTower(1,item, 3))
	{//输出inp文件
		//CreateInp(item);
	}
	//单导实例
	else if (item == ui.treeWidget->topLevelItem(2))
	{
		ui_SingleWire();
	}
	else if (isChildOfSingleWire(item, 0))
	{//创建绝缘子串
		ui_SingleWireSpacer(item);
	}
	else if (isChildOfSingleWire(item, 2))
	{//施加约束
		ui_Constraint(item);
	}
	//塔线实例
	else if (item == ui.treeWidget->topLevelItem(3))
	{
		ui_TowerWireGroup();
	}
	else if (isChildOfGroup(item, 1) || isChildOfSingleWire(item, 1))
	{//导线建模
		ui_Wire_InterFace(item);
	}
	else if (isChildOfGroup(item,2))
	{//输出inp文件
		//CreateGroupInp(item);
	}
	//分析步
	else if (item == ui.treeWidget->topLevelItem(4))
	{
		ui_AnalysisStep* AnalysisStep = new ui_AnalysisStep(this);
		AnalysisStep->show();
	}
	//截面材料
	else if (item == ui.treeWidget->topLevelItem(5))
	{
		ui_Section();
	}
	else if (item == ui.treeWidget->topLevelItem(5)->child(0))
	{
		Manage_Section* manageSection = new Manage_Section(this);
		manageSection->show();
	}
	else if (item == ui.treeWidget->topLevelItem(5)->child(1))
	{//只保存截面
		QString str = QFileDialog::getSaveFileName(this, "保存", "/", "datafile(*.dat);;textfile(*.txt);;c file(*.cpp);;All file(*.*)");
		QFile Qf;
		QDataStream Stream;
		if (str != nullptr)
		{
			qDebug() << str;
			Qf.setFileName(str);
			Qf.open(QIODevice::ReadWrite);

			Stream.setDevice(&Qf);
			if (!Qf.isOpen())
			{
				cout << "文件打开失败！\n";
			}
			else
			{
				Ms.Save(Stream);//保存截面数据
			}
			Qf.close();
		}
	}
	else if (item == ui.treeWidget->topLevelItem(5)->child(2))
	{//只读取截面
		QString str = QFileDialog::getOpenFileName(this, "打开", "/", "datafile(*.dat);;textfile(*.txt);;c file(*.cpp);;All file(*.*)");
		QFile Qf1;
		QDataStream Stream1;
		if (str != nullptr)
		{
			qDebug() << str;

			Qf1.setFileName(str);
			Qf1.open(QIODevice::ReadWrite);

			Stream1.setDevice(&Qf1);
			if (!Qf1.isOpen())
			{
				cout << "文件打开失败！\n";
			}
			else
			{
				int MsSize = Ms.size();//截面
				Ms.Read(Stream1, MsSize);
			}
		}
		Qf1.close();
		emit Msg_CreateModel();
	}

}
void InterFace::onTreeitemClicked(QTreeWidgetItem* item)
{
	//部件切换
	Part_Base* part = OnFindPart(item);

	if (part != nullptr)
	{
		if (item->parent()->parent() == ui.treeWidget->topLevelItem(0))
			Show_Part(part);
	}

	if (item->parent() == ui.treeWidget->topLevelItem(1)|| item->parent() == ui.treeWidget->topLevelItem(2))
	{
		Instance* instance = OnFindInstance(item);
		Show_Tower(instance);
	}
	else if (item->parent() == ui.treeWidget->topLevelItem(2))
	{
		Instance* instance = OnFindInstance(item);
		Show_Wire(instance);
	}
}

//生成塔腿
void InterFace::ui_Foot()
{
	T_Foot* T_foot = new T_Foot(this);//ui
	int ret = T_foot->exec();
	if (ret == QDialog::Accepted)
	{
		QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(0)->child(0);
		QTreeWidgetItem* item = new QTreeWidgetItem(parent);
		AddPartFunction(item);
		TowerPart_leg* t = new TowerPart_leg;
		T_foot->Get_Data(*t);//取数据
		item->setText(0, t->m_Name);//子节点名称为自己命名的名称
		t->Item = item;
		m_Renderer->RemoveAllViewProps();

		t->Create_Mesh();
		t->m_id = parent->childCount();//编号
		t->Show_VTKnode(m_Renderer);
		t->Show_VTKtruss(m_Renderer);
		t->Show_VTKbeam(m_Renderer);

		TP_leg.Add_Entity(t);
		m_Renderer->ResetCamera();
		t_foots.push_back(T_foot);
		
	}

}
void InterFace::ui_Body()
{
	T_Body* T_body = new T_Body(this);//ui
	int ret = T_body->exec();
	if (ret == QDialog::Accepted)
	{
		QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(0)->child(1);
		QTreeWidgetItem* item = new QTreeWidgetItem(parent);
		AddPartFunction(item);
		TowerPart_body* t = new TowerPart_body;
		T_body->Get_Data(t);
		item->setText(0, t->m_Name);
		m_Renderer->RemoveAllViewProps();
		t->Item = item;

		t->Create_Mesh();
		t->m_id=parent->childCount();
		t->Show_VTKnode(m_Renderer);
		t->Show_VTKtruss(m_Renderer);
		t->Show_VTKbeam(m_Renderer);
	
		TP_body.Add_Entity(t);
		m_Renderer->ResetCamera();

		t_bodys.push_back(T_body);
	
	}
}
void InterFace::ui_CrossArm()
{
	T_CrossArm* T_ca = new T_CrossArm(this);//ui
	int ret = T_ca->exec();
	if (ret == QDialog::Accepted)
	{
		QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(0)->child(2);
		QTreeWidgetItem* item = new QTreeWidgetItem(parent);
		AddPartFunction(item);
		QTreeWidgetItem* spacer = new QTreeWidgetItem(item);
		spacer->setText(0, QString("添加绝缘子串"));
		TowerPart_CrossArm* t = new TowerPart_CrossArm;
		T_ca->Get_Data(*t);
		m_Renderer->RemoveAllViewProps();
		t->Item = item;
		item->setText(0, t->m_Name);
		
		t->Create_Mesh();
		t->m_id= parent->childCount();
		t->Show_VTKnode(m_Renderer);
		t->Show_VTKtruss(m_Renderer);
		t->Show_VTKbeam(m_Renderer);
		
		TP_CrossArm.Add_Entity(t);//塔头
		m_Renderer->ResetCamera();

		t_crossarms.push_back(T_ca);//塔头对话框

	}
}
void InterFace::ui_Tower()
{
	Tower_Assembly* T_As = new Tower_Assembly(this);//ui
	int ret = T_As->exec();
	if (ret == QDialog::Accepted)
	{
		QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(1);
		QTreeWidgetItem* item = new QTreeWidgetItem(parent);
		AddPartFunction(item);
		Tower* tw = new Tower;
		for (auto& i : T_As->m_ArryLeg)//塔腿
		{
			tw->addPart(TP_leg.Find_Entity(i));
		}
		for (auto& i : T_As->m_ArryBody)//塔身段
		{
			tw->addPart(TP_body.Find_Entity(i));
		}
		for (auto& i : T_As->m_ArryHead)//塔头
		{
			tw->addPart(TP_CrossArm.Find_Entity(i));
		}

		tw->Check_Beam();
		m_Renderer->RemoveAllViewProps();

		tw->Item = item;
		tw->m_name = T_As->Get_name();
		item->setText(0, tw->m_name);
		tw->m_id = TP.size() + 1;//编号

		tw->Show_VTKnode(m_Renderer);
		tw->Show_VTKtruss(m_Renderer);
		tw->Show_VTKbeam(m_Renderer);

		TP.Add_Entity(tw);
		
		tw->m_BeamActor->VisibilityOn();
		tw->m_TrussActor->VisibilityOn();
		tw->Node_actor->VisibilityOn();
		
		//所有截面编号加入Tower
		vector<int> idSections;
		for (auto& i : Ms)
		{
			idSections.push_back(i.second->m_id);
		}
		tw->AddNewSection(idSections);

		m_Renderer->ResetCamera();
		tower_assembles.push_back(T_As);
	}

}
void InterFace::ui_Section()
{
	Set_Section* M_ab = new Set_Section(this);//ui
	int ret = M_ab->exec();
	if (ret == QDialog::Accepted)
	{
		//所有截面编号加入Part
		vector<int> idSections;
		for (auto& i : Ms)
		{
			idSections.push_back(i.second->m_id);
		}
		for (int i = 0; i < ui.treeWidget->topLevelItem(0)->child(0)->childCount(); i++)//新键一个，塔腿里放一个
		{
			TP_leg.Find_Entity(i + 1)->AddNewSection(idSections);
		}
		for (int i = 0; i < ui.treeWidget->topLevelItem(0)->child(1)->childCount(); i++)//新键一个，塔身里放一个
		{
			TP_body.Find_Entity(i + 1)->AddNewSection(idSections);
		}
		for (int i = 0; i < ui.treeWidget->topLevelItem(0)->child(2)->childCount(); i++)//新键一个，塔头里放一个
		{
			TP_CrossArm.Find_Entity(i + 1)->AddNewSection(idSections);
		}
	}
}
void InterFace::ui_SetSection(QTreeWidgetItem* item)
{
	Part_Base* Part = OnFindPart(item->parent());
	Show_Part(Part);
	Assign_Section* assign_Section = new Assign_Section(this,Part);
	assign_Section->show();

}
void InterFace::ui_SetAllSection(QTreeWidgetItem* item)
{
	Part_Base* Part = OnFindPart(item->parent());
	Show_Part(Part);
	//显示所有单元长度
	for (auto& i : Part->m_Elements_beams)
	{
		int ipt1 = i.m_idNode[0] - 1; //id
		int ipt2 = i.m_idNode[1] - 1;
		double x1 = Part->m_Nodes[ipt1].x; double y1 = Part->m_Nodes[ipt1].y; double z1 = Part->m_Nodes[ipt1].z;
		double x2 = Part->m_Nodes[ipt2].x; double y2 = Part->m_Nodes[ipt2].y; double z2 = Part->m_Nodes[ipt2].z;
		double Vx[3];
		Vx[0] = x2 - x1;
		Vx[1] = y2 - y1;
		Vx[2] = z2 - z1;
		double m_L = sqrt(Vx[0] * Vx[0] + Vx[1] * Vx[1] + Vx[2] * Vx[2]);
		cout << m_L << "\n";
	}
	SetAllSection* SetAll = new SetAllSection(this,Part);
	SetAll->show();

}

void InterFace::ui_Management_PartData()
{
	Manage_PartData* m_data = new Manage_PartData(this);
	m_data->show();
}

void InterFace::ui_Management_InsData()
{
	Manage_InsData* ins_data = new Manage_InsData(this);

	ins_data->show();
}

void InterFace::ui_Interphase_spacer(QTreeWidgetItem* item)
{
	TowerData_CrossArm* Arm = OnFindCrossAem(item->parent());
	TowerWireGroup* t = nullptr;
	Interphase_spacer* IS = new Interphase_spacer(Arm, t,this);

	IS->show();//要互动只能用show

}

void InterFace::ui_Wire_InterFace(QTreeWidgetItem* item)
{
	TowerWireGroup* towerWireGroup = OnFindGroup(item->parent());
	towerWireGroup->Suspensioncombined();
	towerWireGroup->VectorToMap();
	Wire_InterFace* w = new Wire_InterFace(towerWireGroup,this);
	int ret = w->exec();
	
	if (ret == QDialog::Accepted)
	{
		CreatWire* t = new CreatWire;
		w->Get_Data(*t);//取数据
		m_Renderer->RemoveAllViewProps();
		t->Create_Mesh();
		towerWireGroup->AddWireToGroup(t);
		towerWireGroup->Show_VTKnode(m_Renderer);
		towerWireGroup->Show_VTKbeam(m_Renderer);
		towerWireGroup->Show_VTKtruss(m_Renderer);
		m_Renderer->ResetCamera();
	}

}

void InterFace::ui_TowerWireGroup()
{
	m_Renderer->RemoveAllViewProps();
	TowerWireGroup* towerWireGroup = new TowerWireGroup;
	TowerWireGroupAssembly* towerWireGroupAssembly = new TowerWireGroupAssembly(this, towerWireGroup);
	towerWireGroupAssembly->show();

}
void InterFace::ui_SingleWireSpacer(QTreeWidgetItem* item)
{
	TowerData_CrossArm* arm = nullptr;
	TowerWireGroup* t = OnFindGroup(item->parent());
	m_Renderer->RemoveAllViewProps();
	Interphase_spacer* IS = new Interphase_spacer(arm, t, this);
	IS->show();

}

void InterFace::SaveFile()
{
	QString str = QFileDialog::getSaveFileName(this, "保存", "/", "datafile(*.dat);;textfile(*.txt);;c file(*.cpp);;All file(*.*)");
	QFile Qf;
	QDataStream Stream;
	if (str != nullptr)
	{
		qDebug() << str;
		Qf.setFileName(str);
		Qf.open(QIODevice::ReadWrite);

		Stream.setDevice(&Qf);
		if (!Qf.isOpen())
		{
			cout << "文件打开失败！\n";
		}
		else
		{
			TP_leg.Save(Stream);//有多少 保存多少
			TP_body.Save(Stream);
			TP_CrossArm.Save(Stream);
			TP.Save(Stream);
			Ms.Save(Stream);//保存截面数据

		}
		Qf.close();
	}
}
void InterFace::OpenFile()
{
	QString str = QFileDialog::getOpenFileName(this, "打开", "/", "datafile(*.dat);;textfile(*.txt);;c file(*.cpp);;All file(*.*)");
	QFile Qf1;
	QDataStream Stream1;
	if (str != nullptr)
	{
		qDebug() << str;
		
		Qf1.setFileName(str);
		Qf1.open(QIODevice::ReadWrite);

		Stream1.setDevice(&Qf1);
		if (!Qf1.isOpen())
		{
			cout << "文件打开失败！\n";
		}
		else
		{
			int legSize = TP_leg.size();//塔腿
			int bodysize = TP_body.size();//塔身
			int crossArmSize = TP_CrossArm.size();//塔头
			int towerSize = TP.size();//杆塔实例
			int MsSize = Ms.size();//截面

			TP_leg.Read(Stream1, legSize);
			TP_body.Read(Stream1, bodysize);
			TP_CrossArm.Read(Stream1, crossArmSize);
			TP.Read(Stream1, towerSize);
			Ms.Read(Stream1, MsSize);

			for (int i = legSize; i < TP_leg.size(); i++)
			{
				QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(0)->child(0);
				QTreeWidgetItem* item = new QTreeWidgetItem(parent);
				item->setText(0, TP_leg.Find_Entity(i + 1)->m_Name);
				TP_leg.Find_Entity(i + 1)->Item = item;
				AddPartFunction(item);
			}

			for (int i = bodysize; i < TP_body.size(); i++)
			{
				QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(0)->child(1);
				QTreeWidgetItem* item = new QTreeWidgetItem(parent);
				item->setText(0, TP_body.Find_Entity(i + 1)->m_Name);
				TP_body.Find_Entity(i + 1)->Item = item;
				AddPartFunction(item);
			}
			for (int i = crossArmSize; i < TP_CrossArm.size(); i++)
			{
				QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(0)->child(2);
				QTreeWidgetItem* item = new QTreeWidgetItem(parent);
				item->setText(0, TP_CrossArm.Find_Entity(i + 1)->m_Name);
				TP_CrossArm.Find_Entity(i + 1)->Item = item;
				AddPartFunction(item);
				QTreeWidgetItem* spacer = new QTreeWidgetItem(item);
				spacer->setText(0, QString("添加绝缘子串"));
			}
			for (int i = towerSize; i < TP.size(); i++)
			{
				QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(1);
				QTreeWidgetItem* item = new QTreeWidgetItem(parent);
				QString str = QString::number(parent->childCount());     //str转字符
				item->setText(0, TP.Find_Entity(i + 1)->m_name);
				TP.Find_Entity(i + 1)->Item = item;
				AddPartFunction(item);
			}
		}
	}
	Qf1.close();
	emit Msg_CreateModel();
}

void InterFace::contextMenuEvent(QContextMenuEvent* event)
{
	// 获取当前选中的 m_Renderer
	vtkRenderWindowInteractor* interactor = m_renderWindow->GetInteractor();
	int* clickPos = interactor->GetEventPosition();
	m_CurrentRenderer = interactor->FindPokedRenderer(clickPos[0], clickPos[1]);
	m_pMenu->exec(QCursor::pos());
}

Instance* InterFace::OnFindInstance(const QTreeWidgetItem* Item)
{
	QTreeWidgetItemIterator it(ui.treeWidget);
	while (*it)
	{
		//QTreeWidgetItem是否满足条件---这里的条件可以自己修改
		if ((*it) == Item)
		{
			for (auto& i : TP)
			{
				if (i.second->Item == *it) return i.second;
			}
			for (auto& i : TWG)
			{
				if (i.second->Item == *it) return i.second;
			}
		/*	for (auto& i : creatWire)
			{
				if (i.second->Item == *it) return i.second;
			}*/
			return nullptr;
		}
		++it;
	}
	return nullptr;
}

Part_Base* InterFace::OnFindPart(const QTreeWidgetItem* Item)
{
	QTreeWidgetItemIterator it(ui.treeWidget);
	while (*it)
	{
		//QTreeWidgetItem是否满足条件---这里的条件可以自己修改
		if ((*it) == Item)
		{
			for (auto& i : TP_leg)
			{
				if (i.second->Item == *it) return i.second;
			}
			for (auto& i : TP_body)
			{
				if (i.second->Item == *it) return i.second;
			}
			for (auto& i : TP_CrossArm)
			{
				if (i.second->Item == *it) return i.second;
			}
			return nullptr;
		}
		++it;
	}
	return nullptr;
}

Tower* InterFace::OnFindTower(const QTreeWidgetItem* Item)
{
	QTreeWidgetItemIterator it(ui.treeWidget);
	while (*it)
	{
		if ((*it) == Item)
		{
			for (auto& i : TP)
			{
				if (i.second->Item == *it) return i.second;
			}
			return nullptr;
		}
		++it;
	}

	return nullptr;
}

TowerData_CrossArm* InterFace::OnFindCrossAem(const QTreeWidgetItem* Item)
{
	QTreeWidgetItemIterator it(ui.treeWidget);
	while (*it)
	{
		//QTreeWidgetItem是否满足条件---这里的条件可以自己修改
		if ((*it) == Item)
		{
			for (auto& i : TP_CrossArm)
			{
				if (i.second->Item == *it) return i.second;
			}
			return nullptr;
		}
		++it;
	}
	return nullptr;
}

TowerWireGroup* InterFace::OnFindGroup(const QTreeWidgetItem* Item)
{
	QTreeWidgetItemIterator it(ui.treeWidget);
	while (*it)
	{
		//QTreeWidgetItem是否满足条件---这里的条件可以自己修改
		if ((*it) == Item)
		{
			for (auto& i : TWG)
			{
				if (i.second->Item == *it) return i.second;
			}
			return nullptr;
		}
		++it;
	}
	return nullptr;
}

void InterFace::ShowSubstaceActor(Part_Base* Part)
{//生成一个截面就生成一个actor，为了计算不卡，暂时先注释，不显示截面
	//for (auto& i : Part->PartNactor)
	//{
	//	m_Renderer->AddActor(i);
	//}

	m_renderWindow->Render();
	m_Renderer->ResetCamera();

}

void InterFace::initMenu()
{
	m_pMenu = new QMenu(this);
	QAction* pAc1 = new QAction("点选P");
	QAction* pAc2 = new QAction("框选P");
	QAction* pAc3 = new QAction("取消");
	QAction* pAc4 = new QAction("框选E");
	m_pMenu->addAction(pAc1);
	m_pMenu->addAction(pAc2);
	m_pMenu->addAction(pAc3);
	m_pMenu->addAction(pAc4);
	connect(pAc1, &QAction::triggered, [=] {Point_Inqure(); });
	connect(pAc2, &QAction::triggered, [=] {Area_Inqure(); });
	connect(pAc3, &QAction::triggered, [=] {Close_Point(); });
	connect(pAc4, &QAction::triggered, [=] {AreaElement_Inqure(); });
}

void InterFace::AddPartFunction(QTreeWidgetItem* item)
{
	if (item->parent()->parent() == ui.treeWidget->topLevelItem(0))
	{
		//保存指针到part的赋予截面
		QTreeWidgetItem* assign_section = new QTreeWidgetItem(item);
		assign_section->setText(0, QString("赋予截面"));
		//保存指针到part的根据长度赋予截面
		QTreeWidgetItem* SetAllSection = new QTreeWidgetItem(item);
		SetAllSection->setText(0, QString("根据长度赋予截面"));
	}
	if (item->parent()== ui.treeWidget->topLevelItem(1))
	{
		QTreeWidgetItem* AddLoadForce = new QTreeWidgetItem(item);//施加载荷按钮
		QTreeWidgetItem* Constraint = new QTreeWidgetItem(item);//施加载荷按钮
		QTreeWidgetItem* OutPut = new QTreeWidgetItem(item);//施加载荷按钮
		QTreeWidgetItem* Inp = new QTreeWidgetItem(item);//施加载荷按钮
		
		AddLoadForce->setText(0, QString("施加荷载"));
		Constraint->setText(0, QString("添加约束"));
		OutPut->setText(0, QString("输出计算文件"));
		Inp->setText(0, QString("输出ABAQUS计算文件"));

	}


}

void InterFace::Caculate()
{
	if (ui_calculate)
	{
		ui_calculate->show();
	}
	else
	{
		ui_calculate = new Instance_Calculate(this);
		ui_calculate->show();
	}
}

void InterFace::Display()
{
	/*HiddeAllTower();
	if (!display)
	{
		display = new resultVisualize(this);
	}
	display->show();
	vector<Node_Base*> ptr_nodes = s->GetNodes();
	Instance* ins = nullptr;
	for (auto& i : TP)
	{
		i.second->m_name != nullptr;
		ins = i.second;
		break;
	}
	for (auto& i : creatWire)
	{
		i.second->m_name != nullptr;
		ins = i.second;
		break;
	}

	display->addData(ptr_nodes, ins);*/

}

//导线部分
void InterFace::Add_Select(vtkSmartPointer<vtkActor> pActor)
{
	std::list<Node*> idNodes;
	Get_SelectedNode(idNodes);//已经选中的节点

	auto dataArray = pActor->GetMapper()->GetInput()->GetPointData()->GetArray("Address");//取得地址数组
	auto ptrArray = dynamic_cast<vtkIdTypeArray*> (dataArray);//转换为整数数组
	if (ptrArray != nullptr)
	{//转换成功
		vtkIdType nPts = ptrArray->GetNumberOfValues();//对象的个数
		for (vtkIdType k = 0; k < nPts; k++)
		{//提取每个对象的指针
			Node* pEntity = (Node*)(ptrArray->GetValue(k));//将每个元素的值转换为对象指针
			bool bFind = false;
			for (auto i : idNodes)
			{
				if (i == pEntity)
				{
					bFind = true;
					break;
				}
			}
			if (!bFind)
			{
				m_NodeSelected.push_back(pActor);
			}
		}
	}
}
void InterFace::UnSelect_Nodes()
{
	for (auto &i : m_NodeSelected)
	{
		if (i != nullptr) m_CurrentRenderer->RemoveActor(i);
	}
	m_NodeSelected.clear();
}
void InterFace::Get_SelectedNode(std::list<Node*>& Nodes)
{
	for (auto &i : m_NodeSelected)
	{
		auto dataArray = i->GetMapper()->GetInput()->GetPointData()->GetArray("Address");//取得地址数组
		auto ptrArray = dynamic_cast<vtkIdTypeArray*> (dataArray);//转换为整数数组
		if (ptrArray != nullptr)
		{//转换成功
			vtkIdType nPts = ptrArray->GetNumberOfValues();//对象的个数
			for (vtkIdType k = 0; k < nPts; k++)
			{//提取每个对象的指针
				Node* pEntity = (Node*)(ptrArray->GetValue(k));//将每个元素的值转换为对象指针
				Nodes.push_back(pEntity);
			}
		}
	}
}

void InterFace::Get_SelectedNode(std::set<Node*>& Nodes)
{
	for (auto &i : m_NodeSelected)
	{
		auto dataArray = i->GetMapper()->GetInput()->GetPointData()->GetArray("Address");//取得地址数组
		auto ptrArray = dynamic_cast<vtkIdTypeArray*> (dataArray);//转换为整数数组
		if (ptrArray != nullptr)
		{//转换成功
			vtkIdType nPts = ptrArray->GetNumberOfValues();//对象的个数
			for (vtkIdType k = 0; k < nPts; k++)
			{//提取每个对象的指针
				Node* pEntity = (Node*)(ptrArray->GetValue(k));//将每个元素的值转换为对象指针
				Nodes.insert(pEntity);
			}
		}
	}
}

void InterFace::Point_Inqure()	
{
	UnSelect_Nodes();
	// 创建交互器
	vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
	renderWindowInteractor->SetRenderWindow(m_renderWindow);

	// 设置自定义交互类型
	vtkNew<MouseInteractorHighLightActor> style;
	style->m_pInterFace = this;

	// 使用当前选中的 m_Renderer
	style->SetDefaultRenderer(m_CurrentRenderer);

	renderWindowInteractor->SetInteractorStyle(style);

	// 创建坐标轴部件
	vtkAxesActor* Axes = vtkAxesActor::New();
	vtkOrientationMarkerWidget* widgetAxes = vtkOrientationMarkerWidget::New();
	widgetAxes->SetOrientationMarker(Axes);
	widgetAxes->SetInteractor(renderWindowInteractor);
	widgetAxes->SetEnabled(1);
	widgetAxes->SetInteractive(1);

	// 初始化交互器并启动
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();

}

void InterFace::Area_Inqure()
{
	UnSelect_Nodes();
	// 保存主界面的交互器样式
	m_MainStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	// An interactor
	vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
	renderWindowInteractor->SetRenderWindow(m_renderWindow);
	// Set the custom type to use for interaction.
	vtkNew<AreaSelected_InteractorStyle> Framestyle;
	Framestyle->m_pInterFace = this;
	Framestyle->SetDefaultRenderer(m_Renderer);
	renderWindowInteractor->SetInteractorStyle(Framestyle);
	vtkAxesActor* Axes = vtkAxesActor::New();

	vtkOrientationMarkerWidget* widgetAxes = vtkOrientationMarkerWidget::New();
	widgetAxes->SetOrientationMarker(Axes);
	widgetAxes->SetInteractor(renderWindowInteractor);
	widgetAxes->SetEnabled(1);
	widgetAxes->SetInteractive(1);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
	renderWindowInteractor->TerminateApp();
	// 清除 m_Renderer_2 相关资源
	Axes->Delete();
	widgetAxes->Delete();
	widgetAxes->EnabledOff();
	widgetAxes->SetInteractor(nullptr);
	widgetAxes->SetOrientationMarker(nullptr);
	widgetAxes->Delete();

	// 清除交互器
	renderWindowInteractor->RemoveAllObservers();
}

void InterFace::AreaElement_Inqure()
{
	//UnSelect_Nodes();//改成取消单元
	// 创建交互器
	vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
	renderWindowInteractor->SetRenderWindow(m_renderWindow);

	// 设置自定义交互类型--用例子改成单元
	


	for (int i = 1; i < TP.size() + 1; i++)
	{
		appendFilter->AddInputData(TP.Find_Entity(i)->linesPolyData);
		appendFilter->Update();
	}

	vtkNew<vtkIdFilter> idFilter;//保存数据
	idFilter->SetInputConnection(appendFilter->GetOutputPort());

	idFilter->SetCellIdsArrayName("OriginalIds");//保存单元
	idFilter->SetPointIdsArrayName("OriginalIds");//保存点

	//简化成表面
	vtkNew<vtkDataSetSurfaceFilter> surfaceFilter;
	surfaceFilter->SetInputConnection(idFilter->GetOutputPort());
	surfaceFilter->Update();

	vtkPolyData* input = surfaceFilter->GetOutput();
	//std::cout << "points" << input->GetNumberOfPoints() << "cells:" << input->GetNumberOfCells() << std::endl;

	//std::cout << polyData->GetNumberOfCells() << std::endl;
	style->SetPoints(input);//暂时只能找到，第一个实例的单元
	style->m_pInterFace = this;

	// 使用当前选中的 m_Renderer
	style->SetDefaultRenderer(m_CurrentRenderer);

	vtkNew<vtkAreaPicker> areaPicker;
	renderWindowInteractor->SetPicker(areaPicker);
	renderWindowInteractor->SetInteractorStyle(style);

	m_renderWindow->SetInteractor(renderWindowInteractor);

	// 初始化交互器并启动
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
}

bool InterFace::isChildOfTower(int idParent,QTreeWidgetItem* item, int childNumber)
{
	QTreeWidgetItem* topLevelItem3 = ui.treeWidget->topLevelItem(idParent);
	int childCount = topLevelItem3->childCount();

	for (int i = 0; i < childCount; i++)
	{
		QTreeWidgetItem* childItem = topLevelItem3->child(i);
		if (item == childItem->child(childNumber))
		{
			return true;
		}
	}
	return false;
}

bool InterFace::isChildOfGroup(QTreeWidgetItem* item, int childNumber)
{
	QTreeWidgetItem* towerWireGroup = ui.treeWidget->topLevelItem(3);
	int towerWireGroupChildCount = towerWireGroup->childCount();//取得有几个塔线组循环
	for (int i = 0; i < towerWireGroupChildCount; i++)
	{
		QTreeWidgetItem* childItem = towerWireGroup->child(i);
		if (item == childItem->child(childNumber))
		{
			return true;
		}
	}
	return false;
}

bool InterFace::isChildOfPartSetSection(QTreeWidgetItem* item)
{//三个部件的item
	QTreeWidgetItem* LegPart = ui.treeWidget->topLevelItem(0)->child(0);
	QTreeWidgetItem* BodyPart = ui.treeWidget->topLevelItem(0)->child(1);
	QTreeWidgetItem* ArmPart = ui.treeWidget->topLevelItem(0)->child(2);
	int LegchildCount = LegPart->childCount();//取得有几个塔腿部件循环
	for (int i = 0; i < LegchildCount; i++)
	{
		QTreeWidgetItem* childItem = LegPart->child(i);
		if (item == childItem->child(0))
		{
			return true;
		}
	}
	int BodychildCount = BodyPart->childCount();//取得有几个塔身部件循环
	for (int i = 0; i < BodychildCount; i++)
	{
		QTreeWidgetItem* childItem = BodyPart->child(i);
		if (item == childItem->child(0))
		{
			return true;
		}
	}
	int ArmchildCount = ArmPart->childCount();//取得有几个塔头部件循环
	for (int i = 0; i < ArmchildCount; i++)
	{
		QTreeWidgetItem* childItem = ArmPart->child(i);
		if (item == childItem->child(0))
		{
			return true;
		}
	}
	return false;
}

bool InterFace::isChildOfPartSetAllSection(QTreeWidgetItem* item)
{
	//三个部件的item
	QTreeWidgetItem* LegPart = ui.treeWidget->topLevelItem(0)->child(0);
	QTreeWidgetItem* BodyPart = ui.treeWidget->topLevelItem(0)->child(1);
	QTreeWidgetItem* ArmPart = ui.treeWidget->topLevelItem(0)->child(2);
	int LegchildCount = LegPart->childCount();//取得有几个塔腿部件循环
	for (int i = 0; i < LegchildCount; i++)
	{
		QTreeWidgetItem* childItem = LegPart->child(i);
		if (item == childItem->child(1))
		{
			return true;
		}
	}
	int BodychildCount = BodyPart->childCount();//取得有几个塔身部件循环
	for (int i = 0; i < BodychildCount; i++)
	{
		QTreeWidgetItem* childItem = BodyPart->child(i);
		if (item == childItem->child(1))
		{
			return true;
		}
	}
	int ArmchildCount = ArmPart->childCount();//取得有几个塔头部件循环
	for (int i = 0; i < ArmchildCount; i++)
	{
		QTreeWidgetItem* childItem = ArmPart->child(i);
		if (item == childItem->child(1))
		{
			return true;
		}
	}
	return false;
}

bool InterFace::isChildOfPartSetSpacer(QTreeWidgetItem* item)
{
	QTreeWidgetItem* ArmPart = ui.treeWidget->topLevelItem(0)->child(2);
	int ArmchildCount = ArmPart->childCount();//取得有几个塔头部件循环
	for (int i = 0; i < ArmchildCount; i++)
	{
		QTreeWidgetItem* childItem = ArmPart->child(i);
		if (item == childItem->child(2))
		{
			return true;
		}
	}
	return false;
}


bool InterFace::isChildOfSingleWire(QTreeWidgetItem* item, int childNumber)
{
	QTreeWidgetItem* towerWireGroup = ui.treeWidget->topLevelItem(2);
	int singleWireChildCount = towerWireGroup->childCount();//取得有几个单线组循环
	for (int i = 0; i < singleWireChildCount; i++)
	{
		QTreeWidgetItem* childItem = towerWireGroup->child(i);
		if (item == childItem->child(childNumber))
		{
			return true;
		}
	}
	return false;
}

void InterFace::Close_Point()
{
	UnSelect_Nodes();
	// 保存主界面的交互器样式
	m_MainStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	// 在需要切换回主界面时，使用保存的交互器样式重新设置交互器
	m_renderWindow->GetInteractor()->SetInteractorStyle(m_MainStyle);
	
}


void InterFace::ui_CreatLoads(QTreeWidgetItem* item)
{
	Instance* instance = OnFindInstance(item->parent());
	Show_Tower(instance);//显示父节点的实例
	Creat_Loads* creat_load = new Creat_Loads(instance, this);
	creat_load->show();
}

void InterFace::CreateOutPut(QTreeWidgetItem* item)
{
	Instance* instance = OnFindInstance(item->parent());
	instance->CreateOutPut();
}

void InterFace::CreateTowerInp(QTreeWidgetItem* item)
{
	Tower* tower = OnFindTower(item->parent());
	CreateAbaqusInp Inp;
	Inp.CreateInp(tower);
}

void InterFace::CreateGroupInp(QTreeWidgetItem* item)
{
	TowerWireGroup* group = OnFindGroup(item->parent());
	CreateAbaqusInp Inp;
	Inp.CreateInp(group);
}

void InterFace::Show_Part(Part_Base* part)
{
	if (part == nullptr) return;
	m_Renderer->RemoveAllViewProps();
	part->Show_VTKnode(m_Renderer);
	part->Show_VTKtruss(m_Renderer);
	part->Show_VTKbeam(m_Renderer);
	
	for (auto& i : part->PartNactor)
	{
		i->VisibilityOn();
	}
	ShowSubstaceActor(part);
	m_Renderer->ResetCamera();
}

void InterFace::Show_Tower(Instance* instance)
{
	if (instance == nullptr) return;
	m_Renderer->RemoveAllViewProps();

	instance->Show_VTKnode(m_Renderer);
	instance->Show_VTKtruss(m_Renderer);
	instance->Show_VTKbeam(m_Renderer);
	
	for (auto& i : instance->m_LoadActor)
	{
		m_Renderer->AddActor(i);
		i->VisibilityOn();
	}
	for (auto& i : instance->m_ConstraintActor)
	{//暂时切换后不显示约束
		m_Renderer->AddActor(i);
		i->VisibilityOn();
	}
	m_Renderer->ResetCamera();
}

void InterFace::Show_Wire(Instance* instance)
{
	if (instance == nullptr) return;
	m_Renderer->RemoveAllViewProps();
	instance->Node_actor->VisibilityOn();
	instance->m_TrussActor->VisibilityOn();
	
	m_Renderer->ResetCamera();
}

void InterFace::ui_Constraint(QTreeWidgetItem* item)
{
	Instance* instance = OnFindInstance(item->parent());
	instance->Show_VTKnode(m_Renderer);
	instance->Show_VTKbeam(m_Renderer);
	instance->Show_VTKtruss(m_Renderer);
	//Show_Tower(instance);//显示对应的实例杆塔

	//显示选择的约束界面
	Create_Constraint* con = new Create_Constraint(instance, this);
	con->show();
}

void InterFace::ui_SingleWire()
{
	QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(2);
	QTreeWidgetItem* item = new QTreeWidgetItem(parent);
	item->setText(0, "导线");
	QTreeWidgetItem* spacer = new QTreeWidgetItem(item);
	spacer->setText(0, "添加绝缘子串");
	QTreeWidgetItem* creareWire = new QTreeWidgetItem(item);
	creareWire->setText(0, "建立导线");
	QTreeWidgetItem* Constraint = new QTreeWidgetItem(item);
	Constraint->setText(0, "添加约束");
	QTreeWidgetItem* Loads = new QTreeWidgetItem(item);
	Loads->setText(0, "施加荷载");
	QTreeWidgetItem* inp = new QTreeWidgetItem(item);
	inp->setText(0, "计算文件");
	TowerWireGroup* towerWireGroup = new TowerWireGroup;
	towerWireGroup->Item= item;
	towerWireGroup->m_id = TWG.size() + 1;
	TWG.Add_Entity(towerWireGroup);
}

InterFace::~InterFace()
{

}