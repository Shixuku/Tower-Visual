#include "InterFace.h"
#include <vtkPropCollection.h>
#include <vtkProperty.h>
#include<QMessageBox.h>
#include"Manage_PartData.h"
#include"Manage_InsData.h"
#include"Manage_Loads.h"
#include <vtkPointData.h>//->GetArray("Address")
#include <vtkIdTypeArray.h>
#include"MouseInteractorHighLightActor.h"
#include"AreaSelected_InteractorStyle.h"
#include"Interphase_spacer.h"
#include"T_Foot.h"
#include"T_Body.h"
#include"T_CrossArm.h"
#include"Tower_Assembly.h"
#include"Assign_Section.h"
#include"SetAllSection.h"
#include"Wire_InterFace.h"
#include"Create_Constraint.h"
#include <QLineEdit>
#include"CreateAbaqusInp.h"
#include"ConcentrateForce.h"
#include"Creat_Loads.h"
#include"TowerWireGroup.h"
#include"Wind.h"

InterFace::InterFace(QWidget* parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	layout = new QHBoxLayout(ui.widget);//生成布局,widget专门放生成的塔
	m_VtkWidget = new QVTKOpenGLNativeWidget();//生成控件

	m_renderWindow = vtkGenericOpenGLRenderWindow::New();
	m_VtkWidget->setRenderWindow(m_renderWindow);

	m_Renderer = vtkRenderer::New();
	m_renderWindow->AddRenderer(m_Renderer);//添加放部件的vtk

	m_Renderer_2 = vtkRenderer::New();
	m_renderWindow->AddRenderer(m_Renderer_2);//添加放单塔的vtk

	m_Renderer_3 = vtkRenderer::New();
	m_renderWindow->AddRenderer(m_Renderer_3);//添加放塔线组的vtk
	this->setContextMenuPolicy(Qt::DefaultContextMenu);
	m_CurrentRenderer = nullptr;
	initMenu();
	//右边窗口
	SetupCentralWidget();
	//左边窗口
	TreeWidgetShow();
	
	//默认打开的是part的vtk窗口
	switchRenderWindow(0);
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
	connect(ui.actioninit, &QAction::triggered, this, [=]()
		{
			m_Renderer->GetActiveCamera()->SetPosition(0, 0, 100000);
			m_Renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
			m_Renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
			m_renderWindow->Render();
		});
	connect(ui.actionYoZ, &QAction::triggered, this, [=]()
		{
			m_Renderer->GetActiveCamera()->SetPosition(100000, 0, 0);
			m_Renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
			m_Renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
			m_renderWindow->Render();
		});
	
}

