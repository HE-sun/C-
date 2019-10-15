// practice01.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "practice01.h"
#include<commdlg.h>
#include"SimpleFile.h"
#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������
vecLinesContainer fileLines;
HWND hWndEdit;
WCHAR currentFile[260];
bool ifOpen=false;
// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_PRACTICE01, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PRACTICE01));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PRACTICE01));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PRACTICE01);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
   RECT rect;
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   GetClientRect(hWnd, &rect);
   hWndEdit = CreateWindowW(
	   L"EDIT", L"���ı��༭��������Win32 API��ʽ�µ���Ϣ����",
	   WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE |
	   ES_WANTRETURN | WS_VSCROLL | ES_AUTOVSCROLL,
	   rect.left + 5, rect.top + 5,
	   rect.right - 10, rect.bottom - 10,
	   hWnd, nullptr, hInstance, nullptr);
   HFONT hFont = CreateFont(
	   24, 0, 0, 0, 300, false, false, false,
	   DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
	   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	   DEFAULT_PITCH,
	   L"΢���ź�"
   );
   SendMessage(hWndEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
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
	OPENFILENAME ofn; // OpenFileͨ�ýṹ��
	WCHAR szFile[260] = { 0 }; // �ļ���
	int cTxtLen; // �ı�����
	WCHAR* allFileLines; // �ı�����
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
			case IDM_NEW:
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = currentFile;
				ofn.nMaxFile = sizeof(currentFile);
				ofn.lpstrFilter = _T("ALL\0*.*\0Text\0*.TXT\0");
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

				if (GetSaveFileName(&ofn) == TRUE)
				{
					SetWindowText(hWnd, ofn.lpstrFile);
					SetWindowText(hWndEdit, NULL);
					allFileLines =
						(LPWSTR)VirtualAlloc((LPVOID)NULL,
						(DWORD)(1),
							MEM_COMMIT,
							PAGE_READWRITE);
					GetWindowText(hWndEdit, allFileLines, 1);
					if (allFileLines != NULL)
					{
						SimpleFile simplefile;
						simplefile.WriteFile(ofn.lpstrFile, allFileLines);
					}

					ifOpen = true;
				}
				break;
			case IDM_OPEN:
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = currentFile;
				ofn.nMaxFile = sizeof(currentFile);
				ofn.lpstrFilter = _T("ALL\0*.*\0Text\0*.TXT\0");
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				ifOpen = true;
				if (GetOpenFileName(&ofn) == TRUE)
				{
					SetWindowText(hWnd, ofn.lpstrFile);
					SimpleFile simplefile;
					fileLines = simplefile.getFileLines(ofn.lpstrFile);

					vecLinesContainer::iterator it;
					std::wstring allLines = L"";
					for (it = fileLines.begin(); it != fileLines.end(); it++)
					{
						allLines.append(*it);
						allLines.append(L"\r\n");
					}
					SetWindowText(hWndEdit, allLines.c_str());

				}
				break;
			case IDM_SAVE:
				if (ifOpen==true) 
				{
					SetWindowText(hWnd, currentFile);
					cTxtLen = GetWindowTextLength(hWndEdit);
					allFileLines =
						(LPWSTR)VirtualAlloc((LPVOID)NULL,
						(DWORD)(cTxtLen + 1),
							MEM_COMMIT,
							PAGE_READWRITE);
					GetWindowText(hWndEdit, allFileLines, cTxtLen + 1);
					if (allFileLines != NULL)
					{
						SimpleFile simplefile;
						simplefile.WriteFile(currentFile, allFileLines);
					}
				}
				else 
				{
					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = hWnd;
					ofn.lpstrFile = szFile;
					ofn.nMaxFile = sizeof(szFile);
					ofn.lpstrFilter = _T("ALL\0*.*\0Text\0*.TXT\0");
					ofn.nFilterIndex = 1;
					ofn.lpstrFileTitle = NULL;
					ofn.nMaxFileTitle = 0;
					ofn.lpstrInitialDir = NULL;
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

					if (GetSaveFileName(&ofn) == TRUE)
					{
						cTxtLen = GetWindowTextLength(hWndEdit);
						allFileLines =
							(LPWSTR)VirtualAlloc((LPVOID)NULL,
							(DWORD)(cTxtLen + 1),
								MEM_COMMIT,
								PAGE_READWRITE);
						GetWindowText(hWndEdit, allFileLines, cTxtLen + 1);
						if (allFileLines != NULL)
						{
							SimpleFile simplefile;
							simplefile.WriteFile(ofn.lpstrFile, allFileLines);
						}
						ifOpen = true;
					}
				}
				break;
			case IDM_SAVEAS:
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = szFile;
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = _T("ALL\0*.*\0Text\0*.TXT\0");
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

				if (GetSaveFileName(&ofn) == TRUE)
				{
					cTxtLen = GetWindowTextLength(hWndEdit);
					allFileLines =
						(LPWSTR)VirtualAlloc((LPVOID)NULL,
						(DWORD)(cTxtLen + 1),
							MEM_COMMIT,
							PAGE_READWRITE);
					GetWindowText(hWndEdit, allFileLines, cTxtLen + 1);
					if (allFileLines != NULL)
					{
						SimpleFile simplefile;
						simplefile.WriteFile(ofn.lpstrFile, allFileLines);
					}

				}
				break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: �ڴ˴�����ʹ�� hdc ���κλ�ͼ����...
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_SIZE:
		RECT rect;
		GetClientRect(hWnd, &rect);
		MoveWindow(hWndEdit, rect.left + 5, rect.top + 5, rect.right-10, rect.bottom-10,1);
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// �����ڡ������Ϣ��������
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