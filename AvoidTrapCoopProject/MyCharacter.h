#pragma once

#include<iostream>

class MyCharacter {
private:
	int speed;

public:
	//MyCharacter();
	//int Move(int flag);
	void Debuff(int debuffNumber);
	int getSpeed() {
		return speed;
	}
};


