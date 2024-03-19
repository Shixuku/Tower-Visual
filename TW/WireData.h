#pragma once
#include"Instance.h"
#include"Node.h"
class WireData:public Instance
{
public:
	int fenlie = 0;
	double unitMass = 0;//��λ����
	double area = 0;//�����
	int N = 100; //���߷ֶ���
	double rou = 0;//bizai
	double stress = 0;//yingli
	double strainLength = 0;//���Ŵ�����
	double sag = 0;//����
	double angle;
	vector<Node>WireListSus;//�б����ҵ�����
	vector<Node>WireRealSus;//��ʵ���ҵ�����
	vector<Node>allSus;//��ʵ���ҵ�����б�����λ������
	//vector<Node> TempListNodes;//�ڵ�ϼ�
	//vector<Node> TempRealNodes;//�ڵ�ϼ�
	int WireSectionId = 0;//���߽�����
	int InsulatorSectionId = 0;//��Ե�ӽ�����
	int SpacerSectionId = 0;//�����������
	int wireQty = 0;;//����·��
	vector<int >endpoinType1 ;//��·�˵�һ�����ͣ�0Ϊ�Ҿ�Ե���ϣ�1Ϊ����-�����ϣ�
	vector<int> endpoinType2 ;//��·�˵�������ͣ�0Ϊ�Ҿ�Ե���ϣ�1Ϊ����-�����ϣ�
	int Creat_Node(double x, double y, double z, double f,int Type);
	int GetMidId(double x, double y, double z);
	void CreatRealNode();//������ʵ���ҵ�
	void CreateTempWireNode(int wireLogo, int choose, vector<Node>& sus);//���б����ҵ㴴��Ϊ���ѵĻ����ڵ�
	void CreateTempRealWireNode(int wireLogo,  vector<Node>& sus);//����ʵ���ҵ㴴��δ���ѵĻ����ڵ����ں���ȷ��������İ�װλ��
	
	void FindRealSus(int wireLogo, int choose, vector<Node>m_Nodes); //choose=0;1-���ţ�choose=1;2-���ţ�choose=2;1��2-������
	void CreatSpacer(vector<Element_Truss>& m_Elements_Truss, int SectionId,vector<int> ids);//������ɻ� ���ɼ����
	void CreateStrainLine(vector<Element_Beam>& Temp_Truss, double x,double y,double z,vector<int> ids,int ClassId);//�����Ŵ��Ļ�����һ������
	void CreateSpacerDistance(vector<Node>m_TempNodes, int wireLogo);//ȷ��ÿ���������ǰ��˵�ľ���
	vector<int> FindSpacerL(vector<Node>m_TempNodes, int d, int L, int wireLogo);
	//��������ֲ���
	int ChooseWay = 0;//�������װ��ʽ 0-�ȼ�� 1-���ȼ��
	std::vector<int>SpacerNum;//�洢���������

	map<int, vector<Node>>m_Str_realSus;
	map<int, vector<Node>>OneWireLine;
	void CreateStrainTempWireNode(int wireLogo, int choose, int n,double k, vector<Node>& sus, vector<WireProperties> pro);
	void FindStrainRealSus(int wireLogo, int num, int n, vector<Node>m_Nodes);
	void CreateStrainTempRealWireNode(int nz, int wireLogo,double k, int SectionId, vector<Node>& sus);
	void CreateStrainSpacerDistance(int nz, vector<Node>m_TempNodes, vector<Node>sus, int SectionId,int wireLogo);//ȷ��ÿ���������ǰ��˵�ľ��� //f
	vector<int> FindSpacerSpacerL(vector<Node>m_TempNodes, vector<Node>sus, int d, int L, int wireLogo); //f
};

