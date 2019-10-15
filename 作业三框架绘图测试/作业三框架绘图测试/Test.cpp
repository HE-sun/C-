#include"Test.h"

int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	Test* pTest = new Test(h_instance,WIN_CLASS,L"T_Engine���Գ���",NULL,NULL,800,600,RGB(255,0,0));
	T_Engine::pEngine = pTest;
	pTest->SetFrame(10);
	pTest->StartEngine();
	//pTest->SetFullScreen(TRUE);
	return 0;
}
Test::Test(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle,
	WORD Icon, WORD SmIcon, int iWidth, int iHeight, COLORREF bkColor)
	:T_Engine(hInstance, szWindowClass, szTitle,
		Icon, SmIcon, iWidth, iHeight, bkColor)
{
	wnd_width = iWidth;
	wnd_height = iHeight;
}
Test::~Test(void) {

}
void Test::GameInit() {

}
// ��Ϸ�߼�����
void Test::GameLogic() {

}
// ��Ϸ��������

void Test::GameEnd() {

}
// ����GAME_STATEֵ��ʾ��Ϸ����
void Test::GamePaint(HDC hdc) {
	HGDIOBJ mbrush, oldbrush;
	HGDIOBJ mpen, oldpen;
	RECT mrect;
	int cell_width = wnd_width / 20;
	int cell_height = wnd_height / 20;
	for (int r = 0; r < 20; r++)
	{
		for (int c = 0; c < 20; c++)
		{
			mpen = CreatePen(PS_SOLID,0,RGB(240,240,240));
			oldpen = SelectObject(hdc, mpen);
			mbrush = CreateSolidBrush(RGB(rand()%255, rand()%255,rand()%255));
			oldbrush = SelectObject(hdc, mbrush);
			mrect.left = c*cell_width;
			mrect.right = mrect.left + cell_width;
			mrect.top = r*cell_height;
			mrect.bottom = mrect.top + cell_height;
			FillRect(hdc, &mrect, (HBRUSH)mbrush);
			Rectangle(hdc, mrect.left,mrect.top, mrect.right, mrect.bottom);
			SelectObject(hdc, oldbrush);
			DeleteObject(mbrush);
			SelectObject(hdc, oldpen);
			DeleteObject(mpen);
		}
	}
}
// ����KM_ACTIONֵ��������Ϊ
void Test::GameKeyAction(int ActionType) {

}
// ����KM_ACTIONֵ���������Ϊ
void Test::GameMouseAction(int x, int y, int ActionType) {

}