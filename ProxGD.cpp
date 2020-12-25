#include <iostream>
#include <Eigen/Dense>
#include "ProxGD.h"
#include <math.h>
#include<assert.h>
using namespace Eigen;
using namespace std;


Result ProxGD(int fmode, int hmode, int tmode, MatrixXd A, MatrixXd b, MatrixXd x0, double mu, double epsilon, double gamma, int M)
{
	int K = ceil(fabs(log2f(mu)));
	double muk = mu * pow(2, K);
	Result res(0, x0, 0);
	int iter = 0;
	double epsilonk = epsilon * pow(2, K);
	for (int i = 0; i <= K; i++)
	{
		res = ProxGD_one_step(fmode, hmode, tmode, A, b, res.min_point(), muk, epsilonk, gamma, M);
		muk /= 2;
		epsilonk /= 2;
		iter += res.iterations();
	}
	res.modify_iter(iter);
	return res;
}

Result ProxGD_one_step(int fmode, int hmode, int tmode, MatrixXd A, MatrixXd b, MatrixXd x0, double mu, double epsilon, double gamma, int M)
{
	// ���ڼ���f(x)+g(x)����Сֵ������ֵfmode��hmode��tmode�ֱ����f��h�Լ���������ģʽ���ֱ��Ӧ��Ŀ�еĵ����㡢
	// ���ĵ㡢�ڶ��㣩��A��b��f�Ĳ�����x0�ǵ�����ʼֵ��mu��hǰ���ϵ����epsilon�ǵ�����ֹ�����Ĳ���������ֵĿǰ
	// �Ǻ�����Сֵ������Ӧ�û�ĳ�Result�࣬ͬʱ���ص�����������Сֵ�Լ����ŵ�x��

	MatrixXd new_x = x0;
	MatrixXd prev_x = x0;
	MatrixXd delta_x = x0;
	MatrixXd x_star = x0;
	delta_x.setOnes();
	// ��ʼ����������һ��ѭ���л��漰new_x�Լ�prev_x���ֱ��ʾ�µ�x�;ɵ�x��delta_xΪ���ߵĲx_starΪ�㷨��һ��
	// �õ����м�ֵ����delta_x��Ϊȫ1����֤�ܽ��������ѭ����

	int iter = 0;
	double t;
	double *fhs = new double[M];
	Objective f_obj(fmode, A, b);
	fhs[0] = f_obj.f(new_x) + mu * h(hmode, new_x);
	for (int i = 1; i < M; i++)
	{
		fhs[i] = fhs[0];
	}
	fhs[M - 1]++;
	// ��ʼ��������t��ÿ�ε����Ĳ�����iter�ǵ���������fhs��ǰM���ĺ���ֵ��ÿ�ε�������һ��Ԫ�أ�iter % M��ʾ��ǰ��
	// ��ֵ����ǰ����ǰ��ĺ���ֵ����0λ��ǰ���ǵ�M - 1λ����ʼ����0λ�͵�M - 1λ��֤�ܽ���ѭ����

	while ((fabs(fhs[iter % M] - fhs[(iter + M - 1) % M]) > epsilon) && (delta_x.squaredNorm() > epsilon))
	{
		// ������ֹ����Ϊ����ֵ�仯�����Ա����ı仯С�ڵ���epsilon��������Կ���֮ǰ��ʼ��fhs[M - 1]��delta_xʱ��Ŀ�ģ�

		if (tmode == 1)
		{
			t = line_search(f_obj, tmode, new_x, gamma, 1, (double)fhs[iter % M]);
		}
		else if (tmode == 2)
		{
			t = line_search(f_obj, tmode, new_x, gamma, 3, fhs, M, delta_x);
		}
		// ��������������ȷ������

		x_star = new_x - t * f_obj.grad_f(new_x);
		// Ȼ������м����x_star

		prev_x = new_x;
		new_x = prox_h(hmode, t * mu, x_star);// ����prox_h�����µ�x
		delta_x = new_x - prev_x;
		// ����prev_x���ٸ���new_x��Ȼ������µ�delta_x

		iter = iter + 1;
		// ���µ�������

		fhs[iter % M] = f_obj.f(new_x) + mu * h(hmode, new_x);;
		// ����fhs��

	}

	Result res(iter, new_x, fhs[iter % M]);
	return res;
}

