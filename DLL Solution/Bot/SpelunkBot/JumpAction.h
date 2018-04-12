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
	Node _targetNode;

	int _jumpThreshold;
	int _firstJumpPosX;

	int _standingStillCounter;
	int _prevPlayerPosX;
	int _prevPlayerPosY;

	int _startNodeCenter;
	int _centralizeMoveTimer = 0;
	int _centralizeBreakTimer = 0;
	DIRECTIONX _centralizeDir;

	int _finishedTimer = 0;

	enum STATE { CENTRALIZING, WALKING, JUMPING, FALLING, CLIMBING, LANDED_STUCK, FINISHED };
	STATE _state;

	void SetRunning();
	void SetTimers();
	void SetMovementRange();
	void ModifyMoveRangeIfNeeded(int firstJumpPosX);
	void MoveToTarget(ordersStruct *orders);
	void Centralize(ordersStruct *orders, int centralizingPoint);

	bool IsStandingStill(int playerPosX, int playerPosY, int prevPlayerPosX, int prevPlayerPosY);

public:
	JumpAction(IBot * bot, ACTION_TARGET target, int distX, int distY);
	JumpAction(IBot *bot, int distX, int distY);


	ordersStruct GetOrders();
};