#pragma once

class CChildView : public CWnd
{
public:
	CChildView();
	double MyF1(double x);
	void Lagrange();
	double Lagr(CMatrix& X, CMatrix& Y, double x, int size);
	void Bezier();
	void Bezier2();
	void Bezier3();
	void Bezier4();
public:
	
	CRect RW;
	CPlot2D Graph;
	CMatrix X, Y;
	int N_Bezier;
	CMyPen PenLine, PenAxis;
	int Index;
	int ch;
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	virtual ~CChildView();

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBezier();
	afx_msg void OnLagrange();
	afx_msg void OnPlotBezier();
	afx_msg void OnPlotBezier2();
	afx_msg void OnPlotBezier3();
	afx_msg void OnPlotBezier4();
};

