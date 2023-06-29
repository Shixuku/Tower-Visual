#pragma once

#include <QDialog>
#include "ui_IceLoad.h"

class IceLoad : public QDialog
{
	Q_OBJECT

public:
	IceLoad(QWidget *parent = nullptr);
	~IceLoad();
	void CheckBoxState(bool state);
private:
	Ui::IceLoadClass ui;
};
