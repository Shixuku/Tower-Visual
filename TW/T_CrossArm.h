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
	void Set_headertext();//设置表头
	void Set_table(int count);//设置表格
	void Set_zhijia();//设置支架
	void Set_hengdan();//设置横担
	
	void Get_Data(TowerPart_CrossArm&);//取表格里面数据

	InterFace* m_InterFace = nullptr;
	void Set_combobox();
private:
	Ui::T_CrossArmClass ui;
public slots:
	void Change_combo_Tier();//第二种方法，用选择的方式
};
