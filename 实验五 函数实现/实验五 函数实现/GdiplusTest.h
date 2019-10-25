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
	static const long T_LENGTH = 4500;   //���ʱ��
	HBITMAP hbitmap; //��ǰ��λͼ���
	int cell_width, cell_height;         //Ҫ�ָ�ĵ�Ԫ����
	int rows, cols;						//���ڷָ������������
	long startTime;						//��ʱ
	POINT mousePT;						//��ǰ���λ��
	int* randCell;						//��¼�����������
	int ratio;							//���ַŴ���С�ı���
	bool enlarge;					//�Ƿ�Ŵ�
	HBITMAP GetBmpHandle() { return hbitmap; }//���ص�ǰ��λͼ���
	int frameCount;
	int x;
	int y;
	int dir;
	//����ֱ�Ϊ�˶����������
	static const int frame_width = 74;
	static const int frame_height = 115;
	//ͼƬ��ʾ����Ķ���

	Bitmap* HBITMAP_To_Bitmap(HBITMAP hbmp,HDC hdc);
	void PaintRegion(HDC destDC, int destX, int destY,
		int srcX, int srcY, int regionWidth, int regionHeight,
		float ratio, int rotType = 0, BYTE alpha = 255);
	void PaintText(HDC hdc, RectF fontRect, LPCTSTR text, REAL fontSize,
		LPCTSTR fontName, Color fontColor, FontStyle style = FontStyleBold,
		StringAlignment align = StringAlignmentCenter);
	void GetRandomNum(int range, int* iArray);   //��������rangeָ����Χ�ĵȶ������
};