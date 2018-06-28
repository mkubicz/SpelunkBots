#include "stdafx.h"
#include "PathScheduler.h"

PathScheduler::PathScheduler(std::shared_ptr<IBot> bot, std::shared_ptr<Pathfinder> pathfinder)
	: _pathfinder(pathfinder), _bot(bot)
{
	_currentAction = NULL;
}

bool PathScheduler::TryToSchedulePath(Coords target, PATHFINDING_AT_MODE atmode,
	std::vector<Coords> allowedArrowTraps)
{
	return TryToAddPath(GetStartNodeForNextPath(), target, atmode, allowedArrowTraps);
}

bool PathScheduler::TryToInsertPath(Coords target, int i, PATHFINDING_AT_MODE atmode,
	std::vector<Coords> allowedArrowTraps)
{
	while (_pathInfoQ.size() >= i)
		_pathInfoQ.pop_back();

	Coords start = GetStartNodeForNextPath();

	return TryToAddPath(start, target, atmode, allowedArrowTraps);
}

void PathScheduler::PickUpItem(int itemID)
{
	std::vector<std::shared_ptr<IMovementAction>> actions;
	actions.push_back(std::make_shared<PickUpItemAction>(_bot, itemID));

	//_pathInfoQ.push_back(new PathInfo(std::vector<MapNode>(), Coords(-1,-1), actions));
	_pathInfoQ.push_back(std::make_unique<PathInfo>(std::vector<MapNode>(), Coords(-1, -1), actions));
}

void PathScheduler::ScheduleAction(std::shared_ptr<IMovementAction> action)
{
	std::vector<std::shared_ptr<IMovementAction>> actions;
	actions.push_back(action);

	//_pathInfoQ.push_back(new PathInfo(std::vector<MapNode>(), Coords(-1,-1), actions));
	_pathInfoQ.push_back(std::make_unique<PathInfo>(std::vector<MapNode>(), Coords(-1, -1), actions));
}

bool PathScheduler::TryToAddPath(Coords start, Coords target, PATHFINDING_AT_MODE atmode,
	std::vector<Coords> allowedArrowTraps)
{
	if (_pathfinder->TryToCalculatePath(start, target, atmode, allowedArrowTraps))
	//if (_pathfinder->TryToCalculatePathNoGrid(start, target))
	{
		std::vector<MapNode> path = _pathfinder->GetPathCopy();

		std::vector<std::shared_ptr<IMovementAction>> actions = CreateActions(path);

		//_pathInfoQ.push_back(new PathInfo(path, target, actions));
		_pathInfoQ.push_back(std::make_unique<PathInfo>(path, target, actions));
		return true;
	}
	else
		return false;
}


//void PathScheduler::ModifyPathToDangers(std::vector<MapNode>* path)
//{
//	std::vector<MapNode>::iterator pathIt = path->begin();
//	while (pathIt != path->end())
//	{
//		if (pathIt->IsArrowTrapOnWay())
//		{
//
//			if ()
//
//		}
//		else pathIt++;
//	}
//}


ordersStruct PathScheduler::GetOrdersFromCurrentAction()
{
	ordersStruct orders;

	UpdateCurrentAction();

	if (_currentAction != nullptr)
		orders = _currentAction->GetOrders();

	return orders;
}

MOVEMENTACTION PathScheduler::GetCurrentActionType()
{
	if (_currentAction != NULL)
		return _currentAction->ActionType();
	else
		return IDLE;
}

void PathScheduler::UpdateCurrentAction()
{
	if (_currentAction == nullptr || _currentAction->ActionDone())
	{
		_currentAction = GetNextAction();
	}
}

std::shared_ptr<IMovementAction> PathScheduler::GetNextAction()
{
	//lock_guard automatycznie siê zwalnia jak wyjdzie ze scope
	std::lock_guard<std::mutex> lockGuard(_mutex_pathInfoQ);

	if (!_pathInfoQ.empty())
	{
		if (!_pathInfoQ.front()->ActionsExhausted())
		{
			return _pathInfoQ.front()->GetNextAction();
		}
		else
		{
			_pathInfoQ.pop_front();
			return GetNextAction();
		}
	}
	else
		return nullptr;
}


