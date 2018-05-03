#include "stdafx.h"
#include "JumpFromLadderAction.h"

//negative distX means going left! negative distY means going up!
JumpFromLadderAction::JumpFromLadderAction(IBot * bot, ACTION_TARGET target, bool hasMomentum, int distX, int distY)
	: IMovementAction(bot)
{
	_distX = distX;
	_distY = distY;
	_actionTarget = target;
	_hasMomentum = hasMomentum;
	_dirDelayTimer = 0;

	_actionType = JUMPFROMLADDER;

	_state = JUMPING;

	if (_distX < 0) _directionX = xLEFT;
	else if (_distX == 0) _directionX = xNONE;
	else _directionX = xRIGHT;

	if (_distY < 0) _directionY = yUP;
	else if (_distY == 0) _directionY = yNONE;
	else _directionY = yDOWN;

	SetTimers();
	SetMovementRange();
}

JumpFromLadderAction::JumpFromLadderAction(IBot * bot, int distX, int distY)
	: JumpFromLadderAction(bot, GROUND, false, distX, distY)
{
}


void JumpFromLadderAction::SetTimers()
{
	_jumpTimer = 3;

	if (abs(_distX) >= 2)
		_jumpTimer = 5;

	if (_actionTarget == LEDGE || _actionTarget == LADDER)
		_jumpTimer += 3;

	//fresh change
	if (abs(_distX) == 1 && _distY == 0)
		_jumpTimer = 1;

	if (_actionTarget == LEDGE && abs(_distX == 0) && _distY == -1)
		_jumpTimer = 4;
	if (_actionTarget == LEDGE && abs(_distX == 1) && _distY == -1)
		_jumpTimer = 3;

	if (abs(_distX) == 5 && _distY == 0)
		_jumpTimer = 15;

	if (_actionTarget == LEDGE && abs(_distX == 4) && _distY == 0)
		_jumpTimer = 6;

	if (_actionTarget == LEDGE && abs(_distX == 0) && _distY == -1)
		_dirDelayTimer = 3;

	if (_hasMomentum)
	{
		if (abs(_distX == 2) && _distY != -1)
			_dirDelayTimer = 3;

		if (abs(_distX == 4) && (_distY == 3 || _distY == 2))
			_dirDelayTimer = 3;
	}


}


void JumpFromLadderAction::SetMovementRange()
{
	if (_hasMomentum)
	{
		switch (_distY)
		{
		case -1:
			if (abs(_distX) == 1) _moveRange = 12;
			else if (abs(_distX) == 2) _moveRange = 12;
			else _moveRange = 16;
			break;
		case 0:
			if (abs(_distX) == 1) _moveRange = 12;
			else if (abs(_distX) == 5) _moveRange = 16;
			else _moveRange = 20;
			break;
		case 3:
			if (abs(_distX) == 1) _moveRange = 12;
			else if (abs(_distX) == 2) _moveRange = 21;
			else if (abs(_distX) == 5) _moveRange = 22;
			else _moveRange = 20;
			break;
		case 4:
			if (abs(_distX) == 1) _moveRange = 12;
			else if (abs(_distX) == 2) _moveRange = 21;
			else if (abs(_distX) == 4) _moveRange = 24;
			else if (abs(_distX) == 5) _moveRange = 22;
			else _moveRange = 20;
			break;
		case 5:
			if (abs(_distX) == 1) _moveRange = 12;
			else if (abs(_distX) == 2) _moveRange = 21;
			else if (abs(_distX) == 4) _moveRange = 26;
			else if (abs(_distX) == 5) _moveRange = 24;
			else _moveRange = 20;
			break;
		default:
			if (abs(_distX) == 1) _moveRange = 12;
			else if (abs(_distX) == 5) _moveRange = 16;
			else _moveRange = 20;
			break;
		}
	}
	else
	{
		switch (abs(_distX))
		{
		case 0:
			_moveRange = 6;
			break;
		case 1:
			_moveRange = 9;
			break;
		case 2:
			_moveRange = 6;
			if (_distY == 1)
				_moveRange = 9;
			if (_distY == 2 || _distY == 4 || _distY == 5)
				_moveRange = 12;
			break;
		case 3:
			_moveRange = 6;
			if (_distY == 3)
				_moveRange = 9;
			if (_distY == 4 || _distY == 5)
				_moveRange = 12;
			break;
		}
	}
}


