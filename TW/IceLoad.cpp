#include "IceLoad.h"
#include"Creat_Loads.h"
#include"Tower.h"
#include"InterFace.h"
#include"InteractorStyle.h"
#pragma execution_character_set("utf-8")

IceLoad::IceLoad(Creat_Loads * creat_loads, QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);

	m_pCreat_loads = creat_loads;
	m_pTower = m_pCreat_loads->m_tower;//调用父类的tower
	Initialization();//combox初始化
	this->setWindowFlags(Qt::WindowStaysOnTopHint);//设置窗口在最上层
	connect(ui.btn_ok, &QPushButton::clicked, this, &IceLoad::accept);
	connect(ui.btn_cancle, &QPushButton::clicked, this, &IceLoad::reject);
	connect(m_pCreat_loads->m_pInterFace, &InterFace::SignalsPickElements, this, &IceLoad::GetData);
	
}

IceLoad::~IceLoad()
{}

void IceLoad::Initialization()
{
	ui.label_name->setText("名称：" + m_pCreat_loads->m_str);//显示父窗口的名称
	//设置combobox
	int count = m_pCreat_loads->m_pInterFace->ME_ElementIce.size() + 1;
	for (int i = 1; i < count; i++)
	{
		ui.comboBox->addItem("第" + QString::number(i) + "种");
	}
}

void IceLoad::GetData()
{
	vtkIdTypeArray* idTypeArray = m_pCreat_loads->m_pInterFace->style->ids;
	int count = idTypeArray->GetNumberOfTuples();
	QString idElement;
	
	for (vtkIdType i = 0; i < count; i++)
	{
		idElement += QString::number(idTypeArray->GetValue(i) + 1) + " ";
		ui.line_idIceElement->setText(idElement);
	}
	if (count == 0)
	{
		ui.line_idIceElement->clear();
	}

}

