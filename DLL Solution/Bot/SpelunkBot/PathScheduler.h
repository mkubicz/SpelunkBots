#pragma once
#include "Pathfinder.h"
#include "AllActions.h"
#include "PathInfo.h"

class PathScheduler
{
private:
	Pathfinder* _pathfinder;
	IBot * _bot;
	std::deque<PathInfo> _pathInfoQ;
	IMovementAction* _currentAction;

	//Coords GetStartNodeForNextPath(int i);
	void UpdateCurrentAction();
	IMovementAction* GetNextAction();

	bool TryToAddPath(Coords start, Coords target);

	std::vector<IMovementAction*> CreateActions(std::vector<MapNode> path);
	IMovementAction* PathScheduler::CreateAction(MOVEMENTACTION action, ACTION_TARGET actionTarget, MVSTATE mvState, int distX, int distY);

public:
	PathScheduler(IBot* bot, Pathfinder* pathfinder);

	void NewLevel();

	bool TryToSchedulePath(Coords target);
	bool TryToInsertPath(Coords target, int i);


	Coords GetStartNodeForNextPath();
	ordersStruct GetOrdersFromCurrentAction();
	
	bool IsScheduled(Coords target);
	int NumberOfSheduledPaths();
	bool NoSheduledPaths();
	
	Coords GetCurrentTarget();
	std::vector<Coords> GetSheduledTargets();
	
	void ClearSheduledPaths();
};