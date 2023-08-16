#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <QTextStream>
#include <QFile>
#include "dll_ansys_global.h"
#include <QDebug>

class Node;

enum class DataType :int {
	U1, U2, U3, UR1, UR2, UR3, N, M2, M3, Mises
};

struct NodeData
{
	NodeData(const Node& node);


	//λ��
	double displaymentX = 0;
	double displaymentY = 0;
	double displaymentZ = 0;
	//ת��
	double rotationX = 0;
	double rotationY = 0;
	double rotationZ = 0;
	//����
	double stressN = 0;
	double stressM1 = 0;
	double stressM2 = 0;
	double stressM3 = 0;

	double mises = 0;
};

struct DataFrame
{
	double currentTime = 0;

	//ÿһ֡�ڵ�����
	std::map<int, NodeData> nodeDatas;//int Ϊ�ڵ�ı�� dataSet Ϊ��Ӧ�ڵ������

	double getNodeData(int idNode, DataType iType);
};


class DLL_ANSYS_EXPORT Outputter
{//���������ÿһ֡������
	friend class AnalysisStep;
public:
	Outputter();
	~Outputter();
	int m_idStep;

	std::vector<DataFrame*> dataSet; //���ݼ���


	void SaveNodesData(double time, const std::vector<Node>& nodes);

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
	std::vector<double> getBoundaryMises() const { return  boundaryMises; }
private:

	//��һ��Ԫ��Ϊ��Сֵ���ڶ���Ԫ��Ϊ���ֵ
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
	std::vector<double> boundaryMises;


	QString m_FileName = nullptr;
};