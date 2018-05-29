#include "stdafx.h"
#include "HangAction.h"

HangAction::HangAction(IBot * bot, DIRECTIONX directionX)
	: IMovementAction(bot)
{
	_directionX = directionX;
}


ordersStruct HangAction::GetOrders()
{
	ordersStruct orders;

	if (!_actionInProgress)
	{
		int offset = _directionX == xRIGHT ? 1 : -1;
		CalculateTargetNode(offset, 1);

		_actionInProgress = true;
	}

	if (_bot->GetSpelunkerState() != spHANGING && _bot->GetSpelunkerState() != spCLIMBING)
	{
		orders.duck = true;
		_directionX == xRIGHT ? orders.goRight = true : orders.goLeft = true;
	}
	else
		_actionDone = true;

	return orders;
}
