#include"GdiplusTest.h"


GdiplusTest::GdiplusTest(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle,
	WORD Icon, WORD SmIcon, int iWidth, int iHeight, COLORREF bkColor)
	:T_Engine(hInstance, szWindowClass, szTitle,
		Icon, SmIcon, iWidth, iHeight, bkColor)
{
	wnd_width = iWidth;
	wnd_height = iHeight;

	frameCount = 0;
	dir = DIR_RIGHT;
	 x = (wnd_width - frame_width) / 2;
	 y = wnd_height - frame_height - 70;
}
GdiplusTest::~GdiplusTest(void) {

}
Bitmap* GdiplusTest::HBITMAP_To_Bitmap(HBITMAP hbmp, HDC hdc)
{
	BITMAP bmp;
	int width, height;
	// 根据位图句柄获取BITMAP结构体对象的数据
	memset((void*)&bmp, 0, sizeof(BITMAP));
	GetObject(hbmp, sizeof(BITMAP), (void*)&bmp);
	width = bmp.bmWidth;
	height = bmp.bmHeight;
	byte* bmpBytes = (byte*)bmp.bmBits;
	if (width <= 0 && height <= 0) return NULL;

	// 根据位图宽高新建一个PixelFormat32bppPARGB格式的Bitmap对象
	Bitmap* BmpCpy = new Bitmap(width, height, PixelFormat32bppPARGB);
	// 采用LockBits函数为位图像素数据拷贝准备内存缓冲区
	BitmapData bmpData;
	Rect rect(0, 0, width, height);
	BmpCpy->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppPARGB, &bmpData);
	// 计算出位图每行像素数，并将指针指向首行
	int lineSize = width * 4;
	byte* cpyBytes = (byte*)(bmpData.Scan0);
	// 按照位图的高逐行拷贝像素数据至缓冲区
	for (int y = 0; y < height; y++)
	{
		memcpy((y * lineSize) + cpyBytes, ((height - y - 1) * lineSize) + bmpBytes, lineSize);
	}
	// 解锁内存缓冲区
	BmpCpy->UnlockBits(&bmpData);
	return BmpCpy;
}

