#pragma once
#include "IMovementAction.h"

class JumpAction : public IMovementAction
{
private:
	int _distX;
	int _distY;
	int _startPosXside;
	int _startPosXmid;
	int _targetXSide;
	int _climbTimer;
	int _hangTimer;
	int _jumpTimer;
	int _previousPosX;
	int _previousPosY;
	bool _running;
	bool _goingUp;
	JUMP_TARGET _jumpTarget;
	enum STATE { WALKING, JUMPING, FALLING, HANGING, CLIMBING,	SMALLJUMP };
	STATE _state;

	bool _smljmp_backedUp;

	void SetActionType();

public:
	JumpAction(IBot * bot, bool goingRight, bool goingUp, int distX, int distY);
	JumpAction(IBot * bot, bool goingRight, bool goingUp, JUMP_TARGET target, int distX, int distY);
	ordersStruct GetOrders();
};