#pragma once

#include "IMovementAction.h"

class JumpFromLadderAction : public IMovementAction
{
private:
	int _distX;
	int _distY;
	int _jumpTimer;
	int _dirDelayTimer;
	int _moveRange;
	bool _hasMomentum;
	JUMP_TARGET _jumpTarget;
	Node _targetNode;

	enum STATE { JUMPING, FALLING, CLIMBING, HANGING };
	STATE _state;

	void SetTimers();
	void SetMovementRange();
	void MoveToTarget(ordersStruct *orders);

public:
	JumpFromLadderAction(IBot * bot, JUMP_TARGET target, bool hasMomentum, int distX, int distY);
	JumpFromLadderAction(IBot *bot, int distX, int distY);



	ordersStruct GetOrders();
};