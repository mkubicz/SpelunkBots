#pragma once
#include "IMovementAction.h"

class PickUpItemAction : public IMovementAction
{
private:
	int _itemID;
	int _waitTimer;
	bool _pickupByID;

	enum STATE { PICKING_UP, PICKED_UP, WAITING, DROPPING };
	STATE _state;

public:
	PickUpItemAction(IBot *bot, int itemID);
	PickUpItemAction(IBot *bot);

	ordersStruct GetOrders() override;
};