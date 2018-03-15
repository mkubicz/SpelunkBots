#pragma once
#include "IMovementAction.h"

class ClimbFromHangAction : public IMovementAction
{
private:
	bool _jumped;
	int _climbTimer;

public:
	ClimbFromHangAction(IBot *bot, DIRECTIONX directionX);
	ordersStruct GetOrders();
};
