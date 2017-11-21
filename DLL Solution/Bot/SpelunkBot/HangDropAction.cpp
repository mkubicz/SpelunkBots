#include "stdafx.h"
#include "HangDropAction.h";

HangDropAction::HangDropAction(IBot* bot, bool goingRight, bool lookDown)
	: IMovementAction(bot)
{
	_actionInProgress = false;
	_goingRight = goingRight;
	_lookDown = lookDown;
	_lookDownTimer = _lookDown ? 60 : 0;
	_actionDone = false;
	_hanging = false;
	_state = WALKING;
}


ordersStruct HangDropAction::GetOrders()
{
	int playerPosX = (int)_bot->GetPlayerPositionX();
	int playerPosY = (int)_bot->GetPlayerPositionY();
	ordersStruct orders;

	if (!_actionInProgress)
	{
		int nodenr = (int)_bot->GetPlayerPositionXNode();
		_targetX = _goingRight ? (nodenr + 1) * PIXELS_IN_NODE : (nodenr * PIXELS_IN_NODE) - 1;
		_targetY = playerPosY + PIXELS_IN_NODE;

		_actionInProgress = true;
	}

	if (playerPosY == _targetY) _state = HANGING;
	if (playerPosY > _targetY) _state = FALLING;

	switch (_state)
	{
		case WALKING:
			if (_goingRight && _targetX - playerPosX > 0) orders.goRight = true;
			if (!_goingRight && playerPosX - _targetX > 0) orders.goLeft = true;
			if (closeToTarget(playerPosX, _targetX)) orders.duck = true;
			break;
		case HANGING:
			if (_lookDownTimer != 0)
			{
				orders.duck = true;
				_lookDownTimer -= 1;
			}
			else
			{
				orders.duck = true;
				orders.jump = true;
			}
			break;
		case FALLING:
			if (_previousPosY == playerPosY) _actionDone = true;
			break;
	}

	_previousPosY = playerPosY;

	return orders;
	

}