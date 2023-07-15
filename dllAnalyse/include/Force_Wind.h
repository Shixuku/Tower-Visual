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
{//风荷载

	friend class Structure;
public:
	Force_Wind() :m_pair(1.29), alf(0.16), z0(0.03), v10(10){}

	Force_Wind(double p_air, double alf, double z0, double v10, int N, int M, double w_up, double T)
	{//空气密度，地面粗糙度，地表粗糙长度，10m高10分钟平均风速(m/s)，频率等分数， M >= 2 * N，高频截止点，模拟总时
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

	double m_pair;//空气密度
	double alf;/*地面粗糙度，a类地貌（近海海面，沙漠）取0.12；b类地貌（田野，乡村）取0.16；
	c类地貌（密集建筑群城市区）取0.20；d类地貌（密集高建筑群）取0.30。*/
	double z0; //地表粗糙长度，m，沿海区0.005 - 0.02；开阔场地0.03 - 0.1；建筑物不多的郊区0.2 - 0.4; 大城市中心2.0 - 3.0。
	double v10;//10m高10分钟平均风速，m/s

	int N = 3000; //频率等分数
	double w_up = 2 * pi; //高频截止点
	double T = 600; //模拟总时
	int M = 6000; // M >= 2 * N

	std::vector<Node_Base> Points_set; //模拟点集合
	QString m_FilePath;
	//设置文件路径
	void set_FilePath(const QString& FilePath) { m_FilePath = FilePath; }
	//输入模拟点以文件形式
	void input_node(const QString& FileName);
	//输入模拟点
	void input_node(const vector<Node_Base>& inputNodes);
	//输出风速数据
	void out_data_time(const QString& FileName, const MatrixXd& data, const double& dt);
	void simulate();
	bool verify = false;

	vector<double> U_z(const vector<double>& z); //计算每个模拟点的平均风速
	MatrixXcd S0w_f(const double w, const vector<double>& Uz, const vector<double>& y, const vector<double>& z);
	double kaimal(const double w, const double Uzi, const double zi);
	double ch(const double w, const double yi, const double zi, const double yj, const double zj);
	MatrixXcd Bjm(const vector<double>& Uz, const vector<double>& y, const vector<double>& z, const double dw);

	QLineSeries* getQLineSeries(int idNode)const;
	MatrixXd getVelocityMat()const { return v; }//瞬时风速

	void SelectAlf(int iSelect) { select = iSelect; }//选择地貌

	void Get_Cyzm(const double attack_Angle, double& Cy, double& Cz, double& Cm);//计算气动参数
	double CountMiu_h(const double h);//计算风压系数
	double wind_coeff = 0;//风荷载系数
private:
	MatrixXd f;//脉动风速时程
	MatrixXd v;//瞬时风速时程v = 平均风速 + f
	MatrixXd v_2;//瞬时风速的平方

	void Get_WindCoeff(double W0, double afa, double beta_c, double miu_sc, double B, double theta);//计算风荷载系数(风偏计算)
	void Get_WindCoeff();
	int select = 1;//选择地貌

	virtual enum Entity_Type My_EntityType() const
	{
		return ET_Force_Wind;
	}
};

