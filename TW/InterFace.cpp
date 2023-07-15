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

InterFace::InterFace(QWidget* parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	Base::set_InterFace(this);

	layout = new QHBoxLayout(ui.widget);//���ɲ���,widgetר�ŷ����ɵ���
	m_VtkWidget = new QVTKOpenGLNativeWidget();//���ɿؼ�

	m_renderWindow = vtkGenericOpenGLRenderWindow::New();
	m_VtkWidget->setRenderWindow(m_renderWindow);

	m_Renderer = vtkRenderer::New();
	m_renderWindow->AddRenderer(m_Renderer);//��ӷŲ�����vtk

	m_Renderer_2 = vtkRenderer::New();
	m_renderWindow->AddRenderer(m_Renderer_2);//��ӷŵ�����vtk

	m_Renderer_3 = vtkRenderer::New();
	m_renderWindow->AddRenderer(m_Renderer_3);//��ӷ��������vtk
	this->setContextMenuPolicy(Qt::DefaultContextMenu);
	m_CurrentRenderer = nullptr;
	initMenu();
	//�ұߴ���
	SetupCentralWidget();
	//��ߴ���
	TreeWidgetShow();
	
	//Ĭ�ϴ򿪵���part��vtk����
	switchRenderWindow(0);
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &InterFace::onTreeitemClicked);
	connect(ui.treeWidget, &QTreeWidget::itemDoubleClicked, this, &InterFace::onTreeitemDoubleClicked);
	connect(ui.actionLeg, &QAction::triggered, this, &InterFace::ui_Foot);
	connect(ui.actionBody, &QAction::triggered, this, &InterFace::ui_Body);
	connect(ui.actionCrossArm, &QAction::triggered, this, [=]() 
		{
			if (ui.treeWidget->topLevelItem(1)->childCount() == 0)
			{
				QMessageBox::information(this, "Tips", "���ȴ�������");
			}
			else { ui_CrossArm(); }
		});
	
	connect(ui.menuSave, &QAction::triggered, this, &InterFace::SaveFile);
	connect(ui.actionRead, &QAction::triggered, this, &InterFace::OpenFile);
	connect(ui.btn_part, &QPushButton::clicked, this, &InterFace::ui_Management_PartData);
	connect(ui.btn_ins, &QPushButton::clicked, this, &InterFace::ui_Management_InsData);
	connect(ui.actioninit, &QAction::triggered, this, [=]()
		{
			/*m_Renderer->GetActiveCamera()->SetPosition(0, 0, 100000);
			m_Renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
			m_Renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
			m_renderWindow->Render();*/
		});
	connect(ui.actionYoZ, &QAction::triggered, this, [=]()
		{
			/*m_Renderer->GetActiveCamera()->SetPosition(100000, 0, 0);
			m_Renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
			m_Renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
			m_renderWindow->Render();*/
		});
	connect(ui.actionXoZ, &QAction::triggered, this, [=]()
		{
			//m_Renderer->GetActiveCamera()->SetPosition(1, 0, 0);
			//m_Renderer->GetActiveCamera()->SetFocalPoint(0, 100000, 0);
			//m_Renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
			//m_renderWindow->Render();
		});
	connect(ui.btn_caculate, &QPushButton::clicked, this, &InterFace::Caculate);
	connect(ui.btn_display, &QPushButton::clicked, this, &InterFace::Display);
	
}

