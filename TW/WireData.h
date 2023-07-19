#pragma once
#include"Instance.h"
#include"Node.h"
class WireData:public Instance
{
public:
	int fenlie = 0;
	double unitMass = 0;//��λ����
	double area = 0;//�����
	int N = 0; //���߷ֶ���
	double rou = 0;//bizai
	double stress = 0;//yingli
	double strainLength = 0;//���Ŵ�����
	double sag = 0;//����
	double angle;
	vector<Node>WireListSus;//�б����ҵ�����
	vector<Node>WireRealSus;//��ʵ���ҵ�����
	vector<Node>allSus;//��ʵ���ҵ�����б�����λ������
	vector<Node> TempListNodes;//�ڵ�ϼ�
	vector<Node> TempRealNodes;//�ڵ�ϼ�
	double WireSectionId = 0;//������
	int wireQty = 0;//����·��
	int endpoinType1 = 0;//��·�˵�һ�����ͣ�0Ϊ�Ҿ�Ե���ϣ�1Ϊ����-�����ϣ�
	int endpoinType2 = 0;//��·�˵�������ͣ�0Ϊ�Ҿ�Ե���ϣ�1Ϊ����-�����ϣ�
	int Creat_Node(double x, double y, double z, double f);
	void CreatRealNode();//������ʵ���ҵ�
	void CreateTempWireNode(vector<Node>&sus,vector<Node>&Nodes);//���б����ҵ㴴��Ϊ���ѵĻ����ڵ�
	void FindRealSus(int choose); //choose=0;1-���ţ�choose=1;2-���ţ�choose=2;1��2-������
	void CreatSpacer(vector<int> ids);//������ɻ� ���ɼ����
	void CreateStrainLine(double x,double y,double z,vector<int> ids);//�����Ŵ��Ļ�����һ������
	void CreateSpacerDistance();//
	vector<int> FindSpacerL(int d,int L);
	//��������ֲ���
	int ChooseWay = 0;//�������װ��ʽ 0-�ȼ�� 1-���ȼ��
	std::vector<int>SpacerNum;//�洢���������
	std::vector<int>SpacerL;//���������߶˵�ľ��� 
	std::vector<int>SpacerD;//��������ڵĵ�λ



};

