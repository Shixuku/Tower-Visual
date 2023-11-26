#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <Qstring>
#include "dll.h"
class Node;
class Element_Truss;

enum class DataType_ice :int {
	U1, U2, U3, UR1, UR2, UR3, N, M2, M3
};
struct exchangeType_ice
{
	exchangeType_ice(const Element_Truss& element_truss);

	//交换的数据
	double Force_Ln = 0;
	double exchange_X = 0;
	double exchange_Y = 0;
	double exchange_Z = 0;
	
};
struct NodeData_ice
{
	NodeData_ice(const Node& node);


	//位移
	double displaymentX = 0;
	double displaymentY = 0;
	double displaymentZ = 0;
	//转角
	double rotationX = 0;
	double rotationY = 0;
	double rotationZ = 0;
	//内力
	double stressN = 0;
	double stressM1 = 0;
	double stressM2 = 0;
	double stressM3 = 0;
};

struct DataFrame_ice
{
	double currentTime = 0;
	//每一帧节点数据
	std::map<int, NodeData_ice> nodeDatas;//int 为节点的编号 dataSet 为对应节点的数据
	std::map<int, exchangeType_ice> exchangeType_Datas;//int 为节点的编号 dataSet 为对应节点的数据

	double getNodeData(int idNode, DataType_ice iType);
	double getMaxNodeData(DataType_ice iType);
	double get_max_id(DataType_ice iType);
};


class MATHLIBRARY_API Outputter_ice
{//保存计算结果每一帧的数据
	//friend class AnalysisStep;
public:
	Outputter_ice();
	~Outputter_ice();
	int m_idStep;

	std::vector<DataFrame_ice*> dataSet; //数据集合


	void SaveNodesData(double time, const std::vector<Node>& nodes,const std::vector<Element_Truss>& element_truss);
	void FindBoundary();

	inline void FindBoundary_Type(std::vector<double>& boundary, const double min, const double max)
	{
		if (boundary[0] > min)
		{
			boundary[0] = min;
		}
		if (boundary[1] < max)
		{
			boundary[1] = max;
		}
	}

	void outputNodeData(int idNode, std::vector<int>& iTypes);
	void outputNodeData_time(double  idTime, std::vector<int>& iTypes);
	void output_Exchange(int idNode, int idelement, QString filename);

	void seFileName(const QString& fileName) {
		m_FileName = fileName;
	}

	//double getNodeDisplaymentX(int idNode) const;
	//double getNodeDisplaymentY(int idNode) const;
	//double getNodeDisplaymentZ(int idNode) const;

	std::vector<double> getBoundaryDisplaymentX() const { return boundaryDisplaymentX; }
	std::vector<double> getBoundaryDisplaymentY() const { return boundaryDisplaymentY; }
	std::vector<double> getBoundaryDisplaymentZ() const { return boundaryDisplaymentZ; }

	std::vector<double> getBoundaryRotationX() const { return boundaryRotationX; }
	std::vector<double> getBoundaryRotationY() const { return boundaryRotationY; }
	std::vector<double> getBoundaryRotationZ() const { return boundaryRotationZ; }

	std::vector<double> getBoundaryStressN() const { return boundaryStressN; }
	std::vector<double> getBoundaryStressM1() const { return boundaryStressM1; }
	std::vector<double> getBoundaryStressM2() const { return  boundaryStressM2; }
	std::vector<double> getBoundaryStressM3() const { return  boundaryStressM3; }
private:

	//第一个元素为最小值，第二个元素为最大值
	std::vector<double> boundaryDisplaymentX;
	std::vector<double> boundaryDisplaymentY;
	std::vector<double> boundaryDisplaymentZ;

	std::vector<double> boundaryRotationX;
	std::vector<double> boundaryRotationY;
	std::vector<double> boundaryRotationZ;

	std::vector<double> boundaryStressN;
	std::vector<double> boundaryStressM1;
	std::vector<double> boundaryStressM2;
	std::vector<double> boundaryStressM3;


	QString m_FileName = nullptr;
};