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

    //��ʼ��
    int T_towers = m_InterFace->ui.treeWidget->topLevelItem(1)->childCount() + 1;//��������
    QString str("����ʵ��-");
    str += QString::number(T_towers);     //strת�ַ�
    ui.line_name->setText(str);//���ó�ʼֵ

}

Tower_Assembly::~Tower_Assembly()
{}

void Tower_Assembly::Create_combobox()
{
    int T_legs = m_InterFace->TP_leg.size();//��������
    int T_bodys = m_InterFace->TP_body.size();//��������
    int T_heads = m_InterFace->TP_CrossArm.size();//��ͷ����

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
    {//��������һ��������û�еĿ�ʵ��
        QMessageBox::information(this, "Tips", "������ѡ��һ��������"); 
        return;
    }
    else if (m_ArryLeg.size() >= 2)
    {//�����������������غϵ�ʵ��
        QMessageBox::information(this, "Tips", "���Ȳ���һ��ֻ��ѡ��һ����");
        return;
    }
    bool msg_error = false;
    //�����ɸ�����ÿһ����Ԫ�����ж� �Ƿ����˽���
    for (auto& i : m_ArryLeg)
    {//�������Ƿ񸳽�������ж�
        int ElementBeamSize = m_InterFace->TP_leg.Find_Entity(i)->m_Elements_beams.size();
        int ElementTrussSize = m_InterFace->TP_leg.Find_Entity(i)->m_Elements_Trusses.size();
        for (int j = 0; j < ElementBeamSize; j++)
        {
            if (m_InterFace->TP_leg.Find_Entity(i)->m_Elements_beams[j].ClassSectionID == 0)
            {
                QMessageBox::information(this, "Tips", "���" + m_InterFace->TP_leg.Find_Entity(i)->m_Name + "�����棡"); 
                msg_error = true;
                return;
            }
        }
        for (int j = 0; j < ElementTrussSize; j++)
        {
            if (m_InterFace->TP_leg.Find_Entity(i)->m_Elements_Trusses[j].ClassSectionID == 0)
            {
                QMessageBox::information(this, "Tips", "���" + m_InterFace->TP_leg.Find_Entity(i)->m_Name + "�����棡");
                msg_error = true;
                return;
            }
        }
        if (msg_error) break;
    }
    for (auto& i : m_ArryBody)
    {//�������Ƿ񸳽�������ж�
        int ElementBeamSize = m_InterFace->TP_body.Find_Entity(i)->m_Elements_beams.size();
        int ElementTrussSize = m_InterFace->TP_body.Find_Entity(i)->m_Elements_Trusses.size();
        for (int j = 0; j < ElementBeamSize; j++)
        {
            if (m_InterFace->TP_body.Find_Entity(i)->m_Elements_beams[j].ClassSectionID == 0)
            {
                QMessageBox::information(this, "Tips", "���" + m_InterFace->TP_body.Find_Entity(i)->m_name + "�����棡");
                msg_error = true;
                return;
            }
        }
        for (int j = 0; j < ElementTrussSize; j++)
        {
            if (m_InterFace->TP_body.Find_Entity(i)->m_Elements_Trusses[j].ClassSectionID == 0)
            {
                QMessageBox::information(this, "Tips", "���" + m_InterFace->TP_body.Find_Entity(i)->m_name + "�����棡");
                msg_error = true;
                return;
            }
        }
        if (msg_error) break;
    }
    for (auto& i : m_ArryHead)
    {//����ͷ�Ƿ񸳽�������ж�
        int ElementBeamSize = m_InterFace->TP_CrossArm.Find_Entity(i)->m_Elements_beams.size();
        int ElementTrussSize = m_InterFace->TP_CrossArm.Find_Entity(i)->m_Elements_Trusses.size();
        for (int j = 0; j < ElementBeamSize; j++)
        {
            if (m_InterFace->TP_CrossArm.Find_Entity(i)->m_Elements_beams[j].ClassSectionID == 0)
            {
                QMessageBox::information(this, "Tips", "���" + m_InterFace->TP_CrossArm.Find_Entity(i)->m_name + "�����棡");
                msg_error = true;
                return;
            }
        }
        for (int j = 0; j < ElementTrussSize; j++)
        {
            if (m_InterFace->TP_CrossArm.Find_Entity(i)->m_Elements_Trusses[j].ClassSectionID == 0)
            {
                QMessageBox::information(this, "Tips", "���" + m_InterFace->TP_CrossArm.Find_Entity(i)->m_name + "�����棡");
                msg_error = true;
                return;
            }
        }
        if (msg_error) break;
    }
    if (!msg_error) this->accept();
    else return;

}

