#pragma once

#include "IMovementAction.h"

class JumpAction : public IMovementAction
{
private:
	int _distX;
	int _distY;
	bool _running;
	int _jumpTimer;
	int _dirDelayTimer;
	int _moveRange;
	ACTION_TARGET _actionTarget;

	int _prevPlayerPosX;
	int _prevPlayerPosY;

	int _jumpThreshold;
	int _startNodeCenter;
	int _finishedTimer = 0;

	enum STATE { CENTRALIZING, WALKING, JUMPING, FALLING, CLIMBING, LANDED_STUCK, FINISHED };
	STATE _state;

	void SetRunning();
	void SetTimers();
	void SetMovementRange();
	void MoveToTarget(ordersStruct *orders);

public:
	JumpAction(IBot * bot, ACTION_TARGET target, int distX, int distY);
	JumpAction(IBot *bot, int distX, int distY);


	ordersStruct GetOrders();
};