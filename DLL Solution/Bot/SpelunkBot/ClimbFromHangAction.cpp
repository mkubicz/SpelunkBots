#include "stdafx.h"
#include "ClimbFromHangAction.h"

ClimbFromHangAction::ClimbFromHangAction(std::shared_ptr<IBot> const& bot, DIRECTIONX directionX)
	: IMovementAction(bot)
{
	_directionX = directionX;
	_jumped = false;
	_climbTimer = 4;
}

ordersStruct ClimbFromHangAction::GetOrders()
{
	ordersStruct orders;
	int playerPosX = (int)_bot->GetPlayerPositionX();
	int playerPosY = (int)_bot->GetPlayerPositionY();

	if (!_actionInProgress)
	{
		int offset = _directionX == xRIGHT ? 1 : -1;
		CalculateTargetNode(offset, -1);

		_actionInProgress = true;
	}

	if (!_jumped)
	{
		orders.jump = true;
		_jumped = true;
	}
	else
	{
		if (_climbTimer > 0)
		{
			_climbTimer -= 1;
			_directionX == xRIGHT ? orders.goRight = true : orders.goLeft = true;
		}
		
		if (IsStandingStill(playerPosX, playerPosY, _prevPlayerPosX, _prevPlayerPosY))
			Centralize(&orders, _targetNode.GetMidXpixel());
	}

	if (closeToTarget(playerPosX, _targetNode.GetMidXpixel()) && 
		_targetNode.GetMidYpixel() == playerPosY) _actionDone = true;


	_prevPlayerPosX = playerPosX;
	_prevPlayerPosY = playerPosY;

	return orders;
}
