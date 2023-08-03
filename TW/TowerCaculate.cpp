#include "TowerCaculate.h"
#include "InterFace.h"
#include <QFileDialog>

TowerCaculate::TowerCaculate(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_InterFace= dynamic_cast<InterFace*>(parent);

	connect(ui.btn_cancle, &QPushButton::clicked, this, &TowerCaculate::reject);

	
	QString beta = 0x03B2;
	QString gama = 0x03B3;
}

TowerCaculate::~TowerCaculate()
{

}





