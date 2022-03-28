//
//	좌표값을 지정하는 cpp 파일입니다.
//

// 발판, 장애물, 장애물[가시], 아이템박스 개수
const int g_scafNum = 8;
const int g_obsNum = 7;
const int g_obsBottomNum = 3;
const int g_obsThornNum = 5;
const int g_itemBoxNum = 3;
const int g_cloudobjNum = 4;

// 발판 좌표
int g_scafRectLeft[g_scafNum] = {200, 600, 900, 1070, 1340, 1390, 1440, 1490};
int g_scafRectTop[g_scafNum] = {430, 500, 465, 465, 500, 450, 400, 350};

// 장애물 좌표
int g_obsRectLeft[g_obsNum] = {269, 269, 550, 970, 950, 980, 1650};
int g_obsRectTop[g_obsNum] = {490, 552, 470, 465, 385, 305, 492};
int g_obsRectRight[g_obsNum] = {365, 365, 600, 1070, 980, 1010, 1700};
int g_obsRectBottom[g_obsNum] = {520, 750, 500, 495, 415, 335, 522};

// 장애물[바닥] 좌표
int g_obsBottomRectLeft[g_obsBottomNum] = {550, 1200, 1620};
int g_obsBottomRectTop[g_obsBottomNum] = {552, 552, 552};
int g_obsBottomRectRight[g_obsBottomNum] = {600, 1280, 1700};
int g_obsBottomRectBottom[g_obsBottomNum] = {750, 750, 750};

// 장애물[가시] 좌표
int g_obsThornLeft[g_obsThornNum * 3] = {269, 281, 293, 293, 305, 317, 317, 329, 341, 341, 353, 365, 1412, 1414, 1416};
int g_obsThornTop[g_obsThornNum * 3] = {552, 520, 552, 552, 520, 552, 552, 520, 552, 552, 520, 552, 450, 449, 450};

// 아이템 박스 좌표
int g_itemBoxLeft[g_itemBoxNum] = {140, 140, 450};
int g_itemBoxTop[g_itemBoxNum] = {400, 450, 520};

// 트랩 발동 조건 영역
/*
int g_TrapConfirmLeft[3] = {275, 445, 1648};
int g_TrapConfirmTop[3] = {210, 450, 492};
int g_TrapConfirmRight[3] = {305, 475, 1700};
int g_TrapConfirmBottom[3] = {420, 520, 524};
*/
int g_TrapConfirmLeft[2] = {275, 445};
int g_TrapConfirmTop[2] = {210, 450};
int g_TrapConfirmRight[2] = {305, 475};
int g_TrapConfirmBottom[2] = {420, 520};

// 구름 좌표(0번 인덱스는 오브젝트, 나머지는 함정)
int g_cloudobjLeft[g_cloudobjNum] = {1580, 720, 1030, 1160};
int g_cloudobjTop[g_cloudobjNum] = {260, 420, 210, 360};
int g_cloudobjRight[g_cloudobjNum] = {1700, 840, 1150, 1280};
int g_cloudobjBottom[g_cloudobjNum] = {310, 470, 260, 410};