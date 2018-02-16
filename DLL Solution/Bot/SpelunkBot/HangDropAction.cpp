#include "stdafx.h"
#include "HangDropAction.h";

HangDropAction::HangDropAction(IBot* bot, bool goingRight, bool lookDown)
	: IMovementAction(bot)
{
	_goingRight = goingRight;

	if (_goingRight)
		_actionType = HANGDROPRIGHT;
	else
		_actionType = HANGDROPLEFT;

	_actionInProgress = false;
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
		int nodeXnr = (int)_bot->GetPlayerPositionXNode();
		int nodeYnr = (int)_bot->GetPlayerPositionYNode();
		_targetX = _goingRight ? (nodeXnr + 1) * PIXELS_IN_NODE : (nodeXnr * PIXELS_IN_NODE) - 1;
		_targetYHang = playerPosY + PIXELS_IN_NODE;
		
		
		int dropX = _goingRight ? playerPosX + PIXELS_IN_NODE : playerPosX - PIXELS_IN_NODE;
		for (int i = 2; i <= Y_NODES; i++) //if over 8, the bot will take damage
		{
			if (!_bot->IsNodePassable(dropX, (playerPosY + i*PIXELS_IN_NODE), PIXEL_COORDS))
			{
				_targetY = playerPosY + (i-1)*PIXELS_IN_NODE;
				break;
			}
		}
		
		_actionInProgress = true;
	}

	/*
	if (playerPosY == _targetY) _state = HANGING;
	if (playerPosY > _targetY) _state = FALLING;
	*/

	switch (_state)
	{
		case WALKING:
			if (_goingRight && _targetX - playerPosX > 0) orders.goRight = true;
			if (!_goingRight && playerPosX - _targetX > 0) orders.goLeft = true;
			if (closeToTarget(playerPosX, _targetX)) orders.duck = true;
			
			//if (playerPosY == _targetYHang) _state = HANGING;
			if (_bot->GetSpelunkerState() == spHANGING) _state = HANGING;
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

				if (playerPosY >= _targetYHang) _state = FALLING;
			}
			break;
		case FALLING:
			if (playerPosY == _targetY) _actionDone = true;
			break;
	}

	_previousPosY = playerPosY;

	return orders;
	

}