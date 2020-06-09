#include "stdafx.h"
#include "CMatrix.h"

CMatrix::CMatrix()//Создание точки
{
	_nRows = 1;
	_nCols = 1;
	_elements = new double*[_nRows];
	for (int i = 0; i < _nRows; i++)
	{
		_elements[i] = new double[_nCols];
	}
	for (int i = 0; i < _nRows; i++)
	{
		for (int j = 0; j < _nCols; j++)
		{
			_elements[i][j] = 0;
		}
	}
}

CMatrix::CMatrix(int nRows, int nCols)//создание матрицы
{
	_nRows = nRows;
	_nCols = nCols;
	_elements = new double*[_nRows];
	for (int i = 0; i < _nRows; i++)
	{
		_elements[i] = new double[_nCols];
	}
	for (int i = 0; i < _nRows; i++)
	{
		for (int j = 0; j < _nCols; j++)
		{
			_elements[i][j] = 0;
		}
	}
}

CMatrix::CMatrix(int nRows)//создание вектора
{
	_nRows = nRows;
	_nCols = 1;
	_elements = new double*[_nRows];
	for (int i = 0; i < _nRows; i++)
	{
		_elements[i] = new double[_nCols];
	}
	for (int i = 0; i < _nRows; i++)
	{
		for (int j = 0; j < _nCols; j++)
		{
			_elements[i][j] = 0;
		}
	}
}

CMatrix::CMatrix(const CMatrix &other)//копирование матриц
{
	_nRows = other._nRows;
	_nCols = other._nCols;
	_elements = new double*[_nRows];
	for (int i = 0; i < _nRows; i++)
	{
		_elements[i] = new double[_nCols];
	}
	for (int i = 0; i < _nRows; i++)
	{
		for (int j = 0; j < _nCols; j++)
		{
			_elements[i][j] = other._elements[i][j];
		}
	}
}

CMatrix::~CMatrix()//удаление матриц
{
	for (int i = 0; i < _nRows; i++)
	{
		delete[] _elements[i];//удаление каждого элемента
	}
	delete[] _elements;// удаление самой матрицы из памяти
}

double& CMatrix::operator()(int i, int j)//выбор элемента матрицы
{
	if ((i > _nRows - 1) || (j > _nCols - 1))
	{
		CString error = L"CMatrix::operator()(int, int): выход индекса за границу диапазона";
		MessageBox(NULL, error, L"Ошибка", MB_ICONSTOP);
		exit(1);
	}
	return _elements[i][j];
}

double& CMatrix::operator()(int i)//выбор элемента вектора
{
	if (_nCols > 1)//проверка на коллво столбцов
	{
		CString error = L"CMatrix::operator()(int): объект не является вектором";
		MessageBox(NULL, error, L"Ошибка", MB_ICONSTOP);
		exit(1);
	}
	if (i > _nRows - 1)//Проверка на колличество строк
	{
		CString error = L"CMatrix::operator()(int): выход индекса за границу диапазона";
		MessageBox(NULL, error, L"Ошибка", MB_ICONSTOP);
		exit(1);
	}
	return _elements[i][0];
}

CMatrix CMatrix::operator=(const CMatrix& other)// присвоение
{
	if (this == &other)
	{
		return *this;
	}
	int nRows = other.countRows();
	int nCols = other.countCols();
	if ((_nRows == nRows) && (_nCols == nCols))
	{
		for (int i = 0; i < _nRows; i++)
		{
			for (int j = 0; j < _nCols; j++)
			{
				_elements[i][j] = other._elements[i][j];
			}
		}
	}
	else
	{
		CString error = L"CMatrix::operator=: несоответствие размерностей матриц";
		MessageBox(NULL, error, L"Ошибка", MB_ICONSTOP);
		exit(1);
	}
	return *this;
}

CMatrix CMatrix::operator-() //умножение на -1
{
	CMatrix result(_nRows, _nCols);
	for (int i = 0; i < _nRows; i++)
	{
		for (int j = 0; j < _nCols; j++)
		{
			result(i, j) = -_elements[i][j];
		}
	}
	return result;
}

