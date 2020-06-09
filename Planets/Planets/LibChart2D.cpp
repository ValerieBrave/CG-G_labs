#include "stdafx.h"
#include "CMatrix.h"
#include "LibChart2D.h"

void setMode(CDC& dc, CRect& rectWindow, CRectD& rectWorld)
{
	double dsx = rectWorld._right - rectWorld._left;
	double dsy = rectWorld._top - rectWorld._bottom;
	double xsL = rectWorld._left;
	double ysL = rectWorld._bottom;

	int dwx = rectWindow.right - rectWindow.left;
	int dwy = rectWindow.bottom - rectWindow.top;
	int xwL = rectWindow.left;
	int ywH = rectWindow.bottom;

	dc.SetMapMode(MM_ANISOTROPIC);
	dc.SetWindowExt((int)dsx, (int)dsy);
	dc.SetViewportExt(dwx, -dwy);
	dc.SetWindowOrg((int)xsL, (int)ysL);
	dc.SetViewportOrg(xwL, ywH);
}

void CRectD::setRectD(double left, double top, double right, double bottom)
{
	_left = left;
	_top = top;
	_right = right;
	_bottom = bottom;
}

CSizeD CRectD::SizeD()
{
	CSizeD cz;
	cz._cx = fabs(_right - _left);
	cz._cy = fabs(_top - _bottom);
	return cz;
}

CMatrix getTranslation(double xDistance, double yDistance)
{
	CMatrix translator(3, 3);
	translator(0, 0) = 1;
	translator(0, 2) = xDistance;
	translator(1, 1) = 1;
	translator(1, 2) = yDistance;
	translator(2, 2) = 1;
	return translator;
}

CMatrix getRotation(double angle)
{
	double fg = fmod(angle, 360.0);
	double ff = (fg / 180.0) * PI;
	CMatrix rotator(3, 3);
	rotator(0, 0) = cos(ff);
	rotator(0, 1) = -sin(ff);
	rotator(1, 0) = sin(ff);
	rotator(1, 1) = cos(ff);
	rotator(2, 2) = 1;
	return rotator;
}

CSolarSystem::CSolarSystem()
{
	double rS = 100, rE = 70, rM = 30, rMerc = 30;
	double RoE = 10 * rS, RoM = 5 * rE, RoMerc = 5 * rS, RoV = 20 * rS;
	double d = RoE + RoM + rM + RoV;

	_rectWorld.setRectD(-d, d, d, -d);
	_rectWindow.SetRect(100, -200, 900, 600);
	_sun.SetRect(-rS, rS, rS, -rS);
	_earth.SetRect(-rE, rE, rE, -rE);
	_moon.SetRect(-rM, rM, rM, -rM);
	//_mercury.SetRect(-rMerc, rMerc, rMerc, -rMerc);

	_earthOrbit.SetRect(-RoE + 3000, RoE, RoE, -RoE);
	_moonOrbit.SetRect(-RoM, RoM, RoM, -RoM);
	_mercuryOrbit.SetRect(-RoMerc, RoMerc, RoMerc, -RoMerc);

	_earthAngularPosition = 0;
	_moonAngularPosition = 0;
	_mercuryAngularPosition = 0;

	_earthAngularVelocity = -5;
	_moonAngularVelocity = 10;
	_mercuryAngularVelocity = -30;

	_dt = 1;

	_earthCoords.RedimMatrix(3);
	_moonCoords.RedimMatrix(3);
	_mercuryCoords.RedimMatrix(3);

	_earthCoords(2) = 1;
	_moonCoords(2) = 1;
	_mercuryCoords(2) = 1;
}

void CSolarSystem::setCoords()
{
	double RoM = (_moonOrbit.right - _moonOrbit.left) / 2;
	double ff = (_moonAngularPosition / 180.0) * PI;
	double x = RoM * cos(ff);
	double y = RoM * sin(ff);
	_moonCoords(0) = x;
	_moonCoords(1) = y;
	_moonAngularPosition += _moonAngularVelocity * _dt;
	CMatrix P = getRotation(_moonAngularPosition);
	_moonCoords = P * _moonCoords;

	double RoE = (_earthOrbit.right - _earthOrbit.left) / 2;
	ff = (_earthAngularPosition / 180.0) * PI;
	x = RoE * cos(ff);
	y = RoE * sin(ff);
	_earthCoords(0) = x;
	_earthCoords(1) = y;
	P = getTranslation(x, y);
	_moonCoords = P * _moonCoords;

	_earthAngularPosition += _earthAngularVelocity * _dt;
	P = getRotation(_earthAngularPosition);
	_moonCoords = P * _moonCoords;
	_earthCoords = P * _earthCoords;

	/*double RoMerc = (_mercuryOrbit.right - _mercuryOrbit.left) / 2;
	ff = (_mercuryAngularPosition / 180.0) * PI;
	x = RoMerc * cos(ff);
	y = RoMerc * sin(ff);
	_mercuryCoords(0) = x;
	_mercuryCoords(1) = y;

	_mercuryAngularPosition += _mercuryAngularVelocity * _dt;
	P = getRotation(_mercuryAngularPosition);
	_mercuryCoords = P * _mercuryCoords;*/
}

void CSolarSystem::draw(CDC& dc)
{
	CBrush SBrush, EBrush, MBrush, MercBrush, *pOldBrush;
	CRect R;

	SBrush.CreateSolidBrush(RGB(255, 126, 24));
	EBrush.CreateSolidBrush(RGB(44, 130, 242));
	MBrush.CreateSolidBrush(RGB(107, 120, 142));
	MercBrush.CreateSolidBrush(RGB(100, 100, 100));

	dc.SelectStockObject(NULL_BRUSH);
	//dc.Ellipse(_earthOrbit);
	dc.Ellipse(_mercuryOrbit);

	int d = _moonOrbit.right;
	R.SetRect(_earthCoords(0) - d, _earthCoords(1) + d, _earthCoords(0) + d, _earthCoords(1) - d);
	dc.Ellipse(R);

	pOldBrush = dc.SelectObject(&SBrush);
	dc.Ellipse(_sun);

	d = _earth.right;
	R.SetRect(_earthCoords(0) - d, _earthCoords(1) + d, _earthCoords(0) + d, _earthCoords(1) - d);
	dc.SelectObject(&EBrush);
	dc.Ellipse(R);

	/*d = _mercury.right;
	R.SetRect(_mercuryCoords(0) - d, _mercuryCoords(1) + d, _mercuryCoords(0) + d, _mercuryCoords(1) - d);
	dc.SelectObject(&MercBrush);
	dc.Ellipse(R);*/

	d = _moon.right;
	R.SetRect(_moonCoords(0) - d, _moonCoords(1) + d, _moonCoords(0) + d, _moonCoords(1) - d);
	dc.SelectObject(&MBrush);
	dc.Ellipse(R);

	dc.SelectObject(pOldBrush);
}

void CSolarSystem::getRectWorld(CRectD& RSX)
{
	RSX._left = _rectWorld._left;
	RSX._top = _rectWorld._top;
	RSX._right = _rectWorld._right;
	RSX._bottom = _rectWorld._bottom;
}