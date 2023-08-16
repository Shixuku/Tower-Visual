#include "T_Body.h"
#pragma execution_character_set("utf-8")
#include"TowerPart_body.h"
#include<QMessageBox.h>
T_Body::T_Body(InterFace* InterFace, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	Set_headertext();//�����²��Ĳ������
	Set_No1_picture();//��������ͼƬ
	m_InterFace = InterFace;

	connect(ui.btn_ok, &QPushButton::clicked, this, [=]()
		{
			if (ui.tableWidget->rowCount() == 0)
			{
				QMessageBox::information(this, "Tips", "���ȴ���������");
			}
			else this->accept();
		});
	connect(ui.btn_cancle, &QPushButton::clicked, this, &T_Body::reject);
	connect(ui.btn_creat, &QPushButton::clicked, this, [=]() {
		int Tier = ui.line_tier->text().toInt();
		if (Tier == 7)
		{
			Set_body1();
		}
		else if (Tier == 12)
		{
			Set_body2();
		}
		else
		{
			Set_table(Tier);
		}
		});

	connect(ui.combo_b, SIGNAL(currentIndexChanged(int)), this, SLOT(Change_Picture_body()));
	connect(ui.combo_g, SIGNAL(currentIndexChanged(int)), this, SLOT(Change_Picture_gem()));

	ui.tabWidget->setCurrentIndex(0);//����tab��һҳ������ʾ

	int T_bodys = m_InterFace->TP_body.size() + 1;//��������
	QString str("������-");
	str += QString::number(T_bodys);     //strת�ַ�
	ui.part_name->setText(str);//���ó�ʼֵ

}

T_Body::~T_Body()
{}

void T_Body::Set_headertext()
{
	QStringList headertext;//��ͷ
	headertext << "����" << "���(m)" << "��������" << "��������" << "�ϲ�����"<<"�в�����";
	ui.tableWidget->setColumnCount(headertext.count());
	ui.tableWidget->setHorizontalHeaderLabels(headertext);
	ui.tableWidget->verticalHeader()->setVisible(false);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
}

void T_Body::Set_No1_picture()
{
	QImage* img = new QImage;
	img->load("./tuiup1.png");
	ui.label_b->resize(img->width(), img->height());
	ui.label_b->setPixmap(QPixmap::fromImage(*img));

	QImage* img_gem = new QImage;
	img_gem->load("./gemian1.png");
	ui.label_g->resize(img_gem->width(), img_gem->height());
	ui.label_g->setPixmap(QPixmap::fromImage(*img_gem));

}

void T_Body::Set_body1()
{
	ui.line_L0->setText("15.160");
	ui.line_Ln->setText("4.600");
	ui.line_z0->setText("0");
	ui.tableWidget->setRowCount(7);//7��
	QStringList H, TypeFront, TypeSide,U_septum, M_septum;
	H << "6.000" << "11.000"<<"9.500" << "8.000" << "6.500" << "5.000" << "2.000";//���
	TypeFront << "1" << "9" << "9" << "6" << "9" << "9" << "10";//��������
	TypeSide << "1" << "9" << "9" << "6" << "9" << "9" << "10";//��������
	U_septum << "0" << "0" << "0" << "0" << "0" << "0" << "5";//�ϲ�����
	M_septum << "0" << "0" << "0" << "5" << "0" << "0" << "0";//�в�����

	for (int i = 0; i < 7; i++)
	{
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));//����
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(H[i]));//��߳�ʼ��
		ui.tableWidget->setItem(i, 2, new QTableWidgetItem(TypeFront[i]));//��������
		ui.tableWidget->setItem(i, 3, new QTableWidgetItem(TypeSide[i]));//��������
		ui.tableWidget->setItem(i, 4, new QTableWidgetItem(U_septum[i]));//�ϲ�����
		ui.tableWidget->setItem(i, 5, new QTableWidgetItem(M_septum[i]));//�в�����
		for (int j = 0; j < 6; j++)//��ͷ;
		{
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//����������þ���
		}
	}
}

