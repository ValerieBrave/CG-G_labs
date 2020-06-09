#include "stdafx.h"
#include "Funcs.h"
#include "Cmatrix.h"

#define SPACING 50
#define PI 3.14159

void fillMatrix(CMatrix& matrix, double one)
{
	for (int i = 0; i < matrix.rows(); i++)
	{
		for (int j = 0; j < matrix.cols(); j++)
		{
			matrix(i, j) = one;
			one++;
		}
	}
}

void printMatrix(CDC* dc, int x, int y, CString caption, CMatrix& matrix)
{
	CString str = L"";
	dc->TextOut(x, y, caption);
	y += SPACING;
	for (int i = 0; i < matrix.rows(); i++)
	{
		for (int j = 0; j < matrix.cols(); j++)
		{
			str.Format(L"%.2lf", matrix(i, j));
			dc->TextOut(x + j * SPACING, y + i * SPACING, str);
		}
	}
}

void printNumber(CDC* dc, int x, int y, CString caption, double number)
{
	CString str = L"";
	dc->TextOut(x, y, caption);
	str.Format(L"%.2lf", number);
	dc->TextOut(x, y + SPACING, str);
}

double cos(CMatrix& left, CMatrix& right)
{
	return scalarProduct(left, right) / (module(left) * module(right));
}

CMatrix vectorProduct(CMatrix& left, CMatrix& right)
{
	CMatrix result(3);
	result(0) = left(1) * right(2) - left(2) * right(1);
	result(1) = left(2) * right(0) - left(0) * right(2);
	result(2) = left(0) * right(1) - left(1) * right(0);
	return result;
}

double scalarProduct(CMatrix& left, CMatrix& right)
{
	double result = 0;
	for (int i = 0; i < left.rows(); i++)
	{
		result += left(i) * right(i);
	}
	return result;
}

double module(CMatrix& vector)
{
	double sum = 0;
	for (int i = 0; i < vector.rows(); i++)
	{
		sum += vector(i) * vector(i);
	}
	return sqrt(sum);
}

CMatrix sphericalToCartesian(CMatrix& point)
{
	double r = point(0), theta = point(1), phi = point(2);
	CMatrix result(3);
	result(0) = r * sin(theta) * cos(phi);
	result(1) = r * sin(theta) * sin(phi);
	result(2) = r * cos(theta);
	return result;
}