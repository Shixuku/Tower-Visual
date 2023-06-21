#pragma once

#include <QDialog>
#include "ui_Wire_InterFace.h"
#include"Creat_WireData.h"
#include"Draw_Wire.h"

class InterFace;
class Senior;
class Wire_InterFace : public QDialog
{
	Q_OBJECT

public:
	Wire_InterFace(QWidget *parent = nullptr);
	~Wire_InterFace();

	void Insert_Point_Infor(int N);//�б�������Ϣ
	void Insert_Pts_to_vector();//�����ҵ��������
	void Initialize();//Ĭ��ֵ
	void Insert_Data();//����� ����ص���Ϣ�����б�
	//xgd_List(����е����ҵ�) xgd_Real���������Ŵ���ʵ�����ҵ㣩direction=0��1��2,3��ֱ�� ������ ������,���߶����ţ�
	void Line_Segment(vector<Node>xgd_List, vector<Node>xgd_Real, int direction);
	void Show_Senior();//����߼���������
	void Creat_Distance(vector<Node>xgd_real, vector<int>num, vector<int>& l_Spacer, vector<int>& d_Spacer);//�����������˵�ľ���
	void Creat_Spacer(vector<int>L, vector<int>num,  vector<Node>xgd_Real);
	void OK();
	int fenlie;
	double Rou;//��λ����
	double yingli;//Ӧ��
	double area;//�����
	int N; //���߷ֶ���
	double rou;//bizai
	double Sag;
	int nz_len;

	vector<int>L_Spacer;
	vector<int>D_Spacer;
	vector<int>Extreme_Point;//�ж϶˵������
	vector<Node>xgd; //�б����ҵ�
	vector<Node>xgd_real; //ʵ�����ҵ�
	vector<Node> m_Nodes;//���е��ŵ�����
	vector<Node>Node_Temp;
	vector<Node>Node_Base;
	vector<Element_Wire> m_Elements;//���е�Ԫ�ϼ�
	vector<Element_Wire> m_Elements_Wire;//���ߵ�Ԫ�ϼ�
	vector<Element_Wire> m_Elements_Spacer;//�������Ԫ�ϼ�
	vector<Element_Wire> m_Elements_Strain;//���Ŵ���Ԫ�ϼ�
	
	InterFace* m_pInterFace = nullptr;
	Senior* sen = nullptr;
	Creat_WireData* cd;

private:
	Ui::Wire_InterFaceClass ui;
	QStringList headertext;
};
