#include "stdafx.h"
#include "WalkAction.h"
#include "Utilities.h"

//WalkAction::WalkAction(IBot *bot, bool goingRight, double distance, bool usingPixelCoords)
//	: WalkAction(bot, goingRight, false, distance, usingPixelCoords)
//{}
//
//WalkAction::WalkAction(IBot *bot, bool goingRight, bool pressRun, double distance, bool usingPixelCoords)
//	: IMovementAction(bot)
//{
//	if (goingRight) 
//		_actionType = WALKRIGHT;
//	else
//		_actionType = WALKLEFT;
//
//
//	_actionDone = false;
//	_actionInProgress = false;
//	_usingPixelCoords = usingPixelCoords;
//	_goingRight = goingRight;
//	_running = pressRun;
//	_distance = distance;
//}

WalkAction::WalkAction(IBot * bot, int distance)
	: WalkAction(bot, distance, false)
{
}

WalkAction::WalkAction(IBot * bot, int distance, bool run)
	: IMovementAction(bot)
{
	_actionType = WALK;
	_running = run;
	_distance = distance;

	if (_distance > 0) _directionX = xRIGHT;
	if (_distance < 0) _directionX = xLEFT;
	

	//_goingRight = _distance > 0 ? true : false;

}

void WalkAction::AddDistance(int distance)
{
	_distance += distance;
}

ordersStruct WalkAction::GetOrders()
{
	ordersStruct orders;

	//if first time getting orders - set target to walk to
	if (!_actionInProgress)
	{
		int nodenr = (int)_bot->GetPlayerPositionXNode();

		//target calculated from center of current node
		_targetX = (nodenr + _distance) * PIXELS_IN_NODE + (PIXELS_IN_NODE / 2);

		_actionInProgress = true;
	}

	orders.run = _running;

	_directionX == xRIGHT ? orders.goRight = true : orders.goLeft = true;

	//_goingRight ? orders.goRight = true : orders.goLeft = true;

	if (closeToTarget(_bot->GetPlayerPositionX(), _targetX))
		_actionDone = true;


	return orders;
}
