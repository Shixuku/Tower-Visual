#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "dll_ansys_global.h"

class Node;

struct dataSet
{
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

class DLL_ANSYS_EXPORT Outputter
{//保存计算结果每一帧的数据
	friend class AnalysisStep;
public:
	Outputter();

	//每一帧节点数据
	std::map<int, dataSet> nodeData;


	void SaveNodeData(Node* node);

	double getNodeDisplaymentX(int idNode) const;
	double getNodeDisplaymentY(int idNode) const;
	double getNodeDisplaymentZ(int idNode) const;

	void SortData(); //找数据的最值

	std::vector<double> getBoundaryDisplaymentX() const { return boundaryDisplaymentX; }
	std::vector<double> getBoundaryDisplaymentY() const { return boundaryDisplaymentY; }
	std::vector<double> getBoundaryDisplaymentZ() const { return boundaryDisplaymentZ; }
	std::vector<double> getBoundaryStressN() const { return boundaryStressN; }
	std::vector<double> getBoundaryStressM1() const { return boundaryStressM1; }
	std::vector<double> getBoundaryStressM2() const { return  boundaryStressM2; }
	std::vector<double> getBoundaryStressM3() const { return  boundaryStressM3; }
private:
	static bool cmp_X(const std::pair<int, dataSet>& left, const std::pair<int, dataSet>& right)
	{//X 位移
		return left.second.displaymentX < right.second.displaymentX;
	}

	static bool cmp_Y(const std::pair<int, dataSet>& left, const std::pair<int, dataSet>& right)
	{//Y 位移
		return left.second.displaymentY < right.second.displaymentY;
	}

	static bool cmp_Z(const std::pair<int, dataSet>& left, const std::pair<int, dataSet>& right)
	{//Z 位移
		return left.second.displaymentZ < right.second.displaymentZ;
	}

	static bool cmp_N(const std::pair<int, dataSet>& left, const std::pair<int, dataSet>& right)
	{//S11 
		return left.second.stressN < right.second.stressN;
	}

	//第一个元素为最小值，第二个元素为最大值
	std::vector<double> boundaryDisplaymentX{ 0,0 };
	std::vector<double> boundaryDisplaymentY{ 0,0 };
	std::vector<double> boundaryDisplaymentZ{ 0,0 };

	std::vector<double> boundaryStressN{ 0,0 };
	std::vector<double> boundaryStressM1{ 0,0 };
	std::vector<double> boundaryStressM2{ 0,0 };
	std::vector<double> boundaryStressM3{ 0,0 };

};