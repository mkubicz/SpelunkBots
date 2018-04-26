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

bool IMovementAction::IsNearLadderTop(int playerPosX, int playerPosY)
{
	int playerNodeX = (int)_bot->GetPlayerPositionXNode();
	int playerNodeY = (int)_bot->GetPlayerPositionYNode();

	if (_bot->GetNodeState(playerNodeX, playerNodeY, NODE_COORDS) != spLadder &&
		_bot->GetNodeState(playerNodeX, playerNodeY + 1, NODE_COORDS) == spLadder)
		return true;

	return false;
}


void IMovementAction::Centralize(ordersStruct * orders, int centralizingPoint)
{
	int playerPosX = (int)_bot->GetPlayerPositionX();

	if (_centralizeBreakTimer == 0 && _centralizeMoveTimer == 0)
	{
		if (playerPosX < centralizingPoint)
			_centralizeDir = xRIGHT;
		if (playerPosX > centralizingPoint)
			_centralizeDir = xLEFT;

		_centralizeMoveTimer = 3;

		return;
	}


	if (_centralizeMoveTimer != 0)
	{
		if (_centralizeDir == xRIGHT)
			orders->goRight = true;
		else
			orders->goLeft = true;

		_centralizeMoveTimer--;

		if (_centralizeMoveTimer == 0)
		{
			//if (playerPosX == _startNodeCenter)
			if (WithinRangeFromTarget(playerPosX, centralizingPoint, 1))
				_centralizeBreakTimer = 6;
			else
				_centralizeBreakTimer = 2;
		}

		return;
	}

	if (_centralizeBreakTimer != 0)
	{
		_centralizeBreakTimer--;
		return;
	}
}


bool IMovementAction::IsStandingStill(int playerPosX, int playerPosY, int prevPlayerPosX, int prevPlayerPosY)
{
	if (playerPosX == prevPlayerPosX && playerPosY == prevPlayerPosY)
		_standingStillCounter += 1;
	else
		_standingStillCounter = 0;

	if (_standingStillCounter >= 3)
		return true;

	return false;
}

ordersStruct IMovementAction::GetOrders()
{
	return ordersStruct();
}