void InterFace::SetupCentralWidget()
{
	//更改背景颜色
	m_Renderer->SetBackground(1.0, 1.0, 1.0);              // 底部颜色值
	m_Renderer->SetBackground2(0.629, 0.8078, 0.92157);    // 顶部颜色值
	m_Renderer->SetGradientBackground(1);    

	m_Renderer_2->SetBackground(1.0, 1.0, 1.0);              // 底部颜色值
	m_Renderer_2->SetBackground2(0.629, 0.8078, 0.92157);    // 顶部颜色值
	m_Renderer_2->SetGradientBackground(1);

	m_Renderer_3->SetBackground(1.0, 1.0, 1.0);              // 底部颜色值
	m_Renderer_3->SetBackground2(0.629, 0.8078, 0.92157);    // 顶部颜色值
	m_Renderer_3->SetGradientBackground(1);                  // 开启渐变色背景设置

	////添加坐标系
	//vtkAxesActor* axesActor = vtkAxesActor::New();
	////以Widget方式,在左下角的视口中显示坐标系，可进行鼠标交互
	//vtkOrientationMarkerWidget* widget = vtkOrientationMarkerWidget::New();
	//widget->SetOrientationMarker(axesActor);
	//widget->SetInteractor(m_VtkWidget->interactor());
	//widget->SetViewport(0, 0, 0.2, 0.2);
	//widget->SetEnabled(true);
	//widget->InteractiveOn();

}
void InterFace::TreeWidgetShow()
{
	layout->addWidget(m_VtkWidget);//布局中加入控件
	ui.treeWidget->header()->setVisible(false);//设置表头不可见

	QTreeWidgetItem* creat_tower_part = new QTreeWidgetItem(ui.treeWidget);
	creat_tower_part->setText(0, QString("生成杆塔部件"));

	QTreeWidgetItem* part_leg = new QTreeWidgetItem(creat_tower_part);
	part_leg->setText(0, QString("塔腿部件"));

	QTreeWidgetItem* part_body = new QTreeWidgetItem(creat_tower_part);
	part_body->setText(0, QString("塔身部件"));

	QTreeWidgetItem* part_head = new QTreeWidgetItem(creat_tower_part);
	part_head->setText(0, QString("塔头部件"));

	QTreeWidgetItem* creat_tower_instance = new QTreeWidgetItem(ui.treeWidget);
	creat_tower_instance->setText(0, QString("生成杆塔实例"));

	QTreeWidgetItem* creat_towerwire_instance = new QTreeWidgetItem(ui.treeWidget);
	creat_towerwire_instance->setText(0, QString("生成塔线组实例"));

	QTreeWidgetItem* attribute = new QTreeWidgetItem(ui.treeWidget);
	attribute->setText(0, QString("属性"));

	QTreeWidgetItem* att_material = new QTreeWidgetItem(attribute);
	att_material->setText(0, QString("材料属性"));

	QTreeWidgetItem* att_section = new QTreeWidgetItem(attribute);
	att_section->setText(0, QString("截面属性"));

	QTreeWidgetItem* assign_section = new QTreeWidgetItem(attribute);
	assign_section->setText(0, QString("赋予截面"));

	QTreeWidgetItem* pNewItem9 = new QTreeWidgetItem(attribute);
	pNewItem9->setText(0, QString("根据长度赋予截面"));

	QTreeWidgetItem* Loads = new QTreeWidgetItem(ui.treeWidget);
	Loads->setText(0, QString("荷载"));

	QTreeWidgetItem* creat_loads = new QTreeWidgetItem(Loads);
	creat_loads->setText(0, QString("创建载荷"));

	QTreeWidgetItem* edit_loads = new QTreeWidgetItem(Loads);
	edit_loads->setText(0, QString("管理载荷"));

	QTreeWidgetItem* creat_constraints = new QTreeWidgetItem(Loads);
	creat_constraints->setText(0, QString("创建边界条件"));

	QTreeWidgetItem* edit_constraints = new QTreeWidgetItem(Loads);
	edit_constraints->setText(0, QString("编辑边界条件"));

	QTreeWidgetItem* insulator_string = new QTreeWidgetItem(ui.treeWidget);
	insulator_string->setText(0, QString("绝缘子串"));

	QTreeWidgetItem* Wire_modeling = new QTreeWidgetItem(ui.treeWidget);
	Wire_modeling->setText(0, QString("导线建模"));

	QTreeWidgetItem* WIndLoad = new QTreeWidgetItem(ui.treeWidget);
	WIndLoad->setText(0, QString("创建风载荷"));

}
//双击树item相应事件
void InterFace::onTreeitemDoubleClicked(QTreeWidgetItem* item)
{
	//生成杆塔部件
	if (item == ui.treeWidget->topLevelItem(0)->child(0))
	{
		ui_Foot();
	}
	else if (item == ui.treeWidget->topLevelItem(0)->child(1))
	{
		ui_Body();
	}
	else if (item == ui.treeWidget->topLevelItem(0)->child(2))
	{
		if (ui.treeWidget->topLevelItem(0)->child(1)->childCount() == 0)
		{
			QMessageBox::information(this, "Tips", "请先创建塔身！");
		}
		else { ui_CrossArm(); }	
	}
	//生成杆塔实例
	else if (item == ui.treeWidget->topLevelItem(1))
	{
		ui_Tower();
	}
	//属性
	else if (item == ui.treeWidget->topLevelItem(2)->child(1))
	{
		ui_Section();
	}
	else if (isChildOfPartSetSection(item))
	{
		ui_SetSection(item);
	}
	else if (isChildOfPartSetAllSection(item))
	{
		ui_SetAllSection(item);
	}
	//荷载
	else if (item == ui.treeWidget->topLevelItem(4)->child(1))
	{
		ui_ManageLoads();
	}
	else if (item == ui.treeWidget->topLevelItem(4)->child(2))
	{
		Constraint_Tips();
	}
	else if (item == ui.treeWidget->topLevelItem(4)->child(3))
	{
		
	}
	//绝缘子串
	else if (isChildOfPartSetSpacer(item))
	{
		ui_Interphase_spacer(item);
	}
	//导线建模
	else if (item == ui.treeWidget->topLevelItem(6))
	{
		ui_Wire_InterFace();
	}
	else if (isChildOfTower(item,0))
	{
		ui_CreatLoads(item);
	}
	else if (isChildOfTower(item, 1))
	{
		CreateOutPut(item);
	}
	else if (isChildOfTower(item, 2))
	{
		CreateInp(item);
	}
	else if (item == ui.treeWidget->topLevelItem(2))
	{
		ui_TowerWireGroup();
	}
	else if (item == ui.treeWidget->topLevelItem(7))
	{
		ui_Wind();
	}

}
void InterFace::onTreeitemClicked(QTreeWidgetItem* item)
{
	//部件切换
	Part_Base* part = OnFindPart(item);

	if (part != nullptr)
	{
		Show_Part(part);
	}

	if (item->parent() == ui.treeWidget->topLevelItem(1))
	{
		Tower* tower = OnFindTower(item);
		Show_Tower(tower);
	}

}

