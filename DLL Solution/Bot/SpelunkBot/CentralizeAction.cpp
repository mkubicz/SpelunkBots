#include "stdafx.h"
#include "CentralizeAction.h"
#include "Utilities.h"

CentralizeAction::CentralizeAction(std::shared_ptr<IBot> const& bot, int centralizingPoint)
	: IMovementAction(bot)
{
	_actionType = CENTRALIZE;
	_actionDone = false;
	_centralizingPoint = centralizingPoint;
	_standingStillCounter = 3;
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

	if (_standingStillCounter != 0)
		_standingStillCounter--;
	else
	{
		if (playerPosX > _centralizingPoint) orders.leftReleased = true;
		else if (playerPosX < _centralizingPoint) orders.rightReleased = true;
		else _actionDone = true;
	}

	return orders;
}