void InterFace::SetupCentralWidget()
{
	//���ı�����ɫ
	m_Renderer->SetBackground(1.0, 1.0, 1.0);              // �ײ���ɫֵ
	m_Renderer->SetBackground2(0.629, 0.8078, 0.92157);    // ������ɫֵ
	m_Renderer->SetGradientBackground(1);    

	m_Renderer_2->SetBackground(1.0, 1.0, 1.0);              // �ײ���ɫֵ
	m_Renderer_2->SetBackground2(0.629, 0.8078, 0.92157);    // ������ɫֵ
	m_Renderer_2->SetGradientBackground(1);

	m_Renderer_3->SetBackground(1.0, 1.0, 1.0);              // �ײ���ɫֵ
	m_Renderer_3->SetBackground2(0.629, 0.8078, 0.92157);    // ������ɫֵ
	m_Renderer_3->SetGradientBackground(1);                  // ��������ɫ��������

	////�������ϵ
	//vtkAxesActor* axesActor = vtkAxesActor::New();
	////��Widget��ʽ,�����½ǵ��ӿ�����ʾ����ϵ���ɽ�����꽻��
	//vtkOrientationMarkerWidget* widget = vtkOrientationMarkerWidget::New();
	//widget->SetOrientationMarker(axesActor);
	//widget->SetInteractor(m_VtkWidget->interactor());
	//widget->SetViewport(0, 0, 0.2, 0.2);
	//widget->SetEnabled(true);
	//widget->InteractiveOn();

}
void InterFace::TreeWidgetShow()
{
	layout->addWidget(m_VtkWidget);//�����м���ؼ�
	ui.treeWidget->header()->setVisible(false);//���ñ�ͷ���ɼ�

	QTreeWidgetItem* creat_tower_part = new QTreeWidgetItem(ui.treeWidget);
	creat_tower_part->setText(0, QString("���ɸ�������"));

	QTreeWidgetItem* part_leg = new QTreeWidgetItem(creat_tower_part);
	part_leg->setText(0, QString("���Ȳ���"));

	QTreeWidgetItem* part_body = new QTreeWidgetItem(creat_tower_part);
	part_body->setText(0, QString("������"));

	QTreeWidgetItem* part_head = new QTreeWidgetItem(creat_tower_part);
	part_head->setText(0, QString("��ͷ����"));

	QTreeWidgetItem* creat_tower_instance = new QTreeWidgetItem(ui.treeWidget);
	creat_tower_instance->setText(0, QString("���ɸ���ʵ��"));

	creat_towerwire_instance = new QTreeWidgetItem(ui.treeWidget);
	creat_towerwire_instance->setText(0, QString("����������ʵ��"));

	QTreeWidgetItem* attribute = new QTreeWidgetItem(ui.treeWidget);
	attribute->setText(0, QString("����"));

	QTreeWidgetItem* att_material = new QTreeWidgetItem(attribute);
	att_material->setText(0, QString("��������"));

	QTreeWidgetItem* att_section = new QTreeWidgetItem(attribute);
	att_section->setText(0, QString("��������"));

	QTreeWidgetItem* assign_section = new QTreeWidgetItem(attribute);
	assign_section->setText(0, QString("�������"));

	QTreeWidgetItem* pNewItem9 = new QTreeWidgetItem(attribute);
	pNewItem9->setText(0, QString("���ݳ��ȸ������"));

	QTreeWidgetItem* Loads = new QTreeWidgetItem(ui.treeWidget);
	Loads->setText(0, QString("����"));

	QTreeWidgetItem* creat_loads = new QTreeWidgetItem(Loads);
	creat_loads->setText(0, QString("�����غ�"));

	QTreeWidgetItem* edit_loads = new QTreeWidgetItem(Loads);
	edit_loads->setText(0, QString("�����غ�"));

	QTreeWidgetItem* creat_constraints = new QTreeWidgetItem(Loads);
	creat_constraints->setText(0, QString("�����߽�����"));

	QTreeWidgetItem* edit_constraints = new QTreeWidgetItem(Loads);
	edit_constraints->setText(0, QString("�༭�߽�����"));

	QTreeWidgetItem* insulator_string = new QTreeWidgetItem(ui.treeWidget);
	insulator_string->setText(0, QString("��Ե�Ӵ�"));

	QTreeWidgetItem* Wire_modeling = new QTreeWidgetItem(ui.treeWidget);
	Wire_modeling->setText(0, QString("���߽�ģ"));

	QTreeWidgetItem* WIndLoad = new QTreeWidgetItem(ui.treeWidget);
	WIndLoad->setText(0, QString("�������غ�"));

	QTreeWidgetItem* CreatWire = new QTreeWidgetItem(ui.treeWidget);
	CreatWire->setText(0, QString("����ʵ��"));

	QTreeWidgetItem* Calculate = new QTreeWidgetItem(ui.treeWidget);
	Calculate->setText(0, QString("����"));

}
//˫����item��Ӧ�¼�
void InterFace::onTreeitemDoubleClicked(QTreeWidgetItem* item)
{
	//���ɸ�������
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
			QMessageBox::information(this, "Tips", "���ȴ�������");
		}
		else { ui_CrossArm(); }
	}
	//���ɸ���ʵ��
	else if (item == ui.treeWidget->topLevelItem(1))
	{
		ui_Tower();
	}
	//����
	else if (item == ui.treeWidget->topLevelItem(3)->child(1))
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
	//����
	else if (item == ui.treeWidget->topLevelItem(4)->child(1))
	{
		ui_ManageLoads();//������أ���ʱ��û��
	}
	else if (item == ui.treeWidget->topLevelItem(4)->child(2))
	{
		//Constraint_Tips();//ʩ��Լ��
	}
	else if (item == ui.treeWidget->topLevelItem(4)->child(3))
	{//����Լ��

	}
	//��Ե�Ӵ�
	else if (isChildOfPartSetSpacer(item))
	{
		ui_Interphase_spacer(item);
	}
	//���߽�ģ
	else if (isChildOfGroup(item, 1))
	{
		ui_Wire_InterFace(item);
	}
	else if (isChildOfTower(1, item, 0))
	{//ʩ���غ�
		ui_CreatLoads(item);
	}
	else if (isChildOfTower(1, item, 1))
	{//���txt�ļ�
		CreateOutPut(item);
	}
	else if (isChildOfTower(1, item, 2))
	{//���inp�ļ�
		//CreateInp(item);
	}
	else if (isChildOfTower(1, item, 3))
	{//ʩ��Լ��
		Constraint_Tips1(item);
	}
	else if (item == ui.treeWidget->topLevelItem(2))
	{
		ui_TowerWireGroup();
	}
	else if (item == ui.treeWidget->topLevelItem(7))
	{
		ui_Wind();
	}
	else if (item == ui.treeWidget->topLevelItem(8))
	{
		ui_SingleWire();
	}
	else if (isChildOfTower(8, item, 0))
	{//ʩ��Լ��
		Constraint_Tips1(item);
	}
	else if (item == ui.treeWidget->topLevelItem(9))
	{
		//ui_Calculate();
	}
	else if (isChildOfTowerwiregroupWireModeling(item))
	{
		CreateGroupInp(item);
	}
}
void InterFace::onTreeitemClicked(QTreeWidgetItem* item)
{
	//�����л�
	Part_Base* part = OnFindPart(item);

	if (part != nullptr)
	{
		if (item->parent()->parent() == ui.treeWidget->topLevelItem(0))
			Show_Part(part);
	}

	if (item->parent() == ui.treeWidget->topLevelItem(1)|| item->parent() == ui.treeWidget->topLevelItem(8))
	{
		Instance* instance = OnFindInstance(item);
		Show_Tower(instance);
	}
	else if (item->parent() == ui.treeWidget->topLevelItem(8))
	{
		Instance* instance = OnFindInstance(item);
		Show_Wire(instance);
	}
}

