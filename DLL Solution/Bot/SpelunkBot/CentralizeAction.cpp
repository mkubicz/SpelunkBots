#include "stdafx.h"
#include "CentralizeAction.h"
#include "Utilities.h"

CentralizeAction::CentralizeAction(std::shared_ptr<IBot> const& bot, int centralizingPoint)
	: IMovementAction(bot)
{
	_actionType = CENTRALIZE;
	_actionDone = false;
	_centralizingPoint = centralizingPoint;
}

CentralizeAction::CentralizeAction(std::shared_ptr<IBot> const& bot)
	: CentralizeAction(bot, -1)
{
}

ordersStruct CentralizeAction::GetOrders()
{
	ordersStruct orders;
	int playerPosX = (int)_bot->GetPlayerPositionX();

	if (!_actionInProgress)
	{
		if (_centralizingPoint == -1)
		{
			Coords startNode = Coords(_bot->GetPlayerPositionXNode(), _bot->GetPlayerPositionYNode(), NODE_ROUNDDOWN);
			_centralizingPoint = startNode.GetMidXpixel();
		}

		int distXNode = abs(ConvertToNodeCoordinates(_centralizingPoint) - (int)_bot->GetPlayerPositionXNode());

		CalculateTargetNode(distXNode, 0);

		_actionInProgress = true;
	}



	if (_centralizeBreakTimer == 0 && _centralizeMoveTimer == 0)
	{
		if (playerPosX < _centralizingPoint)
			_centralizeDir = xRIGHT;
		if (playerPosX > _centralizingPoint)
			_centralizeDir = xLEFT;

		_centralizeMoveTimer = 3;

		return orders;
	}


	if (_centralizeMoveTimer != 0)
	{
		if (_centralizeDir == xRIGHT)
			orders.goRight = true;
		else
			orders.goLeft = true;

		_centralizeMoveTimer--;

		if (_centralizeMoveTimer == 0)
		{
			if (WithinRangeFromTarget(playerPosX, _centralizingPoint, 1))
				_centralizeBreakTimer = 6;
			else
				_centralizeBreakTimer = 2;
		}

		return orders;
	}

	if (_centralizeBreakTimer != 0)
	{
		_centralizeBreakTimer--;
		return orders;
	}

	return orders;
}
