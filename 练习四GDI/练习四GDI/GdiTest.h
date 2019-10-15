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
	// ��Ϸ�߼�����
	void GameLogic();
	// ��Ϸ��������
	void GameEnd();
	// ����GAME_STATEֵ��ʾ��Ϸ����
	void GamePaint(HDC hdc);
	// ����KM_ACTIONֵ��������Ϊ
	void GameKeyAction(int ActionType = KEY_SYS_NONE);
	// ����KM_ACTIONֵ���������Ϊ
	void GameMouseAction(int x, int y, int ActionType);
private:
	int wnd_width, wnd_height;//��Ϸ���ڿ��

	HBITMAP hBitmap;       //�����Ѿ����ص�λͼ�ľ��
	int bmpWidth, bmpHeight;//�Ѽ���λͼ�Ŀ���
	BOOL LoadBmp(LPTSTR path);//λͼ�ļ��ļ���

	HBITMAP CreateBlankBMP(HDC hDC, int ibWidth, int ibHeight, COLORREF crColor);
	HBITMAP CreateBlankBMP(int ibWidth, int ibHeight, COLORREF crColor);

	void TestBitBlt(HDC hdc);
	void TestStretchBlt(HDC hdc);

	void TestAlphaBlend(HDC hdc);

	int blend;
	void TestAlphaBlend2(HDC hdc);
	
};