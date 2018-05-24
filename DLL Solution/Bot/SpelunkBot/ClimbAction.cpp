#include "stdafx.h"
#include "ClimbAction.h"

ClimbAction::ClimbAction(IBot * bot, int distY)
	: IMovementAction(bot)
{
	_distY = distY;
	_actionType = CLIMB;

	if (_distY < 0) _directionY = yUP;
	if (_distY > 0) _directionY = yDOWN;
}

void ClimbAction::AddDistance(int distance)
{
	_distY += distance;
	_targetNode = Node(_targetNode.GetX(), _targetNode.GetY() + distance);
}

ordersStruct ClimbAction::GetOrders()
{
	ordersStruct orders;
	int playerPosX = (int)_bot->GetPlayerPositionX();
	int playerPosY = (int)_bot->GetPlayerPositionY();
	

	if (!_actionInProgress)
	{
		_targetNode = CalculateTargetNode(0, _distY);
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
