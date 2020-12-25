#include <iostream>
#include <Eigen/Dense>
#include "ProxGD.h"
#include <math.h>
#include<assert.h>
using namespace Eigen;
using namespace std;

Objective::Objective(int mode, MatrixXd A, MatrixXd b) :mode(mode), A(A), b(b) {};

double Objective::f(MatrixXd x)
{
	if (mode == 1)
	{
		return Frob(x);
	}
	else
	{
		return Logistic(x);
	}
}
MatrixXd Objective::grad_f(MatrixXd x)
{
	if (mode == 1)
	{
		return Frob_grad(x);
	}
	else
	{
		return Logistic_grad(x);
	}
}
;

double Objective::Frob(MatrixXd x)
{
	return (A * x - b).squaredNorm() / 2.0;
}

double Objective::Logistic(MatrixXd x)
{
	assert(x.cols() == 1);
	return (1 + (-(A * x).cwiseProduct(b).array()).exp()).log().sum() / (double)x.rows();
}

MatrixXd Objective::Frob_grad(MatrixXd x)
{
	return A.transpose() * (A * x - b);
}

MatrixXd Objective::Logistic_grad(MatrixXd x)
{
	int m = x.rows();
	MatrixXd result = x, Ax = A * x;
	ArrayXd tmp = b.array();
	tmp = (-Ax.array() * tmp).exp();
	tmp *= b.array() / (1.0 + tmp) / (double)m;
	return -A.transpose() * tmp.matrix();
}

int Objective::check(MatrixXd x)
{
	int m = A.rows(),
		r = A.cols(), r1 = x.rows(), n = x.cols(), m1 = b.rows(), n1 = b.cols();
	if ((m != m1) || (r != r1) || (n != n1))
	{
		throw "Incompatible size.";
	}
	return 0;
}

/*
double f(int fmode, MatrixXd A, MatrixXd b, MatrixXd x)
{
	// �����е�f(x)��fmode��ʾѡ����ֺ�����A��b��f�Ĳ�����x�Ǻ����Ա��������غ���ֵresult

	Objective f;
	if (fmode == 1)
	{
		return f.Frob(A, b, x);
	}
}

MatrixXd grad_f(int fmode, MatrixXd A, MatrixXd b, MatrixXd x)
{
	// �����е�f(x)���ݶȣ�fmode��ʾѡ����ֺ�����A��b��f�Ĳ�����x�Ǻ����Ա����������ݶ�result

	MatrixXd result = x;
	if (fmode == 1)
	{
		// Frobenious����ƽ������2���ݶ�

		result = A.transpose() * (A * x - b);
	}
	return result;
}
*/
