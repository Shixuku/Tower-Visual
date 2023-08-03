#pragma once
#include <QDialog>
#include "ui_UI_Calculate.h"
#include"InterFace.h"
#include"TowerCaculate.h"
#include"Instance.h"
#include"resultVisualize.h"
#include<vector>

class Instance_Calculate : public QDialog
{
	Q_OBJECT

public:
	Instance_Calculate(InterFace* InterFace, QWidget *parent = nullptr);
	void Set_headertext();
	~Instance_Calculate();

	void on_btk_ok_clicked();
	void visual();

	void update();

	void CreateActor();

public slots:
	void on_btn_import_clicked();

signals:
	void msg_CreateModel();

private:
	Ui::UI_CalculateClass ui;
	InterFace* m_InterFace = nullptr;
	TowerCaculate* towercaculate = nullptr;
	resultVisualize* display = nullptr;

	std::vector<Instance*> list_Instance;

	Instance* m_ins = nullptr;

	Manage_Entity<Node_Base> m_Nodes;
	Manage_Entity<Element_Base> m_LineElements;
};

