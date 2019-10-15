#include"GdiTest.h"

int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	GdiTest* pTest = new GdiTest(h_instance, WIN_CLASS, L"T_Engine���Գ���", NULL, NULL, WIN_WIDTH, WIN_HEIGHT, RGB(0, 0, 0));
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
BOOL GdiTest::LoadBmp(LPTSTR path)//λͼ�ļ��ļ���
{

	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(0), path,
		IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	if (!hBitmap) return FALSE;  // ���λͼ����ʧ��
	DIBSECTION dib;
	// ���λͼ��Ϣ��ȡʧ��
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
	//����һ���յĵ�ͼ
	HBITMAP bitmap = CreateCompatibleBitmap(hDC, ibWidth, ibHeight);
	if (bitmap == NULL)return FALSE;
	//����һ���ڴ��豸��������λͼ
	HDC memDC = CreateCompatibleDC(hDC);
	//����һ����ɫ��ˢ
	HBRUSH hBrush = CreateSolidBrush(crColor);
	//������λͼ�滻Ϊ�ڴ��豸�е�ԭ������
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, bitmap);
	//���ƶ�����ɫ���λͼ
	RECT rcBitmap = { 0, 0, ibWidth, ibHeight };
	FillRect(memDC, &rcBitmap, hBrush);
	//��ԭ��ʹ��ԭ�������滻�ڴ��豸�е�λͼ����
	SelectObject(memDC, hOldBitmap);
	//ɾ���ڴ��豸
	DeleteDC(memDC);
	//ɾ����ˢ
	DeleteObject(hBrush);
	return bitmap;
};
HBITMAP GdiTest::CreateBlankBMP(int ibWidth, int ibHeight, COLORREF crColor)
{
	void* Bits;				// ָ�򱣴�λͼλֵ�ĵ�ַ
	BITMAPINFOHEADER Header;	// λͼͷ��Ϣ
	Header.biSize = sizeof(BITMAPINFOHEADER);
	Header.biWidth = ibWidth;
	Header.biHeight = ibHeight;
	Header.biBitCount = 24;
	Header.biPlanes = 1;
	Header.biCompression = BI_RGB;
	HBITMAP hbmp = CreateDIBSection(
		NULL, (BITMAPINFO*)&Header, DIB_RGB_COLORS, &Bits, NULL, 0);
	if (hbmp != NULL) //ʹ��ָ����ɫ���հ�λͼ
	{
		//����һ���ڴ��豸��������λͼ
		HDC memDC = CreateCompatibleDC(GetDC(NULL));
		//����һ����ɫ��ˢ����������������Ŀ�λͼ
		HBRUSH hBrush = CreateSolidBrush(crColor);
		//�������Ŀ�λͼ�滻�ڴ��豸�е�ԭ������
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hbmp);
		//��ָ������ɫ���λͼ
		RECT rcBitmap = { 0, 0, ibWidth, ibHeight };
		FillRect(memDC, &rcBitmap, hBrush);
		// �ָ�ԭ�е�λͼ����
		SelectObject(memDC, hOldBitmap);
		DeleteDC(memDC); //ɾ���ڴ��豸
		DeleteObject(hBrush); //ɾ����ˢ
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
	SelectObject(memdc, hOldBmp);//�ָ�ԭ��λͼ����
	DeleteObject(blank);
	blank = NULL;

	blank = CreateBlankBMP(hdc, w, h, RGB(255, 255, 0)); //DDB
	hOldBmp = (HBITMAP)SelectObject(memdc, blank);
	BitBlt(hdc, w, 0, w, h, memdc, 0, 0, SRCCOPY);
	SelectObject(memdc, hOldBmp);//�ָ�ԭ��λͼ����
	DeleteObject(blank);
	blank = NULL;

	blank = CreateBlankBMP(w, h, RGB(0, 255, 0)); //DIB
	hOldBmp = (HBITMAP)SelectObject(memdc, blank);
	BitBlt(hdc, 0, h, w, h, memdc, 0, 0, SRCCOPY);
	SelectObject(memdc, hOldBmp);//�ָ�ԭ��λͼ����
	DeleteObject(blank);
	blank = NULL;

	blank = CreateBlankBMP(w, h, RGB(0,0,255)); //DIB
	hOldBmp = (HBITMAP)SelectObject(memdc, blank);
	BitBlt(hdc, w, h, w, h, memdc, 0, 0, SRCCOPY);
	SelectObject(memdc, hOldBmp);//�ָ�ԭ��λͼ����
	DeleteObject(blank);
	blank = NULL;

	DeleteDC(memdc);//ɾ���ڴ��豸
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
	SelectObject(memdc,hOldBmp); //�ָ�ԭ�е�λͼ����
	DeleteObject(hBitmap);
	hBitmap = NULL;

	LoadBmp(L".\\res\\island.bmp");
	hOldBmp = (HBITMAP)SelectObject(memdc, hBitmap);
	StretchBlt(hdc, w, 0, w, h, memdc, 0, 0, bmpWidth, bmpHeight, SRCCOPY);
	SelectObject(memdc, hOldBmp); //�ָ�ԭ�е�λͼ����
	DeleteObject(hBitmap);
	hBitmap = NULL;

	LoadBmp(L".\\res\\tulip.bmp");
	hOldBmp = (HBITMAP)SelectObject(memdc, hBitmap);
	StretchBlt(hdc, 0, h, w, h, memdc, 0, 0, bmpWidth, bmpHeight, SRCCOPY);
	SelectObject(memdc, hOldBmp); //�ָ�ԭ�е�λͼ����
	DeleteObject(hBitmap);
	hBitmap = NULL;

	LoadBmp(L".\\res\\drop.bmp");
	hOldBmp = (HBITMAP)SelectObject(memdc, hBitmap);
	StretchBlt(hdc, w, h, w, h, memdc, 0, 0, bmpWidth, bmpHeight, SRCCOPY);
	SelectObject(memdc, hOldBmp); //�ָ�ԭ�е�λͼ����
	DeleteObject(hBitmap);
	hBitmap = NULL;

	DeleteDC(memdc);//ɾ���ڴ��豸
};
void GdiTest::TestAlphaBlend(HDC hdc)
{
	int w = 512;
	int h = 384;

	BLENDFUNCTION rBlendProps;
	rBlendProps.BlendOp = AC_SRC_OVER;
	rBlendProps.BlendFlags = 0;
	rBlendProps.AlphaFormat = 0;
	rBlendProps.SourceConstantAlpha =20;//͸���ȣ�0~255�𽥲�͸��

	HDC memdc = CreateCompatibleDC(hdc);

	LoadBmp(L".\\res\\leaf.bmp");
	HBITMAP hOld = (HBITMAP)SelectObject(memdc, hBitmap);
	AlphaBlend(hdc,0,0,bmpWidth,bmpHeight,
				memdc,0,0,bmpWidth,bmpHeight,rBlendProps);
	SelectObject(memdc, hOld); //�ָ�ԭ�е�λͼ����
	DeleteObject(hBitmap);
	hBitmap = NULL;

	rBlendProps.SourceConstantAlpha = 10;

	LoadBmp(L".\\res\\drop.bmp");
	hOld = (HBITMAP)SelectObject(memdc, hBitmap);
	AlphaBlend(hdc, 0, 0, w, h,
		memdc, 0, 0, bmpWidth, bmpHeight, rBlendProps);
	SelectObject(memdc, hOld); //�ָ�ԭ�е�λͼ����
	DeleteObject(hBitmap);
	hBitmap = NULL;

	LoadBmp(L".\\res\\tulip.bmp");
	hOld = (HBITMAP)SelectObject(memdc, hBitmap);
	AlphaBlend(hdc, w, 0, w, h,
		memdc, 0, 0, bmpWidth, bmpHeight, rBlendProps);
	SelectObject(memdc, hOld); //�ָ�ԭ�е�λͼ����
	DeleteObject(hBitmap);
	hBitmap = NULL;

	LoadBmp(L".\\res\\island.bmp");
	hOld = (HBITMAP)SelectObject(memdc, hBitmap);
	AlphaBlend(hdc, 0, h, w, h,
		memdc, 0, 0, bmpWidth, bmpHeight, rBlendProps);
	SelectObject(memdc, hOld); //�ָ�ԭ�е�λͼ����
	DeleteObject(hBitmap);
	hBitmap = NULL;

	LoadBmp(L".\\res\\sea.bmp");
	hOld = (HBITMAP)SelectObject(memdc, hBitmap);
	AlphaBlend(hdc, w, h, w, h,
		memdc, 0, 0, bmpWidth, bmpHeight, rBlendProps);
	SelectObject(memdc, hOld); //�ָ�ԭ�е�λͼ����
	DeleteObject(hBitmap);
	hBitmap = NULL;
};
void GdiTest::TestAlphaBlend2(HDC hdc) 
{
	BLENDFUNCTION rBlendProps;
	rBlendProps.BlendOp = AC_SRC_OVER;
	rBlendProps.BlendFlags = 0;
	rBlendProps.AlphaFormat = 0;
	rBlendProps.SourceConstantAlpha = 255; //͸���ȣ�0��ȫ͸����255��ȫ��͸��

	HDC memdc = CreateCompatibleDC(hdc);

	LoadBmp(L".\\res\\lighttower.bmp");
	HBITMAP hOld = (HBITMAP)SelectObject(memdc, hBitmap);
	AlphaBlend(hdc, 0, 0, bmpWidth, bmpHeight,
		memdc, 0, 0, bmpWidth, bmpHeight, rBlendProps);
	SelectObject(memdc, hOld); //�ָ�ԭ�е�λͼ����
	DeleteObject(hBitmap);
	hBitmap = NULL;

	rBlendProps.SourceConstantAlpha = blend;

	LoadBmp(L".\\res\\fog.bmp");
	hOld = (HBITMAP)SelectObject(memdc, hBitmap);
	AlphaBlend(hdc, 0, 0, bmpWidth, bmpHeight,
		memdc, 0, 0, bmpWidth, bmpHeight, rBlendProps);
	SelectObject(memdc, hOld); //�ָ�ԭ�е�λͼ����
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
// ��Ϸ�߼�����
void GdiTest::GameLogic() {

}
// ��Ϸ��������

void GdiTest::GameEnd() {

}
// ����GAME_STATEֵ��ʾ��Ϸ����
void GdiTest::GamePaint(HDC hdc) {
	//TestBitBlt(hdc);
	//TestStretchBlt(hdc);
	//TestAlphaBlend(hdc);
	TestAlphaBlend2(hdc);
}
// ����KM_ACTIONֵ��������Ϊ
void GdiTest::GameKeyAction(int ActionType) {

}
// ����KM_ACTIONֵ���������Ϊ
void GdiTest::GameMouseAction(int x, int y, int ActionType) {

}