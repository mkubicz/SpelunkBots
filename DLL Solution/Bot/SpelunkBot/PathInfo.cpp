#include "stdafx.h"
#include "PathInfo.h"

PathInfo::PathInfo(std::vector<MapNode> path, Coords target, std::vector<std::shared_ptr<IMovementAction>> actions)
	: _path(path), _target(target), _actions(actions)
{
	//_pathIt = _actions.begin();
	_actionNr = -1;
}

PathInfo::~PathInfo()
{
	//we're using smart pointers now - no need for destructors.
	//for (_pathIt = _actions.begin(); _pathIt != _actions.end(); _pathIt++)
	//	delete (*_pathIt);

	//_actions.clear();
}

Coords PathInfo::GetTarget()
{
	return _target;
}

std::shared_ptr<IMovementAction> PathInfo::GetNextAction()
{
	if (!ActionsExhausted())
	{
		_actionNr++;
		return _actions[_actionNr];
	}

	return nullptr;
}

bool PathInfo::ActionsExhausted()
{
	return _actionNr >= ((int)_actions.size() - 1);
}
