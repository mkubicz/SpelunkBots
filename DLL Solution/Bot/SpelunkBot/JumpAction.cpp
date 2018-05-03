#include "stdafx.h"
#include "JumpAction.h"

JumpAction::JumpAction(IBot * bot, ACTION_TARGET target, int distX, int distY)
	: IMovementAction(bot)
{
	_distX = distX;
	_distY = distY;
	_actionTarget = target;

	_actionType = JUMP;

	//state is set in first call of GetOrders

	if (_distX < 0) _directionX = xLEFT;
	else if (_distX == 0) _directionX = xNONE;
	else _directionX = xRIGHT;

	if (_distY < 0) _directionY = yUP;
	else if (_distY == 0) _directionY = yNONE;
	else _directionY = yDOWN;

	SetRunning();
	SetTimers();
	SetMovementRange();
}

JumpAction::JumpAction(IBot * bot, int distX, int distY)
	: JumpAction(bot, GROUND, distX, distY)
{
}

void JumpAction::SetRunning()
{
	if (IsJumpWithRunning(_distX, _distY, _actionTarget))
		_running = true;
	else
		_running = false;
}

void JumpAction::SetTimers()
{
	//general jumpTimer values
	_jumpTimer = 3;

	if (abs(_distX) >= 2)
		_jumpTimer = 5;

	if (_actionTarget == LEDGE || _actionTarget == LADDER)
		_jumpTimer += 3;

	if (_directionX == xNONE && _directionY == yUP)
	{
		if (_distY == -1) _jumpTimer = 1;
		else _jumpTimer = 3;
	}

	//exceptions from general values
	if (_actionTarget == LEDGE)
	{
		if (abs(_distX) == 0 && _distY == -1)
			_jumpTimer = 3;

		if (abs(_distX) == 1 && _distY == -1)
			_jumpTimer = 2;
	}
	else
	{
		switch (_distY)
		{
		case -1:
			if (abs(_distX) == 1 || abs(_distX) == 2)
				_jumpTimer = 1;
			break;
		case 0:
			if (abs(_distX) == 1)
				_jumpTimer = 1;
			break;
		case 4:
			if (abs(_distX) == 5)
				_jumpTimer = 1;
			if (abs(_distX) == 6)
				_jumpTimer = 1;
			break;
		case 5:
			if (abs(_distX) == 3)
				_jumpTimer = 1;
			if (abs(_distX) == 5)
				_jumpTimer = 10;
			if (abs(_distX) == 7)
				_jumpTimer = 3;
			if (abs(_distX) == 8)
				_jumpTimer = 3;
			break;
		default:
			break;
		}
	}

	//setting delay timers
	_dirDelayTimer = 0;

	if (abs(_distX) == 5 && _distY == 5)
		_dirDelayTimer = 5;

}

void JumpAction::SetMovementRange()
{
	if (_running)
	{
		_moveRange = 16;

		switch (abs(_distX))
		{
		case 4:
			if (_distY == -1)
				_moveRange = 24;
			break;
		case 5:
			if (_distY == 0 || _distY == 1 || _distY == 2)
				_moveRange = 22;
			if (_distY == 3)
				_moveRange = 24;
			if (_distY == 4)
				_moveRange = 26;
			if (_distY == 5)
				_moveRange = 24;
			break;
		case 6:
			if (_distY == 1 || _distY == 2 || _distY == 3)
				_moveRange = 22;
			if (_distY == 4)
				_moveRange = 24;
			if (_distY == 5)
				_moveRange = 24;
			break;
		case 7:
			if (_distY == 2 || _distY == 3)
				_moveRange = 18;
			if (_distY == 4 || _distY == 5)
				_moveRange = 24;
			break;
		case 8:
			if (_distY == 1)
				_moveRange = 4;
			if (_distY == 2)
				_moveRange = 16;
			if (_distY == 3)
				_moveRange = 16;
			if (_distY == 4)
				_moveRange = 26;
			if (_distY == 5)
				_moveRange = 22;
			break;
		default:
			break;
		}
	}
	else
	{
		_moveRange = 8;

		switch (abs(_distX))
		{
		case 1:
			if (_distY > 0)
				_moveRange = 12;
			break;
		case 2:
			if (_distY == 0)
				_moveRange = 10;
			if (_distY == 1 || _distY == 2 || _distY == 3 || _distY == 4 || _distY == 5)
				_moveRange = 14;
			break;
		case 3:
			if (_distY == 0 || _distY == 1 || _distY == 2 || _distY == 3 || _distY == 4)
				_moveRange = 10;
			if (_distY == 5)
				_moveRange = 13;
			break;
		case 4:
			if (_distY == 0)
				_moveRange = 4;
			if (_distY == 4 || _distY == 5)
				_moveRange = 12;
			break;
		default:
			break;
		}
	}
}

