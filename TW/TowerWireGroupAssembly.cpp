#include "TowerWireGroupAssembly.h"
#include"InterFace.h"
TowerWireGroupAssembly::TowerWireGroupAssembly(InterFace* InterFace, TowerWireGroup* TowerWireGroup, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_pInterFace = InterFace;
	towerWireGroup = TowerWireGroup;
	towerWireGroup->groupId = m_pInterFace->TWG.size() + 1;
	CreateComBox();
	TableTowerLists();
	connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
	connect(ui.BtnAdd, &QPushButton::clicked, this, [=]() 
		{
			GetTowerSetData();
			int Index = ui.comboBox->currentIndex()+1;
			mTower = m_pInterFace->TP.Find_Entity(Index);
			int towerId = ui.tableWidget->rowCount() + 1;
			towerWireGroup->AddTowerToGroup(mTower, towerId,x,y,z, angle);
		});
	connect(ui.BtnOk, &QPushButton::clicked, this, [=]()
		{
			towerWireGroup->Show_VTKnode(m_pInterFace->m_Renderer_2);
			towerWireGroup->Show_VTKbeam(m_pInterFace->m_Renderer_2);
			towerWireGroup->Show_VTKtruss(m_pInterFace->m_Renderer_2);
			towerWireGroup->ShowNode();
			towerWireGroup->ShowBeamElement();
			towerWireGroup->ShowTrussElement();
			CreatTowerWierGroupItem();
			towerWireGroup->VectorToMap();
			m_pInterFace->m_Renderer_2->ResetCamera();
			m_pInterFace->TWG.Add_Entity(towerWireGroup);
			this->reject();
		});
}

TowerWireGroupAssembly::~TowerWireGroupAssembly()
{

}

void TowerWireGroupAssembly::CreateComBox()
{
	int towerSize = m_pInterFace->TP.size();
	for (int i = 0; i < towerSize; i++)
	{
		ui.comboBox->addItem("����ʵ��" + QString::number(i + 1));
	}
	ui.comboBox->setCurrentIndex(0);

	selectedItemName = ui.comboBox->itemText(0);

	emit ui.comboBox->currentIndexChanged(0);//����һ��item��ΪĬ��
}

void TowerWireGroupAssembly::TableTowerLists()
{
	headertext << "����" << "X" << "Y" << "Z" << "ת��" ;
	ui.tableWidget->setColumnCount(headertext.count());
	ui.tableWidget->setRowCount(0);//����Ĭ��0��
	ui.tableWidget->setHorizontalHeaderLabels(headertext);
	ui.tableWidget->verticalHeader()->setVisible(false);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void TowerWireGroupAssembly::GetTowerSetData()//�����ݷŽ�����
{
	x = ui.lineEditX->text().toDouble();//����λ�Ƶ�x����
	y = ui.lineEditY->text().toDouble();//����λ�Ƶ�y����
	z = ui.lineEditZ->text().toDouble();//����λ�Ƶ�z����
	angle = ui.lineEditAngle->text().toDouble();//������ת��
	int irow = ui.tableWidget->rowCount();
	ui.tableWidget->setRowCount(irow + 1);
	ui.tableWidget->setItem(irow, 0, new QTableWidgetItem(selectedItemName));
	ui.tableWidget->item(irow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	QString ix= QString::number(x);
	ui.tableWidget->setItem(irow, 1, new QTableWidgetItem(ix));
	ui.tableWidget->item(irow, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	QString iy = QString::number(y);
	ui.tableWidget->setItem(irow, 2, new QTableWidgetItem(iy));
	ui.tableWidget->item(irow, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	QString iz = QString::number(z);
	ui.tableWidget->setItem(irow, 3, new QTableWidgetItem(iz));
	ui.tableWidget->item(irow, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	QString iAngle = QString::number(angle);
	ui.tableWidget->setItem(irow, 4, new QTableWidgetItem(iAngle));
	ui.tableWidget->item(irow, 4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}
void TowerWireGroupAssembly::CreatTowerWierGroupItem()
{
	QTreeWidgetItem* towerWierGroupItem = new QTreeWidgetItem(m_pInterFace->creat_towerwire_instance);
	QString str = QString::number(m_pInterFace->creat_towerwire_instance->childCount());     //�������item
	towerWierGroupItem->setText(0, QString("����ʵ��" + str));
	//����ÿ������
	QTreeWidgetItem* GroupTower = new QTreeWidgetItem(towerWierGroupItem);
	GroupTower->setText(0, QString("����"));
	for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
        QTreeWidgetItem* GroupTowerItem = new QTreeWidgetItem(GroupTower);
		QString str = QString::number(towerWierGroupItem->childCount());     //strת�ַ�
		GroupTowerItem->setText(0, QString("�������" + str));
	}
	towerWireGroup->Item = towerWierGroupItem;
	QTreeWidgetItem* Wire_modeling = new QTreeWidgetItem(towerWierGroupItem);
	Wire_modeling->setText(0, QString("���߽�ģ"));

	QTreeWidgetItem* calculate = new QTreeWidgetItem(towerWierGroupItem);
	calculate->setText(0, QString("�����ļ�"));
}
void TowerWireGroupAssembly::onComboBoxIndexChanged(int index)
{
	selectedItemName = ui.comboBox->itemText(index);
}
