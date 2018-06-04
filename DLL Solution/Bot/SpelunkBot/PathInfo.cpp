#include "stdafx.h"
#include "PathInfo.h"

PathInfo::PathInfo(std::vector<MapNode> path, Coords target, std::vector<IMovementAction*> actions)
	: _path(path), _target(target), _actions(actions)
{
	_pathIt = _actions.begin();
}

Coords PathInfo::GetTarget()
{
	return _target;
}

IMovementAction * PathInfo::GetNextAction()
{
	IMovementAction * a = NULL;
	if (!ActionsExhausted())
	{
		a = *_pathIt;
		_pathIt++;
	}

	return a;
}

bool PathInfo::ActionsExhausted()
{
	return _pathIt == _actions.end();
}