//��������
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
		T_foot->Get_Data(*t);//ȡ����
		item->setText(0, t->m_Name);//�ӽڵ�����Ϊ�Լ�����������
		t->Item = item;
		HiddeAllPart();

		t->Create_Mesh();
		t->m_id = parent->childCount();//���
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
		spacer->setText(0, QString("��Ӿ�Ե�Ӵ�"));
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
		
		TP_CrossArm.Add_Entity(t);//��ͷ
		m_Renderer->ResetCamera();

		t_crossarms.push_back(T_ca);//��ͷ�Ի���

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
		for (auto& i : T_As->m_ArryLeg)//����
		{
			tw->addPart(TP_leg.Find_Entity(i));
		}
		for (auto& i : T_As->m_ArryBody)//�����
		{
			tw->addPart(TP_body.Find_Entity(i));
		}
		for (auto& i : T_As->m_ArryHead)//��ͷ
		{
			tw->addPart(TP_CrossArm.Find_Entity(i));
		}

		tw->Check();
		HiddeAllTower();

		tw->Item = item;
		item->setText(0, T_As->Get_name());
		tw->m_id = TP.size() + 1;//���

		tw->Show_VTKnode(m_Renderer_2);
		tw->Show_VTKtruss(m_Renderer_2);
		tw->Show_VTKbeam(m_Renderer_2);

		TP.Add_Entity(tw);

		tw->m_BeamActor->VisibilityOn();
		tw->m_TrussActor->VisibilityOn();
		tw->Node_actor->VisibilityOn();
		
		//���н����ż���Tower
		vector<int> idSections;
		for (auto& i : Ms)
		{
			idSections.push_back(i.second->m_id);
		}
		tw->AddNewSection(idSections);

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
		//int ClassSection = 0;//ȡ�øý������͵Ĳ���
		//ClassSection = M_ab->ClassSection;
		//string NameSection = M_ab->name;
		//double ia = M_ab->a;
		//double ib = M_ab->b;
		//int iM = M_ab->ClassMa;

		//���н����ż���Part
		vector<int> idSections;
		for (auto& i : Ms)
		{
			idSections.push_back(i.second->m_id);
		}
		for (int i = 0; i < ui.treeWidget->topLevelItem(0)->child(0)->childCount(); i++)//�¼�һ�����������һ��
		{
			TP_leg.Find_Entity(i + 1)->AddNewSection(idSections);
		}
		for (int i = 0; i < ui.treeWidget->topLevelItem(0)->child(1)->childCount(); i++)//�¼�һ�����������һ��
		{
			TP_body.Find_Entity(i + 1)->AddNewSection(idSections);
		}
		for (int i = 0; i < ui.treeWidget->topLevelItem(0)->child(2)->childCount(); i++)//�¼�һ������ͷ���һ��
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
	//��ʾ���е�Ԫ����
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
	Interphase_spacer* IS = new Interphase_spacer(Arm,this);

	IS->show();//Ҫ����ֻ����show

}

