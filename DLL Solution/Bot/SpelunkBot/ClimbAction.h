#pragma once

#include "IMovementAction.h"

class ClimbAction : public IMovementAction
{
private:
	int _distX;
	int _distY;
	Node _targetNode;
public:
	ClimbAction(IBot * bot, int distY);
	ordersStruct GetOrders();
};