//生成塔腿
void InterFace::ui_Foot()
{
	switchRenderWindow(0);
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
		HiddeAllPart();

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

void InterFace::ui_WireTest()
{
	switchRenderWindow(0);
}
void InterFace::ui_Body()
{
	switchRenderWindow(0);
	T_Body* T_body = new T_Body(this);//ui
	int ret = T_body->exec();
	if (ret == QDialog::Accepted)
	{
		QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(0)->child(1);
		QTreeWidgetItem* item = new QTreeWidgetItem(parent);
		AddPartFunction(item);
		TowerPart_body* t = new TowerPart_body;
		T_body->Get_Data(t);
		item->setText(0, t->m_name);
		HiddeAllPart();
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
	switchRenderWindow(0);
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
		HiddeAllPart();
		t->Item = item;
		item->setText(0, t->m_name);
		
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
	switchRenderWindow(1);
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
			int crossArm = TP_CrossArm.Find_Entity(i)->m_id;
			for (auto& i : towerPartInsulator)
			{
				if (i.second->ArmId == crossArm)
				{
					tw->addPart(i.second);
				}
			}
		}
		tw->Check();
		HiddeAllTower();

		tw->Item = item;
		item->setText(0, T_As->Get_name());
		tw->m_id = TP.size() + 1;//编号
		
		double x = 0; double y = 0; double z = 0; double angle = 0;
		T_As->Get_Movedata(x, y, z, angle);//得到平移旋转角度
		tw->rotation(angle);//旋转
		tw->move(x, y, z);//平移
		
		tw->Show_VTKnode(m_Renderer_2);
		tw->Show_VTKtruss(m_Renderer_2);
		tw->Show_VTKbeam(m_Renderer_2);

		TP.Add_Entity(tw);

		tw->m_BeamActor->VisibilityOn();
		tw->m_TrussActor->VisibilityOn();
		tw->Node_actor->VisibilityOn();

		m_Renderer_2->ResetCamera();
		tower_assembles.push_back(T_As);
	}

}
void InterFace::ui_Section()
{
	Set_Section* M_ab = new Set_Section(this);//ui
	int ret = M_ab->exec();
	if (ret == QDialog::Accepted)
	{
		int ClassSection = 0;//取得该截面类型的参数
		ClassSection = M_ab->ClassSection;
		string NameSection = M_ab->name;
		double ia = M_ab->a;
		double ib = M_ab->b;
		int iM = M_ab->ClassMa;
		int id = MS.size() + 1;//从1开始排序
		Section* section = new Section(ia, ib, id, ClassSection, iM);
		MS.push_back(Section(ia, ib, id, ClassSection, iM));
		for (int i = 0; i < ui.treeWidget->topLevelItem(0)->child(0)->childCount(); i++)//新键一个，塔腿里放一个
		{
			TP_leg.Find_Entity(i + 1)->AddNewSection(ia, ib, id, ClassSection, iM);
		}
		for (int i = 0; i < ui.treeWidget->topLevelItem(0)->child(1)->childCount(); i++)//新键一个，塔身里放一个
		{
			TP_body.Find_Entity(i + 1)->AddNewSection(ia, ib, id, ClassSection, iM);
		}
		for (int i = 0; i < ui.treeWidget->topLevelItem(0)->child(2)->childCount(); i++)//新键一个，塔头里放一个
		{
			TP_CrossArm.Find_Entity(i + 1)->AddNewSection(ia, ib, id, ClassSection, iM);
		}
		Ms.Add_Entity(section);
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
	for (auto i : Part->m_Elements_beams)
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
	Part_Base* part = OnFindPart(item->parent());
	Interphase_spacer* IS = new Interphase_spacer(Arm,this);

	IS->show();//要互动只能用show
}

void InterFace::ui_Wire_InterFace()
{
	
	Wire_InterFace* wire = new Wire_InterFace(this);
	wire->show();
}

void InterFace::ui_ManageLoads()
{
	Manage_Loads* manage_loads = new Manage_Loads;
	manage_loads->show();
}

void InterFace::ui_TowerWireGroup()
{
	switchRenderWindow(2);
	TowerWireGroup* towerWireGroup = new TowerWireGroup;
	TowerWireGroupAssembly* towerWireGroupAssembly = new TowerWireGroupAssembly(this, towerWireGroup);
	towerWireGroupAssembly->show();
}
void InterFace::ui_Constraint()
{
	Create_Constraint* con = new Create_Constraint(this);
	con->show();
}

void InterFace::SaveFile()
{
	QString str = QFileDialog::getSaveFileName(this, "保存", "/", "datafile(*.dat);;textfile(*.txt);;c file(*.cpp);;All file(*.*)");

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
			TP_leg.Save(Stream);
			TP_body.Save(Stream);
			TP_CrossArm.Save(Stream);
			TP.Save(Stream);
		}
		Qf.close();
	}
}
void InterFace::OpenFile()
{
	QString str = QFileDialog::getOpenFileName(this, "打开", "/", "datafile(*.dat);;textfile(*.txt);;c file(*.cpp);;All file(*.*)");

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
			TP_leg.Read(Stream);
			TP_body.Read(Stream);
			TP_CrossArm.Read(Stream);
			TP.Read(Stream);

			for (int i = 0; i < TP_leg.size(); i++)
			{
				QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(0)->child(0);
				QTreeWidgetItem* item = new QTreeWidgetItem(parent);
				QString str = QString::number(parent->childCount());     //str转字符
				item->setText(0, QString("塔腿部件" + str));
				TP_leg.Find_Entity(i + 1)->Item = item;
				//TP_leg[i]->Item = item;
			}

			for (int i = 0; i < TP_body.size(); i++)
			{
				QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(0)->child(1);
				QTreeWidgetItem* item = new QTreeWidgetItem(parent);
				QString str = QString::number(parent->childCount());     //str转字符
				item->setText(0, QString("塔身部件" + str));
				TP_body.Find_Entity(i + 1)->Item = item;
			}
			for (int i = 0; i < TP_CrossArm.size(); i++)
			{
				QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(0)->child(2);
				QTreeWidgetItem* item = new QTreeWidgetItem(parent);
				QString str = QString::number(parent->childCount());     //str转字符
				item->setText(0, QString("横担部件" + str));
				TP_CrossArm.Find_Entity(i + 1)->Item = item;
			}
			for (int i = 0; i < TP.size(); i++)
			{
				QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(1);
				QTreeWidgetItem* item = new QTreeWidgetItem(parent);
				QString str = QString::number(parent->childCount());     //str转字符
				item->setText(0, QString("输电塔" + str));
				TP.Find_Entity(i + 1)->Item = item;
			}

		}
	}
	Qf.close();
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

