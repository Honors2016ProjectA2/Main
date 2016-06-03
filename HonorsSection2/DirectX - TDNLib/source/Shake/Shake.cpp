#include "TDNLIB.h"
#include "Shake.h"

namespace SHAKE{
	const float WIDTH = 0.1f;
	const int MAX_TIME = 60;
	const int WAIT_1 = 20;
	const int WAIT_2 = 10;
}

void ShakeManager::Initialize()
{
	move = Vector2(0, 0);
	timer = 0;
	beforeFloor = 0;
	waitTimer = 0;
}

void ShakeManager::Update()
{
	if (waitTimer > 0){
		waitTimer--;
		return;
	}

	if (timer > 0){
		move = Vector2(timer*SHAKE::WIDTH*(rand() % 3 - 1), timer*SHAKE::WIDTH*(rand() % 3 - 1));
		timer--;
	}
	else{
		move = Vector2(0, 0);
	}
}

void ShakeManager::Set()
{
	timer = SHAKE::MAX_TIME;
	//waitTimer = SHAKE::WAIT_1;
}