#pragma once

#include <QDialog>
#include "ui_Senior.h"
#include <QObject>


class Wire_InterFace;
class Spacer;
class Senior : public QDialog
{
	Q_OBJECT

public:
	Senior(Wire_InterFace* Wire_InterFace,QWidget *parent = nullptr);
	~Senior();
	void Initialize();
	int N;
	double unitMass;
	double area;
	double stress;
	double strainL;
	double sag;
	int ChooseWay = 0;//�������װ��ʽ 0-�ȼ�� 1-���ȼ��
	std::vector<int>SpacerNum;//�洢���������
	Wire_InterFace* wireInterFace = nullptr;
	Spacer* sp = nullptr;
	void CreatData(); // ��CreatData()��������Ϊ�ۺ���
public slots:
	void ui_Spacer();

private:
	Ui::SeniorClass ui;
};
