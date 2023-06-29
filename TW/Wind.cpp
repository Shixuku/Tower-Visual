#include "Wind.h"
#include"InterFace.h"
#include"WindList.h"
#include <cmath>
#include <QMessageBox>
#include"RandomWind.h"

Wind::Wind(InterFace* pInterFace, QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);
	//m_pInterFace = dynamic_cast<InterFace*>(parent);
	//Q_ASSERT(m_pInterFace != nullptr);
	m_pInterFace = pInterFace;
	Initialize();
	CreateCombobox();
	connect(ui.rad_t, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(0); });
	connect(ui.rad_f, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(1); });
	connect(ui.rad_stab, &QRadioButton::clicked, this, [=]() {ui.stackedWidget_2->setCurrentIndex(0); });
	connect(ui.rad_ran, &QRadioButton::clicked, this, [=]() {ui.stackedWidget_2->setCurrentIndex(1); });
	void (QComboBox:: * intChanged)(int) = &QComboBox::currentIndexChanged;
	connect(ui.object_com, intChanged, this, &Wind::ShowObject);
	connect(ui.sure_btn_2, &QPushButton::clicked, this, &Wind::ui_Speed);
	connect(ui.count_btn, &QPushButton::clicked, this, &Wind::BtnOk);
}

Wind::~Wind()
{}

void Wind::Initialize()
{
	ui.ice_edi->setText("5");
	ui.wind_edi->setText("10");
	ui.direction_edi->setText("30");

	layout = new QVBoxLayout(ui.widget_5);

	// ���� QVTKOpenGLNativeWidget
	m_VtkWidget = new QVTKOpenGLNativeWidget(this);
	layout->addWidget(m_VtkWidget);

	// ���� VTK ��Ⱦ����
	renderWindow = m_VtkWidget->renderWindow();

	m_Renderer = vtkSmartPointer<vtkRenderer>::New();
	renderWindow->AddRenderer(m_Renderer);

	//�������ϵ
	vtkAxesActor* axesActor = vtkAxesActor::New();
	//��Widget��ʽ,�����½ǵ��ӿ�����ʾ����ϵ���ɽ�����꽻��
	vtkOrientationMarkerWidget* widget = vtkOrientationMarkerWidget::New();
	widget->SetOrientationMarker(axesActor);
	widget->SetInteractor(m_VtkWidget->interactor());
	widget->SetViewport(0, 0, 0.2, 0.2);
	widget->SetEnabled(true);
	widget->InteractiveOn();

	// ���ı�����ɫ
	m_Renderer->SetBackground(1.0, 1.0, 1.0);              // �ײ���ɫֵ

	
}

void Wind::CreateCombobox()
{
	ExampleNum =  m_pInterFace->ui.treeWidget->topLevelItem(1)->childCount();//��������
	
	for (int i = 0; i < ExampleNum; i++)
	{
		ui.object_com->addItem("����ʵ��" + QString::number(i + 1));
		
	}
	
}

void Wind::ShowObject()
{
	int Index = ui.object_com->currentIndex();
	if (Index < ExampleNum)
	{
		id_Part = Index + 1;
		m_Renderer->RemoveAllViewProps();  // ��յ�ǰ��ʾ������actor
		m_pTower = m_pInterFace->TP.Find_Entity(id_Part);
		m_pInterFace->Show_Tower(m_pTower);
		m_pTower->Show_VTKbeam(m_Renderer);
		m_pTower->Show_VTKtruss(m_Renderer);
	}
	m_Renderer->ResetCamera();
	ui.widget_5->update();
}

void Wind::ui_Speed()
{
	if (ui.wind_com->currentIndex() == 0)
	{
		if (w_l == nullptr)
		{
			w_l = new WindList(this);
			w_l->show();
		}
		else
		{
			w_l->show();
		}
	}
	else if (ui.wind_com->currentIndex() == 1)
	{
		if (ran == nullptr)
		{
			ran = new RandomWind(this);
			ran->show();
		}
		else
		{
			ran->show();
		}
	}
	
}

void Wind::ReadData()
{
	QStringList headerLabels;
	headerLabels << "�ڵ���" << "Z(mm)" << "��ѹϵ��";
	ui.tableWidget->setColumnCount(headerLabels.count());
	ui.tableWidget->setHorizontalHeaderLabels(headerLabels);
	ui.tableWidget->verticalHeader()->setVisible(false);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->setRowCount(num);  // Ĭ��N��

	for (int k = 0; k < num; k++)
	{
		const auto& node = m_pTower->m_Nodes[k];
		double uz = CountUz((node.z + 14000) / 1000);

		ui.tableWidget->setItem(k, 0, new QTableWidgetItem(QString::number(node.m_idNode)));
		ui.tableWidget->setItem(k, 1, new QTableWidgetItem(QString::number(node.z)));
		ui.tableWidget->setItem(k, 2, new QTableWidgetItem(QString::number(uz)));
	}
}

void Wind::BtnOk()
{
	if (m_pTower == nullptr)
	{
		QMessageBox::warning(this, "��ʾ", "�봴����������");
		return;
	}

	num = m_pTower->m_Nodes.size();

	bool groundSelected = false;

	QList<QRadioButton*> radioButtons = { ui.rad_A, ui.rad_B, ui.rad_C, ui.rad_D };

	for (QRadioButton* radioButton : radioButtons)
	{
		if (radioButton->isChecked())
		{
			groundSelected = true;
			break;
		}
	}

	if (!groundSelected)
	{
		QMessageBox::warning(this, "��ʾ", "����ѡ�����ֲڳ̶ȣ�");
		return;
	}

	ReadData();
}


double Wind::CountUz(double h)
{
	double uz = 0.0;
	if (ui.rad_A->isChecked())
	{
		uz = 1.284 * pow(h / 10, 0.24);
	}
	else if (ui.rad_B->isChecked())
	{
		uz = 1.000 * pow(h / 10, 0.30);
	}
	else if (ui.rad_C->isChecked())
	{
		uz = 0.544 * pow(h / 10, 0.44);
	}
	else if (ui.rad_D->isChecked())
	{
		uz = 0.262 * pow(h / 10, 0.60);
	}

	return uz;
}
