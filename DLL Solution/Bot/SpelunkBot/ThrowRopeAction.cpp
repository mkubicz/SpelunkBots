#include "stdafx.h"
#include "ThrowRopeAction.h"

ThrowRopeAction::ThrowRopeAction(std::shared_ptr<IBot> const & bot)
	: IMovementAction(bot)
{
	_actionType = THROWROPE;
}

ordersStruct ThrowRopeAction::GetOrders()
{
	ordersStruct orders;

	orders.ropep = true;
	_actionDone = true;

	return orders;
}
