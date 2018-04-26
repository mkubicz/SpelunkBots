#include "stdafx.h"
#include "ClimbFromHangAction.h"

ClimbFromHangAction::ClimbFromHangAction(IBot * bot, DIRECTIONX directionX)
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
		_targetX = (int)_bot->GetPlayerPositionXNode() * PIXELS_IN_NODE + PIXELS_IN_NODE/2;
		_directionX == xRIGHT ? _targetX += PIXELS_IN_NODE : _targetX -= PIXELS_IN_NODE;
		_targetY = ((int)_bot->GetPlayerPositionYNode() - 1) * PIXELS_IN_NODE + PIXELS_IN_NODE / 2;

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
			Centralize(&orders, MiddleXPixel(Node(_targetX, _targetY)));
	}

	if (closeToTarget(playerPosX, _targetX) && _targetY == playerPosY) _actionDone = true;


	_prevPlayerPosX = playerPosX;
	_prevPlayerPosY = playerPosY;

	return orders;
}
