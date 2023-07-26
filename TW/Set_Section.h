#pragma once

#include <QDialog>
#include<QTimer>
#include"ui_Set_Section.h"
#include"Section.h"
#include<vector>
#include<iostream>
#include"InterFace.h"
class Set_Section : public QDialog
{
	Q_OBJECT

public:
	Set_Section(InterFace* m_pInterFace, QWidget* parent = nullptr);
	InterFace* m_pInterFace = nullptr;
	~Set_Section();
private:
	void GetData();
	void Change_Picture_foot();
private:
	Ui::Set_SectionClass ui;

};
