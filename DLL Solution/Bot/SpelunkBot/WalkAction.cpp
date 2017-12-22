#include "stdafx.h"
#include "WalkAction.h"
#include "Utilities.h"

WalkAction::WalkAction(IBot *bot, bool goingRight, double distance, bool usingPixelCoords)
	: WalkAction(bot, goingRight, false, distance, usingPixelCoords)
{}

WalkAction::WalkAction(IBot *bot, bool goingRight, bool pressRun, double distance, bool usingPixelCoords)
	: IMovementAction(bot)
{
	if (goingRight) 
		_actionType = WALKRIGHT; //do I need this?
	else
		_actionType = WALKLEFT;

	_actionDone = false;
	_actionInProgress = false;
	_usingPixelCoords = usingPixelCoords;
	_goingRight = goingRight;
	_pressRun = pressRun;
	_distance = distance;
}

ordersStruct WalkAction::GetOrders()
{
	//if first time getting orders - set target to walk to
	if (!_actionInProgress)
	{
		if (_usingPixelCoords) _distance = ConvertToNodeCoordinates(_distance);
		if (!_goingRight) _distance = -_distance;

		int nodenr = (int)_bot->GetPlayerPositionXNode();

		//target calculated from center of current node
		_targetX = (nodenr + _distance) * PIXELS_IN_NODE + (PIXELS_IN_NODE / 2);

		_actionInProgress = true;
	}

	ordersStruct orders;

	if (closeToTarget(_bot->GetPlayerPositionX(), _targetX)) //TODO if running, close to target function should be different
	{
		_goingRight ? orders.goRight = false : orders.goLeft = false;
		_actionDone = true;
	}
	else
	{
		_goingRight ? orders.goRight = true : orders.goLeft = true;
		_actionDone = false;
	}


	orders.run = _pressRun;

	return orders;
}


/*
ordersStruct WalkAction::GetOrders()
{
	//if first time getting orders - set target to walk to
	if (!_actionInProgress)
	{
		if (!_goingRight) _distance = -_distance;

		if (_usingPixelCoords)
		{
			_targetPositionXNode = _bot->GetPlayerPositionX() + _distance;
			ConvertToNodeCoordinates(_targetPositionXNode); //TODO test
		}
		else
		{
			_targetPositionXNode = _bot->GetPlayerPositionXNode() + _distance;
		}
		_actionInProgress = true;
	}

	ordersStruct orders;
	
	if (CloseToZero(abs(_bot->GetPlayerPositionXNode() - _targetPositionXNode)))
	//if (abs(_bot->GetPlayerPositionXNode() - _targetPositionXNode) == 0.3)
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
*/