#include "stdafx.h"
#include "PickUpItemAction.h"

PickUpItemAction::PickUpItemAction(std::shared_ptr<IBot> const& bot, int itemID)
	: IMovementAction(bot)
{
	_actionType = PICKUPITEM;
	_pickupByID = true;
	_itemID = itemID;
	_waitTimer = 0;
	_state = PICKING_UP;
}

PickUpItemAction::PickUpItemAction(std::shared_ptr<IBot> const& bot)
	: PickUpItemAction(bot, -1)
{
	_pickupByID = false;
}

ordersStruct PickUpItemAction::GetOrders()
{
	ordersStruct orders;

	orders.duck = true;

	switch (_state)
	{
	case PICKING_UP:

		if (_bot->GetSpelunkerState() == spDUCKING && _bot->GetHeldItemID() == 0)
			orders.attack = true;

		if (_bot->IsHoldingItem())
			_state = PICKED_UP;

		break;
	case PICKED_UP:
		if (_pickupByID)
		{
			if (_bot->GetHeldItemID() != _itemID)
				_state = DROPPING;
			else
				_actionDone = true;
		}
		else
			_actionDone = true;
		break;
	case DROPPING:
		if (_bot->IsHoldingItem())
			orders.attack = true;
		else
		{
			_waitTimer = 10;
			_state = WAITING;
		}
		break;
	case WAITING:
		if (_waitTimer > 0)
			_waitTimer--;
		else
			_state = PICKING_UP;
		break;


	}



	

	//orders.duck = true;
	//if (_waitTimer > 0)
	//	_waitTimer--;
	//else if (_bot->GetSpelunkerState() == spDUCKING)
	//	orders.attack = true;

	//if (_bot->GetHeldItemID() != 0)
	//{
	//	if (_bot->GetHeldItemID() == _itemID)
	//		_actionDone = true;
	//	else
	//		_waitTimer = 10;
	//}
	
	return orders;
}
