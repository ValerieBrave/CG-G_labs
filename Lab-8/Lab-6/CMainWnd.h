#pragma once
#include "resource.h"
#include "CMatrix.h"
#include <float.h> // DBL_MAX, DBL_MIN
#include <math.h>
#include <vector>

#include "LibGraph.h"
#include "LibSurface.h"


class CMainWnd : public CFrameWnd
{
private:
	CRect WinRect; // Область в окне
	int Index;
	CMatrix PView;
	CMatrix PView2;
	int red, green, blue;
	CMenu menu;
	DECLARE_MESSAGE_MAP()
	int OnCreate(LPCREATESTRUCT);

public:
	CMainWnd::CMainWnd()
	{
		Create(NULL, L"Lab-8", WS_OVERLAPPEDWINDOW, CRect(10, 10, 700, 700), NULL, NULL);
		Index = 0;
		PView.RedimMatrix(3);
		PView2.RedimMatrix(3);
		red = 0, green = 60, blue = 160;
		PView(0) = 10; PView(1) = 45; PView(2) = 50;
		PView2(0) = 15; PView2(1) = 35; PView2(2) = 65;
	}

	void OnPaint();
	void OnSize(UINT nType, int cx, int cy);

	void OnDiffuseModelCamera1();
	void OnDiffuseModelCamera2();
	void OnDiffuseModelLight1();
	void OnDiffuseModelLight2();
	void OnDiffuseModelColor1();
	void OnDiffuseModelColor2();
	void OnMirrorModelCamera1();
	void OnMirrorModelCamera2();
	void OnMirrorModelLight1();
	void OnMirrorModelLight2();
	void OnMirrorModelColor1();
	void OnMirrorModelColor2();
	void Exit();
};

BEGIN_MESSAGE_MAP(CMainWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_COMMAND(ID_DIFFUSE_CAMERA1, OnDiffuseModelCamera1)
	ON_COMMAND(ID_DIFFUSE_CAMERA2, OnDiffuseModelCamera2)
	ON_COMMAND(ID_DIFFUSE_LIGHT1, OnDiffuseModelLight1)
	ON_COMMAND(ID_DIFFUSE_LIGHT2, OnDiffuseModelLight2)
	ON_COMMAND(ID_DIFFUSE_COLOR1, OnDiffuseModelColor1)
	ON_COMMAND(ID_DIFFUSE_COLOR2, OnDiffuseModelColor2)

	ON_COMMAND(ID_MIRROR_CAMERA1, OnMirrorModelCamera1)
	ON_COMMAND(ID_MIRROR_CAMERA2, OnMirrorModelCamera2)
	ON_COMMAND(ID_MIRROR_LIGHT1, OnMirrorModelLight1)
	ON_COMMAND(ID_MIRROR_LIGHT2, OnMirrorModelLight2)
	ON_COMMAND(ID_MIRROR_COLOR1, OnMirrorModelColor1)
	ON_COMMAND(ID_MIRROR_COLOR2, OnMirrorModelColor2)

	ON_COMMAND(ID_EXIT, Exit)
END_MESSAGE_MAP()
int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	menu.LoadMenu(IDR_MENU1); // Загрузить меню из файла ресурса
	SetMenu(&menu); // Установить меню
	return 0;
}
void CMainWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	WinRect.SetRect(0, cy - 300, 300, 0);
}


void CMainWnd::OnPaint()
{
	CPaintDC dc(this);
	if (Index == 1)
		DrawLightSphere(dc, 10, PView, PView2, WinRect, COLORREF(RGB(red, green, blue)), 0);
	if (Index == 2)
		DrawLightSphere(dc, 10, PView, PView2, WinRect, COLORREF(RGB(red, green, blue)), 1);
}



void CMainWnd::OnDiffuseModelCamera1()
{
	PView(0) = 10; PView(1) = 45; PView(2) = 50;
	Index = 1;
	Invalidate();
}
void CMainWnd::OnDiffuseModelCamera2()
{
	PView(0) = 10; PView(1) = 135; PView(2) = 50;
	Index = 1;
	Invalidate();
}
void CMainWnd::OnDiffuseModelLight1()
{
	PView2(0) = 15; PView2(1) = 35; PView2(2) = 65;
	Index = 1;
	Invalidate();
}
void CMainWnd::OnDiffuseModelLight2()
{
	PView2(0) = 15; PView2(1) = 135; PView2(2) = 65;
	Index = 1;
	Invalidate();
}
void CMainWnd::OnDiffuseModelColor1()
{
	red = 0, green = 60, blue = 160;
	Index = 1;
	Invalidate();
}
void CMainWnd::OnDiffuseModelColor2()
{
	red = 160, green = 60, blue = 0;
	Index = 1;
	Invalidate();
}

void CMainWnd::OnMirrorModelCamera1()
{
	PView(0) = 10; PView(1) = 45; PView(2) = 50;
	Index = 2;
	Invalidate();
}
void CMainWnd::OnMirrorModelCamera2()
{
	PView(0) = 10; PView(1) = 135; PView(2) = 50;
	Index = 2;
	Invalidate();
}
void CMainWnd::OnMirrorModelLight1()
{
	PView2(0) = 15; PView2(1) = 35; PView2(2) = 65;
	Index = 1;
	Invalidate();
}
void CMainWnd::OnMirrorModelLight2()
{
	PView2(0) = 15; PView2(1) = 135; PView2(2) = 65;
	Index = 1;
	Invalidate();
}
void CMainWnd::OnMirrorModelColor1()
{
	red = 0, green = 60, blue = 160;
	Index = 1;
	Invalidate();
}
void CMainWnd::OnMirrorModelColor2()
{
	red = 160, green = 60, blue = 0;
	Index = 1;
	Invalidate();
}


void CMainWnd::Exit()
{
	DestroyWindow();
}