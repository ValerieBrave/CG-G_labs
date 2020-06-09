#pragma once
#include "resource.h"
#include "CMatrix.h"
#include "LibGraph.h"
#include "LibPyramid.h"


class CMainWnd : public CFrameWnd
{
private:
	CPyramid PIR;
	CRect WinRect;
	CMatrix PView; 
	CMatrix PLight; 
	int Index;
	CMenu menu;
	DECLARE_MESSAGE_MAP()
	int OnCreate(LPCREATESTRUCT);

public:
	CMainWnd::CMainWnd() 
	{
		Create(NULL, L"7", WS_OVERLAPPEDWINDOW, CRect(20, 20, 900, 700), NULL, NULL);
		Index = 0;
		PView.RedimMatrix(3);
		PView(0) = 10; PView(1) = 315; PView(2) = 135;	// точка наблюдения
		PLight.RedimMatrix(3);
		PLight(0) = 10; PLight(1) = 315; PLight(2) = 135;	// положение источника свеат

	}

	void OnPaint();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnSize(UINT nType, int cx, int cy);
	void OnPuramid1();
	void Exit();
};

BEGIN_MESSAGE_MAP(CMainWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_COMMAND(ID_30004, OnPuramid1)
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


void CMainWnd::OnPaint()
{
	CPaintDC dc(this);
	if (Index == 1)
		PIR.ColorDraw(dc, PView, WinRect, RGB(70, 200, 5), PLight);
}

void CMainWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
		switch (nChar)
		{
		case VK_UP: 
		{
			double d = PLight(2) - 3;
			if (d >= 0)PLight(2) = d;
			else {
				PLight(2) = d + 360;
			}
			break;
		}
		case VK_DOWN: 
		{
			double d = PLight(2) + 3;
			if (d <= 180)
				PLight(2) = d;
			else {
				PLight(2) = d + 360;
			}
			break;
		}
		case VK_LEFT: 
		{
			double d = PLight(1) - 3;
			if (d >= -180)
				PLight(1) = d;
			else
				PLight(1) = d + 360;
			break;
		}
		case VK_RIGHT: 
		{
			double d = PLight(1) + 3;
			if (d <= 180)
				PLight(1) = d;
			else
				PLight(1) = d - 360;
			break;
		}
		}
		Invalidate();
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMainWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	WinRect.SetRect(100, 100, cx - 100, cy - 100);
}

void CMainWnd::OnPuramid1()
{
	Index = 1;
	Invalidate();
}
void CMainWnd::Exit()
{
	DestroyWindow();
}