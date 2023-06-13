#include "Tower_Assembly.h"
#pragma execution_character_set("utf-8")

Tower_Assembly::Tower_Assembly(InterFace* InterFace, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
    m_InterFace = InterFace;

    Create_combobox();
    connect(ui.ok_btn, &QPushButton::clicked, this, [=]()
        {
            this->accept();
            Get_PartData();
        });
    connect(ui.cancel_btn, &QPushButton::clicked, this, &Tower_Assembly::reject);

    //初始化
    int T_legs = m_InterFace->ui.treeWidget->topLevelItem(1)->childCount() + 1;//塔腿数量
    QString str("杆塔实例-");
    str += QString::number(T_legs);     //str转字符
    ui.line_name->setText(str);//设置初始值

}

Tower_Assembly::~Tower_Assembly()
{}

void Tower_Assembly::Create_combobox()
{
   int T_legs = m_InterFace->ui.treeWidget->topLevelItem(0)->child(0)->childCount();//塔腿数量
   int T_bodys = m_InterFace->ui.treeWidget->topLevelItem(0)->child(1)->childCount();//塔身数量
   int T_heads = m_InterFace->ui.treeWidget->topLevelItem(0)->child(2)->childCount();//塔头数量

    for (int i = 0; i < T_legs; i++)
    {
        QString name = m_InterFace->ui.treeWidget->topLevelItem(0)->child(0)->child(i)->text(0);
        ui.combo_foot->addItem(name);
    }

    for (int i = 0; i < T_bodys; i++)
    {
        QString name = m_InterFace->ui.treeWidget->topLevelItem(0)->child(1)->child(i)->text(0);
        ui.combo_body->addItem(name);
    }

    for (int i = 0; i < T_heads; i++)
    {
        QString name = m_InterFace->ui.treeWidget->topLevelItem(0)->child(2)->child(i)->text(0);
        ui.combo_head->addItem(name);
    }

}

QString Tower_Assembly::Get_name()
{
    QString name = ui.line_name->text();
    return name;
}

void Tower_Assembly::Get_Movedata(double& x, double& y, double& z, double& angle)
{
    x = ui.line_disX->text().toDouble();
    y = ui.line_disY->text().toDouble();
    z = ui.line_disZ->text().toDouble();
    angle=ui.line_angle->text().toDouble();
}

void Tower_Assembly::Get_PartData()
{
    m_ArryLeg = ui.combo_foot->Get_idItems();
    m_ArryHead = ui.combo_head->Get_idItems();
    m_ArryBody = ui.combo_body->Get_idItems();
}

