// AvoidTrapCoopProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "AvoidTrapCoopProject.h"
#include "MyCharacter.h"

#define MAX_LOADSTRING 100

#define IDC_BTN_START 5000  //시작 버튼 ID
#define IDC_BTN_HELP 5001  //도움말 버튼 ID
#define IDC_BTN_EXIT 5002  //종료 버튼 ID

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

//헤더
MyCharacter* myCharacter = new MyCharacter();
//렉트
RECT myClientRect;  //게임 플레이 화면 렉트(참고용)
RECT myCharacterRect;  //내 캐릭터 렉트
//인트형 변수
int gameStarter;  //게임 시작했는지 확인용
int lookForCharacter;  //내 캐릭터가 어디보는지 (1 왼쪽, 2 오른쪽)

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
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

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_AVOIDTRAPCOOPPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_AVOIDTRAPCOOPPROJECT));

    MSG msg;

    // 기본 메시지 루프입니다:
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
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AVOIDTRAPCOOPPROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_AVOIDTRAPCOOPPROJECT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
HWND gameStartBtn, gameHelpBtn, gameExitBtn;  //각 버튼
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        SetWindowPos(hWnd, NULL, 200, 150, 1420, 900, 0);  // 게임창 크기 조절
        GetClientRect(hWnd, &myClientRect);  // 조절된 크기 가져오기
        //내 게임에서 사용할 바텀, 탑 값 조정
        gameStartBtn = CreateWindow(L"button", L"게 임  시 작", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            (myClientRect.right / 2 - 125), 200, 250, 100, hWnd, (HMENU)IDC_BTN_START, NULL, NULL);
        gameHelpBtn = CreateWindow(L"button", L"도 움 말", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            (myClientRect.right / 2 - 125), 350, 250, 100, hWnd, (HMENU)IDC_BTN_HELP, NULL, NULL);
        gameExitBtn = CreateWindow(L"button", L"종    료", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            (myClientRect.right / 2 - 125), 500, 250, 100, hWnd, (HMENU)IDC_BTN_EXIT, NULL, NULL);
        gameStarter = 0;
    }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDC_BTN_START:
                ShowWindow(gameStartBtn, SW_HIDE);
                ShowWindow(gameHelpBtn, SW_HIDE);
                ShowWindow(gameExitBtn, SW_HIDE);

                gameStarter = 1;
                lookForCharacter = 2;

                myCharacterRect.left = 20;
                myCharacterRect.top = 500;
                myCharacterRect.right = 68;
                myCharacterRect.bottom = 548;
                InvalidateRect(hWnd, NULL, FALSE);
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDC_BTN_EXIT:
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_KEYDOWN:
    {
        if (gameStarter == 1) {
            switch (wParam)
            {
            case VK_RIGHT:
                myCharacterRect.left += myCharacter->Move(2);
                myCharacterRect.right += myCharacter->Move(2);
                lookForCharacter = 2;
                break;
            case VK_LEFT:
                myCharacterRect.left += myCharacter->Move(1);
                myCharacterRect.right += myCharacter->Move(1);
                lookForCharacter = 1;
                break;
            case 0x57:
                myCharacter->Debuff(5);
                break;
            default:
                break;
            }
            InvalidateRect(hWnd, NULL, FALSE);
        }
    }
        break;
    case WM_PAINT:
    {
        static HDC hdc, MemDC, tmpDC, imageDC;
        static HBITMAP BackBit, oldBackBit;
        HBITMAP myBitmap, oldBitmap;
        static RECT bufferRT;
        PAINTSTRUCT ps;

        hdc = BeginPaint(hWnd, &ps);
        GetClientRect(hWnd, &bufferRT);
        MemDC = CreateCompatibleDC(hdc);
        BackBit = CreateCompatibleBitmap(hdc, bufferRT.right, bufferRT.bottom);
        oldBackBit = (HBITMAP)SelectObject(MemDC, BackBit);
        PatBlt(MemDC, 0, 0, bufferRT.right, bufferRT.bottom, WHITENESS);
        tmpDC = hdc;
        hdc = MemDC;
        MemDC = tmpDC;

        // TODO: 여기에 그리기 코드를 추가합니다.
        if (gameStarter == 1) {
            imageDC = CreateCompatibleDC(hdc);
            Rectangle(hdc, myCharacterRect.left, myCharacterRect.top, myCharacterRect.right, myCharacterRect.bottom);
            myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITMAP_MYCHARACTER02));
            if (lookForCharacter == 1) {
                myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITMAP_MYCHARACTER02));
            }
            else if (lookForCharacter == 2) {
                myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITMAP_MYCHARACTER01));
            }
            oldBitmap = (HBITMAP)SelectObject(imageDC, myBitmap);
            BitBlt(hdc, myCharacterRect.left, myCharacterRect.top, 48, 48, imageDC, 0, 0, SRCCOPY);  //비트맵 그려주기
            SelectObject(imageDC, oldBitmap);
            DeleteObject(myBitmap);
        }
        // End TODO

        tmpDC = hdc;
        hdc = MemDC;
        MemDC = tmpDC;
        GetClientRect(hWnd, &bufferRT);
        BitBlt(hdc, 0, 0, bufferRT.right, bufferRT.bottom, MemDC, 0, 0, SRCCOPY);
        SelectObject(MemDC, oldBackBit);
        DeleteObject(BackBit);
        DeleteDC(MemDC);
        EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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
