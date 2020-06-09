#pragma once
class CMatrix
{
private:
	double **_elements;					// Элементы матрицы
	int _nRows;							// Число строк
	int _nCols;							// Число столбцов

public:
	CMatrix();							// Конструктор по умолчанию (1 на 1)
	CMatrix(int, int);		    		// Конструктор
	CMatrix(int);						// Конструктор вектора (один столбец)
	CMatrix(const CMatrix&);			// Конструктор копирования
	~CMatrix();							// Деструктор

	double& operator()(int, int);        // Выбор элемента матрицы по индексу
	double& operator()(int);            // Выбор элемента вектора по индексу

	CMatrix operator=(const CMatrix&);	// Присваивание
	CMatrix operator-();			    // Умножение на -1

	CMatrix operator+(CMatrix&);	    // Сложение матриц
	CMatrix operator-(CMatrix&);	    // Вычитание матриц
	CMatrix operator*(CMatrix&);        // Произведение матриц

	CMatrix operator+(double);		    // Прибавление числа к матрице
	CMatrix operator-(double);		    // Вычитание числа из матрицы

	int countRows() const;				// Получение числа строк
	int countCols() const;				// Получение числа столбцов

	CMatrix transpose();				// Получение транспонированной матрицы

	CMatrix getRow(int);			    // Получение строки по номеру
	CMatrix getRow(int, int, int);		// Получение подстроки по номеру и диапазону
	CMatrix getCol(int);			    // Получение столбца по номеру
	CMatrix getCol(int, int, int);		// Получение подстолбца по номеру и диапазону

	CMatrix resize(int, int);			// Изменение размера матрицы с уничтожением данных
	CMatrix resizeSafe(int, int);		// Изменение размера матрицы с сохранением данных
	CMatrix resize(int);				// Изменение размера вектора с уничтожением данных
	CMatrix resizeSafe(int);			// Изменение размера вектора с сохранением данных

	double getMax();					// Получение максимального элемента матрицы
	double getMin();					// Получение минимального элемента матрицы
};