void InterFace::HiddeAllPart()
{
	vtkPropCollection* props = m_Renderer->GetViewProps(); //iterate through and set each visibility to 0
	props->InitTraversal();
	for (int i = 0; i < props->GetNumberOfItems(); i++)
	{
		props->GetNextProp()->VisibilityOff();

		m_Renderer->ResetCamera();
		m_renderWindow->Render();
	}
}

void InterFace::HiddeAllTower()
{
	vtkPropCollection* props = m_Renderer_2->GetViewProps(); //iterate through and set each visibility to 0
	props->InitTraversal();
	for (int i = 0; i < props->GetNumberOfItems(); i++)
	{
		props->GetNextProp()->VisibilityOff();

		m_Renderer_2->ResetCamera();
		m_renderWindow->Render();
	}
}

void InterFace::SubstaceActor(Part_Base* Part)
{
	for (auto& i : Part->Nactor)
	{
		m_Renderer->AddActor(i);
		cout << "number of actor :" << i << "\n";
	}
	for (auto& i : Part->m_Elements_beams)
	{
		cout << i.m_idElement << "  " << i.direction[0] << "  " << i.direction[1] << "  " << i.direction[2] << "\n";
	}
	cout << "Number of actor:" << Part->Nactor.size() << "\n";
	m_renderWindow->Render();
	m_Renderer->ResetCamera();

}

