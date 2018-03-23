#include "stdafx.h"
#include "ClimbAction.h"

ClimbAction::ClimbAction(IBot * bot, int distY)
	: IMovementAction(bot)
{
	_distY = distY;
	_actionType = CLIMB;

	if (_distY < 0) _directionY = yUP;
	if (_distY > 0) _directionY = yDOWN;

	//_distY < 0 ? _goingUp = true : _goingUp = false;
}

void ClimbAction::AddDistance(int distance)
{
	_distY += distance;
}

ordersStruct ClimbAction::GetOrders()
{
	ordersStruct orders;
	int playerPosX = (int)_bot->GetPlayerPositionX();
	int playerPosY = (int)_bot->GetPlayerPositionY();
	

	if (!_actionInProgress)
	{
		_targetNode = CalculateTargetNode((int)_bot->GetPlayerPositionXNode(), (int)_bot->GetPlayerPositionYNode(), 0, _distY);
		_actionInProgress = true;
	}

	if (playerPosY != MiddleYPixel(_targetNode))
	{
		if (playerPosY > MiddleYPixel(_targetNode))
			orders.lookUp = true;
		if (playerPosY < MiddleYPixel(_targetNode))
			orders.duck = true;
	}
	else
	{
		_actionDone = true;
	}


	return orders;
}
