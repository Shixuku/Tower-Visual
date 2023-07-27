#include "Tower_Assembly.h"
#include<QMessageBox.h>
#include<iostream>
#pragma execution_character_set("utf-8")

Tower_Assembly::Tower_Assembly(InterFace* InterFace, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
    m_InterFace = InterFace;

    Create_combobox();
    connect(ui.ok_btn, &QPushButton::clicked, this, &Tower_Assembly::Get_PartData);
    connect(ui.cancel_btn, &QPushButton::clicked, this, &Tower_Assembly::reject);

    //初始化
    int T_towers = m_InterFace->ui.treeWidget->topLevelItem(1)->childCount() + 1;//塔腿数量
    QString str("杆塔实例-");
    str += QString::number(T_towers);     //str转字符
    ui.line_name->setText(str);//设置初始值

}

Tower_Assembly::~Tower_Assembly()
{}

void Tower_Assembly::Create_combobox()
{
    int T_legs = m_InterFace->TP_leg.size();//塔腿数量
    int T_bodys = m_InterFace->TP_body.size();//塔身数量
    int T_heads = m_InterFace->TP_CrossArm.size();//塔头数量

    for (int i = 0; i < T_legs; i++)
    {
        QString name = m_InterFace->TP_leg.Find_Entity(i + 1)->m_Name;
        ui.combo_foot->addItem(name);
    }

    for (int i = 0; i < T_bodys; i++)
    {
        QString name = m_InterFace->TP_body.Find_Entity(i + 1)->m_name;
        ui.combo_body->addItem(name);
    }

    for (int i = 0; i < T_heads; i++)
    {
        QString name = m_InterFace->TP_CrossArm.Find_Entity(i + 1)->m_name;
        ui.combo_head->addItem(name);
    }

}

QString Tower_Assembly::Get_name()
{
    QString name = ui.line_name->text();
    return name;
}

void Tower_Assembly::Get_PartData()
{
    m_ArryLeg = ui.combo_foot->Get_idItems();
    m_ArryHead = ui.combo_head->Get_idItems();
    m_ArryBody = ui.combo_body->Get_idItems();

    if (m_ArryLeg.size() == 0 && m_ArryHead.size() == 0 && m_ArryBody.size() == 0)
    {//避免生成一个部件都没有的空实例
        QMessageBox::information(this, "Tips", "请至少选择一个部件！"); 
        return;
    }
    else if (m_ArryLeg.size() >= 2)
    {//避免生成两个塔腿重合的实例
        QMessageBox::information(this, "Tips", "塔腿部件一次只能选择一个！");
        return;
    }
    bool msg_error = false;
    //对生成杆塔的每一个单元进行判断 是否赋予了截面
    for (auto& i : m_ArryLeg)
    {//对塔腿是否赋截面进行判断
        int ElementBeamSize = m_InterFace->TP_leg.Find_Entity(i)->m_Elements_beams.size();
        int ElementTrussSize = m_InterFace->TP_leg.Find_Entity(i)->m_Elements_Trusses.size();
        for (int j = 0; j < ElementBeamSize; j++)
        {
            if (m_InterFace->TP_leg.Find_Entity(i)->m_Elements_beams[j].ClassSectionID == 0)
            {
                QMessageBox::information(this, "Tips", "请给" + m_InterFace->TP_leg.Find_Entity(i)->m_Name + "赋截面！"); 
                msg_error = true;
                return;
            }
        }
        for (int j = 0; j < ElementTrussSize; j++)
        {
            if (m_InterFace->TP_leg.Find_Entity(i)->m_Elements_Trusses[j].ClassSectionID == 0)
            {
                QMessageBox::information(this, "Tips", "请给" + m_InterFace->TP_leg.Find_Entity(i)->m_Name + "赋截面！");
                msg_error = true;
                return;
            }
        }
        if (msg_error) break;
    }
    for (auto& i : m_ArryBody)
    {//对塔身是否赋截面进行判断
        int ElementBeamSize = m_InterFace->TP_body.Find_Entity(i)->m_Elements_beams.size();
        int ElementTrussSize = m_InterFace->TP_body.Find_Entity(i)->m_Elements_Trusses.size();
        for (int j = 0; j < ElementBeamSize; j++)
        {
            if (m_InterFace->TP_body.Find_Entity(i)->m_Elements_beams[j].ClassSectionID == 0)
            {
                QMessageBox::information(this, "Tips", "请给" + m_InterFace->TP_body.Find_Entity(i)->m_name + "赋截面！");
                msg_error = true;
                return;
            }
        }
        for (int j = 0; j < ElementTrussSize; j++)
        {
            if (m_InterFace->TP_body.Find_Entity(i)->m_Elements_Trusses[j].ClassSectionID == 0)
            {
                QMessageBox::information(this, "Tips", "请给" + m_InterFace->TP_body.Find_Entity(i)->m_name + "赋截面！");
                msg_error = true;
                return;
            }
        }
        if (msg_error) break;
    }
    for (auto& i : m_ArryHead)
    {//对塔头是否赋截面进行判断
        int ElementBeamSize = m_InterFace->TP_CrossArm.Find_Entity(i)->m_Elements_beams.size();
        int ElementTrussSize = m_InterFace->TP_CrossArm.Find_Entity(i)->m_Elements_Trusses.size();
        for (int j = 0; j < ElementBeamSize; j++)
        {
            if (m_InterFace->TP_CrossArm.Find_Entity(i)->m_Elements_beams[j].ClassSectionID == 0)
            {
                QMessageBox::information(this, "Tips", "请给" + m_InterFace->TP_CrossArm.Find_Entity(i)->m_name + "赋截面！");
                msg_error = true;
                return;
            }
        }
        for (int j = 0; j < ElementTrussSize; j++)
        {
            if (m_InterFace->TP_CrossArm.Find_Entity(i)->m_Elements_Trusses[j].ClassSectionID == 0)
            {
                QMessageBox::information(this, "Tips", "请给" + m_InterFace->TP_CrossArm.Find_Entity(i)->m_name + "赋截面！");
                msg_error = true;
                return;
            }
        }
        if (msg_error) break;
    }
    if (!msg_error) this->accept();
    else return;

}

