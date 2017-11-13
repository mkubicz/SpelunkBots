#include "stdafx.h"
#include "WalkAction.h"
#include "Utilities.h"

WalkAction::WalkAction(IBot *bot, bool goingRight, double distance, bool usingPixelCoords)
	: IMovementAction(bot)
{
	_actionType = WALKLEFT;
	_actionDone = false;
	_actionInProgress = false;
	_usingPixelCoords = usingPixelCoords;
	_goingRight = goingRight;
	_distance = distance;
}

ordersStruct WalkAction::GetOrders()
{
	//if first time getting orders - set target to walk to
	if (!_actionInProgress)
	{
		if (!_goingRight) _distance = -_distance;

		if (_usingPixelCoords)
		{
			_targetPositionXNode = _bot->GetPlayerPositionX() + _distance;
			ConvertToNodeCoordinates(_targetPositionXNode);
		}
		else
		{
			_targetPositionXNode = _bot->GetPlayerPositionXNode() + _distance;
		}
		_actionInProgress = true;
	}

	ordersStruct orders;

	if (CloseToZero(abs(_bot->GetPlayerPositionXNode() - _targetPositionXNode)))
	{
		_goingRight ? orders.goRight = false : orders.goLeft = false;
		_actionDone = true;
	}
	else
	{
		_goingRight ? orders.goRight = true : orders.goLeft = true;
		_actionDone = false;
	}

	return orders;
}