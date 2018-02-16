#pragma once
#include "IMovementAction.h"

class ClimbFromHangAction : public IMovementAction
{
private:
	bool _goingRight;
	bool _jumped;
	int _climbTimer;

public:
	ClimbFromHangAction(IBot *bot, bool goingRight);
	ordersStruct GetOrders();
};
