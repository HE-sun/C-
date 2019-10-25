#include"GdiplusTest.h"
int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	GdiplusTest* pTest = new GdiplusTest(h_instance, WIN_CLASS, L"º¯Êý²âÊÔºÍÆÁ±£", NULL, NULL, WIN_WIDTH, WIN_HEIGHT);
	T_Engine::pEngine = pTest;
	pTest->SetFrame(10);
	pTest->StartEngine();
	//pTest->SetFullScreen(TRUE);
	return 0;
}