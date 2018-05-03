#pragma once

#include "IMovementAction.h"

class ClimbAction : public IMovementAction
{
private:
	int _distX;
	int _distY;
public:
	ClimbAction(IBot * bot, int distY);
	void AddDistance(int distance);
	ordersStruct GetOrders();
};
