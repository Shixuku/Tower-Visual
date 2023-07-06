#include "Assign_Section.h"
#pragma execution_character_set("utf-8")
#include"InterFace.h"
#include<QMessageBox.h>
#include<QTextCodec>
#include<QStringLiteral>
#include<QList>
#include<QSet>

Assign_Section::Assign_Section(InterFace* InterFace, Part_Base*Part, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	m_pInterFace = InterFace;
	m_Part = Part;
	Q_ASSERT(m_pInterFace != nullptr);

	ma_ab2 = new Set_Section(m_pInterFace);
	table_Section_Lists();//���ý�����
	Add_created_section();//����Ѿ������Ľ���

	//�������������ʾ��������ԭ���Ļ������½�һ��actor��
	table_Group_Lists(Part);
	connect(ui.Group_Lists, &QTableWidget::itemClicked, this, [=]()
		{highlight_actor(Part);
	m_pInterFace->m_renderWindow->Render(); });

	//ָ�ɽ���
	connect(ui.btn_assign, &QPushButton::clicked, this, [=]()
		{assigh_section(Part); });
	linesactor = vtkSmartPointer<vtkActor>::New();

	//��ʾ��������Ի���
	connect(ui.btn_add, &QPushButton::clicked, this, [=](){
	int ret = ma_ab2->exec();
	if (ret == QDialog::Accepted)
	{
		int ClassSection = 0;//ȡ�øý������͵Ĳ���
		ClassSection = ma_ab2->ClassSection;
		string NameSection = ma_ab2->name;
		double ia = ma_ab2->a;
		double ib = ma_ab2->b;
		int iM = ma_ab2->ClassMa;

		int id = m_pInterFace->Ms.size() + 1;//��1��ʼ����

		int irow = ui.Section_Lists->rowCount();
		ui.Section_Lists->setRowCount(irow + 1);
		QString QName = QString::fromStdString(NameSection);;
		ui.Section_Lists->setItem(irow, 0, new QTableWidgetItem(QName));
		ui.Section_Lists->item(irow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString s;
		if (ClassSection == 0)
		{
			s = "L��";
		}
		else
		{
			s = "Բ��";
		}
		ui.Section_Lists->setItem(irow, 1, new QTableWidgetItem(s));
		ui.Section_Lists->item(irow, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString i_a = QString::number(ia);
		ui.Section_Lists->setItem(irow, 2, new QTableWidgetItem(i_a));
		ui.Section_Lists->item(irow, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString i_b = QString::number(ib);
		ui.Section_Lists->setItem(irow, 3, new QTableWidgetItem(i_b));
		ui.Section_Lists->item(irow, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString i_M = QString::number(iM);
		ui.Section_Lists->setItem(irow, 4, new QTableWidgetItem(i_M));
		ui.Section_Lists->item(irow, 4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString i_id = QString::number(id);
		ui.Section_Lists->setItem(irow, 5, new QTableWidgetItem(i_id));
		ui.Section_Lists->item(irow, 5)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	}});
	//��ӽ���
	//connect(ma_ab2, SIGNAL(SendData(QStringList&)), this, SLOT(GetData(QStringList&)));
	connect(ma_ab2, SIGNAL(SendData(QStringList&)), m_pInterFace, SLOT(GetData(QStringList&)));
	//ɾ������ӵĽ���
	connect(ui.btn_delete, &QPushButton::clicked, this, &Assign_Section::table_Section_Lists_delete);

	//�����ɰ�ť����ɾ��������actor
	connect(ui.btn_finish, &QPushButton::clicked, this, [=]()
		{
			m_pInterFace->m_Renderer->RemoveActor(linesactor);
			
			this->reject();
		});

}

Assign_Section::~Assign_Section()
{

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
	for (auto& i: m_pInterFace->Ms)
	{
		Section* pSection = i.second;  //ȡ������ָ��
		int irow = ui.Section_Lists->rowCount();
		ui.Section_Lists->setRowCount(irow + 1);

		QString QName = QString::fromStdString(pSection->Name);
		ui.Section_Lists->setItem(irow, 0, new QTableWidgetItem(QName));
		ui.Section_Lists->item(irow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString s;
		if (pSection->ClassSe == 0)
		{
			s = "L��";
		}
		else
		{
			s = "Բ��";
		}
		ui.Section_Lists->setItem(irow, 1, new QTableWidgetItem(s));
		ui.Section_Lists->item(irow, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString ia = QString::number(pSection->a);
		ui.Section_Lists->setItem(irow, 2, new QTableWidgetItem(ia));
		ui.Section_Lists->item(irow, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString ib = QString::number(pSection->b);
		ui.Section_Lists->setItem(irow, 3, new QTableWidgetItem(ib));
		ui.Section_Lists->item(irow, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString iM = QString::number(pSection->ClassM);
		ui.Section_Lists->setItem(irow, 4, new QTableWidgetItem(iM));
		ui.Section_Lists->item(irow, 4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QString id = QString::number(pSection->m_id);
		ui.Section_Lists->setItem(irow, 5, new QTableWidgetItem(id));
		ui.Section_Lists->item(irow, 5)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
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

	int id = ui.Section_Lists->item(section_row, 5)->text().toInt();//�ҵ��������ı��

	int group_row = ui.Group_Lists->currentRow() + 1;//ȡ�õ�Ԫ������Ϣ
	QList<QTableWidgetItem*> selectedItems = ui.Group_Lists->selectedItems();

	// �洢ѡ���е�����
	QSet<int> selectedRows;

	// ��ȡ�к�
	for (const auto* item : selectedItems) {
		int row = item->row()+1;
		selectedRows.insert(row);
	}

	Part->AssginSectionGroup(selectedRows, id);
	m_pInterFace->SubstaceActor(Part);

}

//�㡰x���ر��¼�
void Assign_Section::closeEvent(QCloseEvent* e)
{
	m_pInterFace->m_Renderer->RemoveActor(linesactor);
}