void InterFace::ui_Wire_InterFace(QTreeWidgetItem* item)
{
	TowerWireGroup* towerWireGroup = OnFindGroup(item->parent());
	Wire_InterFace* w = new Wire_InterFace(towerWireGroup,this);
	int ret = w->exec();
	
	if (ret == QDialog::Accepted)
	{
		CreatWire* t = new CreatWire;
		w->Get_Data(*t);//ȡ����
		m_Renderer_3->RemoveAllViewProps();
		t->Create_Mesh();
		towerWireGroup->AddWireToGroup(t);
		towerWireGroup->Show_VTKnode(m_Renderer_3);
		towerWireGroup->Show_VTKbeam(m_Renderer_3);
		towerWireGroup->Show_VTKtruss(m_Renderer_3);
		m_Renderer_3->ResetCamera();
	}

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
	//Create_Constraint* con = new Create_Constraint(this);
	//con->show();
}

void InterFace::SaveFile()
{
	QString str = QFileDialog::getSaveFileName(this, "����", "/", "datafile(*.dat);;textfile(*.txt);;c file(*.cpp);;All file(*.*)");
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
			cout << "�ļ���ʧ�ܣ�\n";
		}
		else
		{
			TP_leg.Save(Stream);//�ж��� �������
			TP_body.Save(Stream);
			TP_CrossArm.Save(Stream);
			//towerPartInsulator.Save(Stream);//��Ե�Ӵ�
			TP.Save(Stream);
			Ms.Save(Stream);//�����������
			//TWG.Save(Stream);
		}
		Qf.close();
	}
}
void InterFace::OpenFile()
{
	QString str = QFileDialog::getOpenFileName(this, "��", "/", "datafile(*.dat);;textfile(*.txt);;c file(*.cpp);;All file(*.*)");
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
			cout << "�ļ���ʧ�ܣ�\n";
		}
		else
		{
			int legSize = TP_leg.size();
			int bodysize = TP_body.size();
			int crossArmSize = TP_CrossArm.size();
			int towerSize = TP.size();
			int twgsize = TWG.size();
			//int MsSize = Ms.size();

			TP_leg.Read(Stream1, legSize);
			TP_body.Read(Stream1, bodysize);
			TP_CrossArm.Read(Stream1, crossArmSize);
			//towerPartInsulator.Read(Stream1);
			TP.Read(Stream1, towerSize);
			//TWG.Read(Stream1, twgsize);
			//Ms.Read(Stream1, MsSize);//�򿪽�������
			for (int i = legSize; i < TP_leg.size(); i++)
			{
				QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(0)->child(0);
				QTreeWidgetItem* item = new QTreeWidgetItem(parent);
				QString str = QString::number(parent->childCount());     //strת�ַ�
				item->setText(0, QString("�����Ȳ���" + str));
				//QString str = TP_leg.Find_Entity(i + 1)->m_Name;//����û�л�����
				//item->setText(0, str);
				TP_leg.Find_Entity(i + 1)->Item = item;
				AddPartFunction(item);

				//TP_leg[i]->Item = item;
			}

			for (int i = bodysize; i < TP_body.size(); i++)
			{
				QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(0)->child(1);
				QTreeWidgetItem* item = new QTreeWidgetItem(parent);
				QString str = QString::number(parent->childCount());     //strת�ַ�
				item->setText(0, QString("��������" + str));
				TP_body.Find_Entity(i + 1)->Item = item;
				AddPartFunction(item);
			}
			for (int i = crossArmSize; i < TP_CrossArm.size(); i++)
			{
				QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(0)->child(2);
				QTreeWidgetItem* item = new QTreeWidgetItem(parent);
				QString str = QString::number(parent->childCount());     //strת�ַ�
				item->setText(0, QString("���ᵣ����" + str));
				TP_CrossArm.Find_Entity(i + 1)->Item = item;
				AddPartFunction(item);
				QTreeWidgetItem* spacer = new QTreeWidgetItem(item);
				spacer->setText(0, QString("��Ӿ�Ե�Ӵ�"));
			}
			for (int i = towerSize; i < TP.size(); i++)
			{
				QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(1);
				QTreeWidgetItem* item = new QTreeWidgetItem(parent);
				QString str = QString::number(parent->childCount());     //strת�ַ�
				item->setText(0, QString("�������" + str));
				TP.Find_Entity(i + 1)->Item = item;
				AddPartFunction(item);
			}
			//for (int i = twgsize; i < TWG.size(); i++)
			//{
			//	QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(2);
			//	QTreeWidgetItem* item = new QTreeWidgetItem(parent);
			//	QString str = QString::number(parent->childCount());     //strת�ַ�
			//	item->setText(0, QString("��������" + str));
			//	//TWG.Find_Entity(i + 1)->Item = item;
			//}

			//for (int i = MsSize; i < Ms.size(); i++)
			//{//Ms.Find_Entity(i + 1)ֱ��д�ǿ�ָ��
			//}
		}
	}
	Qf1.close();
	emit Msg_CreateModel();
}

