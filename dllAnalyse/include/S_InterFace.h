#pragma once

#include "dll_ansys_global.h"
#include <vector>

class Node_Base;
class Fem_Element_Base;
class Element_Base;
class Outputter;

template<class ET>
class Manage_Entity;

class DLL_ANSYS_EXPORT S_InterFace
{
public:
	enum Element {
		Beam_CR, Beam_simo, Beam_CR_simo, Truss_3D, Cable3N, Cable_CR
	};

	//接口

	//克隆
	virtual S_InterFace* clone() const = 0;

	//设置模型ID
	virtual void set_ID(int id) = 0;
	//设置模型name
	virtual void set_Name(QString name) = 0;
	//初始化数据库表
	virtual void InitMySQLTable(QString db_name, QString db_ip, QString user_name, QString password) = 0;
	//添加到模型库
	virtual void AddToStructures() = 0;

	//以文件形式导入计算模型
	virtual void Input(QString& FileName) = 0;

	//内部节点信息转化（将用于计算的节点转化为仅含位置信息的节点）
	virtual void transToNodeBase(Manage_Entity<Node_Base>& Nodes) = 0;
	//内部节点信息转化（将用于计算的节点转化为仅含位置信息的节点）
	virtual void transToElementBase(Manage_Entity<Element_Base>& Elements) = 0;

	//读取inp文件
	virtual void Input_inp(QString& filename) = 0;
	//读取标准txt文件
	virtual void Input_Standard(QString& filename) = 0;
	//坐标系转换
	virtual void TransCoordinateSystem() = 0;
	//过滤无效数据
	virtual void DataFilter() = 0;

	//获得idStep分析步输出结果
	virtual Outputter& get_outputter(int idStep) = 0;

	//设置是否使用常数矩阵（简化计算）
	virtual void set_ConstK(bool flag) = 0;

	//设置是否使用稀疏矩阵
	virtual void set_Sparse(bool flag) = 0;

	//设置newmark法的参数β
	virtual void set_beta(double beta) = 0;
	//设置Newmark法的参数γ
	virtual void set_gama(double gama) = 0;

	//设置单元类型
	virtual void setElementType(Element element) = 0;

	//按分析步计算
	virtual void execute_Stepping() = 0;

	////设置时间步长
	//virtual void setTimeStep(double dt) = 0;
	////设置最大迭代数
	//virtual void setMaxIterations(int iterations) = 0;

	//得到总的迭代次数
	virtual int getTotalIterations() const = 0;
	//得到总的分析步数
	virtual int getTotalSteps() const = 0;

	////设置迭代增量的模的最小容许值
	//virtual void set_stol(double s) = 0;
	////残差力的模的最小容许值
	//virtual void set_atol(double a) = 0;
	////时间步内当前残差与初始残差之比的最小容许值
	//virtual void set_rtol(double r) = 0;
	////残差增量的模的最小容许值
	//virtual void set_inftol(double i) = 0;

	//返回节点集合
	virtual std::vector<Node_Base*> GetNodes() = 0;
	//返回单元集合
	virtual std::map<int, Fem_Element_Base*>& GetElements() = 0;

	//虚析构函数 --调用派生类析构函数
	virtual ~S_InterFace() {}
};


DLL_ANSYS_EXPORT S_InterFace* GetStructure();




