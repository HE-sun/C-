#pragma once
#include"T_Engine.h"

class GdiplusTest :public T_Engine
{
public:

	GdiplusTest(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle,
		WORD Icon = NULL, WORD SmIcon = NULL,
		int iWidth = WIN_WIDTH, int iHeight = WIN_HEIGHT,
		COLORREF bkColor = WIN_BKCLR);
	virtual ~GdiplusTest(void);
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
	static const long T_LENGTH = 4500;   //间隔时间
	HBITMAP hbitmap; //当前的位图句柄
	int cell_width, cell_height;         //要分割的单元格宽高
	int rows, cols;						//窗口分割的行数和列数
	long startTime;						//计时
	POINT mousePT;						//当前鼠标位置
	int* randCell;						//记录随机数的数组
	int ratio;							//文字放大缩小的倍数
	bool enlarge;					//是否放大
	HBITMAP GetBmpHandle() { return hbitmap; }//返回当前的位图句柄
	int frameCount;
	int x;
	int y;
	int dir;
	//三项分别为运动方向和坐标
	static const int frame_width = 74;
	static const int frame_height = 115;
	//图片显示区域的定义

	Bitmap* HBITMAP_To_Bitmap(HBITMAP hbmp,HDC hdc);
	void PaintRegion(HDC destDC, int destX, int destY,
		int srcX, int srcY, int regionWidth, int regionHeight,
		float ratio, int rotType = 0, BYTE alpha = 255);
	void PaintText(HDC hdc, RectF fontRect, LPCTSTR text, REAL fontSize,
		LPCTSTR fontName, Color fontColor, FontStyle style = FontStyleBold,
		StringAlignment align = StringAlignmentCenter);
	void GetRandomNum(int range, int* iArray);   //产生参数range指定范围的等额随机数
};