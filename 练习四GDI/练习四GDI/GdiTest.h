#pragma once
#include"T_Engine.h"

class GdiTest :public T_Engine
{
public:

	GdiTest(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle,
		WORD Icon = NULL, WORD SmIcon = NULL,
		int iWidth = WIN_WIDTH, int iHeight = WIN_HEIGHT,
		COLORREF bkColor = WIN_BKCLR);
	virtual ~GdiTest(void);
	void GameInit();
	// 游戏逻辑处理
	void GameLogic();
	// 游戏结束处理
	void GameEnd();
	// 根据GAME_STATE值显示游戏界面
	void GamePaint(HDC hdc);
	// 根据KM_ACTION值处理按键行为
	void GameKeyAction(int ActionType = KEY_SYS_NONE);
	// 根据KM_ACTION值处理鼠标行为
	void GameMouseAction(int x, int y, int ActionType);
private:
	int wnd_width, wnd_height;//游戏窗口宽高

	HBITMAP hBitmap;       //保存已经加载的位图的句柄
	int bmpWidth, bmpHeight;//已加载位图的宽、高
	BOOL LoadBmp(LPTSTR path);//位图文件的加载

	HBITMAP CreateBlankBMP(HDC hDC, int ibWidth, int ibHeight, COLORREF crColor);
	HBITMAP CreateBlankBMP(int ibWidth, int ibHeight, COLORREF crColor);

	void TestBitBlt(HDC hdc);
	void TestStretchBlt(HDC hdc);

	void TestAlphaBlend(HDC hdc);

	int blend;
	void TestAlphaBlend2(HDC hdc);
	
};