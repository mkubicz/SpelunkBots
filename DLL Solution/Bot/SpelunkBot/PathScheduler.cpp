#include "stdafx.h"
#include "PathScheduler.h"

PathScheduler::PathScheduler(IBot *bot, Pathfinder *pathfinder)
	: _pathfinder(pathfinder), _bot(bot)
{
	_currentAction = NULL;
}

bool PathScheduler::TryToSchedulePath(Coords target)
{
	return TryToAddPath(GetStartNodeForNextPath(), target);
}

bool PathScheduler::TryToInsertPath(Coords target, int i)
{
	while (_pathInfoQ.size() >= i)
		_pathInfoQ.pop_back();

	Coords start = GetStartNodeForNextPath();

	return TryToAddPath(start, target);
}

bool PathScheduler::TryToAddPath(Coords start, Coords target)
{
	if (_pathfinder->TryToCalculatePath(start, target))
	{
		std::vector<MapNode> path = _pathfinder->GetPath();

		// TODO: dorób przegl¹danie i modyfikowanie œcie¿ki pod k¹tem niebezpieczeñstw
		//ModifyPathToDangers(path)

		std::vector<IMovementAction*> actions = CreateActions(path);

		_pathInfoQ.push_back(PathInfo(path, target, actions));
		return true;
	}
	else
		return false;
}

ordersStruct PathScheduler::GetOrdersFromCurrentAction()
{
	ordersStruct orders;

	UpdateCurrentAction();

	if (_currentAction != NULL)
		orders = _currentAction->GetOrders();

	return orders;
}

bool PathScheduler::IsScheduled(Coords target)
{
	for each (PathInfo pi in _pathInfoQ)
		if (pi.GetTarget() == target)
			return true;

	return false;
}

int PathScheduler::NumberOfSheduledPaths()
{
	return _pathInfoQ.size();
}

bool PathScheduler::NoSheduledPaths()
{
	return _pathInfoQ.empty();
}

Coords PathScheduler::GetCurrentTarget()
{
	if (!_pathInfoQ.empty())
		return _pathInfoQ.front().GetTarget();
	else
		return Coords(-1, -1);
}

std::vector<Coords> PathScheduler::GetSheduledTargets()
{
	std::vector<Coords> targets;

	for each (PathInfo pi in _pathInfoQ)
		targets.push_back(pi.GetTarget());

	return targets;
}

void PathScheduler::ClearSheduledPaths()
{
	_pathInfoQ.clear();
}

void PathScheduler::NewLevel()
{
	_pathInfoQ.clear();
	delete _currentAction;
}

Coords PathScheduler::GetStartNodeForNextPath()
{
	Coords start;
	if (_pathInfoQ.empty())
		start = _pathfinder->GetBotCoords();
	else
		start = _pathInfoQ.back().GetTarget(); //target of most recently added path is our start node

	return start;
}

//Coords PathScheduler::GetStartNodeForNextPath(int i)
//{
//	Coords start;
//
//	if (_pathInfoQ.size() < i)
//		return GetStartNodeForNextPath();
//	else
//	{
//		while (_pathInfoQ.size() >= i)
//			_pathInfoQ.pop_back();
//		start = _pathInfoQ.back().GetTarget();
//	}
//
//	return start;
//}

void PathScheduler::UpdateCurrentAction()
{
	if (_currentAction == NULL || _currentAction->ActionDone())
	{
		_currentAction = GetNextAction();
	}
}

IMovementAction* PathScheduler::GetNextAction()
{
	if (!_pathInfoQ.empty())
	{
		if (!_pathInfoQ.front().ActionsExhausted())
		{
			return _pathInfoQ.front().GetNextAction();
		}
		else
		{
			_pathInfoQ.pop_front();
			return GetNextAction();
		}
	}
	else
		return NULL;
}