void InterFace::ShowLoadactor(Tower* tower)
{
	for (auto& i : tower->m_LoadActor)
	{
		m_Renderer_2->AddActor(i);
		cout << "number of actor :" << i << "\n";
	}
	cout << "Number of actor:" << tower->m_LoadActor.size() << "\n";
	m_renderWindow->Render();
	m_Renderer->ResetCamera();
}

void InterFace::switchRenderWindow(int index)
{
	Close_Point();
	m_renderWindow->RemoveRenderer(m_Renderer);
	m_renderWindow->RemoveRenderer(m_Renderer_2);
	m_renderWindow->RemoveRenderer(m_Renderer_3);

	if (index == 0)
	{
		m_renderWindow->AddRenderer(m_Renderer);
		// Set up camera and other settings for Renderer 1
		m_Renderer->ResetCamera();
	}
	else if (index == 1)
	{
		m_renderWindow->AddRenderer(m_Renderer_2);
		// Set up camera and other settings for Renderer 2
		m_Renderer_2->ResetCamera();
	}
	else if (index == 2)
	{
		m_renderWindow->AddRenderer(m_Renderer_3);
		// Set up camera and other settings for Renderer 3
		m_Renderer_3->ResetCamera();
	}

	m_renderWindow->Render();
}

void InterFace::initMenu()
{
	m_pMenu = new QMenu(this);
	QAction* pAc1 = new QAction("点选");
	QAction* pAc2 = new QAction("框选");
	QAction* pAc3 = new QAction("取消");
	m_pMenu->addAction(pAc1);
	m_pMenu->addAction(pAc2);
	m_pMenu->addAction(pAc3);
	connect(pAc1, &QAction::triggered, [=] {Point_Inqure(); });
	connect(pAc2, &QAction::triggered, [=] {Area_Inqure(); });
	connect(pAc3, &QAction::triggered, [=] {Close_Point(); });
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
		QTreeWidgetItem* OutPut = new QTreeWidgetItem(item);//施加载荷按钮
		QTreeWidgetItem* Inp = new QTreeWidgetItem(item);//施加载荷按钮
		AddLoadForce->setText(0, QString("施加荷载"));
		OutPut->setText(0, QString("输出计算文件"));
		Inp->setText(0, QString("输出ABAQUS计算文件"));
	}


}

