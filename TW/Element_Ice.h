#pragma once

#include <QDialog>
#include "ui_Element_Ice.h"
#include"ParameterIceElement.h"
class InterFace;

class Element_Ice : public QDialog
{
	Q_OBJECT

public:
	Element_Ice(InterFace* m_pInterFace, QWidget *parent = nullptr);
	~Element_Ice();
	InterFace* m_pInterFace = nullptr;
private:
	void GetData();
	Ui::Element_IceClass ui;
};
