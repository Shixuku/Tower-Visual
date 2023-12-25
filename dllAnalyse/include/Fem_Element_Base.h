#pragma once
#include "Element_Base.h"
#include <Eigen/Sparse> //ʹ��ϡ������� SparseMatrix
#include <list>

typedef Eigen::SparseMatrix<double> SpMat;//��˵��������ϡ�����
typedef Eigen::Triplet<double> Tri;//��˵����Ԫ��

using namespace Eigen;

class Fem_Element_Base : public Element_Base
{//��Ԫ����
public:
	int idWind = -1; //������� -1��ʾ���ܵ�����

	QString element_type;

	vector<int> EI; //��Ԫ���ɶ�
	MatrixXd Kr; //��Ԫ�նȾ���

	MatrixXd Kt; //��Ԫ���߸նȾ���

	void isSyssm()//�Գƻ�
	{
		Kr = 0.5 * (Kr + Kr.transpose());
	}

	double FN0 = 0; //��Ԫ��ʼ����
	Vector3d Ln, L; //���κ󳤶ȡ�ԭ��
	Eigen::VectorXd element_GollapingForce; //��Ԫ�������ص�Ч�ڵ����
	Eigen::VectorXd element_WindForce; //��Ԫ����ص�Ч�ڵ����
	Eigen::VectorXd element_iceForce; //��Ԫ�����ص�Ч�ڵ����
	Eigen::VectorXd element_tempForce; //��Ԫ�¶Ⱥ��ص�Ч�ڵ����

	double attack0;//��ʼ�繥��
	virtual double Get_Attack_Angle(const double r, const double Uz) = 0;//��÷繥��

	double ratio_stress = 0;
	virtual double getStress_n() { return ratio_stress; } //��õ�Ԫ����Ӧ��
	virtual void Calculate_ratio_stress() = 0;

	virtual void Input(QDataStream& fin);
	virtual void Input(QTextStream& fin);
	virtual void Disp();
	virtual void SaveTo(QDataStream& fin) const;

	void Set_Number_NodeDOF();//���ýڵ�����ɶȸ���

	virtual int Get_nDOF_Node() = 0;//���ֵ�Ԫ��ÿ���ڵ�����ɶȸ���

	void Set_DOF();
	virtual void Calculate_ke(MatrixXd& ke) = 0;
	virtual void Calculate_me(MatrixXd& me) = 0;
	void Assemble_Kt(std::list<Tri>& list_Kt) const;
	void Assemble_SparseKs(std::list<Tri>& K11, std::list<Tri>& K21, std::list<Tri>& K22) const;

	virtual void Get_Element_GollapingForce() = 0;//���㵥Ԫ��������(�趯����)
	virtual void Get_Element_WindForce(const double t) = 0;//���㵥Ԫ�����(��ƫ����)


	void Assemble_GollapingForce(VectorXd& windForce);//��װ����أ��趯��
	void Assemble_WindForce(VectorXd& windForce);//��װ����أ���ƫ��
	void Assemble_iceForce(VectorXd& iceForce); //��װ������
	void Assemble_tempForce(VectorXd& tempForce); //��װ�¶Ⱥ���

	void Assemble_equivalentForce(VectorXd& eqForce); //��װ��Ч�ڵ����
};

