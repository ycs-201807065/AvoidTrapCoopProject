// AvoidTrapCoopProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "AvoidTrapCoopProject.h"
#include "MyCharacter.h"

#define MAX_LOADSTRING 100

#define IDC_BTN_START 5000  //시작 버튼 ID
#define IDC_BTN_HELP 5001  //도움말 버튼 ID
#define IDC_BTN_EXIT 5002  //종료 버튼 ID

/// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

/// 헤더
MyCharacter* myCharacter = new MyCharacter();

/// 인트형 변수
int gameStarter;  //게임 시작했는지 확인용
int lookForCharacter;  //내 캐릭터가 어디보는지 (1 왼쪽, 2 오른쪽)

int g_JumpPower = 300;                  // 점프의 힘 조절
int g_Gravity = 4;                      // 점프 후 내려오는 속도(중력) 조절
int g_JumpHeight = 0;                   // 점프 높이

const int g_scafNum = 8;           // 발판 개수
const int g_obsNum = 13;            // 장애물 개수
const int g_obsThornNum = 5;       // 장애물[가시] 개수
const int g_itemBoxNum = 3;

// 발판 좌표
extern int g_scafRectLeft[g_scafNum];
extern int g_scafRectTop[g_scafNum];

// 장애물 좌표
extern int g_obsRectLeft[g_obsNum];
extern int g_obsRectTop[g_obsNum];
extern int g_obsRectRight[g_obsNum];
extern int g_obsRectBottom[g_obsNum];

// 장애물[가시] 좌표
extern int g_obsThornLeft[g_obsThornNum * 3];
extern int g_obsThornTop[g_obsThornNum * 3];

// 아이템 박스 좌표
extern int g_itemBoxLeft[g_itemBoxNum];
extern int g_itemBoxTop[g_itemBoxNum];

// 트랩 발동 조건 영역 좌표
extern int g_TrapConfirmLeft[3];
extern int g_TrapConfirmTop[3];
extern int g_TrapConfirmRight[3];
extern int g_TrapConfirmBottom[3];

int WinWidthS = 1800;       // 창의 가로 크기
int WinHeightS = 750;       // 창의 세로 크기

/// 테스트변수
INT t_time = 0;
INT m_check = 0;
INT j_check = 0;
RECT CrashBottom;

/// 렉트
RECT myClientRect;  //게임 플레이 화면 렉트(참고용)
RECT myCharacterRect;  //내 캐릭터 렉트
RECT NowmyCharacterRect;    // 현재 내 캐릭터 위치
RECT g_scaf[g_scafNum];        // 발판
RECT g_obs[g_obsNum];         // 장애물
RECT g_obsThorn[g_obsThornNum * 3];    // 장애물[가시]
RECT g_bottom;      // 바닥
RECT g_ItemBox[3];   // 아이템 박스
RECT g_TrapConfirm[3];   // 트랩 발동 조건 영역
RECT g_Finish;  // 도착지점
RECT g_cloudobj;    // 구름

/// 불
BOOL move_Left = FALSE;         // 좌측으로 이동하는지 여부
BOOL move_Right = FALSE;        // 우측으로 이동하는지 여부
BOOL jumping = FALSE;           // 점프를 했는지 여부

/// 함수
void MoveMyCharacter();
void JumpMyCharacter();
void CharacterCrash(HWND hWnd);


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

