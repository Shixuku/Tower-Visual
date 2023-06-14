#include "InterFace.h"
#include <vtkPropCollection.h>
#include <vtkProperty.h>
#include<QMessageBox.h>
#include"Manage_PartData.h"
#include"Manage_InsData.h"
#include <vtkPointData.h>//->GetArray("Address")
#include <vtkIdTypeArray.h>
#include"MouseInteractorHighLightActor.h"
#include"Interphase_spacer.h"
#include"T_Foot.h"
#include"T_Body.h"
#include"T_CrossArm.h"
#include"Tower_Assembly.h"
#include"Assign_Section.h"
#include"SetAllSection.h"
#include"Wire_InterFace.h"
#include"AddLoadForce.h"
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
	
}

void InterFace::SetupCentralWidget()
{
	//更改背景颜色
	m_Renderer->SetBackground(1.0, 1.0, 1.0);              // 底部颜色值
	m_Renderer->SetBackground2(0.629, 0.8078, 0.92157);    // 顶部颜色值
	m_Renderer->SetGradientBackground(1);                  // 开启渐变色背景设置

	m_Renderer_2->SetBackground(0.8, 0.8, 0.8);              // 底部颜色值
	m_Renderer_2->SetBackground2(0.4, 0.4, 0.4);    // 顶部颜色值
	m_Renderer_2->SetGradientBackground(1);                  // 开启渐变色背景设置

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

	QTreeWidgetItem* pNewItem1 = new QTreeWidgetItem(ui.treeWidget);
	pNewItem1->setText(0, QString("塔腿部件"));

	QTreeWidgetItem* pNewItem2 = new QTreeWidgetItem(ui.treeWidget);
	pNewItem2->setText(0, QString("塔身部件"));

	QTreeWidgetItem* pNewItem3 = new QTreeWidgetItem(ui.treeWidget);
	pNewItem3->setText(0, QString("塔头部件"));

	QTreeWidgetItem* pNewItem4 = new QTreeWidgetItem(ui.treeWidget);
	pNewItem4->setText(0, QString("生成杆塔实例"));

	QTreeWidgetItem* pNewItem5 = new QTreeWidgetItem(ui.treeWidget);
	pNewItem5->setText(0, QString("属性"));

	QTreeWidgetItem* pNewItem6 = new QTreeWidgetItem(pNewItem5);
	pNewItem6->setText(0, QString("材料属性"));

	QTreeWidgetItem* pNewItem7 = new QTreeWidgetItem(pNewItem5);
	pNewItem7->setText(0, QString("截面属性"));

	QTreeWidgetItem* pNewItem8 = new QTreeWidgetItem(pNewItem5);
	pNewItem8->setText(0, QString("赋予截面"));

	QTreeWidgetItem* pNewItem9 = new QTreeWidgetItem(pNewItem5);
	pNewItem9->setText(0, QString("根据长度赋予截面"));

	QTreeWidgetItem* pNewItem10 = new QTreeWidgetItem(ui.treeWidget);
	pNewItem10->setText(0, QString("绝缘子串"));

	QTreeWidgetItem* pNewItem11 = new QTreeWidgetItem(ui.treeWidget);
	pNewItem11->setText(0, QString("导线建模"));
}

