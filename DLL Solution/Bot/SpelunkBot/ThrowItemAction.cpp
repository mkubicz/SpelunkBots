#include "stdafx.h"
#include "ThrowItemAction.h"

ThrowItemAction::ThrowItemAction(std::shared_ptr<IBot> const& bot)
	: ThrowItemAction(bot, directions{ xNONE, yNONE })
{
}

ThrowItemAction::ThrowItemAction(std::shared_ptr<IBot> const& bot, directions dir)
	: IMovementAction(bot)
{
	_directionX = dir.x;
	_directionY = dir.y;
	_actionType = THROWITEM;
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

		if (_directionY == yDOWN)
		{


			if (_bot->GetSpelunkerState() == spDUCKING || _bot->GetSpelunkerState() == spCLIMBING)
				orders.attack = true;
		}
		else
			orders.attack = true;
	}
	else
	{
		_actionDone = true;
	}


	return orders;
}
