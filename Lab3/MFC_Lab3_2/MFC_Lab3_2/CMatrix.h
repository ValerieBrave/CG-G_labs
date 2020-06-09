#pragma once
class CMatrix
{
private:
	double **_elements;					// �������� �������
	int _nRows;							// ����� �����
	int _nCols;							// ����� ��������

public:
	CMatrix();							// ����������� �� ��������� (1 �� 1)
	CMatrix(int, int);		    		// �����������
	CMatrix(int);						// ����������� ������� (���� �������)
	CMatrix(const CMatrix&);			// ����������� �����������
	~CMatrix();							// ����������

	double& operator()(int, int);        // ����� �������� ������� �� �������
	double& operator()(int);            // ����� �������� ������� �� �������

	CMatrix operator=(const CMatrix&);	// ������������
	CMatrix operator-();			    // ��������� �� -1

	CMatrix operator+(CMatrix&);	    // �������� ������
	CMatrix operator-(CMatrix&);	    // ��������� ������
	CMatrix operator*(CMatrix&);        // ������������ ������

	CMatrix operator+(double);		    // ����������� ����� � �������
	CMatrix operator-(double);		    // ��������� ����� �� �������

	int countRows() const;				// ��������� ����� �����
	int countCols() const;				// ��������� ����� ��������

	CMatrix transpose();				// ��������� ����������������� �������

	CMatrix getRow(int);			    // ��������� ������ �� ������
	CMatrix getRow(int, int, int);		// ��������� ��������� �� ������ � ���������
	CMatrix getCol(int);			    // ��������� ������� �� ������
	CMatrix getCol(int, int, int);		// ��������� ���������� �� ������ � ���������

	CMatrix resize(int, int);			// ��������� ������� ������� � ������������ ������
	CMatrix resizeSafe(int, int);		// ��������� ������� ������� � ����������� ������
	CMatrix resize(int);				// ��������� ������� ������� � ������������ ������
	CMatrix resizeSafe(int);			// ��������� ������� ������� � ����������� ������

	double getMax();					// ��������� ������������� �������� �������
	double getMin();					// ��������� ������������ �������� �������
};