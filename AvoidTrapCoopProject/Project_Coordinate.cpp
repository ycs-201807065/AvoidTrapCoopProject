//
//	��ǥ���� �����ϴ� cpp �����Դϴ�.
//

// ����, ��ֹ�, ��ֹ�[����], �����۹ڽ� ����
const int g_scafNum = 8;
const int g_obsNum = 10;
const int g_obsThornNum = 5;
const int g_itemBoxNum = 3;
const int g_cloudobjNum = 4;

// ���� ��ǥ
int g_scafRectLeft[g_scafNum] = {200, 600, 900, 1070, 1340, 1390, 1440, 1490};
int g_scafRectTop[g_scafNum] = {430, 500, 465, 465, 500, 450, 400, 350};

// ��ֹ� ��ǥ
int g_obsRectLeft[g_obsNum] = {269, 269, 550, 550, 970, 950, 980, 1200, 1620, 1650};
int g_obsRectTop[g_obsNum] = {490, 552, 470, 552, 465, 385, 305, 552, 552, 492};
int g_obsRectRight[g_obsNum] = {365, 365, 600, 600, 1070, 980, 1010, 1280, 1700, 1700};
int g_obsRectBottom[g_obsNum] = {520, 750, 500, 750, 495, 415, 335, 750, 750, 522};

// ��ֹ�[����] ��ǥ
int g_obsThornLeft[g_obsThornNum * 3] = {269, 281, 293, 293, 305, 317, 317, 329, 341, 341, 353, 365, 1412, 1414, 1416};
int g_obsThornTop[g_obsThornNum * 3] = {552, 520, 552, 552, 520, 552, 552, 520, 552, 552, 520, 552, 450, 449, 450};

// ������ �ڽ� ��ǥ
int g_itemBoxLeft[g_itemBoxNum] = {140, 140, 450};
int g_itemBoxTop[g_itemBoxNum] = {400, 450, 520};

// Ʈ�� �ߵ� ���� ����
int g_TrapConfirmLeft[3] = {275, 445, 1648};
int g_TrapConfirmTop[3] = {210, 450, 492};
int g_TrapConfirmRight[3] = {305, 475, 1700};
int g_TrapConfirmBottom[3] = {420, 520, 524};

// ���� ��ǥ(0�� �ε����� ������Ʈ, �������� ����)
int g_cloudobjLeft[g_cloudobjNum] = {1580, 720, 1030, 1160};
int g_cloudobjTop[g_cloudobjNum] = {260, 420, 210, 360};
int g_cloudobjRight[g_cloudobjNum] = {1700, 840, 1150, 1280};
int g_cloudobjBottom[g_cloudobjNum] = {310, 470, 260, 410};