#include "stdafx.h"
#include "CentralizeAction.h"
#include "Utilities.h"

CentralizeAction::CentralizeAction(IBot* bot)
	: IMovementAction(bot)
{
	_actionType = CENTRALIZE;
	_actionDone = false;
}

//Centralize will issue commands to bot that will get him closer to the middle of the node he is currently standing on
//will return true if bot is centralized, otherwise false.
ordersStruct CentralizeAction::GetOrders()
{
	ordersStruct orders;

	double fractpart, intpart, distFromMiddle;
	fractpart = modf(_bot->GetPlayerPositionXNode(), &intpart);
	distFromMiddle = fractpart - 0.5;

	if (CloseToZero(abs(distFromMiddle)))
		_actionDone = true;
	else
	{
		if (distFromMiddle < 0) orders.goRight = true;
		else orders.goLeft = true;
		_actionDone = false;
	}

	return orders;
}
