#pragma once

#include <QDialog>
#include"WireData.h"
#include"Node.h"
#include"ui_Wire_InterFace.h"
#pragma execution_character_set("utf-8")

class InterFace;
class Senior;
class TowerWireGroup;
class Wire_InterFace : public QDialog
{
	Q_OBJECT

public:
	Wire_InterFace(TowerWireGroup* TowerWireGroup,QWidget *parent = nullptr);
	~Wire_InterFace();
	InterFace* m_pInterFace;
	TowerWireGroup* towerWireGroup = nullptr;
	QStringList headertext;
	void SetTableWideget(int row);//�������б���Ϣ
	void ui_Senior();
	void Get_Data(WireData& wd);
	void SaveSusPoint();//�����б��е����ҵ���Ϣ
	int a = 0;//���ɵĵ������
	int N;//�ֶ���
	double unitMass;//
	double area;//���
	double stress;//��͵�Ӧ��
	double strainLength;//���Ŵ�����
	double sag;//����
	double wireLogoQty;
	vector<int> chooseType1 ;//�˵�һ����
	vector<int> chooseType2 ;//�˵������
	std::vector<int>SpacerNum;//�洢���������
	std::vector<Node>WireSusList;//����и������ҵ�
	int  ChooseWay = 0;//�������װ��ʽ 0-�ȼ�� 1-���ȼ��
	Senior* sen = nullptr;
private:
	Ui::Wire_InterFaceClass ui;
};
