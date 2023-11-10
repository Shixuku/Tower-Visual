#pragma once

#include <QDialog>
#include "ui_Manage_PartData.h"
#include"InterFace.h"
#include"TowerPart_leg.h"
#include"TowerPart_body.h"
#include"TowerPart_CrossArm.h"

#include"T_Foot.h"
#include"T_Body.h"
#include"T_CrossArm.h"


class Manage_PartData : public QDialog
{
	Q_OBJECT

public:
	Manage_PartData(InterFace* InterFace, QWidget *parent = nullptr);
	~Manage_PartData();
	void Set_headertext();
	void Modify_Data();//修改数据
	void ModifyDataLeg(int Index);//修改塔腿数据
	void ModifyDataBody(int Index);//修改塔身数据
	void ModifyDataHead(int Index);//修改塔头数据

	void Delete_Data();//删除数据
	void DeleteDataLeg(int Index);//删除塔腿数据
	void DeleteDataBody(int Index);//删除塔身数据
	void DeleteDataHead(int Index);//删除塔头数据

private:
	Ui::Manage_PartDataClass ui;
	InterFace* m_InterFace = nullptr;

};
