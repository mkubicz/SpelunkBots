#include "stdafx.h"
#include "DropAction.h"

DropAction::DropAction(IBot* bot, ACTION_TARGET actionTarget, int distY)
	: IMovementAction(bot)
{
	_actionTarget = actionTarget;
	_distY = distY;

	_state = DROPPING;

	if (_actionTarget == LEDGE)
		std::cout << "ERROR: ActionTarget for DropAction should not be LEDGE";
}

DropAction::DropAction(IBot* bot, int distY)
	: DropAction(bot, GROUND, distY)
{
}

ordersStruct DropAction::GetOrders()
{
	ordersStruct orders;
	int playerPosX = (int)_bot->GetPlayerPositionX();
	int playerPosY = (int)_bot->GetPlayerPositionY();

	if (!_actionInProgress)
	{
		CalculateTargetNode(0, _distY);
		_actionInProgress = true;
	}


	switch (_state)
	{
	case DROPPING:
		if (_bot->GetSpelunkerState() == spCLIMBING || _bot->GetSpelunkerState() == spHANGING)
		{
			orders.duck = true;
			orders.jump = true;

			//if you are right above ground don't jump; instead walk off
			if (!_bot->IsNodePassable(_bot->GetPlayerPositionXNode(), _bot->GetPlayerPositionYNode() + 1, NODE_COORDS))
				orders.jump = false;

			_state = FALLING;
		}
		else if (_bot->GetSpelunkerState() == spSTANDING && _bot->GetNodeState(playerPosX, playerPosY + 16, PIXEL_COORDS) == spLadderTop)
		{
			orders.duck = true;
			_state = FALLING;
		}
		break;
	case FALLING:
		//grabbing ladders
		if (_actionTarget == LADDER &&
			WithinRangeFromTarget(playerPosY, _targetNode.GetMidYpixel(), 8))
			orders.lookUp = true;
		
		if (_bot->GetSpelunkerState() == spCLIMBING)
			_state = CLIMBING;
		break;
	case CLIMBING:
		if (playerPosY != _targetNode.GetMidYpixel())
		{
			if (playerPosY > _targetNode.GetMidYpixel())
				orders.lookUp = true;
			if (playerPosY < _targetNode.GetMidYpixel())
				orders.duck = true;
		}

		//prevents the bot from walking off the ladder accidentally
		if (!_bot->IsNodePassable(_targetNode.GetX(), _targetNode.GetY() + 1, NODE_COORDS) && closeToTarget(playerPosY, _targetNode.GetMidYpixel()))
			_actionDone = true;
	}



	switch (_actionTarget)
	{
	case GROUND:
		if ((_bot->GetSpelunkerState() == spSTANDING || _bot->GetSpelunkerState() == spDUCKING) && playerPosY == _targetNode.GetMidYpixel())
			_actionDone = true;
		break;
	case LADDER:
		if (_bot->GetSpelunkerState() == spCLIMBING && playerPosY == _targetNode.GetMidYpixel())
			_actionDone = true;
		break;
	case LEDGE:
		//this shouldn't happen
		break;
	}



	return orders;
}