CMatrix CMatrix::operator+(CMatrix& other)// сложение матриц
{
	if (_nRows != other.countRows() || _nCols != other.countCols())
	{
		CString error = L"CMatrix::operator+: несоответствие размерностей матриц";
		MessageBox(NULL, error, L"Ошибка", MB_ICONSTOP);
		exit(1);
	}
	CMatrix result(*this);
	for (int i = 0; i < _nRows; i++)
	{
		for (int j = 0; j < _nCols; j++)
		{
			result(i, j) += other(i, j);
		}
	}
	return result;
}

CMatrix CMatrix::operator-(CMatrix& other)//вычитание
{
	bool possible = (_nRows == other.countRows()) && (_nCols == other.countCols());
	if (!possible)
	{
		CString error = L"CMatrix::operator-: несоответствие размерностей матриц";
		MessageBox(NULL, error, L"Ошибка", MB_ICONSTOP);
		exit(1);
	}

	CMatrix Temp(*this);
	for (int i = 0; i < _nRows; i++)
	{
		for (int j = 0; j < _nCols; j++)
		{
			Temp(i, j) -= other(i, j);
		}
	}
	return Temp;
}

CMatrix CMatrix::operator*(CMatrix& other)//произведение двух векторов
{
	double sum;
	int nRows = other.countRows();
	int nCols = other.countCols();
	CMatrix temp(_nRows, nCols);
	if (_nCols == nRows)
	{
		for (int i = 0; i < _nRows; i++)
		{
			for (int j = 0; j < nCols; j++)
			{
				sum = 0;
				for (int k = 0; k < _nCols; k++)
				{
					sum += (*this)(i, k) * other(k, j);
				}
				temp(i, j) = sum;
			}
		}
	}
	else
	{
		CString error = L"CMatrix::operator*: несоответствие размерностей матриц";
		MessageBox(NULL, error, L"Ошибка", MB_ICONSTOP);
		exit(1);
	}
	return temp;
}

CMatrix CMatrix::operator+(double number)//прибавить число к матрице
{
	CMatrix result(*this);
	for (int i = 0; i < _nRows; i++)
	{
		for (int j = 0; j < _nCols; j++)
		{
			result(i, j) += number;
		}
	}
	return result;
}

CMatrix CMatrix::operator-(double number)// отнять число от матрицы
{
	CMatrix result(*this);
	for (int i = 0; i < _nRows; i++)
	{
		for (int j = 0; j < _nCols; j++)
		{
			result(i, j) -= number;
		}
	}
	return result;
}

int CMatrix::countRows() const //получить коллво строк
{
	return _nRows;
}

int CMatrix::countCols() const //получить коллво столбцов
{
	return _nCols;
}

CMatrix CMatrix::transpose()//транспонирование матриц
{
	CMatrix result(_nCols, _nRows);
	for (int i = 0; i < _nCols; i++)
	{
		for (int j = 0; j < _nRows; j++)
		{
			result(i, j) = _elements[j][i];
		}
	}
	return result;
}

CMatrix CMatrix::getRow(int rowNo)
{
	if (rowNo > _nRows - 1)
	{
		CString error = L"CMatrix::getRow(int): параметр rowNo превышает число строк";
		MessageBox(NULL, error, L"Ошибка", MB_ICONSTOP);
		exit(1);
	}
	CMatrix result(1, _nCols);
	for (int i = 0; i < _nCols; i++)
	{
		result(0, i) = (*this)(rowNo, i);
	}
	return result;
}

CMatrix CMatrix::getRow(int rowNo, int begin, int end)
{
	bool b1 = (rowNo >= 0) && (rowNo < _nRows);
	bool b2 = (begin >= 0) && (begin <= end);
	bool b3 = (end >= 0) && (end < _nCols);
	bool b4 = b1 && b2 && b3;
	if (!b4)
	{
		CString error = L"CMatrix::getRow(int, int, int): ошибка в параметрах";
		MessageBox(NULL, error, L"Ошибка", MB_ICONSTOP);
		exit(1);
	}
	int nCols = end - begin + 1;
	CMatrix result(1, nCols);
	for (int i = begin; i <= end; i++)
	{
		result(0, i - begin) = (*this)(rowNo, i);
	}
	return result;
}

