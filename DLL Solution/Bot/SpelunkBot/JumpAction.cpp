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
	_climbTimer = 4;
	_hangTimer = 2;
	if (distX <= 2) _jumpTimer = 1;
	if (distX > 2 && distX <= 5) _jumpTimer = 3;
	if (distX > 5) _jumpTimer = 6;
	_state = WALKING;
	_running = abs(distX) > 3;
}


ordersStruct JumpAction::GetOrders()
{
	int distToTargetY;
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



	distToTargetY = ConvertToNodeCoordinates(abs(playerPosY - _targetY));

	switch (_state)
	{
		case WALKING:
			//walking/running to the edge of node and jumping
			if (_running) orders.run = true;
			_goingRight ? orders.goRight = true : orders.goLeft = true;

			if (WentThrough(_goingRight, playerPosX, _startPosXside))
			{
				orders.jump = true;
				_state = JUMPING;
			}

			break;
		case JUMPING:
			//holding the jump button for a few frames and moving closer to target
			_goingRight ? orders.goRight = true : orders.goLeft = true;
			if (_jumpTimer > 0)
			{
				orders.jump = true;
				_jumpTimer -= 1;
			}

			//if bot is below the target the falling state should not be triggered
			if (playerPosY < (_targetY + 8))
			{
				//if bot is close to target, trigger free falling
				if (closeToTargetFall(playerPosX, _targetX, _running, _distY))
				{
					_state = FALLING;
				}
			}

			break;
		case FALLING:
			//falling free
			if (closeToTarget(playerPosX, _targetX) && _targetY == playerPosY) _actionDone = true;

			break;
		case HANGING:
			if (_hangTimer > 0)
			{
				_hangTimer -= 1;
				_goingRight ? orders.goRight = true : orders.goLeft = true;
			}
			else
			{
				_state = CLIMBING;
				orders.jump = true;
			}

			break;
		case CLIMBING:
			if (_climbTimer > 0)
			{
				_climbTimer -= 1;
				_goingRight ? orders.goRight = true : orders.goLeft = true;
			}
			break;
		default:
			break;
	}


	if (closeToTarget(playerPosX, _targetX) && _targetY == playerPosY) _actionDone = true;
	

	_previousPosX = playerPosX;
	_previousPosY = playerPosY;

	return orders;
}
