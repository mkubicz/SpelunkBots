#include "stdafx.h"
#include "DebugBot.h"

DebugBot::DebugBot()
{
	_actionsQ.push(CENTRALIZE);
	_actionsQ.push(WALKLEFT);
	_actionsQ.push(CENTRALIZE);
	_actionsQ.push(IDLE);
}

bool DebugBot::CloseToZero(double x)
{
	if (x < 0.1 && x > -0.1) return true;
	else return false;
}


//Centralize will issue commands to bot that will get him closer to the middle of the node he is currently standing on
//will return true if bot is centralized, otherwise false.
bool DebugBot::Centralize()
{
	double fractpart, intpart, distFromMiddle;
	fractpart = modf(_playerPositionXNode, &intpart);
	distFromMiddle = fractpart - 0.5;

	if (CloseToZero(abs(distFromMiddle)))
	{
		_goRight = false;
		_goLeft = false;
		return true;
	}
	else
	{
		if (distFromMiddle < 0) _goRight = true;
		else _goLeft = true;
	}
}

//fixed distance? fixed time? move to the center of next node? stop when you enter next node?
//in node coords for now
bool DebugBot::WalkLeft(double distance)
{
	if (!_walkLeftInProgress)
	{
		_targetPositionX = _playerPositionXNode - distance;
		_walkLeftInProgress = true;
	}

	if (CloseToZero(abs(_playerPositionXNode - _targetPositionX)))
	{
		_goLeft = false;
		_walkLeftInProgress = false;
		return true;
	}
	else
	{
		_goLeft = true;
		return false;
	}
}

void DebugBot::Update()
{
	if (_currentAction == IDLE || _actionDone)
	{
		if (!_actionsQ.empty())
		{
			_currentAction = _actionsQ.front();
			_actionsQ.pop();
			_actionDone = false;
		}

	}

	switch (_currentAction)
	{
		case IDLE:
			_actionDone = true;
			break;
		case CENTRALIZE:
			if (Centralize())
				_actionDone = true;
			break;
		case WALKLEFT:
			if (WalkLeft(4))
				_actionDone = true;
			break;
		case WALKRIGHT:
			break;
		default:
			break;
	}


}

