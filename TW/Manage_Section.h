#pragma once

#include <QDialog>
#include "ui_Manage_Section.h"

class InterFace;
class Manage_Section : public QDialog
{
	Q_OBJECT

public:
	Manage_Section(InterFace* m_InterFace, QWidget* parent = nullptr);
	~Manage_Section();
	void Set_headertext();
	void DeleteSection();
	InterFace* m_pInterFace = nullptr;
private:
	Ui::Manage_SectionClass ui;
};
