#include <iostream>
#include <stdarg.h>
#include <Eigen/Dense>
#include "ProxGD.h"
#include <assert.h>
#include <algorithm>
using namespace Eigen;
using namespace std;


double line_search(Objective& f_obj, int tmode, MatrixXd x, double gamma, int n, ...)
{
	// ���������������ھ����㷨�еĲ���t��tmode��ʾ�������ķ�ʽ��A�ǲ��������ز���t
	// ע����������������û��д�꣬������ܻ�Ķ��������Armijo׼��Ļ��϶�Ҫ����������

	double t;

	if (tmode == 1)
	{
		// tmode=1��ʱ��ȡArmijo׼��

		va_list args;
		va_start(args, n);
		double fh = va_arg(args, double);
		va_end(args);

		double l_t = 0;
		double r_t = 1;
		t = 1;
		bool flag = 1;// flag = 1��ʾ����������������
		bool cond;// cond��ʾArmijo�����Ƿ��Ѿ�����

		while (r_t - l_t > 1e-8)
		{
			// ������䳤�Ⱥ�С��ֹͣ����

			cond = (f_obj.f(x - t * f_obj.grad_f(x)) < f_obj.f(x)
				- gamma * t * f_obj.grad_f(x).squaredNorm());// ����Armijo�����Ƿ��Ѿ�����

			if (cond)
			{
				// ������㣬����Ƿ���Ҫ����������������
				if (flag)
				{
					l_t = r_t;
					r_t *= 2;
					t = r_t;
					// �����Ҫ�����������������
				}
				else
				{
					break;
					// ����ֱ��break
				}
			}
			else
			{
				// ��������㣬��ʼ��С����

				flag = 0;// ���Ƚ�flag��Ϊ0����ֹ������������

				r_t = t;
				t = (r_t + l_t) / 2;// Ȼ����С���䣬��������
			}
		}
	}
	else if (tmode == 2)
	{
		// tmode=2��ʱ��ȡBB����

		va_list args;
		va_start(args, n);
		double* fhs = va_arg(args, double*);
		int M = va_arg(args, int);
		MatrixXd delta_x = va_arg(args, MatrixXd);
		va_end(args);
		
		MatrixXd delta_g = f_obj.grad_f(x) - f_obj.grad_f(x - delta_x);
		t = delta_x.squaredNorm() / (delta_x.array().cwiseProduct(delta_g.array())).sum();

		while (f_obj.f(x - t * f_obj.grad_f(x)) > *(max_element(fhs, fhs + M))
			- gamma * t * f_obj.grad_f(x).squaredNorm())
		{
			// �����������������С����

			t /= 2;
		}
	}
	return t;
}