void InterFace::onTreeitemDoubleClicked(QTreeWidgetItem* item)
{
	if (item == ui.treeWidget->topLevelItem(0))
	{
		ui_Foot();
	}

	else if (item == ui.treeWidget->topLevelItem(1))
	{
		ui_Body();
	}

	else if (item == ui.treeWidget->topLevelItem(2))
	{
		if (ui.treeWidget->topLevelItem(1)->childCount() == 0)
		{
			QMessageBox::information(this, "Tips", "请先创建塔身！");
		}
		else { ui_CrossArm(); }
			
	}
	else if (item == ui.treeWidget->topLevelItem(3))
	{
		ui_Tower();
	}
	else if (item == ui.treeWidget->topLevelItem(4)->child(1))
	{
		ui_Section();
	}
	else if (item == ui.treeWidget->topLevelItem(4)->child(2))
	{
		ui_SetSection();
	}
	else if (item == ui.treeWidget->topLevelItem(4)->child(3))
	{
		ui_SetAllSection();
	}
	else if (item == ui.treeWidget->topLevelItem(5))
	{
		ui_Interphase_spacer();
	}
	else if (item == ui.treeWidget->topLevelItem(6))
	{
		ui_Wire_InterFace();
	}
	else if (isChildOfTopLevelItem3(item))
	{
		ui_AddLoadForce(item);
	}
	else if (isChildOfTopLevelItem3OutPut(item))
	{
		CreateOutPut(item);
	}
}
void InterFace::onTreeitemClicked(QTreeWidgetItem* item)
{
	Part_Base* part = OnFindPart(item);

	if (part != nullptr)
	{
		HiddeAllPart();
		switchRenderWindow(0);
		part->m_BeamActor->VisibilityOn();
		part->m_TrussActor->VisibilityOn();
		part->Node_actor->VisibilityOn();
	}

	if (item == ui.treeWidget->topLevelItem(3))
	{
		HiddeAllTower();
		switchRenderWindow(1);
		for (auto& i : TP)
		{
			if (i.second != nullptr)
			{
				i.second->m_BeamActor->VisibilityOn();
				i.second->m_TrussActor->VisibilityOn();
				i.second->Node_actor->VisibilityOn();
			}
		}
	}
	ResetCamera();
}


