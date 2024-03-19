#pragma once
#include"Instance.h"
#include"Node.h"
class WireData:public Instance
{
public:
	int fenlie = 0;
	double unitMass = 0;//单位质量
	double area = 0;//截面积
	int N = 100; //导线分段数
	double rou = 0;//bizai
	double stress = 0;//yingli
	double strainLength = 0;//耐张串长度
	double sag = 0;//弧垂
	double angle;
	vector<Node>WireListSus;//列表悬挂点容器
	vector<Node>WireRealSus;//真实悬挂点容器
	vector<Node>allSus;//真实悬挂点加上列表中首位两个点
	//vector<Node> TempListNodes;//节点合集
	//vector<Node> TempRealNodes;//节点合集
	int WireSectionId = 0;//导线截面编号
	int InsulatorSectionId = 0;//绝缘子截面编号
	int SpacerSectionId = 0;//间隔棒截面编号
	int wireQty = 0;;//总线路数
	vector<int >endpoinType1 ;//线路端点一的类型（0为挂绝缘子上，1为耐张-挂塔上）
	vector<int> endpoinType2 ;//线路端点二的类型（0为挂绝缘子上，1为耐张-挂塔上）
	int Creat_Node(double x, double y, double z, double f,int Type);
	int GetMidId(double x, double y, double z);
	void CreatRealNode();//创建真实悬挂点
	void CreateTempWireNode(int wireLogo, int choose, vector<Node>& sus);//由列表悬挂点创建为分裂的基础节点
	void CreateTempRealWireNode(int wireLogo,  vector<Node>& sus);//由真实悬挂点创建未分裂的基础节点用于后续确定间隔棒的安装位置
	
	void FindRealSus(int wireLogo, int choose, vector<Node>m_Nodes); //choose=0;1-耐张，choose=1;2-耐张，choose=2;1，2-均耐张
	void CreatSpacer(vector<Element_Truss>& m_Elements_Truss, int SectionId,vector<int> ids);//几个点成环 生成间隔棒
	void CreateStrainLine(vector<Element_Beam>& Temp_Truss, double x,double y,double z,vector<int> ids,int ClassId);//将耐张串的环连在一个点上
	void CreateSpacerDistance(vector<Node>m_TempNodes, int wireLogo);//确定每个间隔棒离前面端点的距离
	vector<int> FindSpacerL(vector<Node>m_TempNodes, int d, int L, int wireLogo);
	//间隔棒部分参数
	int ChooseWay = 0;//间隔棒安装方式 0-等间距 1-不等间距
	std::vector<int>SpacerNum;//存储间隔棒个数

	map<int, vector<Node>>m_Str_realSus;
	map<int, vector<Node>>OneWireLine;
	void CreateStrainTempWireNode(int wireLogo, int choose, int n,double k, vector<Node>& sus, vector<WireProperties> pro);
	void FindStrainRealSus(int wireLogo, int num, int n, vector<Node>m_Nodes);
	void CreateStrainTempRealWireNode(int nz, int wireLogo,double k, int SectionId, vector<Node>& sus);
	void CreateStrainSpacerDistance(int nz, vector<Node>m_TempNodes, vector<Node>sus, int SectionId,int wireLogo);//确定每个间隔棒离前面端点的距离 //f
	vector<int> FindSpacerSpacerL(vector<Node>m_TempNodes, vector<Node>sus, int d, int L, int wireLogo); //f
};

