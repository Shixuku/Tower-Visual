#pragma once
#include"WireData.h"
class CreateStrainWire :public WireData
{
public:
	void CreateInsulatorSus(int fenlie, map<int, vector<Insulator_Base>>& SusInfor);
	void CreateRealSus();//������ʵ���ҵ�����ѵ��ߵ�
	void CreateStrainWireInfor(vector<WireProperties>pro);//�������Ŷ˵ĵ��߽ڵ�͵�Ԫ��Ϣ
	void CreateStrainString(int id);//�������Ŵ�
	void CreateGWire(vector<WireProperties>pro);
	void  Create_Mesh();
	void Type_WO(int i,int StrainId, const vector<Insulator_Base>&  data, map<int, vector<Insulator_Base>>& RealSusInfor);
	void Type_WH_1(int i, int StrainId, const vector<Insulator_Base>& data, vector<int>&Id, map<int, vector<Insulator_Base>>& RealSusInfor);
	void Type_WH_4(int i, int StrainId,double Angle, const vector<Insulator_Base>& data, vector<int>& Id, vector<int>& SpacerId, map<int, vector<Insulator_Base>>& RealSusInfor);
	void Type_WH_6(int i, int StrainId, double Angle, const vector<Insulator_Base>& data, vector<int>& Id, vector<int>& SpacerId, map<int, vector<Insulator_Base>>& RealSusInfor);
	void Type_WV_1(int i, int StrainId, double Angle, const vector<Insulator_Base>& data, vector<int>& V_Id1, vector<int>& V_Id2, vector<int>& SpacerId, map<int, vector<Insulator_Base>>& RealSusInfor);
	void Type_WV_4(int i, int StrainId, double Angle, const vector<Insulator_Base>& data, vector<int>& V_Id1, vector<int>& V_Id2, vector<int>& SpacerId, map<int, vector<Insulator_Base>>& RealSusInfor);
	void Type_GO(int i, int StrainId, const vector<Insulator_Base>& data, map<int, vector<Insulator_Base>>& G_RealSusInfor);
	void Type_G(int i, int StrainId, const vector<Insulator_Base>& data, vector<int>& Id, map<int, vector<Insulator_Base>>& G_RealSusInfor);
	vector<int>wireToGroup;
	int NumberOfLine = 0;//����·����
	int wireNumberOfLine = 0;//��������
	int row = 0;//һ�����Ŷ�һ����·�Ĺҵ����
	vector<int>xuanchui;
	vector<int>G_xuanchui;
	vector<int>V;
	vector<int>InsulatorId;
	vector<int>V_InsulatorId;
	vector<double>m_Angle;
	map<int, vector<Node>>EndPoint; 
	int FindGroupIdNode(int idNode) const;
	static int GetSectionId(QString MaterialName, QString SectionName);
	int O_Id = 0;
	int H_Id = 0;
	int V_Id = 0;
	int S_Id = 0;

};

