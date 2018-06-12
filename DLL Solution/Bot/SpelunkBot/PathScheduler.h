#pragma once
#include "Pathfinder.h"
#include "AllActions.h"
#include "PathInfo.h"

class PathScheduler
{
private:
	std::shared_ptr<IBot> _bot;
	std::shared_ptr<Pathfinder> _pathfinder;

	std::deque<std::unique_ptr<PathInfo>> _pathInfoQ;
	std::shared_ptr<IMovementAction> _currentAction;

	//Coords GetStartNodeForNextPath(int i);
	void UpdateCurrentAction();
	std::shared_ptr<IMovementAction> GetNextAction();

	bool TryToAddPath(Coords start, Coords target);

	std::vector<std::shared_ptr<IMovementAction>> CreateActions(std::vector<MapNode> path);
	std::shared_ptr<IMovementAction> PathScheduler::CreateAction(MOVEMENTACTION action, ACTION_TARGET actionTarget, MVSTATE mvState, int distX, int distY);

	//void ModifyPathToDangers(std::vector<MapNode> *path);



public:
	PathScheduler(std::shared_ptr<IBot> bot, std::shared_ptr<Pathfinder> pathfinder);

	void NewLevel();

	bool TryToSchedulePath(Coords target);
	bool TryToInsertPath(Coords target, int i);
	void PickUpItem(int itemID);
	void ScheduleAction(std::shared_ptr<IMovementAction> action);

	Coords GetStartNodeForNextPath();
	ordersStruct GetOrdersFromCurrentAction();
	
	bool IsScheduled(Coords target);
	int NumberOfSheduledPaths();
	bool NoSheduledPaths();
	
	Coords GetCurrentTarget();
	std::vector<Coords> GetSheduledTargets();
	
	void ClearSheduledPaths();
};