void GdiplusTest::PaintRegion(HDC destDC, int destX, int destY,    
	int srcX, int srcY, int regionWidth, int regionHeight,
	float ratio, int rotType, BYTE alpha)
{
	int width = (int)(regionWidth*ratio);
	int height = (int)(regionHeight*ratio);

	BYTE* pBits = NULL;
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = width * height * 4;

	HDC frameDC = ::CreateCompatibleDC(NULL);
	HBITMAP hbitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void **)&pBits, NULL, NULL);
	HBITMAP OldFrameBmp = (HBITMAP)SelectObject(frameDC, hbitmap);

	BLENDFUNCTION frame_bf;
	frame_bf.BlendOp = AC_SRC_OVER;
	frame_bf.BlendFlags = 0;
	frame_bf.SourceConstantAlpha = 255;
	frame_bf.AlphaFormat = AC_SRC_ALPHA;

	HDC memDC = CreateCompatibleDC(destDC);
	HBITMAP OldMemBmp = (HBITMAP)SelectObject(memDC, GetBmpHandle());

	AlphaBlend(frameDC, 0, 0, (int)(regionWidth*ratio), (int)(regionHeight*ratio),
		memDC, srcX, srcY, regionWidth, regionHeight, frame_bf);

	Bitmap* nowFrameBmp = HBITMAP_To_Bitmap(hbitmap, destDC);

	//还原：使用原来对象替换内存设备中的位图对象
	SelectObject(frameDC, OldFrameBmp);
	DeleteDC(frameDC);//删除内存设备
	DeleteObject(OldFrameBmp);//删除位图对象
	DeleteObject(hbitmap);//删除位图对象

						  //还原：使用原来对象替换内存设备中的位图对象
	SelectObject(memDC, OldMemBmp);
	DeleteDC(memDC);//删除内存设备
	DeleteObject(OldMemBmp);//删除位图对象

							//旋转帧图
	int framew = (int)(regionWidth*ratio);
	int frameh = (int)(regionHeight*ratio);
	switch (rotType)
	{
	case TRANS_NONE:
		break;
	case TRANS_NOFLIP_ROT90:
		nowFrameBmp->RotateFlip(Rotate90FlipNone);
		framew = (int)(regionHeight*ratio);
		frameh = (int)(regionWidth*ratio);
		break;
	case TRANS_NOFLIP_ROT180:
		nowFrameBmp->RotateFlip(Rotate180FlipNone);
		framew = (int)(regionWidth*ratio);
		frameh = (int)(regionHeight*ratio);
		break;
	case TRANS_NOFLIP_ROT270:
		nowFrameBmp->RotateFlip(Rotate270FlipNone);
		framew = (int)(regionHeight*ratio);
		frameh = (int)(regionWidth*ratio);
		break;
	case TRANS_VFLIP_NOROT:
		nowFrameBmp->RotateFlip(RotateNoneFlipY);
		framew = (int)(regionWidth*ratio);
		frameh = (int)(regionHeight*ratio);
		break;
	case TRANS_VFLIP_ROT90:
		nowFrameBmp->RotateFlip(Rotate90FlipY);
		framew = (int)(regionHeight*ratio);
		frameh = (int)(regionWidth*ratio);
		break;
	case TRANS_VFLIP_ROT180:
		nowFrameBmp->RotateFlip(Rotate180FlipY);
		framew = (int)(regionWidth*ratio);
		frameh = (int)(regionHeight*ratio);
		break;
	case TRANS_VFLIP_ROT270:
		nowFrameBmp->RotateFlip(Rotate270FlipY);
		framew = (int)(regionHeight*ratio);
		frameh = (int)(regionWidth*ratio);
		break;
	case TRANS_HFLIP_NOROT:
		nowFrameBmp->RotateFlip(RotateNoneFlipX);
		framew = (int)(regionWidth*ratio);
		frameh = (int)(regionHeight*ratio);
		break;
	case TRANS_HFLIP_ROT90:
		nowFrameBmp->RotateFlip(Rotate90FlipX);
		framew = (int)(regionHeight*ratio);
		frameh = (int)(regionWidth*ratio);
		break;
	case TRANS_HFLIP_ROT180:
		nowFrameBmp->RotateFlip(Rotate180FlipX);
		framew = (int)(regionWidth*ratio);
		frameh = (int)(regionHeight*ratio);
		break;
	case TRANS_HFLIP_ROT270:
		nowFrameBmp->RotateFlip(Rotate270FlipX);
		framew = (int)(regionHeight*ratio);
		frameh = (int)(regionWidth*ratio);
		break;
	}

	HDC bufDC = CreateCompatibleDC(destDC);
	HBITMAP hbmp = NULL;
	nowFrameBmp->GetHBITMAP(Color(0, 0, 0, 0), &hbmp);
	HBITMAP OldbufBmp = (HBITMAP)SelectObject(bufDC, hbmp);

	BLENDFUNCTION buf_bf;
	buf_bf.BlendOp = AC_SRC_OVER;
	buf_bf.BlendFlags = 0;
	buf_bf.SourceConstantAlpha = alpha;
	buf_bf.AlphaFormat = AC_SRC_ALPHA;
	AlphaBlend(destDC, destX, destY, framew, frameh,
		bufDC, 0, 0, framew, frameh, buf_bf);

	//还原：使用原来对象替换内存设备中的位图对象
	SelectObject(bufDC, OldbufBmp);
	DeleteDC(bufDC);//删除内存设备
	DeleteObject(OldbufBmp);//删除位图对象
	DeleteObject(hbmp);//删除位图对象
	DeleteObject(nowFrameBmp);//删除位图对象
	delete nowFrameBmp;//删除位图对象
}

void GdiplusTest::PaintText(HDC hdc, RectF fontRect, LPCTSTR text, REAL fontSize,
	LPCTSTR fontName, Color fontColor, FontStyle style,
	StringAlignment align )
{
	//使用GDI方法进行绘制
	Graphics graph(hdc);
	FontFamily fontFamily(fontName);
	Font font(&fontFamily, fontSize, style, UnitPoint);

	StringFormat format;
	format.SetLineAlignment(StringAlignmentCenter);
	format.SetAlignment(align);
	SolidBrush solidBrush1(fontColor);

	graph.SetTextRenderingHint(TextRenderingHintAntiAlias);
	graph.DrawString(text, (INT)wcslen(text), &font, fontRect, &format, &solidBrush1);
	graph.ReleaseHDC(hdc);
}

