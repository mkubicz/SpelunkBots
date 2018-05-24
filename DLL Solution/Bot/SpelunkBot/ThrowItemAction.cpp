#include "stdafx.h"
#include "ThrowItemAction.h"

ThrowItemAction::ThrowItemAction(IBot * bot)
	: ThrowItemAction(bot, xNONE, yNONE)
{
}

ThrowItemAction::ThrowItemAction(IBot * bot, DIRECTIONX dirX, DIRECTIONY dirY)
	: IMovementAction(bot)
{
	_directionX = dirX;
	_directionY = dirY;
}

ordersStruct ThrowItemAction::GetOrders()
{
	ordersStruct orders;

	if (!_actionInProgress)
	{
		if (!_bot->IsHoldingItem())
		{
			_actionDone = true;
			std::cout << "ThrowItemAction: Empty hands while throwing" << std::endl;
		}

		_actionInProgress = true;
	}


	if (_bot->IsHoldingItem())
	{
		if (_directionX == xRIGHT)
			orders.goRight = true;
		else if (_directionX == xLEFT)
			orders.goLeft = true;

		if (_directionY == yUP)
			orders.lookUp = true;
		else if (_directionY == yDOWN)
			orders.duck = true;

		orders.attack = true;
	}
	else
	{
		_actionDone = true;
	}


	return orders;
}
