﻿// AvoidTrapCoopProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "AvoidTrapCoopProject.h"
#include "MyCharacter.h"

#include<time.h>

#define MAX_LOADSTRING 100

#define IDC_BTN_START 5000  //시작 버튼 ID
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
int hungerCount;  //배고픔
int thornTrapCheck = 0;  //가시 트랩 작동했는가?
int itemBoxTimer = 0;  //타이머 동작했는가?

const int JumpPower = 275;          // 점프의 힘(상수)
const int Gravity = 4;              // 점프 후 내려오는 힘(상수)
const int JumpHeight = 0;           // 점프 높이(상수)

int g_JumpPower = JumpPower;                  // 점프의 힘 조절
int g_Gravity = Gravity;                      // 점프 후 내려오는 속도(중력) 조절
int g_JumpHeight = JumpHeight;                // 점프 높이

const int g_scafNum = 8;           // 발판 개수
const int g_obsNum = 2;            // 장애물 개수
const int g_obsBottomNum = 5;       // 장애물[바닥] 개수
const int g_obsThornNum = 5;       // 장애물[가시] 개수
const int g_itemBoxNum = 3;         // 아이템 박스 개수
const int g_cloudobjNum = 4;        // 구름 개수

// 발판 좌표 [Project_Coordinate.ccp]
extern int g_scafRectLeft[g_scafNum];   // 발판 렉트 레프트
extern int g_scafRectTop[g_scafNum];    // 발판 렉트 탑

// 장애물 좌표 [Project_Coordinate.ccp]
extern int g_obsRectLeft[g_obsNum];     // 장애물 렉트 레프트
extern int g_obsRectTop[g_obsNum];      // 장애물 렉트 탑
extern int g_obsRectRight[g_obsNum];    // 장애물 렉트 라이트
extern int g_obsRectBottom[g_obsNum];   // 장애물 렉트 바텀

// 장애물[바닥] 좌표 [Project_Coordinate.ccp]
extern int g_obsBottomRectLeft[g_obsBottomNum];         // 장애물 바닥 렉트 레프트
extern int g_obsBottomRectTop[g_obsBottomNum];          // 장애물 바닥 렉트 탑
extern int g_obsBottomRectRight[g_obsBottomNum];        // 장애물 바닥 렉트 라이트
extern int g_obsBottomRectBottom[g_obsBottomNum];       // 장애물 바닥 렉트 바텀

// 장애물[가시] 좌표 [Project_Coordinate.ccp]
extern int g_obsThornLeft[g_obsThornNum * 3];   // 가시 렉트 레프트
extern int g_obsThornTop[g_obsThornNum * 3];    // 가시 렉트 탑

// 아이템 박스 좌표 [Project_Coordinate.ccp]
extern int g_itemBoxLeft[g_itemBoxNum];         // 아이템박스 렉트 레프트
extern int g_itemBoxTop[g_itemBoxNum];          // 아이템박스 렉트 탑

// 트랩 발동 조건 영역 좌표 [Project_Coordinate.ccp]
extern int g_TrapConfirmLeft[3];                // 트랩 렉트 레프트
extern int g_TrapConfirmTop[3];                 // 트랩 렉트 탑
extern int g_TrapConfirmRight[3];               // 트랩 렉트 라이트
extern int g_TrapConfirmBottom[3];              // 트랩 렉트 바텀

// 구름 좌표 [Project_Coordinate.ccp]
extern int g_cloudobjLeft[g_cloudobjNum];       // 구름 렉트 레프트
extern int g_cloudobjTop[g_cloudobjNum];        // 구름 렉트 탑
extern int g_cloudobjRight[g_cloudobjNum];      // 구름 렉트 라이트
extern int g_cloudobjBottom[g_cloudobjNum];     // 구름 렉트 바텀

int WinWidthS = 1800;       // 창의 가로 크기
int WinHeightS = 750;       // 창의 세로 크기

/// 문자
WCHAR HungryText[128];                          // 배고픔 수치 표시

/// 렉트
RECT myClientRect;                              // 게임 플레이 화면 렉트(참고용)
RECT myCharacterRect;                           // 내 캐릭터 렉트
RECT NowmyCharacterRect;                        // 현재 내 캐릭터 위치
RECT g_scaf[g_scafNum];                         // 발판
RECT g_scafDrop[g_scafNum * 2];                 // 발판 낙하 판정
RECT g_obs[g_obsNum];                           // 장애물
RECT g_obsCrash[g_obsNum];                      // 장애물 판정
RECT g_obsDrop[g_obsNum * 2];                   // 장애물 낙하 판정
RECT g_obsBottomRect[g_obsBottomNum];           // 장애물[바닥]
RECT g_obsBottomDrop[g_obsBottomNum];           // 장애물[바닥] 낙하 영역
RECT g_obsThorn[g_obsThornNum * 3];             // 장애물[가시]
RECT g_bottom;                                  // 바닥
RECT g_ItemBox[g_itemBoxNum];                   // 아이템 박스
RECT g_ItemCrash[g_itemBoxNum];                 // 아이템 박스 판정
RECT g_ItemDrop[(g_itemBoxNum - 1) * 2];        // 아이템 박스 낙하 판정
RECT g_TrapConfirm[2];                          // 트랩 발동 조건 영역6
RECT g_Finish;                                  // 도착지점
RECT g_cloudobj[g_cloudobjNum];                 // 구름(0번째는 오브젝트, 나머지는 함정)

