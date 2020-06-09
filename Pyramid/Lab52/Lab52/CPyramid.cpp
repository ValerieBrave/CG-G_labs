#include "stdafx.h"
#include "CPyramid.h"
#define PI acos(-1)
CMatrix SpaceToWindow(CRectD& rs, CRect& rw)
{
	CMatrix T(3, 3);
	double kx = (rw.right - rw.left) / (rs.right - rs.left),
		ky = (rw.bottom - rw.top) / (rs.top - rs.bottom);
	T(0, 0) = kx;
	T(0, 2) = rw.left - kx * rs.left;
	T(1, 1) = -ky;
	T(1, 2) = rw.bottom + ky * rs.bottom;
	T(2, 2) = 1;
	return T;
}
void CPyramid::GetRect(CMatrix &Vert, CRectD &RectView)
{
	for (unsigned int i = 0; i < Vert.cols(); i++)
	{
		if (Vert(0, i) > RectView.right)
			RectView.right = Vert(0, i);
		if (Vert(0, i) < RectView.left)
			RectView.left = Vert(0, i);
		if (Vert(2, i) > RectView.top)
			RectView.top = Vert(2, i);
		if (Vert(2, i) < RectView.bottom)
			RectView.bottom = Vert(2, i);
	}
}
CPyramid::CPyramid()
{
	Vertices.RedimMatrix(4, 6);
	for (unsigned int i = 0; i < Vertices.cols(); i++)
		Vertices(3, i) = 1;
	// A
	Vertices(0, 0) = 8;
	Vertices(1, 0) = 0;
	Vertices(2, 0) = 0;
	//
	// B
	Vertices(0, 1) = 0;
	Vertices(1, 1) = 0;
	Vertices(2, 1) = 0;
	//
	// C
	Vertices(0, 2) = 0;
	Vertices(1, 2) = 8;
	Vertices(2, 2) = 0;
	//
	// A1
	Vertices(0, 3) = 4;
	Vertices(1, 3) = 0;
	Vertices(2, 3) = 8;
	//
	// B1
	Vertices(0, 4) = 0;
	Vertices(1, 4) = 0;
	Vertices(2, 4) = 8;
	//
	// C1
	Vertices(0, 5) = 0;
	Vertices(1, 5) = 4;
	Vertices(2, 5) = 8;
	//
	View.RedimMatrix(4, 4);
	View = CreateViewCoords(10, 315, 45);
}
void CPyramid::Draw(CDC &dc, CMatrix &P, CMatrix &RW)
{
	CMatrix Buffer(4, 6);
	Buffer = Vertices;
	CMatrix AVecBuf(3), BVecBuf(3), CVecBuf(3), A1VecBuf(3), B1VecBuf(3), C1VecBuf(3);
	CMatrix ViewVector(3), ABCVector(3), A1B1C1Vector(3), AA1B1BVector(3), AA1C1CVector(3), CC1B1BVector(3);
	for (unsigned int i = 0; i < 3; i++)
	{
		AVecBuf(i) = Vertices(i, 0);
		BVecBuf(i) = Vertices(i, 1);
		CVecBuf(i) = Vertices(i, 2);
		A1VecBuf(i) = Vertices(i, 3);
		B1VecBuf(i) = Vertices(i, 4);
		C1VecBuf(i) = Vertices(i, 5);
	}
	CMatrix PBuf(3);
	PBuf = P;
	PBuf(1) = PBuf(1) * PI / 180;
	PBuf(2) = PBuf(2) * PI / 180;
	ViewVector = SphereToCart(PBuf);
	ABCVector = -VectorMult(CVecBuf, AVecBuf);
	A1B1C1Vector = VectorMult(CVecBuf, AVecBuf);
	AA1B1BVector = VectorMult(A1VecBuf - AVecBuf, BVecBuf - AVecBuf);
	CC1B1BVector = VectorMult(B1VecBuf - BVecBuf, CVecBuf - BVecBuf);
	AA1C1CVector = VectorMult(C1VecBuf - CVecBuf, AVecBuf - CVecBuf);
	if (P(2) < 180)
	{
		ABCVector = VectorMult(CVecBuf, AVecBuf);
		A1B1C1Vector = -VectorMult(CVecBuf, AVecBuf);
		AA1B1BVector = -VectorMult(A1VecBuf - AVecBuf, BVecBuf - AVecBuf);
		CC1B1BVector = -VectorMult(B1VecBuf - BVecBuf, CVecBuf - BVecBuf);
		AA1C1CVector = -VectorMult(C1VecBuf - CVecBuf, AVecBuf - CVecBuf);
	}
	View = CreateViewCoords(P(0), P(1), P(2));
	CMatrix X(4), Y(4), Z(4), O(4);
	X(3) = Y(3) = Z(3) = O(3) = 1;
	X(0) = Y(1) = Z(2) = 10;
	O = View * O;
	X = View * X;
	Y = View * Y;
	Z = View * Z;
	Vertices = View * Vertices;
	CRectD ViewRect;
	ViewRect.left = O(0);
	ViewRect.top = O(2);
	ViewRect.right = O(0);
	ViewRect.bottom = O(2);
	if (ViewRect.left > X(0))
		ViewRect.left = X(0);
	if (ViewRect.left > Y(0))
		ViewRect.left = Y(0);
	if (ViewRect.left > Z(0))
		ViewRect.left = Z(0);
	if (ViewRect.bottom > X(2))
		ViewRect.bottom = X(2);
	if (ViewRect.bottom > Y(2))
		ViewRect.bottom = Y(2);
	if (ViewRect.bottom > Z(2))
		ViewRect.bottom = Z(2);
	if (ViewRect.right < X(0))
		ViewRect.right = X(0);
	if (ViewRect.right < Y(0))
		ViewRect.right = Y(0);
	if (ViewRect.right > Z(0))
		ViewRect.right = Z(0);
	if (ViewRect.top < X(2))
		ViewRect.top = X(2);
	if (ViewRect.top < Y(2))
		ViewRect.top = Y(2);
	if (ViewRect.top < Z(2))
		ViewRect.top = Z(2);
	GetRect(Vertices, ViewRect);
	CRect rw;
	rw.left = RW(0);
	rw.top = RW(1);
	rw.right = RW(2);
	rw.bottom = RW(3);
	CMatrix T = SpaceToWindow(ViewRect, rw);
	CMatrix Obuf(3), Xbuf(3), Ybuf(3), Zbuf(3), VerticesBuf(3, Vertices.cols());
	Obuf(0) = O(0);
	Obuf(1) = O(2);
	Xbuf(0) = X(0);
	Xbuf(1) = X(2);
	Ybuf(0) = Y(0);
	Ybuf(1) = Y(2);
	Zbuf(0) = Z(0);
	Zbuf(1) = Z(2);
	Obuf(2) = Xbuf(2) = Ybuf(2) = Zbuf(2) = 1;
	for (unsigned int j = 0; j < VerticesBuf.cols(); j++)
	{
		VerticesBuf(0, j) = Vertices(0, j);
		VerticesBuf(1, j) = Vertices(2, j);
		VerticesBuf(2, j) = 1;
	}
	Obuf = T * Obuf;
	Xbuf = T * Xbuf;
	Ybuf = T * Ybuf;
	Zbuf = T * Zbuf;
	VerticesBuf = T * VerticesBuf;
	// AA1B1B
	if (ScalarMult(AA1B1BVector, ViewVector) >= 0)
	{
		dc.MoveTo(VerticesBuf(0, 0), VerticesBuf(1, 0));
		dc.LineTo(VerticesBuf(0, 3), VerticesBuf(1, 3));
		dc.LineTo(VerticesBuf(0, 4), VerticesBuf(1, 4));
		dc.LineTo(VerticesBuf(0, 1), VerticesBuf(1, 1));
		dc.LineTo(VerticesBuf(0, 0), VerticesBuf(1, 0));
	}
	//
	// AA1C1C
	if (P(2) < 180)
	{
		if (ScalarMult(AA1C1CVector, ViewVector) - 466 - (225 - abs(P(2))) >= 0)
		{
			dc.MoveTo(VerticesBuf(0, 0), VerticesBuf(1, 0));
			dc.LineTo(VerticesBuf(0, 3), VerticesBuf(1, 3));
			dc.LineTo(VerticesBuf(0, 5), VerticesBuf(1, 5));
			dc.LineTo(VerticesBuf(0, 2), VerticesBuf(1, 2));
			dc.LineTo(VerticesBuf(0, 0), VerticesBuf(1, 0));
		}
	}
	else
		if (ScalarMult(AA1C1CVector, ViewVector) + 466 + (225 - abs(P(2))) >= 0)
		{
			dc.MoveTo(VerticesBuf(0, 0), VerticesBuf(1, 0));
			dc.LineTo(VerticesBuf(0, 3), VerticesBuf(1, 3));
			dc.LineTo(VerticesBuf(0, 5), VerticesBuf(1, 5));
			dc.LineTo(VerticesBuf(0, 2), VerticesBuf(1, 2));
			dc.LineTo(VerticesBuf(0, 0), VerticesBuf(1, 0));
		}
	//
	// CC1B1B
	if (ScalarMult(CC1B1BVector, ViewVector) >= 0)
	{
		dc.MoveTo(VerticesBuf(0, 2), VerticesBuf(1, 2));
		dc.LineTo(VerticesBuf(0, 5), VerticesBuf(1, 5));
		dc.LineTo(VerticesBuf(0, 4), VerticesBuf(1, 4));
		dc.LineTo(VerticesBuf(0, 1), VerticesBuf(1, 1));
		dc.LineTo(VerticesBuf(0, 2), VerticesBuf(1, 2));
	}
	//
	CPen Pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen* pOldPen = dc.SelectObject(&Pen);
	CBrush Brush(RGB(255, 0, 0));
	CBrush* pOldBrush = dc.SelectObject(&Brush);
	// ABC
	if (ScalarMult(ABCVector, ViewVector) >= 0)
	{
		CBrush *topBrush = new CBrush(RGB(0, 0, 255));
		dc.SelectObject(topBrush);
		CPoint points[3]{ CPoint(VerticesBuf(0, 0), VerticesBuf(1, 0)), CPoint(VerticesBuf(0, 1), VerticesBuf(1, 1)), CPoint(VerticesBuf(0, 2), VerticesBuf(1, 2)) };
		dc.Polygon(points, 3);
	}
	//
	// A1B1C1
	if (ScalarMult(A1B1C1Vector, ViewVector) >= 0)
	{
		CPoint points[3]{ CPoint(VerticesBuf(0, 3), VerticesBuf(1, 3)), CPoint(VerticesBuf(0, 4), VerticesBuf(1, 4)), CPoint(VerticesBuf(0, 5), VerticesBuf(1, 5)) };
		dc.Polygon(points, 3);
	}
	//

	Vertices = Buffer;
}
void CPyramid::Draw1(CDC &dc, CMatrix &P, CMatrix &RW)
{
	CMatrix Buffer(4, 6);
	Buffer = Vertices;
	View = CreateViewCoords(P(0), P(1), P(2));		// матрица перевода в видовые координаты
	CMatrix X(4), Y(4), Z(4), O(4);
	X(3) = Y(3) = Z(3) = O(3) = 1;
	X(0) = Y(1) = Z(2) = 10;
	O = View * O;		
	X = View * X;
	Y = View * Y;
	Z = View * Z;
	Vertices = View * Vertices;// переводим в видовые
	CRectD ViewRect;
	ViewRect.left = O(0);
	ViewRect.top = O(2);
	ViewRect.right = O(0);
	ViewRect.bottom = O(2);
	if (ViewRect.left > X(0))
		ViewRect.left = X(0);
	if (ViewRect.left > Y(0))
		ViewRect.left = Y(0);
	if (ViewRect.left > Z(0))
		ViewRect.left = Z(0);
	if (ViewRect.bottom > X(2))
		ViewRect.bottom = X(2);
	if (ViewRect.bottom > Y(2))
		ViewRect.bottom = Y(2);
	if (ViewRect.bottom > Z(2))
		ViewRect.bottom = Z(2);
	if (ViewRect.right < X(0))
		ViewRect.right = X(0);
	if (ViewRect.right < Y(0))
		ViewRect.right = Y(0);
	if (ViewRect.right > Z(0))
		ViewRect.right = Z(0);
	if (ViewRect.top < X(2))
		ViewRect.top = X(2);
	if (ViewRect.top < Y(2))
		ViewRect.top = Y(2);
	if (ViewRect.top < Z(2))
		ViewRect.top = Z(2);
	GetRect(Vertices, ViewRect);
	CRect rw;		// для параметров окна
	rw.left = RW(0);
	rw.top = RW(1);
	rw.right = RW(2);
	rw.bottom = RW(3);
	CMatrix T = SpaceToWindow(ViewRect, rw);		// матрица перевода из видовых в оконные
	CMatrix Obuf(3), Xbuf(3), Ybuf(3), Zbuf(3), VerticesBuf(3, Vertices.cols());
	Obuf(0) = O(0);
	Obuf(1) = O(2);
	Xbuf(0) = X(0);
	Xbuf(1) = X(2);
	Ybuf(0) = Y(0);
	Ybuf(1) = Y(2);
	Zbuf(0) = Z(0);
	Zbuf(1) = Z(2);
	Obuf(2) = Xbuf(2) = Ybuf(2) = Zbuf(2) = 1;
	for (unsigned int j = 0; j < VerticesBuf.cols(); j++)
	{
		VerticesBuf(0, j) = Vertices(0, j);
		VerticesBuf(1, j) = Vertices(2, j);
		VerticesBuf(2, j) = 1;
	}
	Obuf = T * Obuf;
	Xbuf = T * Xbuf;
	Ybuf = T * Ybuf;
	Zbuf = T * Zbuf;
	VerticesBuf = T * VerticesBuf;		// переводим из видовых в оконные
	dc.MoveTo(Obuf(0), Obuf(1));
	dc.LineTo(Xbuf(0), Xbuf(1));
	dc.MoveTo(Obuf(0), Obuf(1));
	dc.LineTo(Ybuf(0), Ybuf(1));
	dc.MoveTo(Obuf(0), Obuf(1));
	dc.LineTo(Zbuf(0), Zbuf(1));
	// ABC
	dc.MoveTo(VerticesBuf(0, 0), VerticesBuf(1, 0));		// прорисовка каждой линии
	for (unsigned int j = 1; j < 3; j++)
		dc.LineTo(VerticesBuf(0, j), VerticesBuf(1, j));
	dc.LineTo(VerticesBuf(0, 0), VerticesBuf(1, 0));
	//
	// A1B1C1
	dc.MoveTo(VerticesBuf(0, 3), VerticesBuf(1, 3));
	for (unsigned int j = 3; j < 6; j++)
		dc.LineTo(VerticesBuf(0, j), VerticesBuf(1, j));
	dc.LineTo(VerticesBuf(0, 3), VerticesBuf(1, 3));
	//
	// AA1B1BA
	dc.MoveTo(VerticesBuf(0, 0), VerticesBuf(1, 0));
	dc.LineTo(VerticesBuf(0, 3), VerticesBuf(1, 3));
	dc.LineTo(VerticesBuf(0, 4), VerticesBuf(1, 4));
	dc.LineTo(VerticesBuf(0, 1), VerticesBuf(1, 1));
	dc.LineTo(VerticesBuf(0, 0), VerticesBuf(1, 0));
	//
	// AA1C1CA
	dc.MoveTo(VerticesBuf(0, 0), VerticesBuf(1, 0));
	dc.LineTo(VerticesBuf(0, 3), VerticesBuf(1, 3));
	dc.LineTo(VerticesBuf(0, 5), VerticesBuf(1, 5));
	dc.LineTo(VerticesBuf(0, 2), VerticesBuf(1, 2));
	dc.LineTo(VerticesBuf(0, 0), VerticesBuf(1, 0));
	//
	// CC1B1BC
	dc.MoveTo(VerticesBuf(0, 2), VerticesBuf(1, 2));
	dc.LineTo(VerticesBuf(0, 5), VerticesBuf(1, 5));
	dc.LineTo(VerticesBuf(0, 4), VerticesBuf(1, 4));
	dc.LineTo(VerticesBuf(0, 1), VerticesBuf(1, 1));
	dc.LineTo(VerticesBuf(0, 2), VerticesBuf(1, 2));
	//
	Vertices = Buffer;
}
CMatrix CreateViewCoords(double r, double phi, double theta)
{
	phi = phi * PI / 180;
	theta = theta * PI / 180;
	CMatrix T(4, 4);
	T(0, 0) = -sin(phi);
	T(0, 1) = cos(phi);
	T(1, 0) = -cos(theta)*cos(phi);
	T(1, 1) = -cos(theta)*sin(phi);
	T(1, 2) = sin(theta);
	T(2, 0) = -sin(theta)*cos(phi);
	T(2, 1) = -sin(theta)*sin(phi);
	T(2, 2) = -cos(theta);
	T(2, 3) = r;
	T(0, 2) = T(0, 3) = T(1, 3) = T(3, 0) = T(3, 1) = T(3, 2) = 0;
	T(3, 3) = 1;
	return T;
}