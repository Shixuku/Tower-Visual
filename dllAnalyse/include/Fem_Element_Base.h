#pragma once
#include "Element_Base.h"
#include <Eigen/Sparse> //使用稀疏矩阵类 SparseMatrix
#include <list>

typedef Eigen::SparseMatrix<double> SpMat;//重说明列优先稀疏矩阵
typedef Eigen::Triplet<double> Tri;//重说明三元组

using namespace Eigen;

class Fem_Element_Base : public Element_Base
{//单元基类
public:
	int idWind = -1; //风区编号 -1表示不受到风载

	QChar element_type;

	vector<int> EI; //单元自由度
	MatrixXd Kr; //单元刚度矩阵

	void isSyssm()//对称化
	{
		Kr = 0.5 * (Kr + Kr.transpose());
	}

	double FN0 = 0; //单元初始轴力
	Vector3d Ln, L; //变形后长度、原长
	Eigen::VectorXd element_GollapingForce; //单元气动荷载等效节点荷载
	Eigen::VectorXd element_WindForce; //单元风荷载等效节点荷载
	double attack0;//初始风攻角
	virtual double Get_Attack_Angle(const double r, const double Uz) = 0;//获得风攻角

	virtual double getStress_n() const = 0; //获得单元轴向应力


	virtual void Input(QDataStream& fin);
	virtual void Input(QTextStream& fin);
	virtual void Disp();
	virtual void SaveTo(QDataStream& fin) const;

	void Set_Number_NodeDOF();//设置节点的自由度个数

	virtual int Get_nDOF_Node() = 0;//这种单元，每个节点的自由度个数

	void Set_DOF();
	virtual void Calculate_ke(MatrixXd& ke) = 0;
	virtual void Calculate_me(MatrixXd& me) = 0;
	void Assemble_Ks(MatrixXd& Ks) const;
	void Assemble_SparseKs(std::list<Tri>& K11, std::list<Tri>& K21, std::list<Tri>& K22) const;

	virtual void Get_Element_GollapingForce() = 0;//计算单元气动荷载(舞动计算)
	virtual void Get_Element_WindForce(const double t) = 0;//计算单元风荷载(风偏计算)


	void Assemble_GollapingForce(VectorXd& windForce);//组装风荷载（舞动）
	void Assemble_WindForce(VectorXd& windForce);//组装风荷载（风偏）

};

