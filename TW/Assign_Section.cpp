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
	Set_headertext();

	//�������������ʾ��������ԭ���Ļ������½�һ��actor��
	table_Group_Lists(Part);
	connect(ui.Group_Lists, &QTableWidget::itemClicked, this, [=](){
		highlight_actor(Part);
		m_pInterFace->m_renderWindow->Render(); 
		});

	//ָ�ɽ���
	connect(ui.btn_assign, &QPushButton::clicked, this, [=]()
		{assigh_section(Part); });
	linesactor = vtkSmartPointer<vtkActor>::New();

	//��ʾ��������Ի���
	connect(ui.btn_add, &QPushButton::clicked, this, &Assign_Section::BtnAddSection);

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
		ui.Group_Lists->item(i, 0)->setFlags(ui.Group_Lists->item(i, 0)->flags() & (~Qt::ItemIsEditable));  // ����ֻ������
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
	if (ui.Section_Lists->rowCount() == 0)
	{
		QMessageBox::information(this, "Tips", "��ǰû�н��棡"); return;
	}
	else if (section_rowIndex < 0)
	{
		QMessageBox::information(this, "Tips", "��ѡ��Ҫɾ���Ľ��棡"); return;
	}
	else
	{
		//ȷ���Ƿ�ɾ��
		QMessageBox::StandardButton btn;
		btn = QMessageBox::question(this, "��ʾ", "ȷ��Ҫɾ���� " + QString::number(section_rowIndex + 1) + " �����������",
			QMessageBox::Yes | QMessageBox::No);
		if (btn == QMessageBox::Yes)
		{
			if (section_rowIndex != -1)
			{
				int id = ui.Section_Lists->item(section_rowIndex, 0)->text().toInt();
				ui.Section_Lists->removeRow(section_rowIndex);
				
				cout << id << "\n";
				m_pInterFace->Ms.Delete_Entity(id);//ɾ��m_pInterFace����Ľ�������
				//m_pInterFace->Ms.Delete_Entity(section_rowIndex + 1);//ɾ��m_pInterFace����Ľ�������
			}
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
	int section_row = ui.Section_Lists->currentRow();//ȡ�ý�����Ϣ,���û�б�񣬾͵���-1
	int group_row = ui.Group_Lists->currentRow() + 1;//ȡ�õ�Ԫ������Ϣ
	//�����ж����޽���
	if (ui.Section_Lists->rowCount() == 0)
	{
		QMessageBox::information(this, "Tips", "���ȴ������棡"); return;
	}
	//Ȼ���жϽ�����Ϣ��û�б�ѡ��
	else if (section_row < 0)
	{
		QMessageBox::information(this, "Tips", "��ѡ����棡"); return;
	}
	//�жϸ˼�����Ϣ��û��ѡ��
	else if (group_row < 1)
	{
		QMessageBox::information(this, "Tips", "��ѡ��˼��飡"); return;
	}
	else
	{
		//int id = ui.Section_Lists->item(section_row, 5)->text().toInt();//�ҵ��������ı��
		int id = ui.Section_Lists->item(section_row, 0)->text().toInt();//�޸Ĺ���
		

		QList<QTableWidgetItem*> selectedItems = ui.Group_Lists->selectedItems();

		// �洢ѡ���е�����
		QSet<int> selectedRows;
		// ��ȡ�к�
		for (const auto* item : selectedItems) {
			int row = item->row() + 1;
			selectedRows.insert(row);
		}
		Part->AssginSectionGroup(selectedRows, id);
		m_pInterFace->ShowSubstaceActor(Part);
	}

}

void Assign_Section::BtnAddSection()
{
	int ret = ma_ab2->exec();
	if (ret == QDialog::Accepted)
	{
		int id = m_pInterFace->Ms.size();//��1��ʼ����
		int irow = ui.Section_Lists->rowCount();
		ui.Section_Lists->setRowCount(irow + 1);
		Section* pSection = m_pInterFace->Ms.Find_Entity(id);  //ȡ������ָ��

		ui.Section_Lists->setItem(irow, 0, new QTableWidgetItem(QString::number(pSection->m_id)));//���
		ui.Section_Lists->setItem(irow, 1, new QTableWidgetItem(pSection->m_Name));//����
		QString Material;
		if (pSection->ClassM == 1)
		{
			Material = "Q235";
		}
		else if (pSection->ClassM == 2)
		{
			Material = "Q345";
		}
		else if (pSection->ClassM == 3)
		{
			Material = "Q420";
		}
		QString type;
		if (pSection->ClassSe == 0)//L��
		{
			type = "L";
		}
		else if (pSection->ClassSe == 1)
		{
			//type = "\u03A6";//��дfai--��ʾ��������
			type = "D";
		}
		QString data = QString::number((pSection->a) * 1000) + "X" + QString::number((pSection->b) * 1000);
		ui.Section_Lists->setItem(irow, 2, new QTableWidgetItem(Material + type + data));
		for (int j = 0; j < 3; j++)//��ͷ;
		{
			ui.Section_Lists->item(irow, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}
}

void Assign_Section::Set_headertext()
{
	QStringList headertext;//��ͷ
	headertext << "���" << "����" << "����";
	ui.Section_Lists->setColumnCount(headertext.count());
	ui.Section_Lists->setHorizontalHeaderLabels(headertext);
	ui.Section_Lists->verticalHeader()->setVisible(false);
	ui.Section_Lists->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.Section_Lists->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

	int rowcount = m_pInterFace->Ms.size();
	ui.Section_Lists->setRowCount(rowcount);
	QVector<Section*> ve_section;
	m_pInterFace->Ms.Get_Array(ve_section, true);

	for (int i = 0; i < rowcount; i++)
	{
		Section* section = ve_section[i];
		ui.Section_Lists->setItem(i, 0, new QTableWidgetItem(QString::number(section->m_id)));//���
		ui.Section_Lists->setItem(i, 1, new QTableWidgetItem(section->m_Name));//����
		QString Material;
		if (section->ClassM == 1)
		{
			Material = "Q235";
		}
		else if (section->ClassM == 2)
		{
			Material = "Q345";
		}
		else if (section->ClassM == 3)
		{
			Material = "Q420";
		}
		QString type;
		if (section->ClassSe == 0)//L��
		{
			type = "L";
		}
		else if (section->ClassSe == 1)
		{
			//type = "\u03A6";//��дfai--��ʾ��������
			type = "D";
		}
		QString data = QString::number((section->a) * 1000) + "X" + QString::number((section->b) * 1000);
		ui.Section_Lists->setItem(i, 2, new QTableWidgetItem(Material + type + data));
		for (int j = 0; j < 3; j++)//��ͷ;
		{
			ui.Section_Lists->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}

}
//�㡰x���ر��¼�
void Assign_Section::closeEvent(QCloseEvent* e)
{
	m_pInterFace->m_Renderer->RemoveActor(linesactor);
}



