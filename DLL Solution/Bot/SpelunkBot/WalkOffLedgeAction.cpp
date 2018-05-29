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
	if (abs(_distX) == 3 && _distY == 1) _running = true;
	if (abs(_distX) > 3) _running = true;

	_state = WALKING;
}

bool WalkOffLedgeAction::closeToTargetFall(int playerPositionX, int targetPositionX, bool running, int distY)
{
	if (distY <= 0) //jumpup or jumpflat
	{
		if (running) return abs(playerPositionX - targetPositionX) <= PIXELS_IN_NODE * 1.5;
		else return closeToTarget(playerPositionX, targetPositionX);
	}
	else
	{ //jumpdown
		if (running) return abs(playerPositionX - targetPositionX) <= PIXELS_IN_NODE * 2;
		else return abs(playerPositionX - targetPositionX) <= 12;
	}

}

ordersStruct WalkOffLedgeAction::GetOrders()
{
	int distToTargetY;
	int playerPosX = (int)_bot->GetPlayerPositionX();
	int playerPosY = (int)_bot->GetPlayerPositionY();
	ordersStruct orders;

	if (!_actionInProgress)
	{
		CalculateTargetNode(_distX, _distY);

		int nodenr = (int)_bot->GetPlayerPositionXNode();
		_startPosXside = _directionX == xRIGHT ? (nodenr * PIXELS_IN_NODE) + PIXELS_IN_NODE : (nodenr * PIXELS_IN_NODE);

		_actionInProgress = true;
	}

	if (_running) orders.run = true;

	//flying through hard ladder tops
	if ((_state == FREEFALLING || _state == FALLING) &&
		IsNearLadderTop())
		orders.duck = true;


	switch (_state)
	{
		case WALKING:
			_directionX == xRIGHT ? orders.goRight = true : orders.goLeft = true;

			if (WentThrough(playerPosX, _startPosXside, _directionX))
				_state = FALLING;

			break;
		case FALLING:
			_directionX == xRIGHT ? orders.goRight = true : orders.goLeft = true;

			if (closeToTargetFall(playerPosX, _targetNode.GetMidXpixel(), _running, _distY))
				_state = FREEFALLING;

			break;
		case FREEFALLING:
			if (_targetNode.GetMidYpixel() == playerPosY) _state = LANDED;

			break;
		case LANDED:
			if (closeToTarget(playerPosX, _targetNode.GetMidXpixel())) _actionDone = true;

			if (playerPosX > _targetNode.GetMidXpixel())
				orders.goLeft = true;
			if (playerPosX < _targetNode.GetMidXpixel())
				orders.goRight = true;

			break;
	}


	return orders;
}