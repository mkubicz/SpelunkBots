#include "stdafx.h"
#include "JumpAboveAction.h"

JumpAboveAction::JumpAboveAction(IBot* bot, bool goingRight)
	: IMovementAction(bot)
{
	_goingRight = goingRight;

	if (_goingRight)
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

		int nodenr = (int)_bot->GetPlayerPositionXNode();
		_targetX = nodenr * PIXELS_IN_NODE + (PIXELS_IN_NODE / 2);
		_targetY = playerPosY - (PIXELS_IN_NODE * 2);

		_targetXside = _goingRight ? _targetX + (PIXELS_IN_NODE / 2) : _targetX - (PIXELS_IN_NODE / 2);

		_actionInProgress = true;
	}

	if (_goingRight)
	{
		if (playerPosX <= _targetXside && playerPosY == _startingPosY) orders.goRight = true;
		else
		{
			if (!closeToTarget(playerPosX, _targetX)) orders.goLeft = true;
			if (playerPosY > _targetY) orders.jump = true;
		}
			
	}
	else
	{
		if (playerPosX >= _targetXside && playerPosY == _startingPosY) orders.goLeft = true;
		else
		{
			if (!closeToTarget(playerPosX, _targetX)) orders.goRight = true;
			if (playerPosY > _targetY) orders.jump = true;
		}
	}

	if (closeToTarget(playerPosX, _targetX) && playerPosY == _targetY) _actionDone = true;

	/*
	if (_goingRight && playerPosX < targetXside) orders.goRight = true;
	if (!_goingRight && playerPosX > targetXside) orders.goLeft = true;
	*/

	return orders;

}