/// 불
BOOL Finish = FALSE;                            // 플레이어가 도착지점에 도착했는지 여부
BOOL Finish_op = FALSE;                         // 도착 트리거 한번만 발동하게 설정
BOOL move_Left = FALSE;                         // 좌측으로 이동하는지 여부
BOOL move_Right = FALSE;                        // 우측으로 이동하는지 여부
BOOL jumping = FALSE;                           // 점프를 했는지 여부
BOOL pos_jump = TRUE;                           // 점프 가능 여부
BOOL drop = FALSE;                              // 낙하중인지 여부
BOOL bottom = TRUE;                             // 플레이어의 위치가 바닥인지 여부
BOOL scaf = FALSE;                              // 플레이어의 위치가 발판인지 여부
BOOL obs = FALSE;                               // 플레이어의 위치가 장애물인지 여부
BOOL item = FALSE;                              // 플레이어의 위치가 아이템인지 여부
BOOL live = TRUE;                               // 플레이어의 생존 여부
BOOL ActItem = FALSE;                           // 1번 아이템 활성화 여부
BOOL Move_item = FALSE;;                        // 1번 아이템 이동 여부
BOOL ActItem2 = FALSE;                          // 2번 아이템 활성화 여부
BOOL pos_op = TRUE;                             // 조작 가능 여부
BOOL Active_Trap[2];                            // 트랩 작동 여부

