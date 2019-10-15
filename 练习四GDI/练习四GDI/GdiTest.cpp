#include"GdiTest.h"

int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	GdiTest* pTest = new GdiTest(h_instance, WIN_CLASS, L"T_Engine测试程序", NULL, NULL, WIN_WIDTH, WIN_HEIGHT, RGB(0, 0, 0));
	T_Engine::pEngine = pTest;
	pTest->SetFrame(10);
	pTest->StartEngine();
	//pTest->SetFullScreen(TRUE);
	return 0;
}
GdiTest::GdiTest(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle,
	WORD Icon, WORD SmIcon, int iWidth, int iHeight, COLORREF bkColor)
	:T_Engine(hInstance, szWindowClass, szTitle,
		Icon, SmIcon, iWidth, iHeight, bkColor)
{
	wnd_width = iWidth;
	wnd_height = iHeight;
}
GdiTest::~GdiTest(void) {

}
BOOL GdiTest::LoadBmp(LPTSTR path)//位图文件的加载
{

	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(0), path,
		IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	if (!hBitmap) return FALSE;  // 如果位图加载失败
	DIBSECTION dib;
	// 如果位图信息读取失败
	if (GetObject(hBitmap, sizeof(DIBSECTION), &dib) != sizeof(DIBSECTION))
	{
		DeleteObject(hBitmap);
		hBitmap = 0;
		return FALSE;
	}
	bmpWidth = dib.dsBmih.biWidth;
	bmpHeight = dib.dsBmih.biHeight;
	return TRUE;
};

