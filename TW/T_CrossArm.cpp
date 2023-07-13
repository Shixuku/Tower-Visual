#include "T_CrossArm.h"
#pragma execution_character_set("utf-8")
#include"TowerPart_CrossArm.h"
#include"TowerData_LayerArm.h"
#include"QMessageBox.h"
T_CrossArm::T_CrossArm(InterFace* interFace, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_InterFace = interFace;

	Set_headertext();
	Set_combobox();
	Initialize();

	connect(ui.btn_ok, &QPushButton::clicked, this, &T_CrossArm::accept);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &T_CrossArm::reject);
	connect(ui.rdb_up, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(0);});
	connect(ui.rdb_buttom, &QRadioButton::clicked, this, [=]() {ui.stackedWidget->setCurrentIndex(1); });
	connect(ui.btn_creat, &QPushButton::clicked, this, [=]() {
		int count = ui.line_count->text().toInt();
		if (count == 4)
		{
			Set_zhijia();
		}
		else if (count == 6)
		{
			Set_hengdan();
		}
		else
		{ 
			Set_table(count);
		}
		});

	void (QComboBox::*IndexChanged)(int) = &QComboBox::currentIndexChanged;
	connect(ui.combo_body, IndexChanged, this, &T_CrossArm::Change_combo_Tier);

	connect(ui.combo_BU, SIGNAL(currentIndexChanged(int)), this, SLOT(Change_Picture_BU()));
	connect(ui.combo_S, SIGNAL(currentIndexChanged(int)), this, SLOT(Change_Picture_S()));
	connect(ui.combo_g, SIGNAL(currentIndexChanged(int)), this, SLOT(Change_Picture_gemian()));


}

T_CrossArm::~T_CrossArm()
{}

void T_CrossArm::Set_headertext()
{
	QStringList headertext;//��ͷ
	headertext << "�κ�" << "�γ�(mm)" << "��������" << "��������" << "��������" << "�Ҹ�������";
	ui.tableWidget->setColumnCount(headertext.count());
	ui.tableWidget->setHorizontalHeaderLabels(headertext);
	ui.tableWidget->verticalHeader()->setVisible(false);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

}

void T_CrossArm::Set_table(int count)
{
	ui.tableWidget->setRowCount(count);
	for (int i = 0; i < count; i++)
	{
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));//�κ�
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(1.716)));//�γ�
		ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(2)));//��������
		ui.tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(2)));//��������
		ui.tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(3)));//��������
		ui.tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(0)));//��������
		//����������þ���
		for (int j = 0; j < 6; j++)//��ͷ;
		{
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}
}

void T_CrossArm::Set_zhijia()
{
	ui.tableWidget->setRowCount(4);
	for (int i = 0; i < 4; i++)
	{
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));//�κ�
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(1.716)));//�γ�
		ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(2)));//��������
		ui.tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(2)));//��������
		ui.tableWidget->setItem(3, 3, new QTableWidgetItem(QString::number(4)));//��������
		ui.tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(3)));//��������
		ui.tableWidget->setItem(3, 4, new QTableWidgetItem(QString::number(0)));//��������
		ui.tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(0)));//��������
		//����������þ���
		for (int j = 0; j < 6; j++)//��ͷ;
		{
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}
}

void T_CrossArm::Set_hengdan()
{
	ui.tableWidget->setRowCount(6);
	for (int i = 0; i < 6; i++)
	{
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));//�κ�
		if (i < 2)
		{
			ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(2.778)));//�γ�
			ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(1)));//��������
			ui.tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(1)));//��������
			ui.tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(1)));//��������
			ui.tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(0)));//��������
		}
		else if(i<5)
		{
			ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(1.389)));//�γ�
			ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(2)));//��������
			ui.tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(2)));//��������
			ui.tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(2)));//��������
			ui.tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(0)));//��������
			ui.tableWidget->setItem(1, 5, new QTableWidgetItem(QString::number(2)));//��������
			ui.tableWidget->item(1, 5)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
		else
		{
			ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(1.389)));//�γ�
			ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(3)));//��������
			ui.tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(4)));//��������
			ui.tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(2)));//��������
			ui.tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(0)));//��������
		}
		//����������þ���
		for (int j = 0; j < 6; j++)//��ͷ;
		{
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			
		}
	}
}

void T_CrossArm::Get_Data(TowerPart_CrossArm&tp_ca)
{
	int ret = ui.stackedWidget->currentIndex();//0--֧�� 1�ᵣ
	//���۶˲���
	if (ret == 0)
	{
		tp_ca.m_c1W = ui.line_c1W0->text().toDouble();//֧��
		tp_ca.m_Type = 1;
	}
	else if (ret == 1)
	{
		tp_ca.m_c2Wb= ui.line_c2W0->text().toDouble();//�ᵣ
		tp_ca.m_c2Wup= ui.line_c2Wn->text().toDouble();//�ᵣ
		tp_ca.m_c2H= ui.line_c2H->text().toDouble();//�ᵣ
		tp_ca.m_Type = 3;
	}
	tp_ca.m_count = ui.line_count->text().toInt();//����
	tp_ca.m_position = ui.combo_bodypos->currentIndex();//λ��

	int irow = ui.tableWidget->rowCount();//������ڱ������
	for (int i = 0; i < irow; i++)
	{
		TowerData_LayerArm layer;
		layer.l = ui.tableWidget->item(i, 1)->text().toDouble();//�γ�;
		layer.m_TypeButtom = ui.tableWidget->item(i, 2)->text().toInt();//��������;
		layer.m_TypeUp = ui.tableWidget->item(i, 3)->text().toInt();//��������
		layer.m_TypeSide = ui.tableWidget->item(i, 4)->text().toInt();//��������
		layer.m_TypeSeptum = ui.tableWidget->item(i, 5)->text().toInt();//��������
		tp_ca.m_layerArm.push_back(layer);
	}
	tp_ca.m_name = ui.part_name->text();//ȡ����

	int duan= ui.combo_body->currentIndex() + 1;//�ĸ������
	int ceng = ui.combo_Tier->currentIndex() + 1;

	TowerPart_body* body = m_InterFace->TP_body.Find_Entity(duan);

	tp_ca.m_bodyButtomL = body->Get_LayerL(ceng-1);
	tp_ca.m_bodyUpL = body->Get_LayerL(ceng);

	tp_ca.m_bodyButtomH = body->Get_LayerH(ceng-1);
	tp_ca.m_bodyUpH = body->Get_LayerH(ceng);
	
}

