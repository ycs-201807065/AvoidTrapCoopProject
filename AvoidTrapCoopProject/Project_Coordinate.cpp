//
//	��ǥ���� �����ϴ� cpp �����Դϴ�.
//

// ����, ��ֹ�, ��ֹ�[����], �����۹ڽ� ����
const int g_scafNum = 8;
const int g_obsNum = 3;
const int g_obsBottomNum = 3;
const int g_obsThornNum = 5;
const int g_itemBoxNum = 3;
const int g_cloudobjNum = 4;

// ���� ��ǥ
int g_scafRectLeft[g_scafNum] = {200, 600, 900, 1070, 1340, 1390, 1440, 1490};
int g_scafRectTop[g_scafNum] = {430, 500, 465, 465, 500, 450, 400, 350};

// ��ֹ� ��ǥ
int g_obsRectLeft[g_obsNum] = {269, 950, 980};
int g_obsRectTop[g_obsNum] = {552, 375, 295};
int g_obsRectRight[g_obsNum] = {365, 980, 1010};
int g_obsRectBottom[g_obsNum] = {750, 405, 325};

// ��ֹ�[�ٴ�] ��ǥ
int g_obsBottomRectLeft[g_obsBottomNum] = {550, 1200, 1620};
int g_obsBottomRectTop[g_obsBottomNum] = {552, 552, 552};
int g_obsBottomRectRight[g_obsBottomNum] = {600, 1280, 1700};
int g_obsBottomRectBottom[g_obsBottomNum] = {750, 750, 750};

// ��ֹ�[����] ��ǥ
int g_obsThornLeft[g_obsThornNum * 3] = {269, 281, 293, 293, 305, 317, 317, 329, 341, 341, 353, 365, 1412, 1414, 1416};
int g_obsThornTop[g_obsThornNum * 3] = {552, 520, 552, 552, 520, 552, 552, 520, 552, 552, 520, 552, 450, 449, 450};

// ������ �ڽ� ��ǥ
int g_itemBoxLeft[g_itemBoxNum] = {140, 140, 450};
int g_itemBoxTop[g_itemBoxNum] = {400, 450, 520};

int g_TrapConfirmLeft[2] = {275, 445};
int g_TrapConfirmTop[2] = {210, 450};
int g_TrapConfirmRight[2] = {305, 475};
int g_TrapConfirmBottom[2] = {420, 520};

// ���� ��ǥ(0�� �ε����� ������Ʈ, �������� ����)
int g_cloudobjLeft[g_cloudobjNum] = {1610, 725, 1075, 1160};
int g_cloudobjTop[g_cloudobjNum] = {260, 420, 210, 360};
int g_cloudobjRight[g_cloudobjNum] = {1730, 845, 1190, 1280};
int g_cloudobjBottom[g_cloudobjNum] = {310, 470, 260, 410};