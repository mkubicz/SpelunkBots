#include "stdafx.h"
#include "WalkOffLedgeAction.h"


WalkOffLedgeAction::WalkOffLedgeAction(IBot * bot, int distX, int distY)
	: IMovementAction(bot)
{
	_distX = distX;
	_distY = distY;

	if (_distX < 0) _directionX = xLEFT;
	else if (_distX == 0) _directionX = xNONE;
	else _directionX = xRIGHT;

	if (_distY < 0) _directionY = yUP;
	else if (_distY == 0) _directionY = yNONE;
	else _directionY = yDOWN;

	_actionType = WALKOFFLEDGE;

	_running = false;
	if (abs(distX) == 3 && distY == 1) _running = true;
	if (abs(distX) > 3) _running = true;

	_state = WALKING;
}


ordersStruct WalkOffLedgeAction::GetOrders()
{
	int distToTargetY;
	int playerPosX = (int)_bot->GetPlayerPositionX();
	int playerPosY = (int)_bot->GetPlayerPositionY();
	ordersStruct orders;

	if (!_actionInProgress)
	{
		int nodenr = (int)_bot->GetPlayerPositionXNode();

		_targetX = (nodenr + _distX) * PIXELS_IN_NODE + (PIXELS_IN_NODE / 2);
		_targetY = playerPosY + (_distY * PIXELS_IN_NODE);
		//_targetXSide = _goingRight ? (nodenr + _distX) * PIXELS_IN_NODE : (nodenr + 1 + _distX) * PIXELS_IN_NODE;
		_startPosXside = _directionX == xRIGHT ? (nodenr * PIXELS_IN_NODE) + PIXELS_IN_NODE : (nodenr * PIXELS_IN_NODE);
		//_startPosXmid = (nodenr * PIXELS_IN_NODE) + (PIXELS_IN_NODE / 2);

		_actionInProgress = true;
	}

	if (_running) orders.run = true;

	switch (_state)
	{
		case WALKING:
			_directionX == xRIGHT ? orders.goRight = true : orders.goLeft = true;

			if (WentThrough(playerPosX, _startPosXside, _directionX))
				_state = FALLING;

			break;
		case FALLING:
			_directionX == xRIGHT ? orders.goRight = true : orders.goLeft = true;

			if (closeToTargetFall(playerPosX, _targetX, _running, _distY))
				_state = FREEFALLING;

			break;
		case FREEFALLING:
			if (_targetY == playerPosY) _state = LANDED;

			break;
		case LANDED:
			if (closeToTarget(playerPosX, _targetX)) _actionDone = true;

			if (playerPosX > _targetX)
				orders.goLeft = true;
			if (playerPosX < _targetX)
				orders.goRight = true;

			break;
	}


	return orders;
}