double* InterFace::GetSectionData(int SectionGroup)
{
	double* SectionData = new double;//用于接收截面数据的数组，第一个是L的长边或者圆半径，第二个是L短边或者圆环厚度，第三个是截面类型，第四个是材料编号
	for (auto i : MS)
	{
		if (i.m_id == SectionGroup)
		{
			SectionData[0] = i.a;
			SectionData[1] = i.b;
			SectionData[2] = i.ClassSe;
			SectionData[3] = i.ClassM;
			return SectionData;
		}

	}
	return nullptr;
	return nullptr;
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
	for (auto i : m_NodeSelected)
	{
		if (i != nullptr) m_CurrentRenderer->RemoveActor(i);
	}
	m_NodeSelected.clear();
}
void InterFace::Get_SelectedNode(std::list<Node*>& Nodes)
{
	for (auto i : m_NodeSelected)
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
	for (auto i : m_NodeSelected)
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
	Framestyle->SetDefaultRenderer(m_Renderer_2);
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

bool InterFace::isChildOfTower(QTreeWidgetItem* item, int childNumber)
{
	QTreeWidgetItem* topLevelItem3 = ui.treeWidget->topLevelItem(1);
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

void InterFace::Close_Point()
{
	UnSelect_Nodes();
	// 保存主界面的交互器样式
	m_MainStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	// 在需要切换回主界面时，使用保存的交互器样式重新设置交互器
	m_renderWindow->GetInteractor()->SetInteractorStyle(m_MainStyle);
	
}

void InterFace::GetData(QStringList& sInfo)
{
	QList<QString> list;
	for (int i = 0; i < sInfo.size(); i++)
	{
		list.append(sInfo[i]);
	}
	Scetion_lists.push_back(list);
}

void InterFace::Test_mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		QPoint pos = event->pos();
		cout << "pos.x = " << pos.x()<<"pos.y = " << pos.y() << "\n";
	}
}

void InterFace::ui_CreatLoads(QTreeWidgetItem* item)
{
	Tower* tower = OnFindTower(item->parent());
	Show_Tower(tower);//!!
	Creat_Loads* creat_load = new Creat_Loads(tower, this);
	creat_load->show();
	
}

void InterFace::CreateOutPut(QTreeWidgetItem* item)
{
	Tower* tower = OnFindTower(item->parent());
	tower->CreateOutPut();
}

void InterFace::CreateInp(QTreeWidgetItem* item)
{
	Tower* tower = OnFindTower(item->parent());
	CreateAbaqusInp Inp;
	Inp.CreateInp(tower);
}

void InterFace::Show_Part(Part_Base* part)
{
	if (part == nullptr) return;
	switchRenderWindow(0);
	HiddeAllPart();
	part->m_BeamActor->VisibilityOn();
	part->m_TrussActor->VisibilityOn();
	for (auto& i : part->Nactor)
	{
		i->VisibilityOn();
	}
	SubstaceActor(part);
	m_Renderer->ResetCamera();
}

void InterFace::Show_Tower(Tower* tower)
{
	if (tower == nullptr) return;
	switchRenderWindow(1);
	HiddeAllTower();
	tower->m_BeamActor->VisibilityOn();
	tower->m_TrussActor->VisibilityOn();
	tower->Node_actor->VisibilityOn();
	//for (auto& i : tower->Nactor)
	//{
	//	m_Renderer_2->AddActor(i);
	//}
	for (auto& i : tower->m_LoadActor)
	{
		m_Renderer_2->AddActor(i);
		i->VisibilityOn();
	}
	m_Renderer_2->ResetCamera();
}

void InterFace::Constraint_Tips()
{
	
	// 移除现有的布局管理器
	QLayout* currentLayout = ui.widget_2->layout();
	if (currentLayout)
	{
		QLayoutItem* item;
		while ((item = currentLayout->takeAt(0)) != nullptr)
		{
			QWidget* widget = item->widget();
			delete widget;
			delete item;
		}
		delete currentLayout;
	}
	
	// 创建新的布局管理器
	layout_Tips = new QHBoxLayout(ui.widget_2);

	// 创建标签
	label = new QLabel("选择要创建边界条件的节点", ui.widget_2);

	// 创建行编辑
	Node_Edit = new QLineEdit(ui.widget_2);

	// 重新创建按钮
	OK_Btn = new QPushButton("OK", ui.widget_2);
	Ensure_Btn = new QPushButton("Ensure", ui.widget_2);
	// 添加到布局管理器中
	layout_Tips->addWidget(label);
	layout_Tips->addWidget(Node_Edit);
	layout_Tips->addWidget(OK_Btn);
	layout_Tips->addWidget(Ensure_Btn);

	// 设置布局管理器
	ui.widget_2->setLayout(layout_Tips);
	connect(this, &InterFace::Msg_Select_Nodes, this, &InterFace::Insert_Data);
}