//生成塔腿
void InterFace::ui_Foot()
{
	switchRenderWindow(0);
	T_Foot* T_foot = new T_Foot(this);//ui
	int ret = T_foot->exec();
	if (ret == QDialog::Accepted)
	{
		QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(0);
		QTreeWidgetItem* item = new QTreeWidgetItem(parent);
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
		ResetCamera();

		t_foots.push_back(T_foot);
	/*	TP_leg.Find_Entity(int(t->m_id))->ShowNode();
		TP_leg.Find_Entity(int(t->m_id))->ShowElement();*/
		
	}

}
void InterFace::ui_Body()
{
	switchRenderWindow(0);
	T_Body* T_body = new T_Body(this);//ui
	int ret = T_body->exec();
	if (ret == QDialog::Accepted)
	{
		QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(int(1));
		QTreeWidgetItem* item = new QTreeWidgetItem(parent);
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
		ResetCamera();

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
		QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(2);
		QTreeWidgetItem* item = new QTreeWidgetItem(parent);
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
		ResetCamera();

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
		QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(3);
		QTreeWidgetItem* item = new QTreeWidgetItem(parent);//
		QTreeWidgetItem* AddLoadForce = new QTreeWidgetItem(item);//施加载荷按钮
		QTreeWidgetItem* OutPut = new QTreeWidgetItem(item);//施加载荷按钮
		AddLoadForce->setText(0, QString("施加荷载"));
		OutPut->setText(0, QString("输出计算文件"));
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
		tw->Check();

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

		HiddeAllTower();
		for (auto& i : TP)
		{
			if (i.second != nullptr)
			{
				i.second->m_BeamActor->VisibilityOn();
				i.second->m_TrussActor->VisibilityOn();
				i.second->Node_actor->VisibilityOn();
			}
		}
		ResetCamera();
		tower_assembles.push_back(T_As);
		TP.Find_Entity(int(tw->m_id))->ShowNode();
		TP.Find_Entity(int(tw->m_id))->ShowElement();
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
		for (int i = 0; i < ui.treeWidget->topLevelItem(0)->childCount(); i++)//新键一个，塔腿里放一个
		{
			TP_leg.Find_Entity(i + 1)->AddNewSection(ia, ib, id, ClassSection, iM);
		}
		for (int i = 0; i < ui.treeWidget->topLevelItem(1)->childCount(); i++)//新键一个，塔身里放一个
		{
			TP_body.Find_Entity(i + 1)->AddNewSection(ia, ib, id, ClassSection, iM);
		}
		for (int i = 0; i < ui.treeWidget->topLevelItem(2)->childCount(); i++)//新键一个，塔头里放一个
		{
			TP_CrossArm.Find_Entity(i + 1)->AddNewSection(ia, ib, id, ClassSection, iM);
		}
		Ms.Add_Entity(section);
	}
}
void InterFace::ui_SetSection()
{
	Assign_Section* assign_Section = new Assign_Section(this);
	assign_Section->show();

}
void InterFace::ui_SetAllSection()
{
	SetAllSection* SetAll = new SetAllSection(this);
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

void InterFace::ui_Interphase_spacer()
{
	Interphase_spacer* IS = new Interphase_spacer(this);
	IS->show();//要互动只能用show
	Point_Inqure();
}

void InterFace::ui_Wire_InterFace()
{
	Wire_InterFace* wire = new Wire_InterFace(this);
	wire->show();
	Point_Inqure();

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
				QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(0);
				QTreeWidgetItem* item = new QTreeWidgetItem(parent);
				QString str = QString::number(parent->childCount());     //str转字符
				item->setText(0, QString("塔腿部件" + str));
				TP_leg.Find_Entity(i + 1)->Item = item;
				//TP_leg[i]->Item = item;
			}

			for (int i = 0; i < TP_body.size(); i++)
			{
				QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(1);
				QTreeWidgetItem* item = new QTreeWidgetItem(parent);
				QString str = QString::number(parent->childCount());     //str转字符
				item->setText(0, QString("塔身部件" + str));
				TP_body.Find_Entity(i + 1)->Item = item;
			}
			for (int i = 0; i < TP_CrossArm.size(); i++)
			{
				QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(2);
				QTreeWidgetItem* item = new QTreeWidgetItem(parent);
				QString str = QString::number(parent->childCount());     //str转字符
				item->setText(0, QString("横担部件" + str));
				TP_CrossArm.Find_Entity(i + 1)->Item = item;
			}
			for (int i = 0; i < TP.size(); i++)
			{
				QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(3);
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

void InterFace::ResetCamera() const
{
	m_Renderer->ResetCamera();
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
		if (i != nullptr) m_Renderer->RemoveActor(i);
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

void InterFace::Point_Inqure()
{
	// An interactor
	vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
	renderWindowInteractor->SetRenderWindow(m_renderWindow);
	// Set the custom type to use for interaction.
	vtkNew<MouseInteractorHighLightActor> style;
	style->m_pInterFace = this;
	style->SetDefaultRenderer(m_Renderer_2);
	renderWindowInteractor->SetInteractorStyle(style);

	vtkAxesActor* Axes = vtkAxesActor::New();
	vtkOrientationMarkerWidget* widgetAxes = vtkOrientationMarkerWidget::New();
	widgetAxes->SetOrientationMarker(Axes);
	widgetAxes->SetInteractor(renderWindowInteractor);
	widgetAxes->SetEnabled(1);
	widgetAxes->SetInteractive(0);
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

bool InterFace::isChildOfTopLevelItem3(QTreeWidgetItem* item)
{
	QTreeWidgetItem* topLevelItem3 = ui.treeWidget->topLevelItem(3);
	int childCount = topLevelItem3->childCount();

	for (int i = 0; i < childCount; i++)
	{
		QTreeWidgetItem* childItem = topLevelItem3->child(i);
		if (item == childItem->child(0))
		{
			return true;
		}
	}
	return false;
}

bool InterFace::isChildOfTopLevelItem3OutPut(QTreeWidgetItem* item)
{
	QTreeWidgetItem* topLevelItem3 = ui.treeWidget->topLevelItem(3);
	int childCount = topLevelItem3->childCount();

	for (int i = 0; i < childCount; i++)
	{
		QTreeWidgetItem* childItem = topLevelItem3->child(i);
		if (item == childItem->child(1))
		{
			return true;
		}
	}
	return false;
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

void InterFace::ui_AddLoadForce(QTreeWidgetItem* item)
{
	Tower* tower = OnFindTower(item->parent());
	AddLoadForce* addload = new AddLoadForce(tower,this);
	addload->show();
	Point_Inqure();
}

void InterFace::CreateOutPut(QTreeWidgetItem* item)
{
	Tower* tower = OnFindTower(item->parent());
	tower->CreateOutPut();
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

	ResetCamera();
}



void InterFace::Show_Tower(Tower* tower)
{
	if (tower == nullptr) return;
	switchRenderWindow(1);
	HiddeAllTower();
	tower->m_BeamActor->VisibilityOn();
	tower->m_TrussActor->VisibilityOn();
	for (auto& i : tower->Nactor)
	{
		m_Renderer_2->AddActor(i);
	}
	ResetCamera();
}
InterFace::~InterFace()
{

}