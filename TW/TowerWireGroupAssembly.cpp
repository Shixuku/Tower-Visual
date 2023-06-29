#include "TowerWireGroupAssembly.h"
#include"InterFace.h"
TowerWireGroupAssembly::TowerWireGroupAssembly(InterFace* InterFace, TowerWireGroup* TowerWireGroup, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_pInterFace = InterFace;
	towerWireGroup = TowerWireGroup;
	
	CreateComBox();
	TableTowerLists();
	connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
	connect(ui.BtnAdd, &QPushButton::clicked, this, [=]() 
		{
			GetTowerSetData();
			int Index = ui.comboBox->currentIndex()+1;
			mTower = m_pInterFace->TP.Find_Entity(Index);
			TowerWireGroup->AddTowerToGroup(mTower, Index,x,y,z, angle);
		});
	connect(ui.BtnOk, &QPushButton::clicked, this, [=]()
		{
			TowerWireGroup->Show_VTKnode(m_pInterFace->m_Renderer_3);
			TowerWireGroup->Show_VTKbeam(m_pInterFace->m_Renderer_3);
			TowerWireGroup->Show_VTKtruss(m_pInterFace->m_Renderer_3);
			TowerWireGroup->ShowNode();
			TowerWireGroup->ShowBeamElement();
			TowerWireGroup->ShowTrussElement();
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
void TowerWireGroupAssembly::onComboBoxIndexChanged(int index)
{
	selectedItemName = ui.comboBox->itemText(index);
}