void InterFace::Delete_Constraint()
{
	// 清空布局管理器中的控件
	QLayoutItem* item;
	while ((item = layout_Tips->takeAt(0)) != nullptr)
	{
		QWidget* widget = item->widget();
		delete widget;
		delete item;
	}
}

void InterFace::ui_Wind()
{
	if (wd == nullptr)
	{
		wd = new Wind(this);
		wd->CreateCombobox();
		wd->show();
	}
	else
	{
		wd->CreateCombobox();
		wd->show();
	}
}


void InterFace::Insert_Data()
{
	set<Node*> Nodes;
	Get_SelectedNode(Nodes);
	//QMap<int, QVector<double>> nodeCoordinates; // 存储节点坐标信息
	//QMap<int, int> idMap; // 存储ID与索引的映射关系
	QString existingText = Node_Edit->text();
	QStringList existingIds = existingText.split(" ", Qt::SkipEmptyParts);
	
	for (auto pNode : Nodes)
	{
		int idNode = pNode->m_idNode;
		QString idStr = QString("%1").arg(idNode);

		if (!existingIds.contains(idStr))
		{
			QVector<double> coordinates = { pNode->x, pNode->y, pNode->z };
			nodeCoordinates[index] = coordinates; // 存储坐标信息
			idMap[idNode] = index; // 存储ID与索引的映射关系

			if (!existingText.isEmpty())
				existingText += " ";

			existingText += idStr;
			existingIds.append(idStr);

			index++;
		}
	}

	if (!existingText.isEmpty())
	{
		if (QApplication::keyboardModifiers() == Qt::ShiftModifier)
		{
			Node_Edit->setText(existingText.trimmed()); // 追加所有新的id到lineEdit
		}
		else
		{
			if (existingIds.size() > 1)
			{
				QString clickedId = existingIds.last(); // 获取最后一个点击的节点ID
				existingText = clickedId; // 只保留最后一个点击的节点ID
			}
			Node_Edit->setText(existingText);
		}
	}

	connect(OK_Btn, &QPushButton::clicked, this, [=]() {
		handleOkButtonClicked();
		disconnect(this, &InterFace::Msg_Select_Nodes, this, &InterFace::Insert_Data);
		Close_Point(); });

	connect(Ensure_Btn, &QPushButton::clicked, this, &InterFace::Delete_Constraint);

}

void InterFace::handleOkButtonClicked()
{
	QStringList allIds = Node_Edit->text().split(" ", Qt::SkipEmptyParts);
	// 使用所有显示的ID，执行相应的操作
	for (const QString& id : allIds)
	{
		int nodeId = id.toInt();
		// 获取ID对应的坐标信息
		if (idMap.contains(nodeId))
		{
			int index = idMap[nodeId];
			const QVector<double>& coordinates = nodeCoordinates[index];
			double x = coordinates[0];
			double y = coordinates[1];
			double z = coordinates[2];
			Con_Nodes.push_back(Node(x, y, z));
			// 输出坐标信息
			qDebug() << "ID: " << id << " X: " << x << " Y: " << y << " Z: " << z;
		}
	}
	Delete_Constraint();
	ui_Constraint();
	qDebug() << Con_Nodes.size() << endl;
}

Node* InterFace::Find_Node(int id, vector<Node>& node, vector<Node>& Con_node)
{
	for (auto& i : node)
	{
		if (i.m_idNode == id)
		{
			return &i;
			Con_node.push_back(Node(i.x, i.y, i.z));
		}
	}
	cout << "没找到！\n";
	return nullptr;
}

InterFace::~InterFace()
{

}