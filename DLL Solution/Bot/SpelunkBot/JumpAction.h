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
	JUMP_TARGET _jumpTarget;
	Node _targetNode;

	int _jumpThreshold;
	int _firstJumpPosX;

	int _startNodeCenter;
	int _centralizeMoveTimer = 0;
	int _centralizeBreakTimer = 0;
	DIRECTIONX _centralizeDir;

	enum STATE { CENTRALIZING, WALKING, JUMPING, FALLING, CLIMBING };
	STATE _state;

	void SetRunning();
	void SetTimers();
	void SetMovementRange();
	void ModifyMoveRangeIfNeeded(int firstJumpPosX);
	void MoveToTarget(ordersStruct *orders);
	void Centralize(ordersStruct *orders);


public:
	JumpAction(IBot * bot, JUMP_TARGET target, int distX, int distY);
	JumpAction(IBot *bot, int distX, int distY);


	ordersStruct GetOrders();
};