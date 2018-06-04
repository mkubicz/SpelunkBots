#pragma once
#include "MapNode.h"
#include "IMovementAction.h"

class PathInfo
{
private:
	std::vector<MapNode> _path;
	Coords _target;
	std::vector<IMovementAction*> _actions;
	std::vector<IMovementAction*>::iterator _pathIt;

public:
	PathInfo(std::vector<MapNode> path, Coords target, std::vector<IMovementAction*> actions);

	Coords GetTarget();
	IMovementAction * GetNextAction();
	bool ActionsExhausted();

	//mo�e ola� ten reason? wydaje si� nie by� a� tak potrzebne
	//SCHEDULING_REASON reason;
};