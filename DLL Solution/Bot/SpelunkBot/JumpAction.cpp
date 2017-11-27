#include "stdafx.h"
#include "JumpAction.h"

JumpAction::JumpAction(IBot * bot, bool goingRight, bool goingUp, int distX, int distY)
	: IMovementAction(bot)
{
	_goingRight = goingRight;
	_goingUp = goingUp;
	_distX = distX;
	_distY = distY;
	_previousPosX = (int)_bot->GetPlayerPositionX();
	_previousPosY = (int)_bot->GetPlayerPositionY();
	_jumpTimer = 60;
	_state = WALKING;
	_hangedForOneFrame = false;
	_running = abs(distX) > 3;
}


ordersStruct JumpAction::GetOrders()
{
	int playerPosX = (int)_bot->GetPlayerPositionX();
	int playerPosY = (int)_bot->GetPlayerPositionY();
	ordersStruct orders;

	if (!_actionInProgress)
	{
		int nodenr = (int)_bot->GetPlayerPositionXNode();
		if (_goingUp) _distY = -_distY;
		if (!_goingRight) _distX = -_distX;

		_targetX = (nodenr + _distX) * PIXELS_IN_NODE + (PIXELS_IN_NODE / 2);
		_targetY = playerPosY + (_distY * PIXELS_IN_NODE);
		_targetXSide = _goingRight ? (nodenr + _distX) * PIXELS_IN_NODE : (nodenr + 1 + _distX) * PIXELS_IN_NODE;
		_startPosXside = _goingRight ? (nodenr * PIXELS_IN_NODE) + PIXELS_IN_NODE : (nodenr * PIXELS_IN_NODE);

		_actionInProgress = true;
	}

	//TODO: switch to decent hanging check
	if (_previousPosX == playerPosX && playerPosY == _targetY + PIXELS_IN_NODE && 
		(playerPosX < _targetX + PIXELS_IN_NODE || _goingRight) && 
		(playerPosX > _targetX - PIXELS_IN_NODE || !_goingRight)
		&& _state != CLIMBING) _state = HANGING;


	switch (_state)
	{
		case WALKING:
			if (_running) orders.run = true;
			_goingRight ? orders.goRight = true : orders.goLeft = true;

			if (WentThrough(_goingRight, playerPosX, _startPosXside))
			{
				orders.jump = true;
				_state = JUMPING;
			}

			break;
		case JUMPING:
			//if (_running) orders.run = true;
			_goingRight ? orders.goRight = true : orders.goLeft = true;
			orders.jump = true;

			//if bot starts to lose altitude or if he is close to target and has some vertical distance to travel, state = falling
			if (playerPosY > _previousPosY || (closeToTargetFall(playerPosX, _targetX, orders.run) && _distY > 2)) _state = FALLING;

			break;
		case FALLING:
			if (closeToTargetFall(playerPosX, _targetX, _running) && _distY > 2) {/*i'm falling free yeah*/}
			else //move closer to target
			{
				if (playerPosX < _targetX) orders.goRight = true;
				else orders.goLeft = true;
			}
			if (playerPosY == _targetY) _actionDone = true;
			break;
		case HANGING:
			_state = CLIMBING;
			break;
		case CLIMBING:
			if (playerPosY > _targetY) orders.jump = true;
			else _goingRight ? orders.goRight = true : orders.goLeft = true;
			break;
		default:
			break;
	}


	if (closeToTarget(playerPosX, _targetX) && _targetY == playerPosY) _actionDone = true;

	_previousPosX = playerPosX;
	_previousPosY = playerPosY;

	return orders;
}
