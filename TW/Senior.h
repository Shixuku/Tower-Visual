#pragma once

#include <QDialog>
#include "ui_Senior.h"
class Spacer;

class Senior : public QDialog
{
	Q_OBJECT

public:
	Senior(QWidget *parent = nullptr);
	~Senior();
	void Initialize();//Ĭ��ֵ
	void Draw_Wire(double* p1, double* p2, double* p3, double* p4, double* p5, double* p6); //ȡ���߲���(��λ���� Ӧ�� ���߷ֶ� �����)
	void Get_Data();
	int N_Pts;
	int choose;//�������װ��ʽ��ָ��or��ʽ��
	int choose_way;//�ȼ��or���ȼ��
	int Type_find = 0; //���ͷ�ʽ
	std::vector<int>Num_Spacer;//�洢���������
	Spacer* sp = nullptr;
public slots:
	void Show_Spacer();
private:
	Ui::SeniorClass ui;
};
