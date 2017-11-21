#include "stdafx.h"
#include "IMovementAction.h"


IMovementAction::IMovementAction(IBot* bot)
{
	_bot = bot;
	_actionDone = false;
	_actionInProgress = false;
	_goingRight = false;
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


ordersStruct IMovementAction::GetOrders()
{
	return ordersStruct();
}