/// 함수
void MoveMyCharacter();                         // 캐릭터 이동 처리
void JumpMyCharacter();                         // 캐릭터 점프 처리
void CharacterStatus(HWND statusHWND);          // 캐릭터 상태(현재위치, 장애물, 발판)에 대한 처리
void CharacterDrop();                           // 캐릭터 낙하 처리
void ItemBoxSet(HWND itemHWND);                 // 아이템 박스 처리


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
    if (!InitInstance(hInstance, nCmdShow))
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

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AVOIDTRAPCOOPPROJECT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_AVOIDTRAPCOOPPROJECT);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
        SetWindowPos(hWnd, NULL, 50, 150, WinWidthS, WinHeightS, 0);  // 게임창 위치, 크기 조절
        GetClientRect(hWnd, &myClientRect);  // 조절된 크기 가져오기
        //내 게임에서 사용할 바텀, 탑 값 조정
        gameStartBtn = CreateWindow(L"button", L"게 임  시 작", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            (myClientRect.right / 2 - 125), 200, 250, 100, hWnd, (HMENU)IDC_BTN_START, NULL, NULL);
        gameExitBtn = CreateWindow(L"button", L"종    료", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            (myClientRect.right / 2 - 125), 400, 250, 100, hWnd, (HMENU)IDC_BTN_EXIT, NULL, NULL);
        gameStarter = 0;
        //testHWND = hWnd;
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
            hungerCount = 4500;
            thornTrapCheck = 0;
            itemBoxTimer = 0;

            // 바닥 생성
            g_bottom.left = 0;
            g_bottom.top = WinHeightS - 200;
            g_bottom.right = myClientRect.right;
            g_bottom.bottom = WinHeightS;

            // 캐릭터 초기 좌표    <<캐릭터 크기 : 48 * 48>>
            myCharacterRect.left = 20;
            myCharacterRect.top = g_bottom.top - 48;
            myCharacterRect.right = myCharacterRect.left + 48;
            myCharacterRect.bottom = myCharacterRect.top + 48;;

            // 도착 지점 생성
            g_Finish.left = 1720;
            g_Finish.top = 400;
            g_Finish.right = myClientRect.right;
            g_Finish.bottom = g_bottom.top;

            // 발판 생성
            for (int i = 0; i < g_scafNum; i++) {
                g_scaf[i].left = g_scafRectLeft[i];
                g_scaf[i].top = g_scafRectTop[i];
                g_scaf[i].right = g_scaf[i].left + 70;
                g_scaf[i].bottom = g_bottom.top;
            }

            // 발판 낙하 판정 생성
            for (int i = 0; i < g_scafNum * 2; (i += 2)) {
                // left
                g_scafDrop[i].right = g_scaf[i / 2].left - 48;
                g_scafDrop[i].bottom = g_scaf[i / 2].top - 40;
                g_scafDrop[i].left = g_scafDrop[i].right - 1;
                g_scafDrop[i].top = g_scafDrop[i].bottom - 1;

                // right
                g_scafDrop[i + 1].left = g_scaf[i / 2].right + 48;
                g_scafDrop[i + 1].right = g_scafDrop[i + 1].left + 1;
                g_scafDrop[i + 1].bottom = g_scaf[i / 2].top - 40;
                g_scafDrop[i + 1].top = g_scafDrop[i + 1].bottom - 1;
            }

            // 장애물 생성
            for (int i = 0; i < g_obsNum; i++) {
                g_obs[i].left = g_obsRectLeft[i];
                g_obs[i].top = g_obsRectTop[i];
                g_obs[i].right = g_obsRectRight[i];
                g_obs[i].bottom = g_obsRectBottom[i];
            }

            // 장애물 판정 생성
            for (int i = 0; i < g_obsNum; i++) {
                // bottom
                g_obsCrash[i].left = g_obs[i].left + 1;
                g_obsCrash[i].top = g_obs[i].bottom;
                g_obsCrash[i].right = g_obs[i].right - 1;
                g_obsCrash[i].bottom = g_obsCrash[i].top + 2;
            }

            // 장애물 낙하 판정 생성
            for (int i = 0; i < g_obsNum * 2; (i += 2)) {
                // left
                g_obsDrop[i].right = g_obs[i / 2].left - 47;
                g_obsDrop[i].bottom = g_obs[i / 2].top - 40;
                g_obsDrop[i].left = g_obsDrop[i].right - 1;
                g_obsDrop[i].top = g_obsDrop[i].bottom - 1;

                // right
                g_obsDrop[i + 1].left = g_obs[i / 2].right + 47;
                g_obsDrop[i + 1].right = g_obsDrop[i + 1].left + 1;
                g_obsDrop[i + 1].bottom = g_obs[i / 2].top - 40;
                g_obsDrop[i + 1].top = g_obsDrop[i + 1].bottom - 1;
            }

            // 장애물[바닥] 생성
            for (int i = 0; i < g_obsBottomNum; i++) {
                g_obsBottomRect[i].left = g_obsBottomRectLeft[i];
                g_obsBottomRect[i].top = g_obsBottomRectTop[i] - 3;
                g_obsBottomRect[i].right = g_obsBottomRectRight[i];
                g_obsBottomRect[i].bottom = g_obsBottomRectBottom[i] - 3;
            }

            // 장애물[바닥] 낙하 영역 생성
            for (int i = 0; i < g_obsBottomNum; i++) {
                // top
                g_obsBottomDrop[i].left = g_obsBottomRectLeft[i] + 2;
                g_obsBottomDrop[i].right = g_obsBottomRectRight[i] - 2;
                g_obsBottomDrop[i].bottom = g_obsBottomRectTop[i];
                g_obsBottomDrop[i].top = g_obsBottomDrop[i].bottom - 3;
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

            // 아이템 박스 판정 생성
            for (int i = 0; i < (g_itemBoxNum - 1); i++) {
                // bottom
                g_ItemCrash[i].left = g_ItemBox[i].left + 1;
                g_ItemCrash[i].top = g_ItemBox[i].bottom;
                g_ItemCrash[i].right = g_ItemBox[i].right - 1;
                g_ItemCrash[i].bottom = g_ItemCrash[i].top + 2;
            }

            // 아이템 낙하 판정 생성
            for (int i = 0; i < ((g_itemBoxNum - 1) * 2); (i += 2)) {
                // left
                g_ItemDrop[i].right = g_ItemBox[i / 2].left - 47;
                g_ItemDrop[i].bottom = g_ItemBox[i / 2].top - 40;
                g_ItemDrop[i].left = g_ItemDrop[i].right - 1;
                g_ItemDrop[i].top = g_ItemDrop[i].bottom - 1;

                // right
                g_ItemDrop[i + 1].left = g_ItemBox[i / 2].right + 47;
                g_ItemDrop[i + 1].right = g_ItemDrop[i + 1].left + 1;
                g_ItemDrop[i + 1].bottom = g_ItemBox[i / 2].top - 40;
                g_ItemDrop[i + 1].top = g_ItemDrop[i + 1].bottom - 1;
            }

            // 트랩 발동 조건 영역 생성
            for (int i = 0; i < 2; i++) {
                g_TrapConfirm[i].left = g_TrapConfirmLeft[i];
                g_TrapConfirm[i].top = g_TrapConfirmTop[i];
                g_TrapConfirm[i].right = g_TrapConfirmRight[i];
                g_TrapConfirm[i].bottom = g_TrapConfirmBottom[i];
            }

            // 구름 생성
            for (int i = 0; i < g_cloudobjNum; i++) {
                g_cloudobj[i].left = g_cloudobjLeft[i];
                g_cloudobj[i].top = g_cloudobjTop[i];
                g_cloudobj[i].right = g_cloudobjRight[i];
                g_cloudobj[i].bottom = g_cloudobjBottom[i];
            }

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
        if (gameStarter == 1 && TRUE == live && TRUE == pos_op) {
            switch (wParam) {
            case VK_RIGHT:
                lookForCharacter = 2;
                move_Right = TRUE;
                break;
            case VK_LEFT:
                lookForCharacter = 1;
                move_Left = TRUE;
                break;
                /*
            case 0x57:
                myCharacterRect.left = 1400;
                myCharacterRect.top = 300;
                myCharacterRect.right = 1448;
                myCharacterRect.bottom = 348;
                break;
                */
            case VK_UP:
            case VK_SPACE:
                if (TRUE == pos_jump) {
                    jumping = TRUE;
                    bottom = FALSE;
                }
                break;
            }
        }
    }
    break;

    case WM_KEYUP:
        if (gameStarter == 1 && TRUE == live && TRUE == pos_op) {
            switch (wParam) {
            case VK_LEFT:
                move_Left = FALSE;
                break;
            case VK_RIGHT:
                move_Right = FALSE;
                break;
            }
        }
        break;
    case WM_PAINT:
    {
        static HDC hdc, MemDC, tmpDC;
        HDC imageDC;
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

            // 움직임 속도 조절
            Sleep(10);

            // 발판, 장애물[가시], 장애물, 아이템박스 그리기
            for (int i = 0; i < g_scafNum; i++) {
                Rectangle(hdc, g_scaf[i].left, g_scaf[i].top, g_scaf[i].right, g_scaf[i].bottom);
            }            

            //가시 보이기
            if (thornTrapCheck == 0) {
                //큰 가시 보이기
                myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITMAP_OBSTHORN01));
                oldBitmap = (HBITMAP)SelectObject(imageDC, myBitmap);
                BitBlt(hdc, g_scaf[0].right, (g_bottom.top - 30), 96, 29, imageDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                SelectObject(imageDC, oldBitmap);
                DeleteObject(myBitmap);
            }
            //큰 가시 함정 발동
            else if (thornTrapCheck == 1) {
                myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITMAP_OBSTHORN01));
                oldBitmap = (HBITMAP)SelectObject(imageDC, myBitmap);
                BitBlt(hdc, 265, g_obsThorn[0].top, 96, 29, imageDC, 0, 0, SRCCOPY);  //첫번째 가시함정 위치
                SelectObject(imageDC, oldBitmap);
                DeleteObject(myBitmap);
            }
            //작은 가시 보이기
            Rectangle(hdc, 1412, 449, 1416, 450);

            for (int i = 1; i < g_itemBoxNum; i++) {
                Rectangle(hdc, g_ItemBox[i].left, g_ItemBox[i].top, g_ItemBox[i].right, g_ItemBox[i].bottom);
            }


            //중앙 박스 2개 그리기
            for (int i = 0; i < g_obsNum; i++) {
                Rectangle(hdc, g_obs[i].left, g_obs[i].top, g_obs[i].right, g_obs[i].bottom);
            }

            //짧은 아이템박스 무적타임
            if (itemBoxTimer >= 1) {
                if (itemBoxTimer >= 15) {
                    ActItem = TRUE;
                }
                else {
                    itemBoxTimer++;
                }
            }

            if (TRUE == ActItem) {
                Rectangle(hdc, g_ItemBox[0].left, g_ItemBox[0].top, g_ItemBox[0].right, g_ItemBox[0].bottom);
            }

            // 바닥 그리기
            myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITMAP_BOTTOM01));
            for (int g_i = 0; g_i <= 18; g_i++) {
                oldBitmap = (HBITMAP)SelectObject(imageDC, myBitmap);
                BitBlt(hdc, (g_i * 100), g_bottom.top, 100, 100, imageDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                SelectObject(imageDC, oldBitmap);
            }
            myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITMAP_BOTTOM02));
            for (int g_i = 0; g_i <= 18; g_i++) {
                oldBitmap = (HBITMAP)SelectObject(imageDC, myBitmap);
                BitBlt(hdc, (g_i * 100), g_bottom.top + 100, 100, 100, imageDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                SelectObject(imageDC, oldBitmap);
            }
            DeleteObject(myBitmap);

            // 도착지점 그리기
            myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITMAP_WINGATE));
            oldBitmap = (HBITMAP)SelectObject(imageDC, myBitmap);
            BitBlt(hdc, g_Finish.left, g_Finish.top, 64, 150, imageDC, 0, 0, SRCCOPY);  //비트맵 그려주기
            SelectObject(imageDC, oldBitmap);
            DeleteObject(myBitmap);


            // 구름 그리기
            for (int i = 0; i < g_cloudobjNum; i++) {
                if (i == 0) {
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITMAP_CLOUD01));
                }
                else if (i == 1) {
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITMAP_HITCLOUD01));
                }
                else if (i == 2) {
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITMAP_HITCLOUD02));
                }
                else if (i == 3) {
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITMAP_HITCLOUD03));
                }
                oldBitmap = (HBITMAP)SelectObject(imageDC, myBitmap);
                BitBlt(hdc, g_cloudobj[i].left, g_cloudobj[i].top, 120, 50, imageDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                SelectObject(imageDC, oldBitmap);
                DeleteObject(myBitmap);
            }

            // 함수 호출
            MoveMyCharacter();
            JumpMyCharacter();
            CharacterStatus(hWnd);
            CharacterDrop();
            ItemBoxSet(hWnd);

            //내 캐릭 살았을때랑 죽었을때
            if (live) {
                // 플레이어가 도착지점 도착 시 배고픔 수치 멈춤
                if (Finish_op == FALSE) {
                    hungerCount--;  //살아있으면 배고파야지
                }
                if (lookForCharacter == 1) {
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITMAP_MYCHARACTER02));
                }
                else if (lookForCharacter == 2) {
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITMAP_MYCHARACTER01));
                }
            }
            else {
                myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITMAP_MYCHARACTERHIT01));
            }

            //11시 방향 UI
            wsprintfW(HungryText, L"배고픔 : %d", hungerCount);
            TextOut(hdc, 20, 20, HungryText, lstrlenW(HungryText));
            oldBitmap = (HBITMAP)SelectObject(imageDC, myBitmap);
            BitBlt(hdc, myCharacterRect.left, myCharacterRect.top + g_JumpHeight, 48, 48, imageDC, 0, 0, SRCCOPY);  //비트맵 그려주기
            SelectObject(imageDC, oldBitmap);
            DeleteObject(myBitmap);
            ReleaseDC(hWnd, imageDC);

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


