#include "stdafx.h"
#include "WalkOffLedgeAction.h"


WalkOffLedgeAction::WalkOffLedgeAction(IBot * bot, bool goingRight, int distX, int distY)
	: IMovementAction(bot)
{
	_goingRight = goingRight;
	_distX = distX;
	_distY = distY;

	if (_goingRight)
		_actionType = WALKOFFLEDGERIGHT;
	else
		_actionType = WALKOFFLEDGELEFT;

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
		if (!_goingRight) _distX = -_distX;

		_targetX = (nodenr + _distX) * PIXELS_IN_NODE + (PIXELS_IN_NODE / 2);
		_targetY = playerPosY + (_distY * PIXELS_IN_NODE);
		//_targetXSide = _goingRight ? (nodenr + _distX) * PIXELS_IN_NODE : (nodenr + 1 + _distX) * PIXELS_IN_NODE;
		_startPosXside = _goingRight ? (nodenr * PIXELS_IN_NODE) + PIXELS_IN_NODE : (nodenr * PIXELS_IN_NODE);
		//_startPosXmid = (nodenr * PIXELS_IN_NODE) + (PIXELS_IN_NODE / 2);

		_actionInProgress = true;
	}

	if (_running) orders.run = true;

	switch (_state)
	{
		case WALKING:
			_goingRight ? orders.goRight = true : orders.goLeft = true;

			if (WentThrough(_goingRight, playerPosX, _startPosXside))
				_state = FALLING;

			break;
		case FALLING:
			_goingRight ? orders.goRight = true : orders.goLeft = true;

			if (closeToTargetFall(playerPosX, _targetX, _running, _distY))
				_state = FREEFALLING;

			break;
		case FREEFALLING:
			if (_targetY == playerPosY) _state = LANDED;

			break;
		case LANDED:
			if (closeToTarget(playerPosX, _targetX)) _actionDone = true;

			break;
	}


	return orders;
}