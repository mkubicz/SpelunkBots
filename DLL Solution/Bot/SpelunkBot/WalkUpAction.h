#pragma once
#include "IMovementAction.h"

class WalkUpAction : public IMovementAction
{
private:
	bool _againstTheWall;
	double _previousPosX;

public:
	WalkUpAction(IBot *bot, bool goingRight);
	~WalkUpAction() {};

	ordersStruct GetOrders() override;
};