#pragma once
#include "IMovementAction.h"

class JumpAction : public IMovementAction
{
private:
	int _distX;
	int _distY;
	int _startPosXside;
	int _targetXSide;
	int _jumpTimer;
	int _previousPosX;
	int _previousPosY;
	bool _running;
	bool _goingUp;
	bool _hangedForOneFrame;
	enum STATE { WALKING, JUMPING, FALLING, HANGING, CLIMBING };
	STATE _state;

public:
	JumpAction(IBot * bot, bool goingRight, bool goingUp, int distX, int distY);
	ordersStruct GetOrders();
};