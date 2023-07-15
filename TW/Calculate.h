#pragma once
#include <QDialog>
#include "ui_Calculate.h"

class InterFace;
class Calculate : public QDialog
{
	Q_OBJECT

public:
	Calculate(InterFace* InterFace, QWidget *parent = nullptr);
	~Calculate();
	InterFace* m_pInterFace = nullptr;
	void CreateTreeWight();
private:
	//Ui::CalculateClass ui;
};
