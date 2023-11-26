#pragma once
#ifdef MATHLIBRARY_EXPORTS
#define MATHLIBRARY_API __declspec(dllimport)
#else
#define MATHLIBRARY_API __declspec(dllexport)
#endif


//MATHLIBRARY_API void run_procedure(QString& input);
//MATHLIBRARY_API Outputter& get_outputter();

//MATHLIBRARY_API Structure* GetStructure(QString& input);



#include <vector>
#include "Node.h"
#include <QString>

class Node_Base;
class Fem_Element_Base;
class Element_Base;
class Outputter_ice;

template<class ET>
class Manage_Entity;

class MATHLIBRARY_API S_InterFace_ice
{
public:
	//enum Element {
	//	Beam_CR, Beam_simo, Beam_CR_simo, Truss_3D, Cable3N, Cable_CR
	//};

	//接口

	////克隆
	//virtual S_InterFace_ice* clone() const = 0;

	////以文件形式导入计算模型
	//virtual void Input(QString& FileName) = 0;

	//内部节点信息转化（将用于计算的节点转化为仅含位置信息的节点）
	//virtual void transToNode(Manage_Entity<Node>& Nodes) = 0;
	////内部节点信息转化（将用于计算的节点转化为仅含位置信息的节点）
	//virtual void transToElement_Truss(Manage_Entity<Element_Truss>& Element_Truss) = 0;
	//virtual void transToElement_Beam(Manage_Entity<Element_Beam>& Element_Truss) = 0;

	//读取inp文件
	virtual bool Input_FemData(const QString& FileName) = 0;

	virtual void Solve() = 0;
	virtual void set_id(int id) = 0;
	virtual void set_Name(QString name)=0;
	virtual void AddToStructures() =0;
	////读取标准txt文件
	//virtual void Input_Standard(QString& filename) = 0;
	////坐标系转换
	////virtual void TransCoordinateSystem() = 0;
	////过滤无效数据
	////virtual void DataFilter() = 0;

	////获得idStep分析步输出结果
	virtual Outputter_ice& get_outputter(int idStep) = 0;

	//设置是否使用常数矩阵（简化计算）
	//virtual void set_ConstK(bool flag) = 0;

	////设置是否使用稀疏矩阵
	//virtual void set_Sparse(bool flag) = 0;

	////设置newmark法的参数β
	//virtual void set_beta(double beta) = 0;
	////设置Newmark法的参数γ
	//virtual void set_gama(double gama) = 0;

	////设置单元类型
	//virtual void setElementType(Element element) = 0;

	////按分析步计算
	//virtual void execute_Stepping() = 0;

	//////设置时间步长
	//// irtual void setTimeStep(double dt) = 0;
	//////设置最大迭代数
	//// irtual void setMaxIterations(int iterations) = 0;

	////得到总的迭代次数
	//virtual int getTotalIterations() const = 0;
	////得到总的分析步数
	//virtual int getTotalSteps() const = 0;

	//////设置迭代增量的模的最小容许值
	// virtual void set_stol(double s) = 0;
	//////残差力的模的最小容许值
	// virtual void set_atol(double a) = 0;
	//////时间步内当前残差与初始残差之比的最小容许值
	// virtual void set_rtol(double r) = 0;
	//////残差增量的模的最小容许值
	// virtual void set_inftol(double i) = 0;

	//返回节点集合
    virtual std::list<std::vector<double>>Get_node_ice() = 0;
    virtual std::list<std::vector<double>>Get_node_ice1() = 0;
	virtual std::list<std::vector<int>>Get_truss_ice() = 0;
	virtual std::list<std::vector<int>>Get_beam_ice() = 0;
	//输出节点集合，单元集合
	//返回单元集合
	//virtual std::map<int, Fem_Element_Base*>& GetElements() = 0;

	//虚析构函数 --调用派生类析构函数
	virtual ~S_InterFace_ice() {}
};


MATHLIBRARY_API S_InterFace_ice* GetStructure_ice();

