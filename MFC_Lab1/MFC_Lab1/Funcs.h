#pragma once
#include "Cmatrix.h"
void fillMatrix(CMatrix&, double);
void printMatrix(CDC* dc, int, int, CString, CMatrix&);
void printNumber(CDC* dc, int, int, CString, double);
double cos(CMatrix&, CMatrix&);
CMatrix vectorProduct(CMatrix&, CMatrix&);
double scalarProduct(CMatrix&, CMatrix&);
double module(CMatrix&);
CMatrix sphericalToCartesian(CMatrix&);