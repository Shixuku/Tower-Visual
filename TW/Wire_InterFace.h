#pragma once

#include <QDialog>
#include "ui_Wire_InterFace.h"

class InterFace;
class Wire_InterFace : public QDialog
{
	Q_OBJECT

public:
	Wire_InterFace(QWidget *parent = nullptr);
	~Wire_InterFace();

	void Insert_Point_Infor(int N);//列表点相关信息
	void Initialize();//默认值
	void Insert_Data();//鼠标点击 将相关点信息加入列表

	InterFace* m_pInterFace = nullptr;

private:
	Ui::Wire_InterFaceClass ui;
	QStringList headertext;
};