void T_Body::Set_body2()
{
	ui.line_L0->setText("4.600");
	ui.line_Ln->setText("2.300");
	ui.line_z0->setText("48.000");
	ui.tableWidget->setRowCount(12);//12��
	QStringList H, TypeFront, TypeSide, U_septum, M_septum;
	H << "4.000" << "2.500" << "3.500" << "3.500" << "3.000" << "2.500" << "2.500"
		<< "2.500" << "2.500" << "2.500" << "2.000" << "2.000";//���
	TypeFront << "6" << "7" << "6" << "6" << "6" << "7" << "6" << "6" << "6" << "6" << "7" << "6";//��������
	TypeSide << "6" << "8" << "6" << "6" << "6" << "8" << "6" << "6" << "6" << "6" << "8" << "6";//��������
	U_septum << "5" << "6" << "0" << "0" << "5" << "6" << "0" << "0" << "0" << "5" << "3" << "2";//�ϲ�����
	M_septum << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";//�в�����

	for (int i = 0; i < 12; i++)
	{
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));//����
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(H[i]));//��߳�ʼ��
		ui.tableWidget->setItem(i, 2, new QTableWidgetItem(TypeFront[i]));//��������
		ui.tableWidget->setItem(i, 3, new QTableWidgetItem(TypeSide[i]));//��������
		ui.tableWidget->setItem(i, 4, new QTableWidgetItem(U_septum[i]));//�ϲ�����
		ui.tableWidget->setItem(i, 5, new QTableWidgetItem(M_septum[i]));//�в�����
		for (int j = 0; j < 6; j++)//��ͷ;
		{
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//����������þ���
		}
	}


}

void T_Body::Set_table(int tier)
{
	ui.tableWidget->setRowCount(tier);
	for (int i = 0; i < tier; i++)
	{
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));//����
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(2)));//��߳�ʼ��
		ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(6)));//��������
		ui.tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(6)));//��������
		ui.tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(0)));//�ϲ�����
		ui.tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(0)));//�в�����
		for (int j = 0; j < 6; j++)//��ͷ;
		{
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//����������þ���
		}
	}

}

void T_Body::Get_Data(TowerPart_body* body)
{
	body->m_L0 = ui.line_L0->text().toDouble();//�ײ����
	body->m_Z0 = ui.line_z0->text().toDouble();//�ײ�z����
	body->m_Ln = ui.line_Ln->text().toDouble();//�������
	body->m_Tier = ui.line_tier->text().toInt();//�ܲ���

	int irow = ui.tableWidget->rowCount();//������ڱ������

	for (int i = 0; i < irow; i++)
	{
		TowerData_Layer layer;
		layer.m_h = ui.tableWidget->item(i, 1)->text().toDouble();//���;
		layer.m_TypeFront = ui.tableWidget->item(i, 2)->text().toInt();//��������;
		layer.m_TypeSide = ui.tableWidget->item(i, 3)->text().toInt();//��������
		layer.m_TypeSeptumUp = ui.tableWidget->item(i, 4)->text().toInt();//�ϲ���������
		layer.m_TypeSeptumMiddle = ui.tableWidget->item(i, 5)->text().toInt();//�ϲ���������
		body->m_layer.push_back(layer);
	}
	body->m_Name = ui.part_name->text();
}

QString T_Body::Get_line_Zn(int tier)
{
	double zn = 0;
	for (int i = 0; i < tier; i++)
	{
		zn+= ui.tableWidget->item(i, 1)->text().toDouble();//���;
	}
	QString line_zn = QString::number(zn);
	return line_zn;
}

void T_Body::Change_Picture_gem()
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

void T_Body::Change_Picture_body()
{
	int Index = ui.combo_b->currentIndex();
	QImage* img = new QImage;
	switch (Index)
	{
	case 0:
		img->load("./tuiup1.png");
		break;
	case 1:
		img->load("./tuiup2.png");
		break;
	case 2:
		img->load("./tuiup3.png");
		break;
	case 3:
		img->load("./shen1.png");
		break;
	case 4:
		img->load("./shen2.png");
		break;
	case 5:
		img->load("./shen3.png");
		break;
	case 6:
		img->load("./shen4.png");
		break;
	case 7:
		img->load("./shen5.png");
		break;
	case 8:
		img->load("./shen6.png");
		break;
	case 9:
		img->load("./shen7.png");
		break;
	}
	ui.label_b->resize(img->width(), img->height());
	ui.label_b->setPixmap(QPixmap::fromImage(*img));
}