std::vector<IMovementAction*> PathScheduler::CreateActions(std::vector<MapNode> path)
{
	std::vector<IMovementAction*> actions;
	int distX, distY;
	Coords currC;
	MVSTATE mvState;
	ACTION_TARGET actionTarget;
	MOVEMENTACTION action;

	for (int i = 0; i < path.size(); i++)
	{
		//first node in a path is start node
		if (i == 0)
		{
			currC = path[0].GetCoords();
			continue;
		}

		distX = path[i].GetX() - currC.GetX();
		distY = path[i].GetY() - currC.GetY();

		action = path[i].GetActionToReach();
		actionTarget = path[i].GetActionTarget();
		mvState = path[i - 1].GetMvState();

		actions.push_back(CreateAction(action, actionTarget, mvState, distX, distY));

		currC = path[i].GetCoords();
	}

	return actions;
}

IMovementAction* PathScheduler::CreateAction(MOVEMENTACTION action, ACTION_TARGET actionTarget, MVSTATE mvState, int distX, int distY)
{
	DIRECTIONX directionX;
	DIRECTIONY directionY;

	if (distX > 0) directionX = xRIGHT;
	else if (distX == 0) directionX = xNONE;
	else directionX = xLEFT;

	if (distY > 0) directionY = yDOWN;
	else if (distY == 0) directionY = yNONE;
	else directionY = yUP;

	switch (action)
	{
	case IDLE:
		break;
	case CENTRALIZE:
		return new CentralizeAction(_bot);
		break;
	case WALK:
		//if (distX == 2), then bot is walking over a hole which should be runned
		if (abs(distX) == 2)
		{
			return new WalkAction(_bot, distX, RUN);
			break;
		}

		//turning off ActionBatching™ for now
		//if (!_actionsQ.empty())
		//{
		//	if (_actionsQ.back()->ActionType() == WALK &&
		//		((distX > 0 && _actionsQ.back()->GetDirectionX() == xRIGHT) ||
		//		(distX < 0 && _actionsQ.back()->GetDirectionX() == xLEFT)))
		//	{
		//		//previous action is also a WalkAction, and its going in the same direction,
		//		//so we increase its distance instead of creating a new command (ActionBatching™)
		//		dynamic_cast<WalkAction*>(_actionsQ.back())->AddDistance(distX);
		//		break;
		//	}
		//}

		return new WalkAction(_bot, distX);

		break;
	case HANG:
		return new HangAction(_bot, directionX);
		break;
	case DROP:
		return new DropAction(_bot, actionTarget, distY);
		break;
	case CLIMBFROMHANG:
		return new ClimbFromHangAction(_bot, directionX);
		break;
	case CLIMB:
		//turning off ActionBatching™ for now
		//if (!_actionsQ.empty())
		//{
		//	if (_actionsQ.back()->ActionType() == CLIMB &&
		//		((distY > 0 && _actionsQ.back()->GetDirectionY() == yDOWN) ||
		//		(distY < 0 && _actionsQ.back()->GetDirectionY() == yUP)))
		//	{
		//		//previous action is also a ClimbAction, and its going in the same direction,
		//		//so we increase its distance instead of creating a new command (ActionBatching™)
		//		dynamic_cast<ClimbAction*>(_actionsQ.back())->AddDistance(distY);
		//		break;
		//	}
		//}

		return new ClimbAction(_bot, distY);
		break;
	case JUMPABOVERIGHT:
		return new JumpAboveAction(_bot, xRIGHT);
		break;
	case JUMPABOVELEFT:
		return new JumpAboveAction(_bot, xLEFT);
		break;
	case JUMP:
		return new JumpAction(_bot, actionTarget, distX, distY);
		break;
	case JUMPFROMLADDER:
		if (mvState == mvCLIMBING_WITH_MOMENTUM)
			return new JumpFromLadderAction(_bot, actionTarget, true, distX, distY);
		else if (mvState == mvCLIMBING)
			return new JumpFromLadderAction(_bot, actionTarget, false, distX, distY);
		else
			std::cout << "Error: MVSTATE is not CLIMBING when creating JUMPFROMLADDER" << std::endl;
		break;
	case WALKOFFLEDGE:
		//TODO: walkoffledge to ladders
		//return new WalkOffLedgeAction(_bot, distX, distY, jumpTarget));
		return new WalkOffLedgeAction(_bot, distX, distY);
		break;
	default:
		break;
	}
}

