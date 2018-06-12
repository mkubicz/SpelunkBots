#pragma once
#include "MapNode.h"
#include "IMovementAction.h"

class PathInfo
{
private:
	std::vector<MapNode> _path;
	Coords _target;
	std::vector<std::shared_ptr<IMovementAction>> _actions;
	int _actionNr;


public:
	PathInfo(std::vector<MapNode> path, Coords target, std::vector<std::shared_ptr<IMovementAction>> actions);
	~PathInfo();

	Coords GetTarget();
	std::shared_ptr<IMovementAction> GetNextAction();
	bool ActionsExhausted();

	//mo¿e olaæ ten reason? wydaje siê nie byæ a¿ tak potrzebne
	//SCHEDULING_REASON reason;
};