void InterFace::contextMenuEvent(QContextMenuEvent* event)
{
	// ��ȡ��ǰѡ�е� m_Renderer
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
		//QTreeWidgetItem�Ƿ���������---��������������Լ��޸�
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
			for (auto& i : creatWire)
			{
				if (i.second->Item == *it) return i.second;
			}
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
		//QTreeWidgetItem�Ƿ���������---��������������Լ��޸�
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
		//QTreeWidgetItem�Ƿ���������---��������������Լ��޸�
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
		//QTreeWidgetItem�Ƿ���������---��������������Լ��޸�
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

CreatWire* InterFace::OnFindWire(const QTreeWidgetItem* Item)
{
	QTreeWidgetItemIterator it(ui.treeWidget);
	while (*it)
	{
		if ((*it) == Item)
		{
			for (auto& i : creatWire)
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

void InterFace::HiddeAllWire()
{
	vtkPropCollection* props = m_Renderer_3->GetViewProps(); //iterate through and set each visibility to 0
	props->InitTraversal();
	for (int i = 0; i < props->GetNumberOfItems(); i++)
	{
		props->GetNextProp()->VisibilityOff();

		m_Renderer_3->ResetCamera();
		m_renderWindow->Render();
	}
}

void InterFace::ShowSubstaceActor(Part_Base* Part)
{//����һ�����������һ��actor��Ϊ�˼��㲻������ʱ��ע�ͣ�����ʾ����
	for (auto& i : Part->PartNactor)
	{
		m_Renderer->AddActor(i);
	}

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
	QAction* pAc1 = new QAction("��ѡ");
	QAction* pAc2 = new QAction("��ѡ");
	QAction* pAc3 = new QAction("ȡ��");
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
		//����ָ�뵽part�ĸ������
		QTreeWidgetItem* assign_section = new QTreeWidgetItem(item);
		assign_section->setText(0, QString("�������"));
		//����ָ�뵽part�ĸ��ݳ��ȸ������
		QTreeWidgetItem* SetAllSection = new QTreeWidgetItem(item);
		SetAllSection->setText(0, QString("���ݳ��ȸ������"));
	}
	if (item->parent()== ui.treeWidget->topLevelItem(1))
	{
		QTreeWidgetItem* AddLoadForce = new QTreeWidgetItem(item);//ʩ���غɰ�ť
		QTreeWidgetItem* OutPut = new QTreeWidgetItem(item);//ʩ���غɰ�ť
		QTreeWidgetItem* Inp = new QTreeWidgetItem(item);//ʩ���غɰ�ť
		QTreeWidgetItem* Constraint = new QTreeWidgetItem(item);//ʩ���غɰ�ť
		AddLoadForce->setText(0, QString("ʩ�Ӻ���"));
		OutPut->setText(0, QString("��������ļ�"));
		Inp->setText(0, QString("���ABAQUS�����ļ�"));
		Constraint->setText(0, QString("���Լ��"));
	}


}

void InterFace::Caculate()
{
	Instance_Calculate* ui_c = new Instance_Calculate(this);
	ui_c->show();

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

//���߲���
void InterFace::Add_Select(vtkSmartPointer<vtkActor> pActor)
{
	std::list<Node*> idNodes;
	Get_SelectedNode(idNodes);//�Ѿ�ѡ�еĽڵ�

	auto dataArray = pActor->GetMapper()->GetInput()->GetPointData()->GetArray("Address");//ȡ�õ�ַ����
	auto ptrArray = dynamic_cast<vtkIdTypeArray*> (dataArray);//ת��Ϊ��������
	if (ptrArray != nullptr)
	{//ת���ɹ�
		vtkIdType nPts = ptrArray->GetNumberOfValues();//����ĸ���
		for (vtkIdType k = 0; k < nPts; k++)
		{//��ȡÿ�������ָ��
			Node* pEntity = (Node*)(ptrArray->GetValue(k));//��ÿ��Ԫ�ص�ֵת��Ϊ����ָ��
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
		auto dataArray = i->GetMapper()->GetInput()->GetPointData()->GetArray("Address");//ȡ�õ�ַ����
		auto ptrArray = dynamic_cast<vtkIdTypeArray*> (dataArray);//ת��Ϊ��������
		if (ptrArray != nullptr)
		{//ת���ɹ�
			vtkIdType nPts = ptrArray->GetNumberOfValues();//����ĸ���
			for (vtkIdType k = 0; k < nPts; k++)
			{//��ȡÿ�������ָ��
				Node* pEntity = (Node*)(ptrArray->GetValue(k));//��ÿ��Ԫ�ص�ֵת��Ϊ����ָ��
				Nodes.push_back(pEntity);
			}
		}
	}
}

void InterFace::Get_SelectedNode(std::set<Node*>& Nodes)
{
	for (auto &i : m_NodeSelected)
	{
		auto dataArray = i->GetMapper()->GetInput()->GetPointData()->GetArray("Address");//ȡ�õ�ַ����
		auto ptrArray = dynamic_cast<vtkIdTypeArray*> (dataArray);//ת��Ϊ��������
		if (ptrArray != nullptr)
		{//ת���ɹ�
			vtkIdType nPts = ptrArray->GetNumberOfValues();//����ĸ���
			for (vtkIdType k = 0; k < nPts; k++)
			{//��ȡÿ�������ָ��
				Node* pEntity = (Node*)(ptrArray->GetValue(k));//��ÿ��Ԫ�ص�ֵת��Ϊ����ָ��
				Nodes.insert(pEntity);
			}
		}
	}
}

void InterFace::Point_Inqure()	
{
	UnSelect_Nodes();
	// ����������
	vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
	renderWindowInteractor->SetRenderWindow(m_renderWindow);

	// �����Զ��彻������
	vtkNew<MouseInteractorHighLightActor> style;
	style->m_pInterFace = this;

	// ʹ�õ�ǰѡ�е� m_Renderer
	style->SetDefaultRenderer(m_CurrentRenderer);

	renderWindowInteractor->SetInteractorStyle(style);

	// ���������Ჿ��
	vtkAxesActor* Axes = vtkAxesActor::New();
	vtkOrientationMarkerWidget* widgetAxes = vtkOrientationMarkerWidget::New();
	widgetAxes->SetOrientationMarker(Axes);
	widgetAxes->SetInteractor(renderWindowInteractor);
	widgetAxes->SetEnabled(1);
	widgetAxes->SetInteractive(1);

	// ��ʼ��������������
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
}

void InterFace::Area_Inqure()
{
	UnSelect_Nodes();
	// ����������Ľ�������ʽ
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
	// ��� m_Renderer_2 �����Դ
	Axes->Delete();
	widgetAxes->Delete();
	widgetAxes->EnabledOff();
	widgetAxes->SetInteractor(nullptr);
	widgetAxes->SetOrientationMarker(nullptr);
	widgetAxes->Delete();

	// ���������
	renderWindowInteractor->RemoveAllObservers();
}

bool InterFace::isChildOfTower(int idParent, QTreeWidgetItem* item, int childNumber)
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
	QTreeWidgetItem* towerWireGroup = ui.treeWidget->topLevelItem(2);
	int towerWireGroupChildCount = towerWireGroup->childCount();//ȡ���м���������ѭ��
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
{//����������item
	QTreeWidgetItem* LegPart = ui.treeWidget->topLevelItem(0)->child(0);
	QTreeWidgetItem* BodyPart = ui.treeWidget->topLevelItem(0)->child(1);
	QTreeWidgetItem* ArmPart = ui.treeWidget->topLevelItem(0)->child(2);
	int LegchildCount = LegPart->childCount();//ȡ���м������Ȳ���ѭ��
	for (int i = 0; i < LegchildCount; i++)
	{
		QTreeWidgetItem* childItem = LegPart->child(i);
		if (item == childItem->child(0))
		{
			return true;
		}
	}
	int BodychildCount = BodyPart->childCount();//ȡ���м���������ѭ��
	for (int i = 0; i < BodychildCount; i++)
	{
		QTreeWidgetItem* childItem = BodyPart->child(i);
		if (item == childItem->child(0))
		{
			return true;
		}
	}
	int ArmchildCount = ArmPart->childCount();//ȡ���м�����ͷ����ѭ��
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
	//����������item
	QTreeWidgetItem* LegPart = ui.treeWidget->topLevelItem(0)->child(0);
	QTreeWidgetItem* BodyPart = ui.treeWidget->topLevelItem(0)->child(1);
	QTreeWidgetItem* ArmPart = ui.treeWidget->topLevelItem(0)->child(2);
	int LegchildCount = LegPart->childCount();//ȡ���м������Ȳ���ѭ��
	for (int i = 0; i < LegchildCount; i++)
	{
		QTreeWidgetItem* childItem = LegPart->child(i);
		if (item == childItem->child(1))
		{
			return true;
		}
	}
	int BodychildCount = BodyPart->childCount();//ȡ���м���������ѭ��
	for (int i = 0; i < BodychildCount; i++)
	{
		QTreeWidgetItem* childItem = BodyPart->child(i);
		if (item == childItem->child(1))
		{
			return true;
		}
	}
	int ArmchildCount = ArmPart->childCount();//ȡ���м�����ͷ����ѭ��
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
	int ArmchildCount = ArmPart->childCount();//ȡ���м�����ͷ����ѭ��
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

bool InterFace::isChildOfTowerwiregroupWireModeling(QTreeWidgetItem* item)
{
	QTreeWidgetItem* towerWireGroup = ui.treeWidget->topLevelItem(2);
	int towerWireGroupChildCount = towerWireGroup->childCount();//ȡ���м���������ѭ��
	for (int i = 0; i < towerWireGroupChildCount; i++)
	{
		QTreeWidgetItem* childItem = towerWireGroup->child(i);
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
	// ����������Ľ�������ʽ
	m_MainStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	// ����Ҫ�л���������ʱ��ʹ�ñ���Ľ�������ʽ�������ý�����
	m_renderWindow->GetInteractor()->SetInteractorStyle(m_MainStyle);
	
}


void InterFace::ui_CreatLoads(QTreeWidgetItem* item)
{
	Tower* tower = OnFindTower(item->parent());
	Show_Tower(tower);//��ʾ���ڵ��ʵ��
	Creat_Loads* creat_load = new Creat_Loads(tower, this);
	creat_load->show();
	
}

void InterFace::CreateOutPut(QTreeWidgetItem* item)
{
	Tower* tower = OnFindTower(item->parent());
	tower->CreateOutPut();
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
	switchRenderWindow(0);
	HiddeAllPart();
	if (part->m_BeamActor != nullptr)
	{
		part->Node_actor->VisibilityOn();
		part->m_BeamActor->VisibilityOn();
		part->m_TrussActor->VisibilityOn();
	}
	else
	{ 
		part->Show_VTKnode(m_Renderer);
		part->Show_VTKtruss(m_Renderer);
		part->Show_VTKbeam(m_Renderer);
	}
	////part->m_BeamActor->VisibilityOn();
	//for (auto& i : part->m_BeamActors)
	//{
	//	m_Renderer->AddActor(i);
	//	//cout << "number of actor :" << i << "\n";
	//}
	//������ʱע��
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
	switchRenderWindow(1);
	HiddeAllTower();

	if (instance->m_BeamActor != nullptr)
	{
		instance->Node_actor->VisibilityOn();
		instance->m_BeamActor->VisibilityOn();
		instance->m_TrussActor->VisibilityOn();
	}
	else
	{
		instance->Show_VTKnode(m_Renderer_2);
		instance->Show_VTKtruss(m_Renderer_2);
		instance->Show_VTKbeam(m_Renderer_2);
	}

	for (auto& i : instance->m_LoadActor)
	{
		m_Renderer_2->AddActor(i);
		i->VisibilityOn();
	}
	for (auto& i : instance->m_ConstraintActor)
	{//��ʱ�л�����ʾԼ��
		m_Renderer_2->AddActor(i);
		i->VisibilityOn();
	}
	m_Renderer_2->ResetCamera();
}

void InterFace::Show_Wire(Instance* instance)
{
	if (instance == nullptr) return;
	switchRenderWindow(2);
	HiddeAllWire();
	instance->Node_actor->VisibilityOn();
	instance->m_TrussActor->VisibilityOn();
	
	m_Renderer_3->ResetCamera();
}

void InterFace::Constraint_Tips1(QTreeWidgetItem* item)
{
	Instance* instance = OnFindInstance(item->parent());
	Show_Tower(instance);//��ʾ��Ӧ��ʵ������

	//��ʾѡ���Լ������
	Create_Constraint* con = new Create_Constraint(instance, this);
	con->show();
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

void InterFace::ui_SingleWire()
{
	switchRenderWindow(2);
	TowerWireGroup* towerWireGroup = nullptr;
	Wire_InterFace* w = new Wire_InterFace(towerWireGroup, this);
	int ret = w->exec();
	if (ret == QDialog::Accepted)
	{
		CreatWire* t = new CreatWire;
		QTreeWidgetItem* parent = ui.treeWidget->topLevelItem(8);
		QTreeWidgetItem* item = new QTreeWidgetItem(parent);
		QTreeWidgetItem* Constraint = new QTreeWidgetItem(item);//ʩ���غɰ�ť
		Constraint->setText(0, QString("���Լ��"));
		item->setText(0, "����");
		w->Get_Data(*t);//ȡ����
		t->m_id = creatWire.size() + 1;
		t->Item = item;
		t->Create_Mesh();
		t->Show_VTKnode(m_Renderer_3);
		t->Show_VTKtruss(m_Renderer_3);
		m_Renderer_3->ResetCamera();
		creatWire.Add_Entity(t);
	}
}

InterFace::~InterFace()
{

}