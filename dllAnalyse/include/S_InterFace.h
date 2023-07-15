#pragma once

#include "dll_ansys_global.h"
#include <vector>

class Node;
class Node_Base;
class Element_Base;

template<class ET>
class Manage_Entity;

class DLL_ANSYS_EXPORT S_InterFace
{
public:
	enum Method { STATICS, DYNAMIC_IMPLICIT, NONE };
	enum Element {
		Beam_CR, Beam_simo, Beam_CR_simo, Truss_3D, Cable3N, Cable_CR
	};

	//接口

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

	//设置输出文件的路径和节点号
	virtual void set_outInfo(QString filePath, std::vector<int> pid) = 0;

	//设置是否使用常数矩阵（简化计算）
	virtual void set_ConstK(bool flag) = 0;

	//设置是否使用稀疏矩阵
	virtual void set_Sparse(bool flag) = 0;

	//设置是否计算自平衡
	virtual void set_CalculateEq(bool flag) = 0;
	//设置计算脱冰
	virtual void set_Ice_shedding(bool flag) = 0;
	//设置计算舞动
	virtual void set_Galloping(bool flag) = 0;
	//设置计算风偏
	virtual void set_Windage(bool flag) = 0;

	//设置newmark法的参数β
	virtual void set_beta(double beta) = 0;
	//设置Newmark法的参数γ
	virtual void set_gama(double gama) = 0;
	//设置求解器
	virtual void setDiffEqSolver(Method method) = 0;
	//求解器
	virtual Method getMethod() = 0;
	//设置单元类型
	virtual void setElementType(Element element) = 0;

	// 设置输出位移时程曲线（节点号，方向1、2、3）
	virtual void out_disptime(QString filePath, int pid, bool direction1, bool direction2, bool direction3) const = 0;
	//virtual void out_stress(int pid, bool direction1, bool direction2, bool direction3) const = 0;

	//计算
	virtual void  execute() = 0;
	//计算自平衡
	virtual void  executeEq() = 0;

	//设置加载步数
	virtual void setStep(int N) = 0;
	//设置时间步长
	virtual void setTimeStep(double dt) = 0;
	//设置最大迭代数
	virtual void setMaxIterations(int iterations) = 0;
	//得到总的迭代次数
	virtual int getTotalIterations() const = 0;

	//设置迭代增量的模的最小容许值
	virtual void set_stol(double s) = 0;
	//残差力的模的最小容许值
	virtual void set_atol(double a) = 0;
	//时间步内当前残差与初始残差之比的最小容许值
	virtual void set_rtol(double r) = 0;
	//残差增量的模的最小容许值
	virtual void set_inftol(double i) = 0;

	//返回节点集合
	virtual std::vector<Node_Base*> GetNodes() = 0;
};


DLL_ANSYS_EXPORT S_InterFace* GetStructure();