void GdiplusTest::GameInit()
{	
	/*舞台动画
	Bitmap* pBmp = NULL;
	pBmp = Bitmap::FromFile(L".\\res\\walkinggirl74x115.png");
	//pBmp = Bitmap::FromResource(m_hInstance, MAKEINTRESOURCE());
	Status status = pBmp->GetHBITMAP(NULL, &hbitmap);
	if (status != S_OK)
	{
		hbitmap = NULL;
	}
	delete pBmp;
	*/
	//屏保
	Bitmap* pBmp = NULL;
	pBmp = Bitmap::FromFile(L".\\res\\Bing.jpg");
	Status status = pBmp->GetHBITMAP(NULL, &hbitmap);
	if (status != S_OK)  hbitmap = NULL;
	delete pBmp;
	mousePT.x = 0;
	mousePT.y = 0;
	rows = 5;
	cols = 5;
	cell_width = wnd_width / cols;
	cell_height = wnd_height / rows;
	startTime = GetTickCount();
	randCell = new int[rows*cols];
	ratio = 20;
	enlarge = true;
}
// 游戏逻辑处理
void GdiplusTest::GameLogic() {
	/*舞台动画
	GameKeyAction();//处理游戏按键
	if (frameCount < 8)
	{
		frameCount++;
	}
	if (frameCount >= 8)
	{
		frameCount = 0;
	}
	switch (dir)
	{
	case DIR_RIGHT:
		x = x + 10;
		if (x > wnd_width + frame_width)
		{
			x = wnd_width + frame_width;
			dir = DIR_LEFT;
		}
		break;
	case DIR_LEFT:
		x = x -10;
		if (x < 0 - frame_width)
		{
			x = 0 - frame_width;
			dir = DIR_RIGHT;
		}
		break;
	}
	*/
	//屏保
	GameKeyAction();
	if (enlarge == true)
	{
		ratio = ratio - 1;
		if (ratio < 0)
		{
			ratio = 0;
			enlarge = false;
		}
	}
	if (enlarge == false)
	{
		ratio = ratio + 1;
		if (ratio >20)
		{
			ratio = 20;
			enlarge = true;
		}
	}
}
// 游戏结束处理
void  GdiplusTest::GetRandomNum(int range, int* iArray)
{
	int i;
	srand((unsigned)time(NULL));	//初始化随机数
	for (int i = 0; i < range; ++i)
	{
		iArray[i] = i;
	}
	for (i = range; i > 0; --i)
	{
		int r = rand() % i;
		swap(iArray[i - 1], iArray[r]);
	}

}
void GdiplusTest::GameEnd() {

}
// 根据GAME_STATE值显示游戏界面
void GdiplusTest::GamePaint(HDC hdc) {
	/*
	//1.GDI+简单绘制
	Graphics g(hdc);
	Image* pImg = new Image(L".\\res\\Bing.bmp");
	//Bitmap* pBmp = Bitmap::FromFile(L".\\res\\C.bmp");
	g.DrawImage(pImg, 0, 0);
	//g.DrawImage(pBmg, 0, 0);
	g.ReleaseHDC(hdc);
	delete pImg;
	*/
	/*
	//2.GDI+局部选取绘制
	Graphics g(hdc);
	Image* pImg = new Image(L".\\res\\C.bmp");
	Gdiplus::Rect rec;
	rec.X = 0;
	rec.Y = 0;
	rec.Width = wnd_width;
	rec.Height = wnd_height;
	g.DrawImage(pImg, rec, 300, 170, 450, 400, UnitPixel);
	delete pImg;
	*/
	 /*
	//3.使用GameInit()中加载的位图绘制背景
	HDC memdc = CreateCompatibleDC(hdc);
	HBITMAP oldbmp = (HBITMAP)SelectObject(memdc, hbitmap);
	BitBlt(hdc, 0, 0, wnd_width, wnd_height, memdc, 0, 0, SRCCOPY);

	Gdiplus::RectF textRect;
	textRect.X = 0;
	textRect.Y = 0;
	textRect.Width = wnd_width;
	textRect.Height = wnd_height;
	//绘制文字
	//PaintText(hdc, textRect, L"HELLO WORLD", 200, L"黑体", Color::Red);

	SelectObject(memedc,oldbmp);
	DeleteDC(memdc);
	DeleteObject(oldbmp);
	oldbom = NULL;
	*/
	/*
	//4.使用GameInit()中加载的动画图像绘制动画
	Graphics g(hdc);
	Image* pImg = new Image(L".\\res\\stage.jpg");
	Gdiplus::Rect rec;
	rec.X = 0;
	rec.Y = 0;
	rec.Width = wnd_width;
	rec.Height = wnd_height;
	g.DrawImage(pImg, rec, 0, 0, 800, 600, UnitPixel);
	g.ReleaseHDC(hdc);
	delete pImg;

	if (dir == DIR_RIGHT)
	{
		PaintRegion(hdc, x, y, frameCount*frame_width, 0, frame_width, frame_height, 1);
	}
	else
	{
		PaintRegion(hdc, x, y, frameCount*frame_width, 0, frame_width, frame_height, 1,TRANS_HFLIP_NOROT);
	}
	*/
	//屏保
	if (GetTickCount() - startTime<T_LENGTH)
	{
		HDC memdc = CreateCompatibleDC(hdc);
		HBITMAP oldbmp = (HBITMAP)SelectObject(memdc, hbitmap);
		BitBlt(hdc, 0, 0, wnd_width, wnd_height, memdc, 0, 0, SRCCOPY);
		RectF textRect;
		textRect.Width = (REAL)(2 * wnd_width - ratio*wnd_width / 10);
		textRect.Height = (REAL)(2 * wnd_height - ratio*wnd_height / 10);
		textRect.X = (REAL)(wnd_width - textRect.Width) / 2;
		textRect.Y = (REAL)(wnd_height - textRect.Height) / 2;
		int px = (int)(textRect.Width / 11);
		REAL fsize = (REAL)((px * 72) / 96);
		Color color(rand() % 255, rand() % 255, rand() % 255, rand() % 255);
		PaintText(hdc, textRect, L"HELLO WORLD", fsize, L"宋体", color);
		SelectObject(memdc, oldbmp);
		DeleteObject(oldbmp);
		oldbmp = NULL;
	}
	if (GetTickCount() - startTime >= T_LENGTH)
	{
		GetRandomNum(25, randCell);
		for (int i = 0; i < rows*cols; i++)
		{
			int dx = i%cols;
			int dy = i / cols;
			int img_clo = randCell[i] % cols;
			int img_row = randCell[i] / cols;
			PaintRegion(hdc, dx*cell_width, dy*cell_height, img_clo*cell_width, img_row*cell_height, cell_width, cell_height, 1);
			RECT mrect;
			for (int r = 0; r < rows; r++)
			{
				for (int c = 0; c < cols; c++)
				{
					mrect.left = c*cell_width;
					mrect.right = mrect.left + cell_width;
					mrect.top = r*cell_height;
					mrect.bottom = mrect.top + cell_height;
					HPEN linePen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					SelectObject(hdc, linePen);
					SelectObject(hdc, GetStockObject(NULL_BRUSH));
					Rectangle(hdc, mrect.left, mrect.top, mrect.right, mrect.bottom);
					DeleteObject(linePen);
				}
			}
			if (GetTickCount() - startTime >= 2 * T_LENGTH)   startTime = GetTickCount();
		}
	}
}
// 根据KM_ACTION值处理按键行为
void GdiplusTest::GameKeyAction(int ActionType) {

}
// 根据KM_ACTION值处理鼠标行为
void GdiplusTest::GameMouseAction(int x, int y, int ActionType) {
	if (ActionType == MOUSE_MOVE)
	{
		if (mousePT.x == 0 && mousePT.y == 0)
		{
			mousePT.x = x; mousePT.y = y;
		}
	}
	if (mousePT.x != x&&mousePT.y != y)
		PostQuitMessage(0);
}