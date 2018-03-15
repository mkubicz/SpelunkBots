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

		//if you are right above ground don't jump; instead walk off
		if (!_bot->IsNodePassable(_bot->GetPlayerPositionXNode(), _bot->GetPlayerPositionYNode() + 1, NODE_COORDS))
			orders.jump = false;
	}
	if (_bot->GetSpelunkerState() == spSTANDING || _bot->GetSpelunkerState() == spDUCKING)
		_actionDone = true;

	return orders;
}
