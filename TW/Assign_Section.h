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

	//int id_Part = 0;//���ҵ���part��Ϊ��Ա����
	//int T_legs = 0;
	//int T_bodys = 0;//��������
	//int T_heads = 0;//��ͷ����
	//�رնԻ���ĺ���
	void closeEvent(QCloseEvent* e);


	//list<Element_Beam*>B_Elements; //����
	//list<Element_Truss*>T_Elements; //����
public slots:
	//void GetData(QStringList&);
private:
	Ui::Assign_SectionClass ui;

	QStringList headertext;
	Set_Section*ma_ab2;
	//DataTreat* m_data;

};
