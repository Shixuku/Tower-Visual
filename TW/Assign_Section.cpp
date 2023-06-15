#include "Assign_Section.h"
#pragma execution_character_set("utf-8")
#include"InterFace.h"
#include<QMessageBox.h>
#include<QTextCodec>
#include<QStringLiteral>
Assign_Section::Assign_Section(InterFace* InterFace, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);


	m_pInterFace = InterFace;
	
	Q_ASSERT(m_pInterFace != nullptr);

	Create_combobox();
	ma_ab2 = new Set_Section;
	table_Section_Lists();//���ý�����
	Add_created_section();//����Ѿ������Ľ���

	//connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ShowComboVTK()));
	void (QComboBox:: * intChanged)(int) = &QComboBox::currentIndexChanged;
	connect(ui.comboBox, intChanged, this, &Assign_Section::ShowComboVTK);

	linesactor = vtkSmartPointer<vtkActor>::New();

	//��ʾ��������Ի���
	connect(ui.btn_add, &QPushButton::clicked, this, [=](){ma_ab2->show();});
	//��ӽ���
	connect(ma_ab2, SIGNAL(SendData(QStringList&)), this, SLOT(GetData(QStringList&)));
	//ɾ������ӵĽ���
	connect(ui.btn_delete, &QPushButton::clicked, this, &Assign_Section::table_Section_Lists_delete);

	//�������������ʾ��������ԭ���Ļ������½�һ��actor��
	//connect(ui.Group_Lists, &QTableWidget::itemClicked, this, &Assign_Section::highlight_actor);

	//ָ�ɽ���
	//connect(ui.btn_assign, &QPushButton::clicked, this, &Assign_Section::assigh_section);

	//�����ɰ�ť����ɾ��������actor
	connect(ui.btn_finish, &QPushButton::clicked, this, [=]()
		{
			m_pInterFace->m_Renderer->RemoveActor(linesactor);
			int size = m_pInterFace->TP_leg.Find_Entity(id_Part)->m_Elements_beams.size();
			for (int i = 0; i < size; i++)
			{
				cout << "���ķ���" << m_pInterFace->TP_leg.Find_Entity(id_Part)->m_Elements_beams[i].direction[0] << "  "
				<< m_pInterFace->TP_leg.Find_Entity(id_Part)->m_Elements_beams[i].direction[1] << "  "
				<< m_pInterFace->TP_leg.Find_Entity(id_Part)->m_Elements_beams[i].direction[2] << "\n";
			}	
			this->reject();
		});

}

Assign_Section::~Assign_Section()
{

}

void Assign_Section::ShowComboVTK()
{
	
	T_legs = m_pInterFace->ui.treeWidget->topLevelItem(0)->childCount();//��������
	T_bodys = m_pInterFace->ui.treeWidget->topLevelItem(1)->childCount();//��������
	T_heads = m_pInterFace->ui.treeWidget->topLevelItem(2)->childCount();//��ͷ����
	int Index = ui.comboBox->currentIndex();
	if (Index < T_legs)
	{
		id_Part = Index + 1;
		//return m_pInterFace->TP_leg.Find_Entity(id_Part);
		m_pInterFace->Show_Part(m_pInterFace->TP_leg.Find_Entity(id_Part));
		table_Group_Lists(m_pInterFace->TP_leg.Find_Entity(id_Part));
		//�������������ʾ��������ԭ���Ļ������½�һ��actor��
		connect(ui.Group_Lists, &QTableWidget::itemClicked, this, [=]()
			{highlight_actor(m_pInterFace->TP_leg.Find_Entity(id_Part));
		m_pInterFace->m_renderWindow->Render(); });

		//ָ�ɽ���
		connect(ui.btn_assign, &QPushButton::clicked, this, [=]() 
			{assigh_section(m_pInterFace->TP_leg.Find_Entity(id_Part)); });

	}	
	else if (Index < T_legs+ T_bodys)
	{
		id_Part = Index + 1- T_legs;
		m_pInterFace->Show_Part(m_pInterFace->TP_body.Find_Entity(id_Part));
		table_Group_Lists(m_pInterFace->TP_body.Find_Entity(id_Part));
		connect(ui.Group_Lists, &QTableWidget::itemClicked, this, [=]() 
			{highlight_actor(m_pInterFace->TP_body.Find_Entity(id_Part)); });
		//ָ�ɽ���
		connect(ui.btn_assign, &QPushButton::clicked, this, [=]()
			{assigh_section(m_pInterFace->TP_body.Find_Entity(id_Part)); });
	}
	else if (Index < T_legs + T_bodys+ T_heads)
	{
		id_Part = Index + 1 - T_legs- T_bodys;
		m_pInterFace->Show_Part(m_pInterFace->TP_CrossArm.Find_Entity(id_Part));
		connect(ui.Group_Lists, &QTableWidget::itemClicked, this, [=]() 
			{highlight_actor(m_pInterFace->TP_CrossArm.Find_Entity(id_Part)); });
		//ָ�ɽ���
		connect(ui.btn_assign, &QPushButton::clicked, this, [=]()
			{assigh_section(m_pInterFace->TP_CrossArm.Find_Entity(id_Part)); });
	}


}

