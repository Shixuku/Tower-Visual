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
	//text
	//void ShowComboVTK();
	void table_Section_Lists();
	void table_Group_Lists(Part_Base*Part);
	void table_Section_Lists_delete();
	void highlight_actor(Part_Base* Part);
	void assigh_section(Part_Base* Part);

	
	void Add_created_section();

	//void Create_combobox();

	InterFace* m_pInterFace = nullptr;
	Set_Section* set_section = nullptr;
	Part_Base* m_Part = nullptr;
	vtkSmartPointer<vtkActor> linesactor;

	//int id_Part = 0;//将找到的part作为成员变量
	//int T_legs = 0;
	//int T_bodys = 0;//塔身数量
	//int T_heads = 0;//塔头数量
	//关闭对话框的函数
	void closeEvent(QCloseEvent* e);


	//list<Element_Beam*>B_Elements; //高亮
	//list<Element_Truss*>T_Elements; //高亮
public slots:
	//void GetData(QStringList&);
private:
	Ui::Assign_SectionClass ui;

	QStringList headertext;
	Set_Section*ma_ab2;
	//DataTreat* m_data;

};
