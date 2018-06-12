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
	ACTION_TARGET _actionTarget;
	int _finishedTimer = 0;

	enum STATE { JUMPING, FALLING, CLIMBING, HANGING, FINISHED };
	STATE _state;

	void SetTimers();
	void SetMovementRange();
	void MoveToTarget(ordersStruct *orders);

public:
	JumpFromLadderAction(std::shared_ptr<IBot> const& bot, ACTION_TARGET target, bool hasMomentum, int distX, int distY);
	JumpFromLadderAction(std::shared_ptr<IBot> const& bot, int distX, int distY);

	ordersStruct GetOrders();
};