void JumpAction::MoveToTarget(ordersStruct *orders)
{
	int playerPosX = (int)_bot->GetPlayerPositionX();

	if (!WithinRangeFromTarget(playerPosX, MiddleXPixel(_targetNode), _moveRange)
		|| _actionTarget == LEDGE)
	{
		if (_directionX == xRIGHT) orders->goRight = true;
		if (_directionX == xLEFT) orders->goLeft = true;
	}
}

ordersStruct JumpAction::GetOrders()
{
	ordersStruct orders;
	int playerPosX = (int)_bot->GetPlayerPositionX();
	int playerPosY = (int)_bot->GetPlayerPositionY();

	if (!_actionInProgress)
	{
		_targetNode = CalculateTargetNode(_distX, _distY);
		_startNodeCenter = MiddlePixelOfNode((int)_bot->GetPlayerPositionXNode());

		//makes the bot grab the ledge even when distX=0
		//priority - right
		if (abs(_distX) == 0 && _actionTarget == LEDGE)
		{
			if (!_bot->IsNodePassable(_targetNode.GetX() + 1, _targetNode.GetY(), NODE_COORDS) &&
				_bot->IsNodePassable(_targetNode.GetX() + 1, _targetNode.GetY() - 1, NODE_COORDS))
				_directionX = xRIGHT;
			else if (!_bot->IsNodePassable(_targetNode.GetX() - 1, _targetNode.GetY(), NODE_COORDS) &&
					 _bot->IsNodePassable(_targetNode.GetX() - 1, _targetNode.GetY() - 1, NODE_COORDS))
				_directionX = xLEFT;
		}

		//setting the initial state
		if (playerPosX != _startNodeCenter && _actionTarget == LADDER && _distY > 3)
			_state = CENTRALIZING;
		else
		{
			if (abs(_distX) == 0 && _actionTarget == LEDGE)
				_state = CENTRALIZING;
			else
				_state = WALKING;
		}

		//TODO: this condition will never be true now? fix this 
		if (_directionX == xNONE && _directionY == yUP && _actionTarget == LADDER)
			_state = JUMPING;


		//calculating the threshold; after crossing it bot will jump
		if ((abs(_distX) == 1 && _actionTarget != LEDGE) || (abs(_distX) == 0 && _actionTarget == LEDGE))
		{
			_jumpThreshold = _startNodeCenter;
			if (_directionX == xRIGHT)
				_jumpThreshold -= 4;
			else
				_jumpThreshold += 4;
		}
		else
		{
			_jumpThreshold = EdgeOfNodeX((int)_bot->GetPlayerPositionXNode(), _directionX);
			//makes the bot jump from the furthest point of a ledge, so he can jump out of narrow tunnels (when there is a full tile above his head)
			if (_directionX == xRIGHT)
				_jumpThreshold += 1;
			else
				_jumpThreshold -= 1;
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
	case CENTRALIZING:
		Centralize(&orders, _startNodeCenter);

		if (WithinRangeFromTarget(playerPosX, _startNodeCenter, 1))
		{
			_state = WALKING;
		}

		break;
	case WALKING:
		if (_running) orders.run = true;
		_directionX == xRIGHT ? orders.goRight = true : orders.goLeft = true;

		if (WentThrough(playerPosX, _jumpThreshold, _directionX))
		{
			//ModifyMoveRangeIfNeeded(playerPosX);

			orders.jump = true;
			_state = JUMPING;
		}

		break;
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

		if (IsStandingStill(playerPosX, playerPosY, _prevPlayerPosX, _prevPlayerPosY))
			_state = LANDED_STUCK;

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
	case LANDED_STUCK:
		Centralize(&orders, MiddleXPixel(_targetNode));
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


	_prevPlayerPosX = playerPosX;
	_prevPlayerPosY = playerPosY;

	return orders;
}
