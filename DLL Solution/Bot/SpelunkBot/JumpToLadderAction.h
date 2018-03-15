#pragma once

#include "IMovementAction.h"

//jump from ground to ladder
class JumpToLadderAction : public IMovementAction
{
private:
	int _distX;
	int _distY;
	Node _targetNode;
	bool _running = false;
	int _jumpTimer;
	int _directionBlockTimer;
	int _microAdjXTimer;

	int _startPosEdgeX;

	enum STATE { WALKING, JUMPING, FALLING, CLIMBING };
	STATE _state;
public:
	JumpToLadderAction(IBot *bot, int distX, int distY);
	ordersStruct GetOrders();
};