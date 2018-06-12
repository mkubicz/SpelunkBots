#include "stdafx.h"
#include "IMovementAction.h"


IMovementAction::IMovementAction(std::shared_ptr<IBot> const& bot)
	: _bot(bot)
{
	_actionDone = false;
	_actionInProgress = false;
	_directionX = DIRECTIONX::xNONE;
	_directionY = DIRECTIONY::yNONE;
	_targetNode = Coords(-1,-1);
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

Coords IMovementAction::GetTargetNode()
{
	return _targetNode;
}

void IMovementAction::CalculateTargetNode(int distXNode, int distYNode)
{
	_targetNode = Coords(_bot->GetPlayerPositionXNode() + distXNode, _bot->GetPlayerPositionYNode() + distYNode, NODE_ROUNDDOWN);
}

bool IMovementAction::ShouldTryToGrabLadder()
{
	int playerPosX = (int)_bot->GetPlayerPositionX();
	int playerPosY = (int)_bot->GetPlayerPositionY();

	if (WithinRangeFromTarget(playerPosX, _targetNode.GetMidXpixel(), PIXELS_IN_NODE))
	{
		if (_directionY == yUP)
		{
			if (WithinRangeFromTarget(playerPosY, _targetNode.GetMidYpixel(), PIXELS_IN_NODE))
				return true;
		}
		else
		{
			//while falling down, the bot will try to grab the ladder even if he overshoots the target
			if (WithinRangeFromTarget(playerPosY, _targetNode.GetMidYpixel(), PIXELS_IN_NODE) ||
				playerPosY > _targetNode.GetMidYpixel())
				return true;
		}
	}

	return false;
}

bool IMovementAction::IsNearLadderTop()
{
	int playerNodeX = (int)_bot->GetPlayerPositionXNode();
	int playerNodeY = (int)_bot->GetPlayerPositionYNode();

	if (_bot->GetNodeState(playerNodeX, playerNodeY, NODE_COORDS) != spLadderTop &&
		_bot->GetNodeState(playerNodeX, playerNodeY + 1, NODE_COORDS) == spLadderTop)
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


