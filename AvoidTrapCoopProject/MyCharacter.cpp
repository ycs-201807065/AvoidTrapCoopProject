#include "myCharacter.h"


MyCharacter::MyCharacter() {
	speed = 10;
}

int MyCharacter::Move(int flag) {
	//flag 1 -> 왼쪽 이동
	//flag 2 -> 오른쪽 이동
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
	//느려지는거 설정
	speed -= debuffNumber;
}