// 함수 정의 시작

void MoveMyCharacter() {
    if (move_Left && live && pos_op) {
        if (myCharacterRect.left >= 0) {
            myCharacterRect.left -= 4;
            myCharacterRect.right -= 4;
        }
    }
    if (move_Right && live) {
        myCharacterRect.left += 4;
        myCharacterRect.right += 4;
    }
}

void JumpMyCharacter() {
    RECT CrashBottom;

    // 점프중이라면
    if (jumping && live && pos_op && drop == FALSE) {
        scaf = FALSE;
        obs = FALSE;
        item = FALSE;

        for (int i = 0; i < g_scafNum; i++) {
            // 점프 중 발판과 닿으면 변화 값 초기상태로 복구
            if (IntersectRect(&CrashBottom, &g_scaf[i], &NowmyCharacterRect)) {
                if (myCharacterRect.right > g_scaf[i].left && myCharacterRect.left < g_scaf[i].right && CrashBottom.top > (CrashBottom.bottom - 30)) {
                    scaf = TRUE;
                    g_JumpPower = JumpPower;
                    g_JumpHeight = JumpHeight;
                    myCharacterRect.bottom = CrashBottom.top;
                    myCharacterRect.top = myCharacterRect.bottom - 48;
                    jumping = FALSE;
                    drop = FALSE;
                    return;
                }
            }
        }

        for (int i = 0; i < g_obsNum; i++) {
            // 점프 중 장애물과 닿으면 변화 값 초기상태로 복구
            if (IntersectRect(&CrashBottom, &g_obs[i], &NowmyCharacterRect)) {
                if (myCharacterRect.right > g_obs[i].left && myCharacterRect.left < g_obs[i].right && CrashBottom.top >(CrashBottom.bottom - 30)) {
                    obs = TRUE;
                    g_JumpPower = JumpPower;
                    g_JumpHeight = JumpHeight;
                    myCharacterRect.bottom = CrashBottom.top;
                    myCharacterRect.top = myCharacterRect.bottom - 48;
                    jumping = FALSE;
                    drop = FALSE;
                    return;
                }
            }
        }

        for (int i = 0; i < (g_itemBoxNum - 1); i++) {
            // 점프 중 아이템과 닿으면 변화 값 초기상태로 복구
            if (IntersectRect(&CrashBottom, &g_ItemBox[i], &NowmyCharacterRect)) {
                if (myCharacterRect.right > g_ItemBox[i].left && myCharacterRect.left < g_ItemBox[i].right && CrashBottom.top >(CrashBottom.bottom - 30)) {
                    item = TRUE;
                    g_JumpPower = JumpPower;
                    g_JumpHeight = JumpHeight;
                    myCharacterRect.bottom = CrashBottom.top;
                    myCharacterRect.top = myCharacterRect.bottom - 48;
                    jumping = FALSE;
                    drop = FALSE;
                    return;
                }
            }
        }

        for (int i = 0; i < g_obsBottomNum; i++) {
            // 점프 중 진흙탕과 닿으면 변화 값 초기상태로 복구
            if (IntersectRect(&CrashBottom, &g_obsBottomDrop[i], &NowmyCharacterRect)) {
                g_JumpPower = JumpPower;
                g_JumpHeight = JumpHeight;
                myCharacterRect.bottom = CrashBottom.bottom;
                myCharacterRect.top = myCharacterRect.bottom - 48;
                jumping = FALSE;
                drop = FALSE;
                return;
            }
        }

        // 점프 중 바닥과 닿으면 변화 값 초기상태로 복구
        if (IntersectRect(&CrashBottom, &g_bottom, &NowmyCharacterRect)) {
            g_JumpPower = JumpPower;
            g_JumpHeight = JumpHeight;
            myCharacterRect.bottom = CrashBottom.top;
            myCharacterRect.top = myCharacterRect.bottom - 48;
            jumping = FALSE;
            drop = FALSE;
            bottom = TRUE;
            return;
        }

        // 점프 높이와 점프 힘을 감소 및 중력 증가
        g_JumpHeight -= g_JumpPower * 0.04;
        g_JumpPower -= g_Gravity * 4;
    }
}