void T_CrossArm::Set_combobox()
{
	//�����
	int a= m_InterFace->ui.treeWidget->topLevelItem(0)->child(1)->childCount();//���������
	for (int i = 0; i < a; i++)
	{
		QString name = m_InterFace->ui.treeWidget->topLevelItem(0)->child(1)->child(i)->text(0);
		ui.combo_body->addItem(name);
	}
	//����
	int m_body = 1;//Ĭ�ϵ�һ������εĲ���
	TowerPart_body* body = m_InterFace->TP_body.Find_Entity(m_body);
	int tier = body->m_Tier;
	for (int i = 0; i < tier; i++)
	{
		ui.combo_Tier->addItem("��" + QString::number(i + 1) + "��");
	}
}

void T_CrossArm::Initialize()
{
	ui.tabWidget->setCurrentIndex(0);//����Ĭ��page
	ui.stackedWidget->setCurrentIndex(0);//����Ĭ��page
	//����ȱʡֵ

	//֧�ܲ���
	ui.line_c1W0->setText("1.3");
	//�ᵣ����
	ui.line_c2H->setText("0.5"); ui.line_c2W0->setText("1.292"); ui.line_c2Wn->setText("1.287");

	ui.combo_bodypos->setCurrentIndex(2);//����combobox��ʼindex
	int T_bodys = m_InterFace->ui.treeWidget->topLevelItem(0)->child(2)->childCount() + 1;//��������
	QString str("��ͷ����-");
	str += QString::number(T_bodys);     //strת�ַ�
	ui.part_name->setText(str);//���ó�ʼֵ
	//BU����ͼ
	QImage* img_BU = new QImage;
	img_BU->load("./headBU1.png");
	ui.label_BU->resize(img_BU->width(), img_BU->height());
	ui.label_BU->setPixmap(QPixmap::fromImage(*img_BU));
	//S����ͼ
	QImage* img_S = new QImage;
	img_S->load("./headS1.png");
	ui.label_S->resize(img_S->width(), img_S->height());
	ui.label_S->setPixmap(QPixmap::fromImage(*img_S));
	//��������ͼ
	QImage* img_g = new QImage;
	img_g->load("./gemian1.png");
	ui.label_g->resize(img_g->width(), img_g->height());
	ui.label_g->setPixmap(QPixmap::fromImage(*img_g));
}

void T_CrossArm::Change_Picture_BU()
{
	int Index = ui.combo_BU->currentIndex();
	QImage* img = new QImage;
	switch (Index)
	{
	case 0:
		img->load("./headBU1.png");
		break;
	case 1:
		img->load("./headBU2.png");
		break;
	case 2:
		img->load("./headBU3.png");
		break;
	case 3:
		img->load("./headBU4.png");
		break;
	}
	ui.label_BU->resize(img->width(), img->height());
	ui.label_BU->setPixmap(QPixmap::fromImage(*img));
}

void T_CrossArm::Change_Picture_S()
{
	int Index = ui.combo_S->currentIndex();
	QImage* img = new QImage;
	switch (Index)
	{
	case 0:
		img->load("./headS1.png");
		break;
	case 1:
		img->load("./headS2.png");
		break;
	case 2:
		img->load("./headS3.png");
		break;

	}
	ui.label_S->resize(img->width(), img->height());
	ui.label_S->setPixmap(QPixmap::fromImage(*img));
}

void T_CrossArm::Change_Picture_gemian()
{
	int Index = ui.combo_g->currentIndex();
	QImage* img = new QImage;
	switch (Index)
	{
	case 0:
		img->load("./gemian1.png");
		break;
	case 1:
		img->load("./gemian2.png");
		break;
	case 2:
		img->load("./gemian3.png");
		break;
	case 3:
		img->load("./gemian4.png");
		break;
	case 4:
		img->load("./gemian5.png");
		break;
	}
	ui.label_g->resize(img->width(), img->height());
	ui.label_g->setPixmap(QPixmap::fromImage(*img));
}

void T_CrossArm::Change_combo_Tier()
{
	ui.combo_Tier->clear();
	int m_body = ui.combo_body->currentIndex() + 1;//����α��
	TowerPart_body* body = m_InterFace->TP_body.Find_Entity(m_body);
	int tier = body->m_Tier;
	for (int i = 0; i < tier; i++)
	{
		ui.combo_Tier->addItem("��" + QString::number(i + 1) + "��");
	}

}
