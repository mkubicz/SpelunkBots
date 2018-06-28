#pragma once
#include "stdafx.h"
#include "IMovementAction.h"

class ThrowItemAction : public IMovementAction
{
private:

	//enum STATE { PICKING_UP, PICKED_UP, WAITING, DROPPING };
	//STATE _state;

public:
	ThrowItemAction(std::shared_ptr<IBot> const& bot);
	ThrowItemAction(std::shared_ptr<IBot> const& bot, directions dir);

	ordersStruct GetOrders() override;
};