void CharacterStatus(HWND statusHWND) {
    RECT dst;               // 임시용

    // 현재 캐릭터의 위치 저장
    NowmyCharacterRect.top = myCharacterRect.top + g_JumpHeight;
    NowmyCharacterRect.left = myCharacterRect.left;
    NowmyCharacterRect.bottom = myCharacterRect.bottom + g_JumpHeight;
    NowmyCharacterRect.right = myCharacterRect.right;

    // 플레이어가 도착지점과 닿았을 때 처리
    if (IntersectRect(&dst, &NowmyCharacterRect, &g_Finish) && Finish_op == FALSE) {
        Finish = TRUE;
    }

    if (TRUE == Finish) {
        MessageBox(statusHWND, L"승리", L"도착", NULL);
        Finish = FALSE;
        pos_op = FALSE;
        Finish_op = TRUE;
        DestroyWindow(statusHWND);
    }

    // 구름 함정과 닿았을 때 처리
    for (int i = 1; i < g_cloudobjNum; i++) {
        if (TRUE == IntersectRect(&dst, &NowmyCharacterRect, &g_cloudobj[i]) && TRUE == live) {
            live = FALSE;
        }
    }

    // 장애물[가시]와 닿았을 때 처리
    for (int i = 0; i < (g_obsThornNum * 3); i++) {
        if (IntersectRect(&dst, &NowmyCharacterRect, &g_obsThorn[i]) && TRUE == live) {
            live = FALSE;
        }
    }

    // 발판에 부딫히는 경우
    for (int i = 0; i < g_scafNum; i++) {
        if (IntersectRect(&dst, &NowmyCharacterRect, &g_scaf[i])) {
            // 좌측에 부딫히는 경우
            if (NowmyCharacterRect.right > g_scaf[i].left && NowmyCharacterRect.right < g_scaf[i].right && (dst.left + 10) > dst.right) {
                myCharacterRect.right = g_scaf[i].left - 2;
                myCharacterRect.left = myCharacterRect.right - 48;
                return;
            }

            // 우측에 부딫히는 경우
            if (NowmyCharacterRect.left < g_scaf[i].right && NowmyCharacterRect.left > g_scaf[i].left && (dst.left + 10) > dst.right) {
                myCharacterRect.left = g_scaf[i].right + 2;
                myCharacterRect.right = myCharacterRect.left + 48;
                return;
            }
        }
    }

    // 측면에서 떨어질 때 낙하 처리
    for (int i = 0; i < g_scafNum * 2; i++) {
        if (IntersectRect(&dst, &NowmyCharacterRect, &g_scafDrop[i]) && item == FALSE && obs == FALSE && jumping == FALSE) {
            drop = TRUE;
        }
    }

    // 장애물에 부딫히는 경우
    for (int i = 0; i < g_obsNum; i++) {
        if (IntersectRect(&dst, &NowmyCharacterRect, &g_obs[i])) {
            // 좌측에 부딫히는 경우
            if (NowmyCharacterRect.right > g_obs[i].left && NowmyCharacterRect.right < g_obs[i].right && (dst.left + 10) > dst.right) {
                myCharacterRect.right = g_obs[i].left - 1;
                myCharacterRect.left = myCharacterRect.right - 48;
                return;
            }

            // 우측에 부딫히는 경우
            if (NowmyCharacterRect.left < g_obs[i].right && NowmyCharacterRect.left > g_obs[i].left && (dst.left + 10) > dst.right) {
                myCharacterRect.left = g_obs[i].right + 1;
                myCharacterRect.right = myCharacterRect.left + 48;
                return;
            }
        }
    }

    // 측면에서 떨어질 때 낙하 처리
    for (int i = 0; i < g_obsNum * 2; i++) {
        if (IntersectRect(&dst, &NowmyCharacterRect, &g_obsDrop[i]) && scaf == FALSE && item == FALSE && jumping == FALSE) {
            drop = TRUE;
        }
    }

    // 아래쪽과 닿았을 때 낙하 처리
    for (int i = 0; i < g_obsNum; i++) {
            if (IntersectRect(&dst, &NowmyCharacterRect, &g_obsCrash[i])) {
                drop = TRUE;
                jumping = FALSE;
                return;
            }
    }

    // 아이템에 부딫히는 경우
    for (int i = 0; i < (g_itemBoxNum - 1); i++) {
        if (IntersectRect(&dst, &NowmyCharacterRect, &g_ItemBox[i])) {
            // 좌측에 부딫히는 경우
            if (NowmyCharacterRect.right > g_ItemBox[i].left && NowmyCharacterRect.right < g_ItemBox[i].right && (dst.left + 10) > dst.right) {
                myCharacterRect.right = g_ItemBox[i].left - 1;
                myCharacterRect.left = myCharacterRect.right - 48;
                return;
            }

            // 우측에 부딫히는 경우
            if (NowmyCharacterRect.left < g_ItemBox[i].right && NowmyCharacterRect.left > g_ItemBox[i].left && (dst.left + 10) > dst.right) {
                myCharacterRect.left = g_ItemBox[i].right + 1;
                myCharacterRect.right = myCharacterRect.left + 48;
                return;
            }
        }
    }

    // 측면에서 떨어질 때 낙하 처리
    for (int i = 0; i < ((g_itemBoxNum - 1) * 2); i++) {
        if (IntersectRect(&dst, &NowmyCharacterRect, &g_ItemDrop[i]) && scaf == FALSE && obs == FALSE && jumping == FALSE) {
            drop = TRUE;
        }
    }

    // 아래쪽과 닿았을 때 낙하 처리
    for (int i = 0; i < g_itemBoxNum; i++) {
        if (IntersectRect(&dst, &NowmyCharacterRect, &g_ItemCrash[i])) {
            drop = TRUE;
            jumping = FALSE;
            return;
        }
    }

    //바닥에 닿고 점프 안하면 바로 사망하게 하기
    //진흙탕같은 컨셉(점점 빠지고 안죽기위해선 계속 점프하며 탈출해야함)
    for (int i = 0; i < g_obsBottomNum; i++) {

        // 장애물[바닥]과 닿았을 때
        if (IntersectRect(&dst, &NowmyCharacterRect, &g_obsBottomRect[i]) == TRUE && TRUE == live) {

            //내려가라 내려가라
            myCharacterRect.bottom += 2;
            myCharacterRect.top = myCharacterRect.bottom - 48;
            
        }
    }

    // 플레이어, 바닥 떨어짐 처리(내 캐릭 머리가 땅바닥 머리보다 아래있으면 죽게하기
    if (myCharacterRect.top >= g_bottom.top) {
        live = FALSE;
        return;
    }

    // 트랩 발동 영역과 닿았을 때 분기 저장
    for (int i = 0; i < 2; i++) {
        if(IntersectRect(&dst, &NowmyCharacterRect, &g_TrapConfirm[i])){

            if (Active_Trap[i] == TRUE) {
                break;
            }

            if (i == 0) {
                Active_Trap[0] = TRUE;
                thornTrapCheck = 1;  //처음 가시 트랩 발동함
                return;
            }

            if (i == 1) {
                Active_Trap[1] = TRUE;
                return;
            }
        }
    }

    // 가시 함정 위로 올라감
    if (TRUE == Active_Trap[0]) {
        for (int i = 0; i < 12; i++) {
            g_obsThorn[i].top -= 15;
            g_obsThorn[i].bottom -= 15;

            if (g_obsThorn[i].bottom < -300) {
                Active_Trap[0] = FALSE;
                break;
            }
        }
    }

    // 아이템 박스 위로 올라감
    if (TRUE == Active_Trap[1]) {
        g_ItemBox[2].top -= 10;
        g_ItemBox[2].bottom -= 10;

        if (g_ItemBox[2].bottom < -300) {
            Active_Trap[1] = FALSE;
        }
    }

    //배고파서 사망
    if (hungerCount <= 0) {
        live = FALSE;
    }
}

