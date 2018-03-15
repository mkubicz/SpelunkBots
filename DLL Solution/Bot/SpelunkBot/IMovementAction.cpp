#include "stdafx.h"
#include "IMovementAction.h"


IMovementAction::IMovementAction(IBot* bot)
{
	_bot = bot;
	_actionDone = false;
	_actionInProgress = false;
	_directionX = DIRECTIONX::xNONE;
	_directionY = DIRECTIONY::yNONE;
	_targetX = bot->GetPlayerPositionX();
	_targetY = bot->GetPlayerPositionY();
}

IMovementAction::~IMovementAction()
{
}

MOVEMENTACTION IMovementAction::ActionType()
{
	return _actionType;
}

bool IMovementAction::ActionDone()
{
	return _actionDone;
}

DIRECTIONX IMovementAction::GetDirectionX()
{
	return _directionX;
}

DIRECTIONY IMovementAction::GetDirectionY()
{
	return _directionY;
}

//bool IMovementAction::IsRunning()
//{
//	return _running;
//}


bool IMovementAction::ShouldTryToGrabLadder(Node target)
{
	int playerPosX = (int)_bot->GetPlayerPositionX();
	int playerPosY = (int)_bot->GetPlayerPositionY();

	if (WithinRangeFromTarget(playerPosX, MiddleXPixel(target), PIXELS_IN_NODE))
	{
		if (_directionY == yUP)
		{
			if (WithinRangeFromTarget(playerPosY, MiddleYPixel(target), PIXELS_IN_NODE))
				return true;
		}
		else
		{
			//while falling down, the bot will try to grab the ladder even if he overshoots the target
			if (WithinRangeFromTarget(playerPosY, MiddleYPixel(target), PIXELS_IN_NODE) ||
				playerPosY > MiddleYPixel(target))
				return true;
		}
	}

	return false;
}


ordersStruct IMovementAction::GetOrders()
{
	return ordersStruct();
}