HBITMAP GdiTest::CreateBlankBMP(HDC hDC, int ibWidth, int ibHeight, COLORREF crColor)
{
	//创建一个空的地图
	HBITMAP bitmap = CreateCompatibleBitmap(hDC, ibWidth, ibHeight);
	if (bitmap == NULL)return FALSE;
	//创建一个内存设备用来绘制位图
	HDC memDC = CreateCompatibleDC(hDC);
	//创建一个单色画刷
	HBRUSH hBrush = CreateSolidBrush(crColor);
	//创建的位图替换为内存设备中的原来对象
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, bitmap);
	//用制定的颜色填充位图
	RECT rcBitmap = { 0, 0, ibWidth, ibHeight };
	FillRect(memDC, &rcBitmap, hBrush);
	//还原：使用原来对象替换内存设备中的位图对象
	SelectObject(memDC, hOldBitmap);
	//删除内存设备
	DeleteDC(memDC);
	//删除笔刷
	DeleteObject(hBrush);
	return bitmap;
};
HBITMAP GdiTest::CreateBlankBMP(int ibWidth, int ibHeight, COLORREF crColor)
{
	void* Bits;				// 指向保存位图位值的地址
	BITMAPINFOHEADER Header;	// 位图头信息
	Header.biSize = sizeof(BITMAPINFOHEADER);
	Header.biWidth = ibWidth;
	Header.biHeight = ibHeight;
	Header.biBitCount = 24;
	Header.biPlanes = 1;
	Header.biCompression = BI_RGB;
	HBITMAP hbmp = CreateDIBSection(
		NULL, (BITMAPINFO*)&Header, DIB_RGB_COLORS, &Bits, NULL, 0);
	if (hbmp != NULL) //使用指定颜色填充空白位图
	{
		//创建一个内存设备用来绘制位图
		HDC memDC = CreateCompatibleDC(GetDC(NULL));
		//创建一个单色画刷，用于填充所创建的空位图
		HBRUSH hBrush = CreateSolidBrush(crColor);
		//将创建的空位图替换内存设备中的原来对象
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hbmp);
		//用指定的颜色填充位图
		RECT rcBitmap = { 0, 0, ibWidth, ibHeight };
		FillRect(memDC, &rcBitmap, hBrush);
		// 恢复原有的位图对象
		SelectObject(memDC, hOldBitmap);
		DeleteDC(memDC); //删除内存设备
		DeleteObject(hBrush); //删除笔刷
		DeleteObject(Bits);
		Bits = NULL;
	}
	return hbmp;
};
void GdiTest::TestBitBlt(HDC hdc)
{
	int w = 512; int h = 384;

	HDC memdc = CreateCompatibleDC(hdc);

	HBITMAP blank = CreateBlankBMP(hdc, w, h, RGB(255, 0, 0)); //DDB
	HBITMAP hOldBmp = (HBITMAP)SelectObject(memdc,blank);
	BitBlt(hdc, 0, 0, w, h, memdc, 0, 0,SRCCOPY);
	SelectObject(memdc, hOldBmp);//恢复原有位图对象
	DeleteObject(blank);
	blank = NULL;

	blank = CreateBlankBMP(hdc, w, h, RGB(255, 255, 0)); //DDB
	hOldBmp = (HBITMAP)SelectObject(memdc, blank);
	BitBlt(hdc, w, 0, w, h, memdc, 0, 0, SRCCOPY);
	SelectObject(memdc, hOldBmp);//恢复原有位图对象
	DeleteObject(blank);
	blank = NULL;

	blank = CreateBlankBMP(w, h, RGB(0, 255, 0)); //DIB
	hOldBmp = (HBITMAP)SelectObject(memdc, blank);
	BitBlt(hdc, 0, h, w, h, memdc, 0, 0, SRCCOPY);
	SelectObject(memdc, hOldBmp);//恢复原有位图对象
	DeleteObject(blank);
	blank = NULL;

	blank = CreateBlankBMP(w, h, RGB(0,0,255)); //DIB
	hOldBmp = (HBITMAP)SelectObject(memdc, blank);
	BitBlt(hdc, w, h, w, h, memdc, 0, 0, SRCCOPY);
	SelectObject(memdc, hOldBmp);//恢复原有位图对象
	DeleteObject(blank);
	blank = NULL;

	DeleteDC(memdc);//删除内存设备
};
void GdiTest::TestStretchBlt(HDC hdc) 
{
	int w = 512;
	int h = 384;
	HBITMAP hOldBmp = NULL;

	HDC memdc = CreateCompatibleDC(hdc);
	SetStretchBltMode(hdc, COLORONCOLOR);

	LoadBmp(L".\\res\\leaf.bmp");
	hOldBmp = (HBITMAP)SelectObject(memdc, hBitmap);
	StretchBlt(hdc, 0, 0, w, h, memdc, 0, 0, bmpWidth, bmpHeight, SRCCOPY);
	SelectObject(memdc,hOldBmp); //恢复原有的位图对象
	DeleteObject(hBitmap);
	hBitmap = NULL;

	LoadBmp(L".\\res\\island.bmp");
	hOldBmp = (HBITMAP)SelectObject(memdc, hBitmap);
	StretchBlt(hdc, w, 0, w, h, memdc, 0, 0, bmpWidth, bmpHeight, SRCCOPY);
	SelectObject(memdc, hOldBmp); //恢复原有的位图对象
	DeleteObject(hBitmap);
	hBitmap = NULL;

	LoadBmp(L".\\res\\tulip.bmp");
	hOldBmp = (HBITMAP)SelectObject(memdc, hBitmap);
	StretchBlt(hdc, 0, h, w, h, memdc, 0, 0, bmpWidth, bmpHeight, SRCCOPY);
	SelectObject(memdc, hOldBmp); //恢复原有的位图对象
	DeleteObject(hBitmap);
	hBitmap = NULL;

	LoadBmp(L".\\res\\drop.bmp");
	hOldBmp = (HBITMAP)SelectObject(memdc, hBitmap);
	StretchBlt(hdc, w, h, w, h, memdc, 0, 0, bmpWidth, bmpHeight, SRCCOPY);
	SelectObject(memdc, hOldBmp); //恢复原有的位图对象
	DeleteObject(hBitmap);
	hBitmap = NULL;

	DeleteDC(memdc);//删除内存设备
};
void GdiTest::TestAlphaBlend(HDC hdc)
{
	int w = 512;
	int h = 384;

	BLENDFUNCTION rBlendProps;
	rBlendProps.BlendOp = AC_SRC_OVER;
	rBlendProps.BlendFlags = 0;
	rBlendProps.AlphaFormat = 0;
	rBlendProps.SourceConstantAlpha =20;//透明度：0~255逐渐不透明

	HDC memdc = CreateCompatibleDC(hdc);

	LoadBmp(L".\\res\\leaf.bmp");
	HBITMAP hOld = (HBITMAP)SelectObject(memdc, hBitmap);
	AlphaBlend(hdc,0,0,bmpWidth,bmpHeight,
				memdc,0,0,bmpWidth,bmpHeight,rBlendProps);
	SelectObject(memdc, hOld); //恢复原有的位图对象
	DeleteObject(hBitmap);
	hBitmap = NULL;

	rBlendProps.SourceConstantAlpha = 10;

	LoadBmp(L".\\res\\drop.bmp");
	hOld = (HBITMAP)SelectObject(memdc, hBitmap);
	AlphaBlend(hdc, 0, 0, w, h,
		memdc, 0, 0, bmpWidth, bmpHeight, rBlendProps);
	SelectObject(memdc, hOld); //恢复原有的位图对象
	DeleteObject(hBitmap);
	hBitmap = NULL;

	LoadBmp(L".\\res\\tulip.bmp");
	hOld = (HBITMAP)SelectObject(memdc, hBitmap);
	AlphaBlend(hdc, w, 0, w, h,
		memdc, 0, 0, bmpWidth, bmpHeight, rBlendProps);
	SelectObject(memdc, hOld); //恢复原有的位图对象
	DeleteObject(hBitmap);
	hBitmap = NULL;

	LoadBmp(L".\\res\\island.bmp");
	hOld = (HBITMAP)SelectObject(memdc, hBitmap);
	AlphaBlend(hdc, 0, h, w, h,
		memdc, 0, 0, bmpWidth, bmpHeight, rBlendProps);
	SelectObject(memdc, hOld); //恢复原有的位图对象
	DeleteObject(hBitmap);
	hBitmap = NULL;

	LoadBmp(L".\\res\\sea.bmp");
	hOld = (HBITMAP)SelectObject(memdc, hBitmap);
	AlphaBlend(hdc, w, h, w, h,
		memdc, 0, 0, bmpWidth, bmpHeight, rBlendProps);
	SelectObject(memdc, hOld); //恢复原有的位图对象
	DeleteObject(hBitmap);
	hBitmap = NULL;
};
void GdiTest::TestAlphaBlend2(HDC hdc) 
{
	BLENDFUNCTION rBlendProps;
	rBlendProps.BlendOp = AC_SRC_OVER;
	rBlendProps.BlendFlags = 0;
	rBlendProps.AlphaFormat = 0;
	rBlendProps.SourceConstantAlpha = 255; //透明度：0完全透明；255完全不透明

	HDC memdc = CreateCompatibleDC(hdc);

	LoadBmp(L".\\res\\lighttower.bmp");
	HBITMAP hOld = (HBITMAP)SelectObject(memdc, hBitmap);
	AlphaBlend(hdc, 0, 0, bmpWidth, bmpHeight,
		memdc, 0, 0, bmpWidth, bmpHeight, rBlendProps);
	SelectObject(memdc, hOld); //恢复原有的位图对象
	DeleteObject(hBitmap);
	hBitmap = NULL;

	rBlendProps.SourceConstantAlpha = blend;

	LoadBmp(L".\\res\\fog.bmp");
	hOld = (HBITMAP)SelectObject(memdc, hBitmap);
	AlphaBlend(hdc, 0, 0, bmpWidth, bmpHeight,
		memdc, 0, 0, bmpWidth, bmpHeight, rBlendProps);
	SelectObject(memdc, hOld); //恢复原有的位图对象
	DeleteObject(hBitmap);
	hBitmap = NULL;

	DeleteDC(memdc);

	blend = blend + 10;
	if(blend>=255)
	{
		blend = 10;
	}
	Sleep(100);
};
void GdiTest::GameInit()
{

}
// 游戏逻辑处理
void GdiTest::GameLogic() {

}
// 游戏结束处理

void GdiTest::GameEnd() {

}
// 根据GAME_STATE值显示游戏界面
void GdiTest::GamePaint(HDC hdc) {
	//TestBitBlt(hdc);
	//TestStretchBlt(hdc);
	//TestAlphaBlend(hdc);
	TestAlphaBlend2(hdc);
}
// 根据KM_ACTION值处理按键行为
void GdiTest::GameKeyAction(int ActionType) {

}
// 根据KM_ACTION值处理鼠标行为
void GdiTest::GameMouseAction(int x, int y, int ActionType) {

}