void CharacterDrop() {
    RECT CrashBottom;

    if (drop == TRUE && TRUE == live && FALSE == bottom && TRUE == pos_op) {
        if (jumping == TRUE) {
            drop = FALSE;
        }
        else {
            pos_jump = FALSE;
            scaf = FALSE;
            obs = FALSE;
            item = FALSE;
            // 낙하 상태로 전환
            g_JumpPower = 2;
            g_Gravity = 4;
            g_JumpHeight += g_JumpPower * 4;
            g_JumpPower += g_Gravity * 4;

            for (int i = 0; i < g_scafNum; i++) {
                // 낙하 중 발판과 닿으면 변화 값 초기상태로 복구
                if (IntersectRect(&CrashBottom, &g_scaf[i], &NowmyCharacterRect)) {
                    if (myCharacterRect.right > g_scaf[i].left && myCharacterRect.left < g_scaf[i].right && CrashBottom.top >(CrashBottom.bottom - 30)) {
                        scaf = TRUE;
                        g_JumpPower = JumpPower;
                        g_JumpHeight = JumpHeight;
                        myCharacterRect.bottom = CrashBottom.top;
                        myCharacterRect.top = myCharacterRect.bottom - 48;
                        jumping = FALSE;
                        drop = FALSE;
                        pos_jump = TRUE;
                    }
                    return;
                }
            }

            for (int i = 0; i < g_obsNum; i++) {
                // 낙하 중 장애물과 닿으면 변화 값 초기상태로 복구
                if (IntersectRect(&CrashBottom, &g_obs[i], &NowmyCharacterRect)) {
                    if (myCharacterRect.right > g_obs[i].left && myCharacterRect.left < g_obs[i].right && CrashBottom.top >(CrashBottom.bottom - 30)) {
                        obs = TRUE;
                        g_JumpPower = JumpPower;
                        g_JumpHeight = JumpHeight;
                        myCharacterRect.bottom = CrashBottom.top;
                        myCharacterRect.top = myCharacterRect.bottom - 48;
                        jumping = FALSE;
                        drop = FALSE;
                        pos_jump = TRUE;
                        return;
                    }
                }
            }

            for (int i = 0; i < (g_itemBoxNum - 1); i++) {
                // 낙하 중 아이템과 닿으면 변화 값 초기상태로 복구
                if (IntersectRect(&CrashBottom, &g_ItemBox[i], &NowmyCharacterRect)) {
                    if (myCharacterRect.right > g_ItemBox[i].left && myCharacterRect.left < g_ItemBox[i].right && CrashBottom.top >(CrashBottom.bottom - 30)) {
                        item = TRUE;
                        g_JumpPower = JumpPower;
                        g_JumpHeight = JumpHeight;
                        myCharacterRect.bottom = CrashBottom.top;
                        myCharacterRect.top = myCharacterRect.bottom - 48;
                        jumping = FALSE;
                        drop = FALSE;
                        pos_jump = TRUE;
                        return;
                    }
                }
            }

            for (int i = 0; i < g_obsBottomNum; i++) {
                // 낙하 중 진흙탕과 닿으면 변화 값 초기상태로 복구
                if (IntersectRect(&CrashBottom, &g_obsBottomDrop[i], &NowmyCharacterRect)) {
                    g_JumpPower = JumpPower;
                    g_JumpHeight = JumpHeight;
                    myCharacterRect.bottom = CrashBottom.bottom;
                    myCharacterRect.top = myCharacterRect.bottom - 48;
                    jumping = FALSE;
                    drop = FALSE;
                    pos_jump = TRUE;
                    return;
                }
            }

            // 바닥과 닿으면 점프 값 초기상태로 전환하고 플레이어 위치 조정
            if (TRUE == IntersectRect(&CrashBottom, &g_bottom, &NowmyCharacterRect)) {
                g_JumpPower = JumpPower;
                g_JumpHeight = JumpHeight;
                g_Gravity = Gravity;
                myCharacterRect.bottom = CrashBottom.top;
                myCharacterRect.top = myCharacterRect.bottom - 48;
                jumping = FALSE;
                drop = FALSE;
                bottom = TRUE;
                pos_jump = TRUE;
                return;
            }
        }
    }
}

