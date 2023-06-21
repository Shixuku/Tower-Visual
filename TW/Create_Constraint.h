#pragma once

#include <QDialog>
#include "ui_Create_Constraint.h"
#include"Constrained_image.h"
class InterFace;

class Create_Constraint : public QDialog
{
	Q_OBJECT

public:
	Create_Constraint(QWidget *parent = nullptr);
	~Create_Constraint();

	void Draw_Con();//¹Ì¶¨Ô¼Êø
	InterFace* m_pInterFace = nullptr;
	Constrained_image* Con;
private:
	Ui::Create_ConstraintClass ui;
};
