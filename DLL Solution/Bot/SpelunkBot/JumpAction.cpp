#include "stdafx.h"
#include "JumpAction.h"

JumpAction::JumpAction(IBot * bot, bool goingRight, bool goingUp, JUMP_TARGET target, int distX, int distY)
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
	_jumpTarget = target;

	SetActionType();

	//if target is a ledge, bot has to jump a bit further than normally, thus we increase distances 
	//(simpler than making a specialized case for those jumps)
	if (_jumpTarget == LEDGE)
	{
		_distY += 1;
		_distX += 1;
	}


	_smljmp_backedUp = false;
	if (_distX == 1)
		_state = SMALLJUMP;
	else
		_state = WALKING;

	_running = abs(_distX) > 3 && !(abs(_distX) == 4 && _distY == 0);

	if (_state == SMALLJUMP)
	{
		if (_distY == 2) _jumpTimer = 3;
		else _jumpTimer = 1;
	}
	else
	{
		if (_distX <= 2) _jumpTimer = 1;
		if (_distX > 2 && _distX <= 5) _jumpTimer = 3;
		if (_distX > 5) _jumpTimer = 6;
	}



}

JumpAction::JumpAction(IBot * bot, bool goingRight, bool goingUp, int distX, int distY)
	: JumpAction(bot, goingRight, goingUp, GROUND, distX, distY)
{
}


void JumpAction::SetActionType()
{
	if (_goingRight)
		if (_goingUp)
			_actionType = JUMPUPRIGHT;
		else
			_actionType = JUMPDOWNRIGHT;
	else
		if (_goingUp)
			_actionType = JUMPUPLEFT;
		else
			_actionType = JUMPDOWNLEFT;
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
		_startPosXmid = (nodenr * PIXELS_IN_NODE) + (PIXELS_IN_NODE / 2);

		_actionInProgress = true;
	}


	if (_jumpTarget == LADDER) orders.lookUp = true;


	distToTargetY = ConvertToNodeCoordinates(abs(playerPosY - _targetY));

	switch (_state)
	{
		case SMALLJUMP:
			//only when x=1
			
			//back up a little bit
			if (!_smljmp_backedUp)
			{
				(!_goingRight) ? orders.goRight = true : orders.goLeft = true;
				if (WentThrough(!_goingRight, playerPosX, _startPosXmid))
				{
					_smljmp_backedUp = true;
				}
			}
			else
			{
				orders.jump = true;
				_state = JUMPING;
				//orders.jump = true;
				//_goingRight ? orders.goRight = true : orders.goLeft = true;
			}

			break;
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
		default:
			break;
	}


	if ((closeToTarget(playerPosX, _targetX) && _targetY == playerPosY) || _bot->GetSpelunkerState() == spHANGING) _actionDone = true;

	if (_jumpTarget == LADDER && abs(playerPosX - _targetX) <= 2 && (_targetY-8 < playerPosY && _targetY + 24 > playerPosY)) 
		_actionDone = true;
	

	_previousPosX = playerPosX;
	_previousPosY = playerPosY;

	return orders;
}