/// 캐릭터 우측으로 이동안함, 점프 시 점프 및 낙하 속도 빠름, 점프 시 바닥에 닿아도 계속 추락함

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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
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
        SetWindowPos(hWnd, NULL, 1970, 150, WinWidthS, WinHeightS, 0);  // 게임창 위치, 크기 조절
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

                // 바닥 생성
                g_bottom.left = 0;
                g_bottom.top = WinHeightS - 200;
                g_bottom.right = myClientRect.right;
                g_bottom.bottom = WinHeightS;

                // 캐릭터 초기 좌표    <<캐릭터 크기 : 48 * 48>>
                myCharacterRect.left = 20;
                myCharacterRect.top = g_bottom.top - 48;
                myCharacterRect.right = myCharacterRect.left + 48;
                myCharacterRect.bottom = g_bottom.top;

                // 도착 지점 생성
                g_Finish.left = 1720;
                g_Finish.top = 400;
                g_Finish.right = myClientRect.right;
                g_Finish.bottom = g_bottom.top;

                // 구름 생성
                g_cloudobj.left = 1580;
                g_cloudobj.top = 260;
                g_cloudobj.right = 1700;
                g_cloudobj.bottom = 310;

                // 발판 생성
                for (int i = 0; i < g_scafNum; i++) {
                    g_scaf[i].left = g_scafRectLeft[i];
                    g_scaf[i].top = g_scafRectTop[i];
                    g_scaf[i].right = g_scaf[i].left + 70;
                    g_scaf[i].bottom = g_bottom.top;
                }

                // 장애물 생성
                for (int i = 0; i < g_obsNum; i++) {
                    g_obs[i].left = g_obsRectLeft[i];
                    g_obs[i].top = g_obsRectTop[i];
                    g_obs[i].right = g_obsRectRight[i];
                    g_obs[i].bottom = g_obsRectBottom[i];
                }

                // 장애물[가시] 생성
                for (int i = 0; i < g_obsThornNum * 3; i++) {
                    g_obsThorn[i].left = g_obsThornLeft[i];
                    g_obsThorn[i].top = g_obsThornTop[i] - 1;
                    g_obsThorn[i].right = g_obsThorn[i].left + 1;
                    g_obsThorn[i].bottom = g_obsThorn[i].top + 1;
                }

                // 아이템 박스 생성
                for (int i = 0; i < g_itemBoxNum; i++) {
                    g_ItemBox[i].left = g_itemBoxLeft[i];
                    g_ItemBox[i].top = g_itemBoxTop[i];
                    g_ItemBox[i].right = g_ItemBox[i].left + 30;
                    g_ItemBox[i].bottom = g_ItemBox[i].top + 30;
                }

                // 트랩 발동 조건 영역 생성
                for (int i = 0; i < 3; i++) {
                    g_TrapConfirm[i].left = g_TrapConfirmLeft[i];
                    g_TrapConfirm[i].top = g_TrapConfirmTop[i];
                    g_TrapConfirm[i].right = g_TrapConfirmRight[i];
                    g_TrapConfirm[i].bottom = g_TrapConfirmBottom[i];
                }

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
                /*
                myCharacterRect.left += myCharacter->Move(2);
                myCharacterRect.right += myCharacter->Move(2);
                */
                lookForCharacter = 2;
                move_Right = TRUE;
                ///테스트
                m_check = 2;
                break;
            case VK_LEFT:
                /*
                myCharacterRect.left += myCharacter->Move(1);
                myCharacterRect.right += myCharacter->Move(1);
                */
                lookForCharacter = 1;
                move_Left = TRUE;
                ///테스트
                m_check = 1;
                break;
            case 0x57:
                myCharacter->Debuff(5);
                break;
            case VK_UP:
            case VK_SPACE:
                jumping = TRUE;
                ///테스트
                j_check = 1;
                break;
            default:
                break;
            }

            
            InvalidateRect(hWnd, NULL, FALSE);
        }
    }
    break;

    case WM_KEYUP:
        switch (wParam) {
        case VK_LEFT:
            move_Left = FALSE;
            ///테스트
            m_check = 3;
            break;
        case VK_RIGHT:
            move_Right = FALSE;
            ///테스트
            m_check = 4;
            break;
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

        //  테스트용 변수
        WCHAR test[128];
        //
        

        // TODO: 여기에 그리기 코드를 추가합니다.
        /// 게임시작
        if (gameStarter == 1) {
            imageDC = CreateCompatibleDC(hdc);
            Rectangle(hdc, myCharacterRect.left, myCharacterRect.top + g_JumpHeight, myCharacterRect.right, myCharacterRect.bottom + g_JumpHeight);
            wsprintfW(test, L"left : %d || top : %d || bottom : %d || check : %d", myCharacterRect.left, myCharacterRect.top + g_JumpHeight, g_JumpPower, j_check);
            TextOut(hdc, 30, 30, test, lstrlenW(test));

            // 발판, 장애물, 장애물[가시], 아이템박스 그리기
            for (int i = 0; i < g_scafNum; i++) {
                Rectangle(hdc, g_scaf[i].left, g_scaf[i].top, g_scaf[i].right, g_scaf[i].bottom);
            }

            for (int i = 0; i < g_obsNum; i++) {
                Rectangle(hdc, g_obs[i].left, g_obs[i].top, g_obs[i].right, g_obs[i].bottom);
            }

            for (int i = 1; i < (g_obsThornNum * 3); (i += 3)) {
                MoveToEx(hdc, g_obsThorn[i].left, g_obsThorn[i].top, NULL);
                LineTo(hdc, g_obsThorn[i - 1].left, g_obsThorn[i - 1].top);
                MoveToEx(hdc, g_obsThorn[i].left, g_obsThorn[i].top, NULL);
                LineTo(hdc, g_obsThorn[i + 1].left, g_obsThorn[i + 1].top);
            }

            for (int i = 0; i < g_itemBoxNum; i++) {
                Rectangle(hdc, g_ItemBox[i].left, g_ItemBox[i].top, g_ItemBox[i].right, g_ItemBox[i].bottom);
            }

/// ※테스트 후 삭제
            // 트랩 발동 조건 영역 그리기
            for (int i = 0; i < 3; i++) {
                Rectangle(hdc, g_TrapConfirm[i].left, g_TrapConfirm[i].top, g_TrapConfirm[i].right, g_TrapConfirm[i].bottom);
            }
/// ※테스트 후 삭제

            // 바닥 그리기
            Rectangle(hdc, g_bottom.left, g_bottom.top, g_bottom.right, g_bottom.bottom);

            // 도착지점 그리기
            Rectangle(hdc, g_Finish.left, g_Finish.top, g_Finish.right, g_Finish.bottom);

            // 구름 그리기
            Rectangle(hdc, g_cloudobj.left, g_cloudobj.top, g_cloudobj.right, g_cloudobj.bottom);
            

            myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITMAP_MYCHARACTER02));

            if (lookForCharacter == 1) {
                myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITMAP_MYCHARACTER02));
            }
            else if (lookForCharacter == 2) {
                myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITMAP_MYCHARACTER01));
            }

            oldBitmap = (HBITMAP)SelectObject(imageDC, myBitmap);
            BitBlt(hdc, myCharacterRect.left, myCharacterRect.top + g_JumpHeight, 48, 48, imageDC, 0, 0, SRCCOPY);  //비트맵 그려주기
            SelectObject(imageDC, oldBitmap);
            DeleteObject(myBitmap);

            //CharacterCrash(hWnd);
            MoveMyCharacter();
            JumpMyCharacter();
            InvalidateRect(hWnd, NULL, FALSE);
            
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

