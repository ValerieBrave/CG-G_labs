#include "stdafx.h"
#include "Laba9.h"
#include "ChildView.h"
#include "MainFrm.h"
#include "Bezier.h"
#include "Lagrange.h"
#include "math.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

COLORREF Color = RGB(255, 40, 40);
CChildView::CChildView()
{
	Index = 0;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_COMMAND(ID_32780, &CChildView::OnBezier)
	ON_COMMAND(ID_32781, &CChildView::OnLagrange)
	ON_COMMAND(ID_PLOT_BEZIER, &CChildView::OnPlotBezier)
	ON_COMMAND(ID_PLOT_BEZIER2, &CChildView::OnPlotBezier2)
	ON_COMMAND(ID_PLOT_BEZIER3, &CChildView::OnPlotBezier3)
	ON_COMMAND(ID_PLOT_BEZIER4, &CChildView::OnPlotBezier4)
END_MESSAGE_MAP()

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return TRUE;
}

void CChildView::OnPaint()
{
	CPaintDC dc(this);
	if (Index == 1)
	{
		//опорная линия
		/*PenLine.Set(PS_SOLID, 0, RGB(0, 255, 0));
		Graph.SetPenLine(PenLine);
		Graph.Draw(dc, 1, 1);*/

		// Вторая линия		
		PenLine.Set(PS_SOLID, 2, RGB(255, 0, 0));
		Graph.SetPenLine(PenLine);
		Graph.DrawBezier(dc, 20);
	}
	if (Index == 2)
	{
		
		Graph.Draw(dc, 1, 1);
	}
}

void CChildView::OnBezier()
{
	Index = 1;
	double dt = pi / 4;
	int N = 5;
	X.RedimMatrix(N);
	Y.RedimMatrix(N);
	for (int i = 0; i < N; i++)
	{
		X(i) = i*dt;
		Y(i) = pow(2 + cos(i*dt), sin(2*i*dt));
	}
	N_Bezier = 50;
	RW.SetRect(100, 50, 500, 350);
	Graph.SetParams(X, Y, RW);
	this->Invalidate();
}

double CChildView::MyF1(double x)
{
	double y = pow(2+cos(x), sin(2*x));
	return y;
}

void CChildView::OnLagrange()
{
	double dx = 0.02;
	double xH = pi;
	int N = xH / dx;
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);
	for (double i = 0; i <= N; i++)
	{
		X(i) = i*dx;
		Y(i) = MyF1(X(i));
	}
	PenLine.Set(PS_SOLID, 1, RGB(255, 0, 0));
	PenAxis.Set(PS_SOLID, 1, RGB(0, 0, 255));
	RW.SetRect(100, 50, 500, 450);
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);
	Lagrange();
	Index = 2;
	this->Invalidate();
}

double CChildView::Lagr(CMatrix& X, CMatrix& Y, double x, int size) 
{
	double lagrange_pol = 0;
	double basics_pol;

	for (int i = 0; i < size; i++)
	{
		basics_pol = 1;
		for (int j = 0; j < size; j++)
		{
			if (j == i) continue;
			basics_pol *= (x - X(j)) / (X(i) - X(j));
		}
		lagrange_pol += basics_pol * Y(i);
	}
	return lagrange_pol;
}

void CChildView::Lagrange() 
{
	double dx = pi / 4;
	CPaintDC dc(this);
	CPen MyPen(PenLine.PenStyle, PenLine.PenWidth, PenLine.PenColor);
	CPen* pOldPen = dc.SelectObject(&MyPen);
	double xL = 0;
	double xH = pi;

	int N = (xH - xL) / dx;
	
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);
	
	for (int i = 0; i <= N; i++)
	{
		X(i) = xL + i * dx;
		Y(i) = pow(2 + cos(X(i)), sin(2 * X(i)));
	}

	dx = 0.1;
	int NL = (xH - xL) / dx;
	CMatrix XL(NL + 1);
	CMatrix YL(NL + 1);

	for (int i = 0; i <= NL; i++)
	{
		XL(i) = xL + i * dx;
		YL(i) = Lagr(X, Y, XL(i), N + 1);
	}
	PenLine.Set(PS_SOLID, 1, RGB(255, 0, 0));
	PenAxis.Set(PS_SOLID, 2, RGB(255, 0, 0));
	
	Graph.SetParams(XL, YL, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);
	dc.MoveTo((int)XL(0), (int)YL(0));
	for (int i = 1; i < XL.rows(); i++)	dc.LineTo((int)XL(i), (int)YL(i));
	dc.SelectObject(pOldPen);
}

void CChildView::Bezier() //координаты для безье
{

	CMatrix X(3);//в точке х
	CMatrix Y(3);
	X(0) = 0;
	Y(0) = 0;
	X(1) = -15; //	X(1) = 5; //для равномерной кривой
	Y(1) = 5;
	X(2) = 10; //	X(2) = 0; //для прямой безье
	Y(2) = 0;


	PenLine.Set(PS_SOLID, 1, RGB(255, 0, 0));//цвет графика

	//задаем параметры
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);
}

void CChildView::Bezier2() //координаты для безье
{
	CMatrix X(3);//в точке х
	CMatrix Y(3);
	X(0) = 0;
	Y(0) = 0;
	X(1) = 5; //для равномерной кривой
	Y(1) = 5;
	X(2) = 10; //	X(2) = 0; //для прямой безье
	Y(2) = 0;

	PenLine.Set(PS_SOLID, 1, RGB(255, 0, 0));//цвет графика

											 //задаем параметры
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);
}

void CChildView::Bezier3() //координаты для безье
{
	//на подобии синусоиды

	CMatrix X(4);//в точке х
	CMatrix Y(4);
	X(0) = 0;
	Y(0) = 0;
	X(1) = 5;
	Y(1) = 10;
	X(2) = 10;
	Y(2) = -10;
	X(3) = 15;
	Y(3) = 0;

	PenLine.Set(PS_SOLID, 1, RGB(255, 0, 0));//цвет графика

											 //задаем параметры
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);
}

void CChildView::Bezier4() //координаты для безье
{
	//Закорючка

	CMatrix X(4);//в точке х
	CMatrix Y(4);
	X(0) = 0;
	Y(0) = 0;
	X(1) = 20;
	Y(1) = 10;
	X(2) = -5;
	Y(2) = 10;
	X(3) = 15;
	Y(3) = 0;


	PenLine.Set(PS_SOLID, 1, RGB(255, 0, 0));//цвет графика

											 //задаем параметры
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);
}

void CChildView::OnPlotBezier()
{
	RW.SetRect(100, 20, 700, 400);
	Bezier();
	Index = 1;
	this->Invalidate();
}

void CChildView::OnPlotBezier2()
{
	RW.SetRect(100, 20, 700, 400);
	Bezier2();
	ch = 2;
	Index = 1;
	this->Invalidate();
}
void CChildView::OnPlotBezier3()
{
	RW.SetRect(100, 20, 700, 400);
	Bezier3();
	Index = 1;
	this->Invalidate();
}
void CChildView::OnPlotBezier4()
{
	RW.SetRect(100, 20, 700, 400);
	Bezier4();
	Index = 1;
	this->Invalidate();
}