void JumpFromLadderAction::MoveToTarget(ordersStruct *orders)
{
	int playerPosX = (int)_bot->GetPlayerPositionX();

	if (!WithinRangeFromTarget(playerPosX, MiddleXPixel(_targetNode), _moveRange)
		|| _actionTarget == LEDGE)
	{
		if (_directionX == xRIGHT) orders->goRight = true;
		if (_directionX == xLEFT) orders->goLeft = true;
	}
}

ordersStruct JumpFromLadderAction::GetOrders()
{
	ordersStruct orders;
	int playerPosX = (int)_bot->GetPlayerPositionX();
	int playerPosY = (int)_bot->GetPlayerPositionY();

	if (!_actionInProgress)
	{
		_targetNode = CalculateTargetNode(_distX, _distY);

		//makes the bot grab the ledge even when distX=0
		//priority - right
		if (_distX == 0 && _actionTarget == LEDGE)
		{
			if (!_bot->IsNodePassable(_targetNode.GetX() + 1, _targetNode.GetY(), NODE_COORDS))
				_directionX = xRIGHT;
			else if (!_bot->IsNodePassable(_targetNode.GetX() - 1, _targetNode.GetY(), NODE_COORDS))
				_directionX = xLEFT;
		}

		_actionInProgress = true;
	}


	//grabbing ladders
	if (_state != CLIMBING && 
		_actionTarget == LADDER &&
		ShouldTryToGrabLadder(_targetNode))
		orders.lookUp = true;

	//flying through hard ladder tops
	if ((_state == JUMPING || _state == FALLING) &&
		IsNearLadderTop(playerPosX, playerPosY))
		orders.duck = true;


	switch (_state)
	{
	case JUMPING:
		
		if (_dirDelayTimer == 0)
			MoveToTarget(&orders);
		else
			_dirDelayTimer -= 1;

		if (_jumpTimer > 0)
		{
			orders.jump = true;
			_jumpTimer -= 1;

		}
		else
		{
			_state = FALLING;
		}

		break;
	case FALLING:

		if (_dirDelayTimer == 0)
			MoveToTarget(&orders);
		else
			_dirDelayTimer -= 1;
		

		if (_bot->GetSpelunkerState() == spCLIMBING)
			_state = CLIMBING;

		break;
	case CLIMBING:
		if (playerPosY != MiddleYPixel(_targetNode))
		{
			if (playerPosY > MiddleYPixel(_targetNode))
				orders.lookUp = true;
			if (playerPosY < MiddleYPixel(_targetNode))
				orders.duck = true;
		}
		
		//prevents the bot from walking off the ladder accidentally
		if (!_bot->IsNodePassable(_targetNode.GetX(), _targetNode.GetY() + 1, NODE_COORDS) && closeToTarget(playerPosY, MiddleYPixel(_targetNode)))
			_actionDone = true;

		break;
	case HANGING:
		break;
	case FINISHED:
		_finishedTimer += 1;
		if (_finishedTimer == 5)
			_actionDone = true;
		break;
	}


	switch (_actionTarget)
	{
	case GROUND:
		if (_bot->GetSpelunkerState() == spSTANDING && closeToTarget(playerPosX, MiddleXPixel(_targetNode)))
			_actionDone = true;
			//_state = FINISHED;
		break;
	case LADDER:
		if (_bot->GetSpelunkerState() == spCLIMBING && playerPosY == MiddleYPixel(_targetNode) && playerPosX == MiddleXPixel(_targetNode))
			_actionDone = true;
			//_state = FINISHED;
		break;
	case LEDGE:
		if (_bot->GetSpelunkerState() == spHANGING && playerPosY == MiddleYPixel(_targetNode))
			//_actionDone = true;
			_state = FINISHED;
		break;
	}


	return orders;
}
