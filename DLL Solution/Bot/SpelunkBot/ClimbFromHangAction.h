#pragma once
#include "IMovementAction.h"

class ClimbFromHangAction : public IMovementAction
{
private:
	bool _jumped;
	int _climbTimer;

	int _prevPlayerPosX;
	int _prevPlayerPosY;

public:
	ClimbFromHangAction(std::shared_ptr<IBot> const& bot, DIRECTIONX directionX);
	ordersStruct GetOrders();
};