bool PathScheduler::IsScheduled(Coords target)
{
	//<typ> const& - referencja która nie pozwala zmieniaæ obiektu, tylko odczytywaæ
	for each (std::unique_ptr<PathInfo> const& pi in _pathInfoQ)
		if (pi->GetTarget() == target)
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

bool PathScheduler::ActionInProgress()
{
	return _currentAction != nullptr;
}

Coords PathScheduler::GetCurrentTarget()
{
	if (!_pathInfoQ.empty())
		return _pathInfoQ.front()->GetTarget();
	else
		return Coords(-1, -1);
}

std::vector<Coords> PathScheduler::GetSheduledTargets()
{
	std::vector<Coords> targets;

	for each (auto const& pi in _pathInfoQ)
		targets.push_back(pi->GetTarget());

	return targets;
}

void PathScheduler::ClearSheduledPaths()
{
	_mutex_pathInfoQ.lock();

	_pathInfoQ.clear();

	_mutex_pathInfoQ.unlock();
}

void PathScheduler::NewLevel()
{
	_pathInfoQ.clear();
	_currentAction = NULL;
}

Coords PathScheduler::GetStartNodeForNextPath()
{
	Coords start;
	if (_pathInfoQ.empty())
		start = _pathfinder->GetBotCoords();
	else
		start = _pathInfoQ.back()->GetTarget(); //target of most recently added path is our start node

	return start;
}


std::vector<std::shared_ptr<IMovementAction>> PathScheduler::CreateActions(std::vector<MapNode> path)
{
	std::vector<std::shared_ptr<IMovementAction>> actions;
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
		
		if (action == DROP && _pathfinder->LadderTop(currC.OffsetY(1)))
			actions.push_back(std::make_shared<CentralizeAction>(_bot));

		actions.push_back(CreateAction(action, actionTarget, mvState, distX, distY));

		currC = path[i].GetCoords();
	}

	return actions;
}

std::shared_ptr<IMovementAction> PathScheduler::CreateAction(MOVEMENTACTION action, ACTION_TARGET actionTarget, MVSTATE mvState, int distX, int distY)
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
		return std::make_shared<CentralizeAction>(_bot);
		break;
	case WALK:
		//if (distX == 2), then bot is walking over a hole which should be runned
		if (abs(distX) == 2)
		{
			return std::make_shared<WalkAction>(_bot, distX, RUN);
			break;
		}
		//turning off ActionBatching™, with the new InstantCommandSwitching™ its not needed
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
		return std::make_shared<WalkAction>(_bot, distX);

		break;
	case HANG:
		return std::make_shared<HangAction>(_bot, directionX);
		break;
	case DROP:
		return std::make_shared<DropAction>(_bot, actionTarget, distY);
		break;
	case HANGDROP:
		return std::make_shared<HangDropAction>(_bot, directionX);
	case CLIMBFROMHANG:
		return std::make_shared<ClimbFromHangAction>(_bot, directionX);
		break;
	case CLIMB:
		//turning off ActionBatching™, with the new InstantCommandSwitching™ its not needed
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

		return std::make_shared<ClimbAction>(_bot, distY);
		break;
	case JUMPABOVERIGHT:
		return std::make_shared<JumpAboveAction>(_bot, xRIGHT);
		break;
	case JUMPABOVELEFT:
		return std::make_shared<JumpAboveAction>(_bot, xLEFT);
		break;
	case JUMP:
		return std::make_shared<JumpAction>(_bot, actionTarget, distX, distY);
		break;
	case JUMPFROMLADDER:
		if (mvState == mvCLIMBING_WITH_MOMENTUM)
			return std::make_shared<JumpFromLadderAction>(_bot, actionTarget, true, distX, distY);
		else if (mvState == mvCLIMBING)
			return std::make_shared<JumpFromLadderAction>(_bot, actionTarget, false, distX, distY);
		else
			std::cout << "Error: MVSTATE is not CLIMBING when creating JUMPFROMLADDER" << std::endl;
		break;
	case WALKOFFLEDGE:
		//TODO: walkoffledge to ladders
		//return std::make_shared<WalkOffLedgeAction>(_bot, distX, distY, jumpTarget));
		return std::make_shared<WalkOffLedgeAction>(_bot, distX, distY);
		break;
	default:
		break;
	}
}