//���ý����б�
void Assign_Section::table_Section_Lists()
{
	headertext << "����" << "��״" << "a" << "b" << "���ϱ��" << "���";
	ui.Section_Lists->setColumnCount(headertext.count());
	ui.Section_Lists->setRowCount(0);//����Ĭ��0��
	ui.Section_Lists->setHorizontalHeaderLabels(headertext);
	ui.Section_Lists->verticalHeader()->setVisible(false);
	ui.Section_Lists->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

//�������õĽ������ݷ�������
void Assign_Section::Add_created_section()
{
	for (int j = 0; j < m_pInterFace->MS.size(); j++)
	{
		int irow = ui.Section_Lists->rowCount();
		ui.Section_Lists->setRowCount(irow + 1);

		QString QName = QString::fromStdString(m_pInterFace->MS[j].Name);
		ui.Section_Lists->setItem(irow, 0, new QTableWidgetItem(QName));
		ui.Section_Lists->item(irow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString s;
		if (m_pInterFace->MS[j].ClassSe == 0)
		{
			s = "L��";
		}
		else
		{
			s = "Բ��";
		}
		ui.Section_Lists->setItem(irow, 1, new QTableWidgetItem(s));
		ui.Section_Lists->item(irow, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString ia = QString::number(m_pInterFace->MS[j].a);
		ui.Section_Lists->setItem(irow, 2, new QTableWidgetItem(ia));
		ui.Section_Lists->item(irow, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString ib = QString::number(m_pInterFace->MS[j].b);
		ui.Section_Lists->setItem(irow, 3, new QTableWidgetItem(ib));
		ui.Section_Lists->item(irow, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString iM = QString::number(m_pInterFace->MS[j].ClassM);
		ui.Section_Lists->setItem(irow, 4, new QTableWidgetItem(iM));
		ui.Section_Lists->item(irow, 4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString id = QString::number(m_pInterFace->MS[j].m_id);
		ui.Section_Lists->setItem(irow, 5, new QTableWidgetItem(id));
		ui.Section_Lists->item(irow, 5)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	}
}

void Assign_Section::Create_combobox()
{
	T_legs = m_pInterFace->ui.treeWidget->topLevelItem(0)->child(0)->childCount();//��������
	T_bodys = m_pInterFace->ui.treeWidget->topLevelItem(0)->child(1)->childCount();//��������
	T_heads = m_pInterFace->ui.treeWidget->topLevelItem(0)->child(2)->childCount();//��ͷ����

	for (int i = 0; i < T_legs; i++)
	{
		ui.comboBox->addItem("���Ȳ���" + QString::number(i + 1));
	}
	for (int i = 0; i < T_bodys; i++)
	{
		ui.comboBox->addItem("������" + QString::number(i + 1));
	}
	for (int i = 0; i < T_heads; i++)
	{
		ui.comboBox->addItem("��ͷ����" + QString::number(i + 1));
	}
}

//���õ�Ԫ���б�
void Assign_Section::table_Group_Lists(Part_Base* Part)
{
	QStringList headertext;
	int BeamSize= Part->id_BeamSection;
	int TrussSize = Part->id_TrussSection;
	int rowcount = BeamSize+ TrussSize;
	headertext << "�˼����б�";
	ui.Group_Lists->setColumnCount(headertext.count());
	ui.Group_Lists->setRowCount(rowcount);
	ui.Group_Lists->setHorizontalHeaderLabels(headertext);
	ui.Group_Lists->verticalHeader()->setVisible(false);
	ui.Group_Lists->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	for (int i = 0; i < rowcount; i++)
	{
		ui.Group_Lists->setItem(i, 0, new QTableWidgetItem("�� "+ QString::number(i + 1) + " ��"));
		ui.Group_Lists->item(i, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	}

	//֧�ֶ�ѡģʽ������Ctrl��Shift�� Ctrl+A�����������»�����ѡ
	ui.Group_Lists->setSelectionMode(QAbstractItemView::ExtendedSelection);
	//���õ���ѡ��
	ui.Group_Lists->setSelectionBehavior(QAbstractItemView::SelectRows);
}

//���ܡ�����ӽ���
void Assign_Section::GetData(QStringList& sInfo)
{
	//����У����ҽ���Ϣ�Ž�ȥ
	int irow = ui.Section_Lists->rowCount();
	ui.Section_Lists->setRowCount(irow + 1);

	for (int i = 0; i < headertext.count(); i++)
	{
		QString str= sInfo[i];

		ui.Section_Lists->setItem(irow, i, new QTableWidgetItem(str));
		ui.Section_Lists->item(irow, i)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		qDebug() << str;
	}

}

//���ܡ���ɾ������
void Assign_Section::table_Section_Lists_delete()
{
	int section_rowIndex = ui.Section_Lists->currentRow();

	//ȷ���Ƿ�ɾ��
	QMessageBox::StandardButton btn;
	btn = QMessageBox::question(this, "��ʾ", "ȷ��Ҫɾ���� " + QString::number(section_rowIndex + 1) + " �����������",
		QMessageBox::Yes | QMessageBox::No);
	if (btn == QMessageBox::Yes) 
	{
		if (section_rowIndex != -1)
		{
			ui.Section_Lists->removeRow(section_rowIndex);
		}
	}
}

//���ܡ���������Ԫ��
void Assign_Section::highlight_actor(Part_Base* Part)
{
	list<Element_Beam*>B_Elements; //����
	list<Element_Truss*>T_Elements; //����
	int group_rowIndex = ui.Group_Lists->currentRow()+1;

	for (auto& i : Part->m_Elements_beams)
	{
		if (i.sectionID == group_rowIndex)
		{
			B_Elements.push_back(&i);
		}
		
	}
	for (auto& i : Part->m_Elements_Trusses)
	{
		if (i.sectionID == group_rowIndex- Part->id_BeamSection)
		{
			T_Elements.push_back(&i);
		}

	}
	vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkPoints> linepts = vtkSmartPointer<vtkPoints>::New();
	vector<Node> m_Nodes = Part->m_Nodes;
	if (B_Elements.size() != 0)
	{
		for (auto& i : B_Elements)
		{
			int ip1 = i->m_idNode[0] - 1;
			int ip2 = i->m_idNode[1] - 1;

			double x = m_Nodes[ip1].x;
			double y = m_Nodes[ip1].y;
			double z = m_Nodes[ip1].z;

			double x2 = m_Nodes[ip2].x;
			double y2 = m_Nodes[ip2].y;
			double z2 = m_Nodes[ip2].z;

			vtkIdType pid[2];
			pid[0] = linepts->InsertNextPoint(x, y, z);
			pid[1] = linepts->InsertNextPoint(x2, y2, z2);

			vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
			line->GetPointIds()->SetId(0, pid[0]);
			line->GetPointIds()->SetId(1, pid[1]);
			lines->InsertNextCell(line);
		}
	}
	else 
	{
		for (auto& i : T_Elements)
		{
			int ip1 = i->m_idNode[0] - 1;
			int ip2 = i->m_idNode[1] - 1;

			double x = m_Nodes[ip1].x;
			double y = m_Nodes[ip1].y;
			double z = m_Nodes[ip1].z;

			double x2 = m_Nodes[ip2].x;
			double y2 = m_Nodes[ip2].y;
			double z2 = m_Nodes[ip2].z;

			vtkIdType pid[2];
			pid[0] = linepts->InsertNextPoint(x, y, z);
			pid[1] = linepts->InsertNextPoint(x2, y2, z2);

			vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
			line->GetPointIds()->SetId(0, pid[0]);
			line->GetPointIds()->SetId(1, pid[1]);
			lines->InsertNextCell(line);
		}
	}

	vtkSmartPointer<vtkPolyData> linesPolyData = vtkSmartPointer<vtkPolyData>::New();
	linesPolyData->SetPoints(linepts);
	linesPolyData->SetLines(lines);
	vtkSmartPointer<vtkPolyDataMapper> linesmapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	linesmapper->SetInputData(linesPolyData);

	if (linesactor != nullptr)
	{
		m_pInterFace->m_Renderer->RemoveActor(linesactor);
	}
	
	linesactor->SetMapper(linesmapper);
	linesactor->GetProperty()->SetColor(1, 0, 0);
	linesactor->GetProperty()->SetLineWidth(3);
	m_pInterFace->m_Renderer->AddActor(linesactor);

}

//���ܡ���ָ��
void Assign_Section::assigh_section(Part_Base* Part)
{
	int section_row = ui.Section_Lists->currentRow();//ȡ�ý�����Ϣ

	int id = ui.Section_Lists->item(section_row, 5)->text().toDouble();//�ҵ��������ı��

	int group_row = ui.Group_Lists->currentRow() + 1;//ȡ�õ�Ԫ������Ϣ

	Part->AssginSectionGroup(group_row, id);
	m_pInterFace->SubstaceActor(Part);

}

//�㡰x���ر��¼�
void Assign_Section::closeEvent(QCloseEvent* e)
{
	m_pInterFace->m_Renderer->RemoveActor(linesactor);
}



