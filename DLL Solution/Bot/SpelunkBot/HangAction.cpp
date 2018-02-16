#include "stdafx.h"
#include "HangAction.h"

HangAction::HangAction(IBot* bot, bool goingRight)
	: IMovementAction(bot)
{
	_goingRight = goingRight;
}

ordersStruct HangAction::GetOrders()
{
	ordersStruct orders;

	if (_bot->GetSpelunkerState() != spHANGING && _bot->GetSpelunkerState() != spCLIMBING)
	{
		orders.duck = true;
		_goingRight ? orders.goRight = true : orders.goLeft = true;
	}
	else
		_actionDone = true;

	return orders;
}
