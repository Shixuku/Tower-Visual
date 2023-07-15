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

	//�ӿ�

	//���ļ���ʽ�������ģ��
	virtual void Input(QString& FileName) = 0;

	//�ڲ��ڵ���Ϣת���������ڼ���Ľڵ�ת��Ϊ����λ����Ϣ�Ľڵ㣩
	virtual void transToNodeBase(Manage_Entity<Node_Base>& Nodes) = 0;
	//�ڲ��ڵ���Ϣת���������ڼ���Ľڵ�ת��Ϊ����λ����Ϣ�Ľڵ㣩
	virtual void transToElementBase(Manage_Entity<Element_Base>& Elements) = 0;

	//��ȡinp�ļ�
	virtual void Input_inp(QString& filename) = 0;
	//��ȡ��׼txt�ļ�
	virtual void Input_Standard(QString& filename) = 0;

	//��������ļ���·���ͽڵ��
	virtual void set_outInfo(QString filePath, std::vector<int> pid) = 0;

	//�����Ƿ�ʹ�ó������󣨼򻯼��㣩
	virtual void set_ConstK(bool flag) = 0;

	//�����Ƿ�ʹ��ϡ�����
	virtual void set_Sparse(bool flag) = 0;

	//�����Ƿ������ƽ��
	virtual void set_CalculateEq(bool flag) = 0;
	//���ü����ѱ�
	virtual void set_Ice_shedding(bool flag) = 0;
	//���ü����趯
	virtual void set_Galloping(bool flag) = 0;
	//���ü����ƫ
	virtual void set_Windage(bool flag) = 0;

	//����newmark���Ĳ�����
	virtual void set_beta(double beta) = 0;
	//����Newmark���Ĳ�����
	virtual void set_gama(double gama) = 0;
	//���������
	virtual void setDiffEqSolver(Method method) = 0;
	//�����
	virtual Method getMethod() = 0;
	//���õ�Ԫ����
	virtual void setElementType(Element element) = 0;

	// �������λ��ʱ�����ߣ��ڵ�ţ�����1��2��3��
	virtual void out_disptime(QString filePath, int pid, bool direction1, bool direction2, bool direction3) const = 0;
	//virtual void out_stress(int pid, bool direction1, bool direction2, bool direction3) const = 0;

	//����
	virtual void  execute() = 0;
	//������ƽ��
	virtual void  executeEq() = 0;

	//���ü��ز���
	virtual void setStep(int N) = 0;
	//����ʱ�䲽��
	virtual void setTimeStep(double dt) = 0;
	//������������
	virtual void setMaxIterations(int iterations) = 0;
	//�õ��ܵĵ�������
	virtual int getTotalIterations() const = 0;

	//���õ���������ģ����С����ֵ
	virtual void set_stol(double s) = 0;
	//�в�����ģ����С����ֵ
	virtual void set_atol(double a) = 0;
	//ʱ�䲽�ڵ�ǰ�в����ʼ�в�֮�ȵ���С����ֵ
	virtual void set_rtol(double r) = 0;
	//�в�������ģ����С����ֵ
	virtual void set_inftol(double i) = 0;

	//���ؽڵ㼯��
	virtual std::vector<Node_Base*> GetNodes() = 0;
};


DLL_ANSYS_EXPORT S_InterFace* GetStructure();




