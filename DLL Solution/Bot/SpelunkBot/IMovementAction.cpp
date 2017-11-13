#include "stdafx.h"
#include "IMovementAction.h"


IMovementAction::IMovementAction(IBot* bot)
{
	_bot = bot;
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