void MoveMyCharacter() {
    if (move_Left) {
        myCharacterRect.left -= 1;
        myCharacterRect.right -= 1;
        ///테스트
        t_time++;
    }
    if (move_Right) {
        myCharacterRect.left += 1;
        myCharacterRect.right += 1;
        ///테스트
        t_time++;
    }
}

void JumpMyCharacter() {
    if (jumping) {
        // 점프 중 바닥과 닿으면 변화 값 초기상태로 복구
        if (IntersectRect(&CrashBottom, &g_bottom, &NowmyCharacterRect)) {
            g_JumpPower = 300;
            g_JumpHeight = 0;
            myCharacterRect.bottom = CrashBottom.top;
            myCharacterRect.top = myCharacterRect.bottom - 48;
            jumping = FALSE;
            ///테스트
            j_check = 0;
            return;
        }

        // 점프 높이와 점프 힘을 감소 및 중력 증가
        g_JumpHeight -= g_JumpPower * 0.04;
        g_JumpPower -= g_Gravity * 4;

        NowmyCharacterRect.top = myCharacterRect.top + g_JumpHeight;
        NowmyCharacterRect.left = myCharacterRect.left;
        NowmyCharacterRect.bottom = myCharacterRect.bottom + g_JumpHeight;
        NowmyCharacterRect.right = myCharacterRect.right;
    }
}

void CharacterCrash(HWND hWnd) {
    RECT dst;
    for (int obsThornI = 0; obsThornI <= g_obsThornNum * 3; obsThornI++) {
        if (TRUE == IntersectRect(&dst, &myCharacterRect, &g_obsThorn[obsThornI])) {
            int i = 0;  //반복 제어용1
            int j = 0;  //반복 제어용2
            while (i <= 10) {
                myCharacterRect.top -= 5;
                myCharacterRect.bottom -= 5;
                InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
                i++;
                Sleep(15);
            }
            for (j = 0; j <= 5; j++) {
                myCharacterRect.left -= 5;
                myCharacterRect.right -= 5;
                InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
                Sleep(20);
            }
            for (j = 0; j <= 10; j++) {
                myCharacterRect.left += 5;
                myCharacterRect.right += 5;
                InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
                Sleep(20);
            }
            for (j = 0; j <= 5; j++) {
                myCharacterRect.left -= 5;
                myCharacterRect.right -= 5;
                InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
                Sleep(20);
            }
            Sleep(300);
            i = 0;
            while (i <= 60) {
                myCharacterRect.top += 5;
                myCharacterRect.bottom += 5;
                InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
                i++;
                Sleep(10);
            }
        }
    }
}