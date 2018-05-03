#include "stdafx.h"
#include "JumpAboveAction.h"

JumpAboveAction::JumpAboveAction(IBot* bot, DIRECTIONX directionX)
	: IMovementAction(bot)
{
	_directionX = directionX;

	if (_directionX == xRIGHT)
		_actionType = JUMPABOVERIGHT;
	else
		_actionType = JUMPABOVELEFT;

	_jumped = false;
}

ordersStruct JumpAboveAction::GetOrders()
{
	int playerPosX = (int)_bot->GetPlayerPositionX();
	int playerPosY = (int)_bot->GetPlayerPositionY();
	ordersStruct orders;

	if (!_actionInProgress)
	{
		_startingPosY = playerPosY;

		_targetNode = CalculateTargetNode(0, -2);
		_targetXmid = MiddlePixelOfNode(_targetNode.GetX());
		_targetYmid = MiddlePixelOfNode(_targetNode.GetY());
		_targetXside = _directionX == xRIGHT ? _targetXmid + (PIXELS_IN_NODE / 2) : _targetXmid - (PIXELS_IN_NODE / 2);

		_actionInProgress = true;
	}

	if (_directionX == xRIGHT)
	{
		if (playerPosX <= _targetXside && playerPosY == _startingPosY) orders.goRight = true;
		else
		{
			if (!closeToTarget(playerPosX, _targetXmid)) orders.goLeft = true;
			if (playerPosY > _targetYmid) orders.jump = true;
		}
			
	}
	else
	{
		if (playerPosX >= _targetXside && playerPosY == _startingPosY) orders.goLeft = true;
		else
		{
			if (!closeToTarget(playerPosX, _targetXmid)) orders.goRight = true;
			if (playerPosY > _targetYmid) orders.jump = true;
		}
	}

	if (closeToTarget(playerPosX, _targetXmid) && playerPosY == _targetYmid) _actionDone = true;

	return orders;

}