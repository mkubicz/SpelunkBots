#include "stdafx.h"
#include "WalkAction.h"
#include "Utilities.h"

WalkAction::WalkAction(std::shared_ptr<IBot> const& bot, int distance)
	: WalkAction(bot, distance, false)
{
}

WalkAction::WalkAction(std::shared_ptr<IBot> const& bot, int distance, bool run)
	: IMovementAction(bot)
{
	_actionType = WALK;
	_running = run;
	_distance = distance;

	if (_distance > 0) _directionX = xRIGHT;
	if (_distance < 0) _directionX = xLEFT;
}

void WalkAction::AddDistance(int distance)
{
	_distance += distance;
	_targetNode = Coords(_targetNode.GetX() + distance, _targetNode.GetY());
}

ordersStruct WalkAction::GetOrders()
{
	ordersStruct orders;

	//if first time getting orders - set target to walk to
	if (!_actionInProgress)
	{
		int nodenr = (int)_bot->GetPlayerPositionXNode();

		CalculateTargetNode(_distance, 0);

		_actionInProgress = true;
	}

	orders.run = _running;

	_directionX == xRIGHT ? orders.goRight = true : orders.goLeft = true;

	if (closeToTarget(_bot->GetPlayerPositionX(), _targetNode.GetMidXpixel()))
		_actionDone = true;


	return orders;
}
