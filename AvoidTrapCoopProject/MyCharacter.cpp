#include "myCharacter.h"


MyCharacter::MyCharacter() {
	speed = 10;
}

int MyCharacter::Move(int flag) {
	//flag 1 -> ���� �̵�
	//flag 2 -> ������ �̵�
	int moveing = 0;
	if (flag == 1) {
		moveing -= speed;
	}
	else if (flag == 2) {
		moveing += speed;
	}
	return moveing;
}


void MyCharacter::Debuff(int debuffNumber) {
	//�������°� ����
	speed -= debuffNumber;
}