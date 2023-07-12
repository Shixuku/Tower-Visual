#pragma once
#include "Node_Base.h"
#include <Eigen/Dense>
#include <QtCharts/QLineSeries>
#include <QDebug>

QT_CHARTS_USE_NAMESPACE

using namespace Eigen;

#define pi std::acos(-1)
class DLL_ANSYS_EXPORT Force_Wind : public EntityBase
{//�����
public:
	Force_Wind() :m_pair(1.29), alf(0.16), z0(0.03), v10(30){}

	Force_Wind(double p_air, double alf, double z0, double v10, int N, int M, double w_up, double T)
	{//�����ܶȣ�����ֲڶȣ��ر�ֲڳ��ȣ�10m��10����ƽ������(m/s)��Ƶ�ʵȷ����� M >= 2 * N����Ƶ��ֹ�㣬ģ����ʱ
		this->m_pair = p_air;
		this->alf = alf;
		this->z0 = z0;
		this->v10 = v10;
		this->N = N;
		this->M = M;
		this->w_up = w_up;
		this->T = T;
	}

	void Input(QDataStream& fin);
	void Input(QTextStream& fin);
	void Disp();
	double Get_Windvelocity();

	double m_pair;//�����ܶ�
	double alf;/*����ֲڶȣ�a���ò���������棬ɳĮ��ȡ0.12��b���ò����Ұ����壩ȡ0.16��
	c���ò���ܼ�����Ⱥ��������ȡ0.20��d���ò���ܼ��߽���Ⱥ��ȡ0.30��*/
	double z0; //�ر�ֲڳ��ȣ�m���غ���0.005 - 0.02����������0.03 - 0.1�������ﲻ��Ľ���0.2 - 0.4; ���������2.0 - 3.0��
	double v10;//10m��10����ƽ�����٣�m/s

	int N = 3000; //Ƶ�ʵȷ���
	double w_up = 2 * pi; //��Ƶ��ֹ��
	double T = 600; //ģ����ʱ
	int M = 6000; // M >= 2 * N

	std::vector<Node_Base> Points_set; //ģ��㼯��
	QString m_FilePath;
	//�����ļ�·��
	void set_FilePath(const QString& FilePath) { m_FilePath = FilePath; }
	//����ģ������ļ���ʽ
	void input_node(const QString& FileName);
	//����ģ���
	void input_node(const vector<Node_Base>& inputNodes);
	//�����������
	void out_data_time(const QString& FileName, const MatrixXd& data, const double& dt);
	void simulate();
	bool verify = false;

	vector<double> U_z(const vector<double>& z); //����ÿ��ģ����ƽ������
	MatrixXcd S0w_f(const double w, const vector<double>& Uz, const vector<double>& y, const vector<double>& z);
	double kaimal(const double w, const double Uzi, const double zi);
	double ch(const double w, const double yi, const double zi, const double yj, const double zj);
	MatrixXcd Bjm(const vector<double>& Uz, const vector<double>& y, const vector<double>& z, const double dw);

	QLineSeries* getQLineSeries(int idNode)const;
	MatrixXd getVelocityMat()const { return f; }
	MatrixXd getMeanVelocityMat()const { return v; }//˲ʱ����

private:
	MatrixXd f;//��������ʱ��
	MatrixXd v;//˲ʱ����ʱ��v = ƽ������ + f
	MatrixXd v_2;//˲ʱ���ٵ�ƽ��

	virtual enum Entity_Type My_EntityType() const
	{
		return ET_Force_Wind;
	}
};

