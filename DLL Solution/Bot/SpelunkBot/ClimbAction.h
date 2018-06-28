#pragma once

#include "IMovementAction.h"

class ClimbAction : public IMovementAction
{
private:
	int _distX;
	int _distY;
	bool _targetIsGround;
public:
	ClimbAction(std::shared_ptr<IBot> const& bot, int distY);
	void AddDistance(int distance);
	ordersStruct GetOrders();
};
