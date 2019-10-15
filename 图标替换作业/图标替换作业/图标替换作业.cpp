// ͼ���滻��ҵ.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ͼ���滻��ҵ.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//*******************************
int startX = 200, startY = 100;
RECT rect;             
HBITMAP bufferBitmap;  //�ڴ滺��λͼ
HDC bufferDC;//�ڴ滺���豸
PAINTSTRUCT ps;
//*******************************
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;

    // ����Ϣѭ��: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CIRCLE));
    wcex.hCursor        = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CIRCLE));
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_CIRCLE));

    return RegisterClassExW(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	HDC hdc = BeginPaint(hWnd, &ps);
	HBRUSH hbrush;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �����˵�ѡ��: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_CREATE:
		hdc = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd,&rect);//�����ڴ滺���豸���ڴ滺��λͼ
		bufferDC = CreateCompatibleDC(hdc);
		bufferBitmap = CreateCompatibleBitmap(GetDC(hWnd), rect.right - rect.left, rect.bottom - rect.top);
		SelectObject(bufferDC, bufferBitmap);

		hbrush = CreateSolidBrush(RGB(255, 255, 255));
		FillRect(bufferDC,&rect,hbrush);
		DeleteObject(hbrush);

		break;
	case WM_CLOSE:
		if (IDOK == MessageBox(NULL, L"��ȷ��Ҫ�˳���",
			L"��ʾ", MB_OKCANCEL | MB_ICONQUESTION)) {
			DestroyWindow(hWnd);
		}
		break;
	case WM_LBUTTONDOWN:
		DrawIcon(bufferDC,LOWORD(lParam),HIWORD(lParam),LoadIcon(hInst,MAKEINTRESOURCE(IDI_CIRCLE)));
		RedrawWindow(hWnd,NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW);

		startX = LOWORD(lParam);
		startY = HIWORD(lParam);
		break;
	case WM_MOUSEMOVE:
		startX = LOWORD(lParam);
		startY = HIWORD(lParam);
		if (wParam == MK_LBUTTON) {
			DrawIcon(bufferDC, startX, startY, LoadIcon(hInst, MAKEINTRESOURCE(IDI_CIRCLE)));
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
		break;
	case WM_KEYDOWN:
		if (wParam == VK_LEFT)
		{
			startX = startX - 5;
		}
		if (wParam == VK_RIGHT)
		{
			startX = startX + 5;
		}
		if (wParam == VK_UP)
		{
			startY = startY - 5;
		}
		if (wParam == VK_DOWN)
		{
			startY = startY + 5;
		}
		DrawIcon(bufferDC,startX,startY,LoadIcon(hInst,MAKEINTRESOURCE(IDI_CIRCLE)));
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		break;
	case WM_PAINT:
	{
		GetClientRect(hWnd, &rect);
		// ���ڴ��豸�л��Ƶ����ݻ浽��Ļ��
		BitBlt(hdc, 0, 0,rect.right-rect.left,rect.bottom-rect.top,bufferDC,0,0,SRCCOPY);
		EndPaint(hWnd, &ps);
	}
	break;
    case WM_DESTROY:
		ReleaseDC(hWnd, bufferDC);
		DeleteObject(bufferBitmap);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
