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

	//�ӿ�

	////��¡
	//virtual S_InterFace_ice* clone() const = 0;

	////���ļ���ʽ�������ģ��
	//virtual void Input(QString& FileName) = 0;

	//�ڲ��ڵ���Ϣת���������ڼ���Ľڵ�ת��Ϊ����λ����Ϣ�Ľڵ㣩
	//virtual void transToNode(Manage_Entity<Node>& Nodes) = 0;
	////�ڲ��ڵ���Ϣת���������ڼ���Ľڵ�ת��Ϊ����λ����Ϣ�Ľڵ㣩
	//virtual void transToElement_Truss(Manage_Entity<Element_Truss>& Element_Truss) = 0;
	//virtual void transToElement_Beam(Manage_Entity<Element_Beam>& Element_Truss) = 0;

	//��ȡinp�ļ�
	virtual bool Input_FemData(const QString& FileName) = 0;

	virtual void Solve() = 0;
	virtual void set_id(int id) = 0;
	virtual void set_Name(QString name)=0;
	virtual void AddToStructures() =0;
	////��ȡ��׼txt�ļ�
	//virtual void Input_Standard(QString& filename) = 0;
	////����ϵת��
	////virtual void TransCoordinateSystem() = 0;
	////������Ч����
	////virtual void DataFilter() = 0;

	////���idStep������������
	virtual Outputter_ice& get_outputter(int idStep) = 0;

	//�����Ƿ�ʹ�ó������󣨼򻯼��㣩
	//virtual void set_ConstK(bool flag) = 0;

	////�����Ƿ�ʹ��ϡ�����
	//virtual void set_Sparse(bool flag) = 0;

	////����newmark���Ĳ�����
	//virtual void set_beta(double beta) = 0;
	////����Newmark���Ĳ�����
	//virtual void set_gama(double gama) = 0;

	////���õ�Ԫ����
	//virtual void setElementType(Element element) = 0;

	////������������
	//virtual void execute_Stepping() = 0;

	//////����ʱ�䲽��
	//// irtual void setTimeStep(double dt) = 0;
	//////������������
	//// irtual void setMaxIterations(int iterations) = 0;

	////�õ��ܵĵ�������
	//virtual int getTotalIterations() const = 0;
	////�õ��ܵķ�������
	//virtual int getTotalSteps() const = 0;

	//////���õ���������ģ����С����ֵ
	// virtual void set_stol(double s) = 0;
	//////�в�����ģ����С����ֵ
	// virtual void set_atol(double a) = 0;
	//////ʱ�䲽�ڵ�ǰ�в����ʼ�в�֮�ȵ���С����ֵ
	// virtual void set_rtol(double r) = 0;
	//////�в�������ģ����С����ֵ
	// virtual void set_inftol(double i) = 0;

	//���ؽڵ㼯��
    virtual std::list<std::vector<double>>Get_node_ice() = 0;
    virtual std::list<std::vector<double>>Get_node_ice1() = 0;
	virtual std::list<std::vector<int>>Get_truss_ice() = 0;
	virtual std::list<std::vector<int>>Get_beam_ice() = 0;
	//����ڵ㼯�ϣ���Ԫ����
	//���ص�Ԫ����
	//virtual std::map<int, Fem_Element_Base*>& GetElements() = 0;

	//���������� --������������������
	virtual ~S_InterFace_ice() {}
};


MATHLIBRARY_API S_InterFace_ice* GetStructure_ice();

