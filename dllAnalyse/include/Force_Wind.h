#pragma once
#include "Node_Base.h"
#include <QtCharts/QLineSeries>
#include <Eigen/Dense>
#include <QDebug>

QT_CHARTS_USE_NAMESPACE

using namespace Eigen;

class Fem_Element_Base;

#define pi std::acos(-1)
class DLL_ANSYS_EXPORT Force_Wind : public EntityBase
{//�����

	friend class Structure;
public:
	Force_Wind() :m_pair(1.29), alf(0.16), z0(0.03), v10(10){}

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
	MatrixXd getVelocityMat()const { return v; }//˲ʱ����

	void SelectAlf(int iSelect) { select = iSelect; }//ѡ���ò

	void Get_Cyzm(const double attack_Angle, double& Cy, double& Cz, double& Cm);//������������
	double CountMiu_h(const double h);//�����ѹϵ��
	double wind_coeff = 0;//�����ϵ��
private:
	MatrixXd f;//��������ʱ��
	MatrixXd v;//˲ʱ����ʱ��v = ƽ������ + f
	MatrixXd v_2;//˲ʱ���ٵ�ƽ��

	void Get_WindCoeff(double W0, double afa, double beta_c, double miu_sc, double B, double theta);//��������ϵ��(��ƫ����)
	void Get_WindCoeff();
	int select = 1;//ѡ���ò

	virtual enum Entity_Type My_EntityType() const
	{
		return ET_Force_Wind;
	}
};

