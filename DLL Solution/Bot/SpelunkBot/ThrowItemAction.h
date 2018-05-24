#pragma once
#include "stdafx.h"
#include "IMovementAction.h"

class ThrowItemAction : public IMovementAction
{
private:

	//enum STATE { PICKING_UP, PICKED_UP, WAITING, DROPPING };
	//STATE _state;

public:
	ThrowItemAction(IBot *bot);
	ThrowItemAction(IBot *bot, DIRECTIONX dirX, DIRECTIONY dirY);

	ordersStruct GetOrders() override;
};