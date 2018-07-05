#include "stdafx.h"
#include "ClimbAction.h"

ClimbAction::ClimbAction(std::shared_ptr<IBot> const& bot, int distY)
	: IMovementAction(bot)
{
	_distY = distY;
	_actionType = CLIMB;
	_targetIsGround = false;

	if (_distY < 0) _directionY = yUP;
	if (_distY > 0) _directionY = yDOWN;
}

void ClimbAction::AddDistance(int distance)
{
	_distY += distance;
	_targetNode = Coords(_targetNode.GetX(), _targetNode.GetY() + distance);
}

ordersStruct ClimbAction::GetOrders()
{
	ordersStruct orders;
	int playerPosX = (int)_bot->GetPlayerPositionX();
	int playerPosY = (int)_bot->GetPlayerPositionY();
	

	if (!_actionInProgress)
	{
		CalculateTargetNode(0, _distY);
		if (!_bot->IsNodePassable(_targetNode.GetX(), _targetNode.GetY() + 1, NODE_COORDS))
			_targetIsGround = true;

		//initial boost when starting from ground
		if (_bot->GetSpelunkerState() != spCLIMBING)
			orders.jump = true;

		_actionInProgress = true;
	}


	if (!_targetIsGround)
	{
		if (playerPosY != _targetNode.GetMidYpixel())
		{
			if (playerPosY > _targetNode.GetMidYpixel())
				orders.lookUp = true;
			if (playerPosY < _targetNode.GetMidYpixel())
				orders.duck = true;
		}
		else
		{
			_actionDone = true;
		}
	}
	else
	{
		if (_bot->GetSpelunkerState() == spCLIMBING)
		{
			orders.duck = true;
		}
		else
		{
			_actionDone = true;
		}
	}




	return orders;
}
