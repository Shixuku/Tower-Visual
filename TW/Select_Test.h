#pragma once

#include <QDialog>
#include "ui_Select_Test.h"
#include"Node.h"

class InterFace;
class Select_Test : public QDialog
{
	Q_OBJECT

public:
	Select_Test(QWidget *parent = nullptr);
	~Select_Test();
	void Get_PickedNode();

	InterFace* m_pInterFace = nullptr;
	
	double x1, y1, z1;
	double x2, y2, z2;
	void Btn_OK();
	std::vector<Node>Nodes;

private:
	Ui::Select_TestClass ui;
};