CMatrix CMatrix::getCol(int colNo)
{
	if (colNo > _nCols - 1)
	{
		CString error = L"CMatrix::getCol(int): параметр colNo превышает число столбцов";
		MessageBox(NULL, error, L"Ошибка", MB_ICONSTOP);
		exit(1);
	}
	CMatrix result(_nRows, 1);
	for (int i = 0; i < _nRows; i++)
	{
		result(i, 0) = (*this)(i, colNo);
	}
	return result;
}

CMatrix CMatrix::getCol(int colNo, int begin, int end)
{
	bool b1 = (colNo >= 0) && (colNo < _nCols);
	bool b2 = (begin >= 0) && (begin <= end);
	bool b3 = (end >= 0) && (end < _nRows);
	bool b4 = b1 && b2 && b3;
	if (!b4)
	{
		CString error = L"CMatrix::getCol(int, int, int): ошибка в параметрах";
		MessageBox(NULL, error, L"Ошибка", MB_ICONSTOP);
		exit(1);
	}
	int nRows = end - begin + 1;
	CMatrix result(nRows, 1);
	for (int i = begin; i <= end; i++)
	{
		result(i - begin, 0) = (*this)(i, colNo);
	}
	return result;
}

CMatrix CMatrix::resize(int nRows, int nCols)
{
	for (int i = 0; i < _nRows; i++)
	{
		delete[] _elements[i];
	}
	delete[] _elements;
	_nRows = nRows;
	_nCols = nCols;
	_elements = new double*[_nRows];
	for (int i = 0; i < _nRows; i++)
	{
		_elements[i] = new double[_nCols];
	}
	for (int i = 0; i < _nRows; i++)
	{
		for (int j = 0; j < _nCols; j++)
		{
			_elements[i][j] = 0;
		}
	}
	return (*this);
}

CMatrix CMatrix::resizeSafe(int nRows, int nCols)
{
	CMatrix buffer = (*this);
	this->resize(nRows, nCols);
	int min_rows = (buffer.countRows() < (*this).countRows()) ? buffer.countRows() : (*this).countRows();
	int min_cols = (buffer.countCols() < (*this).countCols()) ? buffer.countCols() : (*this).countCols();
	for (int i = 0; i < min_rows; i++)
	{
		for (int j = 0; j < min_cols; j++)
		{
			(*this)(i, j) = buffer(i, j);
		}
	}
	return (*this);
}

CMatrix CMatrix::resize(int nRows)
{
	for (int i = 0; i < _nRows; i++)
	{
		delete[] _elements[i];
	}
	delete[] _elements;
	_nRows = nRows;
	_nCols = 1;
	_elements = new double*[_nRows];
	for (int i = 0; i < _nRows; i++)
	{
		_elements[i] = new double[_nCols];
	}
	for (int i = 0; i < _nRows; i++)
	{
		for (int j = 0; j < _nCols; j++)
		{
			_elements[i][j] = 0;
		}
	}
	return (*this);
}

CMatrix CMatrix::resizeSafe(int nRows)
{
	CMatrix buffer = (*this);
	this->resize(nRows);
	int min_rows = (buffer.countRows() < (*this).countRows()) ? buffer.countRows() : (*this).countRows();
	for (int i = 0; i < min_rows; i++)
	{
		(*this)(i) = buffer(i);
	}
	return (*this);
}

double CMatrix::getMax()
{
	double max = (*this)(0, 0);
	for (int i = 0; i < (this->countRows()); i++)
	{
		for (int j = 0; j < (this->countCols()); j++)
		{
			if ((*this)(i, j) > max)
			{
				max = (*this)(i, j);
			}
		}
	}
	return max;
}

double CMatrix::getMin()
{
	double min = (*this)(0, 0);
	for (int i = 0; i < (this->countRows()); i++)
	{
		for (int j = 0; j < (this->countCols()); j++)
		{
			if ((*this)(i, j) < min)
			{
				min = (*this)(i, j);
			}
		}
	}
	return min;
}