#include "UI_Calculate.h"
#include"TowerWireGroup.h"

Instance_Calculate::Instance_Calculate(InterFace* InterFace, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_InterFace = InterFace;
	Set_headertext();
	connect(ui.btn_ok, &QPushButton::clicked, this, &Instance_Calculate::on_btk_ok_clicked);
	connect(ui.btn_show, &QPushButton::clicked, this, &Instance_Calculate::visual);

}

void Instance_Calculate::Set_headertext()
{
	QStringList headertext;//��ͷ
	headertext << "����" << "�����ļ�" << "˵��";
	ui.tableWidget->setColumnCount(headertext.count());
	ui.tableWidget->setHorizontalHeaderLabels(headertext);
	ui.tableWidget->verticalHeader()->setVisible(false);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

	int T_tower = m_InterFace->TP.size();//��ʵ������
	int T_wire = m_InterFace->TWG.size();//��ʵ������

	ui.tableWidget->setRowCount(T_tower+T_wire);//��������

	QVector<Tower*> ve_tower;//!
	m_InterFace->TP.Get_Array(ve_tower, true);//!

	QVector<TowerWireGroup*> ve_wire;//!
	m_InterFace->TWG.Get_Array(ve_wire, true);//!

	for (int i = 0; i < T_tower; i++)
	{
		Tower* tower = ve_tower[i];//!
		list_Instance.push_back(tower);

		//��������
		QString name = tower->m_name;
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem("InstanceTower"));
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(name));
		for (int j = 0; j < 2; j++)//��ͷ;
		{
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}

	for (int i = 0; i < T_wire; i++)
	{
		TowerWireGroup* wire = ve_wire[i];//!
		list_Instance.push_back(wire);

		//��������
		QString name = wire->m_name;
		ui.tableWidget->setItem(T_tower + i, 0, new QTableWidgetItem("InstanceWire"));
		ui.tableWidget->setItem(T_tower + i, 1, new QTableWidgetItem(name));
		for (int j = 0; j < 2; j++)//��ͷ;
		{
			ui.tableWidget->item(T_tower + i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}
}

Instance_Calculate::~Instance_Calculate()
{}

void Instance_Calculate::on_btk_ok_clicked()
{
	if (!towercaculate)
	{
		towercaculate = new TowerCaculate(this);
	}
	int ret = towercaculate->exec();
	if (ret == QDialog::Accepted)
	{
		int index = ui.tableWidget->currentRow();
		Instance* ins = nullptr;
		ins = list_Instance[index];
		if (!ins) return;
		QString str = ins->m_name;
		if (str != nullptr)
		{
			qDebug() << str;
			if (ins->s) delete ins->s;//�ͷ�֮ǰ�Ķ���
			ins->s = GetStructure();//����structure�����
			ins->s->Input(str);
		}
		else
		{
			cout << "str = nullptr!" << "\n";
			return;
		}
		QString FilePath("./");//�������ļ�·��
		//int idNode = towercaculate->IdNode;//Ҫ����ĵ�ı��
		ins->s->set_outInfo(FilePath, towercaculate->IdNode);//��һ����Ҫ�����·���ĵڶ�������
		//���ò���...
		if (towercaculate->dynamic)
		{
			ins->s->setDiffEqSolver(S_InterFace::DYNAMIC_IMPLICIT);
		}
		else
		{
			ins->s->setDiffEqSolver(S_InterFace::STATICS);
		}
		ins->s->setElementType(S_InterFace::Beam_CR);
		ins->s->set_Sparse(true);
		if (ins->s->getMethod() == S_InterFace::STATICS)
		{
			int Step = towercaculate->static_stepnum;
			ins->s->setStep(Step);
		}
		else
		{//��������
			ins->s->set_beta(0.25);
			ins->s->set_gama(0.5);
			ins->s->setTimeStep(towercaculate->dynamic_step);
			ins->s->setStep(towercaculate->dynamic_stepnum);
			//ins->s->set_CalculateEq(true);
			ins->s->set_Galloping(towercaculate->galloping);
			ins->s->set_Windage(towercaculate->windage);
		}
		ins->s->set_stol(towercaculate->stol);
		ins->s->setMaxIterations(towercaculate->MaxIterations);
		clock_t start, end;//��ʱ
		start = clock();
		ins->s->execute();//����
		end = clock();
		int Totalit = ins->s->getTotalIterations();
		qDebug() << "�ܵ��������� " << Totalit;
		double rtime = (end - start);
		qDebug() << "�����ܺ�ʱ�� " << rtime << " ms";
	}

}

void Instance_Calculate::visual()
{
	if (!display)
	{
		display = new resultVisualize(m_InterFace);
	}
	display->show();
	int index = ui.tableWidget->currentRow();
	Instance* ins = nullptr;
	ins = list_Instance[index];
	if (!ins) return;
	vector<Node_Base*> ptr_nodes = ins->s->GetNodes();
	
	display->addData(ptr_nodes, ins);
}
