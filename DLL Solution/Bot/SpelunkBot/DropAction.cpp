#include "stdafx.h"
#include "DropAction.h"

DropAction::DropAction(IBot* bot)
	: IMovementAction(bot)
{
}

ordersStruct DropAction::GetOrders()
{
	ordersStruct orders;

	if (_bot->GetSpelunkerState() == spCLIMBING || _bot->GetSpelunkerState() == spHANGING)
	{
		orders.duck = true;
		orders.jump = true;
	}
	if (_bot->GetSpelunkerState() == spSTANDING)
		_actionDone = true;

	return orders;
}
