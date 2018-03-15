#include "stdafx.h"
#include "JumpToLadderAction.h"

JumpToLadderAction::JumpToLadderAction(IBot * bot, int distX, int distY)
	: IMovementAction(bot)
{
	_distX = distX;
	_distY = distY;


	_distX < 0 ? _directionX = xLEFT : _directionX = xRIGHT;
	_distY < 0 ? _directionY = yUP : _directionY = yDOWN;

	_actionType = JUMP;

	//_microAdjXTimer = 2;

	if (abs(_distX) == 1)
		_state = JUMPING;
	else
		_state = WALKING;

	//set direction block timer
	/*if (abs(distX) == 1 && distY == -2)
		_directionBlockTimer = 3;
	else
		_directionBlockTimer = 0;*/

	//na razie dzia³a dla y= -2, -1, x= 1,2,3
	//jeszcze x=2..7 i y=0..5

	//set jump timer
	//if (abs(_distX) <= 2) _jumpTimer = 1;
	if (abs(_distX) <= 2) _jumpTimer = 3;
	if (abs(_distX) > 2 && abs(_distX) <= 5) _jumpTimer = 3;
	if (abs(_distX) > 5) _jumpTimer = 6;
	//if (_distY == -2 && abs(_distX) == 1) _jumpTimer += 6;
	if (_distY == -2) _jumpTimer += 6;


	//set running
	if (abs(_distX) > 3) _running = true;
	if (abs(_distX) == 3 && _distY == -2) _running = true;
}

ordersStruct JumpToLadderAction::GetOrders()
{
	ordersStruct orders;
	int playerPosX = (int)_bot->GetPlayerPositionX();
	int playerPosY = (int)_bot->GetPlayerPositionY();
	int playerPosXNode = (int)_bot->GetPlayerPositionXNode();
	int playerPosYNode = (int)_bot->GetPlayerPositionYNode();


	if (!_actionInProgress)
	{
		_targetNode = CalculateTargetNode(playerPosXNode, playerPosYNode, _distX, _distY);
		_startPosEdgeX = EdgeOfNodeX(playerPosXNode, _directionX);

		_actionInProgress = true;
	}


	if (_bot->GetSpelunkerState() == spCLIMBING) _state = CLIMBING;

	//if (_state != CLIMBING && closeToTarget(playerPosX, MiddleXPixel(_targetNode)) && WithinRangeFromTarget(playerPosY, MiddleYPixel(_targetNode), PIXELS_IN_NODE))
	//	orders.lookUp = true;


	//Grabbing the ledge
	//if (_state != CLIMBING &&
	//	WithinRangeFromTarget(playerPosX, MiddleXPixel(_targetNode), PIXELS_IN_NODE))
	//{
	//	if (_goingUp)
	//	{
	//		if (WithinRangeFromTarget(playerPosY, MiddleYPixel(_targetNode), PIXELS_IN_NODE))
	//			orders.lookUp = true;
	//	}
	//	else
	//	{
	//		//while falling down, the bot will try to grab the ladder even if he overshots the target
	//		if (WithinRangeFromTarget(playerPosY, MiddleYPixel(_targetNode), PIXELS_IN_NODE) ||
	//			playerPosY > MiddleYPixel(_targetNode))
	//			orders.lookUp = true;
	//	}
	//}
	if (_state != CLIMBING &&
		ShouldTryToGrabLadder(_targetNode))
		orders.lookUp = true;


 	switch (_state)
	{
	case JumpToLadderAction::WALKING:
		if (_running) orders.run = true;
		_directionX == xRIGHT ? orders.goRight = true : orders.goLeft = true;

		if (WentThrough(playerPosX, _startPosEdgeX, _directionX))
		{
			orders.jump = true;
			_state = JUMPING;
		}

		break;
	case JumpToLadderAction::JUMPING:
		//if (_bot->GetSpelunkerState() == spFALLING) _state = FALLING;
		//if (_bot->GetSpelunkerState() == spJUMPING) orders.jump = true;
		//orders.jump = true;

		if (_jumpTimer > 0)
		{
			orders.jump = true;
			_jumpTimer -= 1;
		}

		if (!closeToTargetFallToLadder(playerPosX, MiddleXPixel(_targetNode), _running, _distX, _distY))
			_directionX == xRIGHT ? orders.goRight = true : orders.goLeft = true;
		//mikropoprawki - chyba na razie je olaæ
		//else
		//{
		//	if (_microAdjXTimer == 0 || _microAdjXTimer == 1)// || _microAdjXTimer == 2)
		//	{
		//		if ((abs(playerPosX - MiddleXPixel(_targetNode)) >= 4))
		//		{
		//			if (playerPosX > MiddleXPixel(_targetNode))
		//				orders.goLeft = true;
		//			else
		//				orders.goRight = true;
		//		}
		//		
		//		if (_microAdjXTimer == 0) _microAdjXTimer = 2;
		//		else _microAdjXTimer -= 1;
		//	}
		//	else
		//	{
		//		_microAdjXTimer -= 1;
		//	}
		//}

		

		break;
	//case JumpToLadderAction::FALLING:
	//	if (!closeToTargetFall(playerPosX, MiddleXPixel(_targetNode), _running, _distY))
	//		_goingRight ? orders.goRight = true : orders.goLeft = true;
	//	break;
	case JumpToLadderAction::CLIMBING:
		if (playerPosY != MiddleYPixel(_targetNode))
		{
			if (playerPosY > MiddleYPixel(_targetNode))
				orders.lookUp = true;
			if (playerPosY < MiddleYPixel(_targetNode))
				orders.duck = true;
		}

		if (playerPosY == MiddleYPixel(_targetNode)) _actionDone = true;

		//prevents the bot from walking off the ladder accidentaly
		if (!_bot->IsNodePassable(_targetNode.GetX(), _targetNode.GetY() + 1, NODE_COORDS) && closeToTarget(playerPosY, MiddleYPixel(_targetNode)))
			_actionDone = true;

		break;
	default:
		break;
	}


	return orders;
}
