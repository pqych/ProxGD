#pragma once
using namespace Eigen;
using namespace std;

class Objective
{
public:
	Objective(int mode, MatrixXd A, MatrixXd b);
	double f(MatrixXd x);
	MatrixXd grad_f(MatrixXd x);
	double Frob(MatrixXd x);
	MatrixXd Frob_grad(MatrixXd x);
	double Logistic(MatrixXd x);
	MatrixXd Logistic_grad(MatrixXd x);
	int check(MatrixXd x);
private:
	int mode;
	MatrixXd A;
	MatrixXd b;
};

class Penalty
{
private:
	MatrixXd L1_soft(MatrixXd x, double mu);

public:
	int is_positive(MatrixXd x);
	double L_0(MatrixXd x);
	MatrixXd L_0_prox(MatrixXd x, double mu);
	double L_1(MatrixXd x);
	MatrixXd L_1_prox(MatrixXd x, double mu);
	double L_2(MatrixXd x);
	MatrixXd L_2_prox(MatrixXd x, double mu);
	double L_12(MatrixXd x);
	MatrixXd L_12_prox(MatrixXd x, double mu);
	double L_21(MatrixXd x);
	MatrixXd L_21_prox(MatrixXd x, double mu);
	double L_inf(MatrixXd x);
	MatrixXd L_inf_prox(MatrixXd x, double mu);
	double Nuclear(MatrixXd x);
	MatrixXd Nuclear_prox(MatrixXd x, double mu);
	double GLasso(MatrixXd x, VectorXd w);
	MatrixXd GLasso_prox(MatrixXd x, VectorXd w, double mu);
	double Log_barrier(MatrixXd x);
	MatrixXd Log_barrier_prox(MatrixXd x, double mu);
	double Elastic(MatrixXd x, double alpha);
	MatrixXd Elastic_prox(MatrixXd x, double alpha, double mu);

	MatrixXd Ind_L_0_prox(MatrixXd x, int R);
	MatrixXd Ind_L_1_prox(MatrixXd x, double R);
	MatrixXd Ind_L_F_prox(MatrixXd x, double R);
	MatrixXd Ind_L_inf_prox(MatrixXd x, double R);
	MatrixXd Ind_box_prox(MatrixXd x, MatrixXd L, MatrixXd U);
	MatrixXd Ind_positive_prox(MatrixXd x);
	MatrixXd Ind_negative_prox(MatrixXd x);
	MatrixXd Ind_half_prox(MatrixXd x, MatrixXd A, double alpha);
	MatrixXd Ind_affine_prox(MatrixXd x, MatrixXd A, MatrixXd b);
	MatrixXd Ind_nuclear_prox(MatrixXd x, double R);
	MatrixXd Ind_psd_prox(MatrixXd x);
	MatrixXd Ind_rank_prox(MatrixXd x, int R);
};

class Result
{// Result�������������ļ������������ݰ������ŵ�x����������Сֵ�Լ�����������������໹û�п�ʼд��
public:
	Result(int iter, MatrixXd x, double min_value);
	Result();
	~Result();
	void show();// show����չʾ��������Ŀǰ����չʾiter��min_value
	MatrixXd min_point();
	double min_loss();
	int iterations();
	int modify_iter(int iter);

private:
	MatrixXd x;// ���ŵ�x
	int iter;// ��������
	double min_value;// ��������Сֵ

};

Result ProxGD(int fmode, int hmode, int tmode, MatrixXd A, MatrixXd b, MatrixXd x0, double mu, double epsilon, double gamma, int M);
// ���ڼ���f(x)+g(x)����Сֵ����ϸ���ܼ�ProxGD.cpp

Result ProxGD_one_step(int fmode, int hmode, int tmode, MatrixXd A, MatrixXd b, MatrixXd x0, double mu, double epsilon, double gamma, int M);
// ���ڼ���f(x)+g(x)����Сֵ����ϸ���ܼ�ProxGD.cpp

/*
double f(int fmode, MatrixXd A, MatrixXd b, MatrixXd x);
// �����е�f(x)����ϸ���ܼ�f.cpp

MatrixXd grad_f(int fmode, MatrixXd A, MatrixXd b, MatrixXd x);
// �����е�f(x)���ݶȣ���ϸ���ܼ�f.cpp
*/

double h(int fmode, MatrixXd x);
// �����е�h(x)����ϸ���ܼ�h.cpp

MatrixXd prox_h(int hmode, double mu, MatrixXd x);
// �����е�h(x)���ݶȣ���ϸ���ܼ�h.cpp

double line_search(Objective& f_obj, int tmode, MatrixXd x, double gamma, int n, ...);
// ���������������ھ����㷨�еĲ���t����ϸ���ܼ�line_search.cpp