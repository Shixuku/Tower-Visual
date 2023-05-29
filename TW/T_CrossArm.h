#pragma once

#include <QDialog>
#include "ui_T_CrossArm.h"
#include "InterFace.h"
class TowerPart_CrossArm;
class T_CrossArm : public QDialog
{
	Q_OBJECT

public:
	T_CrossArm(InterFace* interFace, QWidget* parent = nullptr);
	~T_CrossArm();
	void Set_headertext();//���ñ�ͷ
	void Set_table(int count);//���ñ��
	void Set_zhijia();//����֧��
	void Set_hengdan();//���úᵣ
	
	void Get_Data(TowerPart_CrossArm&);//ȡ�����������

	InterFace* m_InterFace = nullptr;
	void Set_combobox();
private:
	Ui::T_CrossArmClass ui;
public slots:
	void Change_combo_Tier();//�ڶ��ַ�������ѡ��ķ�ʽ
};
