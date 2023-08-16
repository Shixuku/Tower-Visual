#pragma once
#include"Manage_Entity.h"
#include <QDialog>
#include "ui_Assign_Section.h"
#include"Set_Section.h"
#include"InterFace.h"
#include<QCloseEvent>
#include"Part_Base.h"
#include"Section.h"
class InterFace;

class Assign_Section : public QDialog
{
	Q_OBJECT

public:
	Assign_Section(InterFace* InterFace, Part_Base*Part,QWidget *parent = nullptr);
	~Assign_Section();
	void table_Group_Lists(Part_Base*Part);
	void table_Section_Lists_delete();
	void highlight_actor(Part_Base* Part);
	void assigh_section(Part_Base* Part);
	void BtnAddSection();
	void Set_headertext();

	InterFace* m_pInterFace = nullptr;
	Set_Section* set_section = nullptr;
	Part_Base* m_Part = nullptr;
	vtkSmartPointer<vtkActor> linesactor;

	//关闭对话框的函数
	void closeEvent(QCloseEvent* e);

private:
	Ui::Assign_SectionClass ui;

	QStringList headertext;
	Set_Section*ma_ab2;


};
