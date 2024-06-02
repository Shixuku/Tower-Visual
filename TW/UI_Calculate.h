#pragma once
#include <QDialog>
#include "ui_UI_Calculate.h"
#include"InterFace.h"
#include"TowerCaculate.h"
#include"Instance.h"
#include"Element_Truss.h"
#include"Element_Beam.h"
#include"Instance.h"
#include"resultVisualize.h"

#include<vector>
#include<map>

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
	void on_btn_import_clicked();

	
signals:
	void msg_CreateModel();

protected:
	void showEvent(QShowEvent* event) override {
		update();
		QWidget::showEvent(event);
	}


private:
	Ui::UI_CalculateClass ui;
	InterFace* m_InterFace = nullptr;
	TowerCaculate* towercaculate = nullptr;
	resultVisualize* display = nullptr;

	std::map<int, Instance*> list_Instance;

	Instance* m_ins = nullptr;
	vector<Node> m_nodes;//包含位移的点
	vector<Element_Truss> m_truss;//包含位移的杆单元
	vector<Element_Truss> m_beam;//包含位移的杆单元
	Manage_Entity<Node_Base> m_Nodes;
	Manage_Entity<Element_Base> m_LineElements;
	QString m_str;
	QString m_fileName;
};

