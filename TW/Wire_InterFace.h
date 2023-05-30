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

	void Insert_Point_Infor(int N);//�б�������Ϣ
	void Initialize();//Ĭ��ֵ
	void Insert_Data();//����� ����ص���Ϣ�����б�

	InterFace* m_pInterFace = nullptr;

private:
	Ui::Wire_InterFaceClass ui;
	QStringList headertext;
};