void ItemBoxSet(HWND itemHWND) {
    RECT item;                  // 아이템 곂침 영역 확인용
    RECT drop_item;             // 아이템과 플레이어의 곂침 영역 저장
    RECT item_bottom;           // 아이템 바닥 영역 저장

    item_bottom.left = g_ItemBox[1].left;
    item_bottom.top = g_ItemBox[1].bottom + 1;
    item_bottom.right = g_ItemBox[1].right;
    item_bottom.bottom = item_bottom.top + 1;

    // 1번 아이템(먹으면 다른 아이템이 나옴)
    if (IntersectRect(&item, &NowmyCharacterRect, &item_bottom) && Move_item == FALSE) {
        if (itemBoxTimer == 0) {
            itemBoxTimer = 1; //무적 시간 만들어주기
        }
        return;
    }

    // 1번 아이템을 먹으면 다른 아이템이 나옴(발판용)
    if (TRUE == ActItem && Move_item == FALSE) {
        
        // 아이템이 지속적으로 밑으로 이동함
        g_ItemBox[0].left -= 1;
        g_ItemBox[0].top += 1;
        g_ItemBox[0].right -= 1;
        g_ItemBox[0].bottom += 1;

        // 플레이어와 아이템이 닿았을 시 해당위치에 아이템 위치 및 관련 값들 고정
        if (IntersectRect(&drop_item, &NowmyCharacterRect, &g_ItemBox[0]) && Move_item == FALSE) {
            g_ItemBox[0].left = NowmyCharacterRect.left;
            g_ItemBox[0].top = NowmyCharacterRect.top;
            g_ItemBox[0].right = g_ItemBox[0].left + 30;
            g_ItemBox[0].bottom = g_ItemBox[0].top + 30;

            // 충돌 영역 : bottom
            g_ItemCrash[0].left = g_ItemBox[0].left + 1;
            g_ItemCrash[0].top = g_ItemBox[0].bottom;
            g_ItemCrash[0].right = g_ItemBox[0].right - 1;
            g_ItemCrash[0].bottom = g_ItemCrash[0].top + 2;

            // 낙하 영역 : left
            g_ItemDrop[0].right = g_ItemBox[0].left - 30;
            g_ItemDrop[0].bottom = g_ItemBox[0].top - 1;
            g_ItemDrop[0].left = g_ItemDrop[0].right - 1;
            g_ItemDrop[0].top = g_ItemDrop[0].bottom - 1;

            // 낙하 영역 : right
            g_ItemDrop[1].left = g_ItemBox[0].right + 30;
            g_ItemDrop[1].right = g_ItemDrop[1].left + 1;
            g_ItemDrop[1].bottom = g_ItemBox[0].top - 1;
            g_ItemDrop[1].top = g_ItemDrop[1].bottom - 1;

            Move_item = TRUE;
            return;
        }
    }

    // 2번 아이템(먹으면 위로 올라가서 죽음)
    if (IntersectRect(&item, &NowmyCharacterRect, &g_ItemBox[2])) {
        ActItem2 = TRUE;
        return;
    }

    if (TRUE == ActItem2) {
        g_ItemBox[2].left = -100;
        g_ItemBox[2].top = -100;
        g_ItemBox[2].right = -100;
        g_ItemBox[2].bottom = -100;
        myCharacterRect.top -= 10;
        myCharacterRect.bottom -= 10;
        if (myCharacterRect.top < 100) {
            live = FALSE;
            ActItem2 = FALSE;
        }
    }
}

// 함수 정의 끝