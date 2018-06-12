#include "stdafx.h"
#include "MapNode.h"

MapNode::MapNode()
	: MapNode(0, 0, IDLE, GROUND, mvSTANDING)
{
}

MapNode::MapNode(int x, int y)
	: MapNode(x, y, IDLE, GROUND, mvSTANDING)
{
}

MapNode::MapNode(int x, int y, MOVEMENTACTION actionToReach)
	: MapNode(x, y, actionToReach, GROUND, mvSTANDING)
{
}

MapNode::MapNode(int x, int y, MOVEMENTACTION actionToReach, ACTION_TARGET jumpTarget)
	: MapNode(x, y, actionToReach, jumpTarget, mvSTANDING)
{
}

MapNode::MapNode(int x, int y, MOVEMENTACTION actionToReach, MVSTATE mvState)
	: MapNode(x, y, actionToReach, GROUND, mvState)
{
}

MapNode::MapNode(int x, int y, MOVEMENTACTION actionToReach, ACTION_TARGET jumpTarget, MVSTATE mvState)
	: _coords(Coords(x, y, NODE_ROUNDDOWN))
{
	_actionToReach = actionToReach;
	_actionToReachTarget = jumpTarget;
	_mvState = mvState;

	_actionToReachCandidate = IDLE;
	_actionToReachTargetCandidate = GROUND;
	_mvStateCandidate = mvSTANDING;

	_gScore = 0;
	_hScore = 0;
	_parent = NULL;

	_opened = false;
	_closed = false;
	_visited = false;

	_CCnr = 0;
	_dij_dist = INT_MAX;
	_dij_prev = NULL;

	_arrowTrapOnWay = false;
	_arrowTrapOnWayCandidate = false;
}

void MapNode::Reset()
{
	_actionToReach = IDLE;
	_actionToReachTarget = GROUND;
	_mvState = mvSTANDING;

	_actionToReachCandidate = IDLE;
	_actionToReachTargetCandidate = GROUND;
	_mvStateCandidate = mvSTANDING;

	_gScore = 0;
	_hScore = 0;
	_parent = NULL;

	_opened = false;
	_closed = false;
	_visited = false;

	_CCnr = 0;
	_dij_dist = INT_MAX;
	_dij_prev = NULL;

	_arrowTrapOnWay = false;
	_arrowTrapOnWayCandidate = false;
}

int MapNode::GetX()
{
	return _coords.GetX();
}

int MapNode::GetY()
{
	return _coords.GetY();
}

Coords MapNode::GetCoords()
{
	return _coords;
}

MOVEMENTACTION MapNode::GetActionToReach()
{
	return _actionToReach;
}

ACTION_TARGET MapNode::GetActionTarget()
{
	return _actionToReachTarget;
}

MVSTATE MapNode::GetMvState()
{
	return _mvState;
}

bool MapNode::IsArrowTrapOnWay()
{
	return _arrowTrapOnWay;
}

void MapNode::SolidifyCandidates()
{
	_actionToReach = _actionToReachCandidate;
	_actionToReachTarget = _actionToReachTargetCandidate;
	_mvState = _mvStateCandidate;

	_arrowTrapOnWay = _arrowTrapOnWayCandidate;

	//if there is an arrowtrap on the way to node's parent, then there is an arrowtrap on the way to node itself
	if (_parent != NULL)
		if (_parent->_arrowTrapOnWay == true)
			_arrowTrapOnWay = true;
	if (_dij_prev != NULL)
		if (_dij_prev->_arrowTrapOnWay == true)
			_arrowTrapOnWay = true;
}

double MapNode::CalculateGScore(MapNode * parentCandidate)
{
	//manhattan
	return parentCandidate->_gScore + 
		abs(parentCandidate->GetX() - GetX()) + 
		abs(parentCandidate->GetY() - GetY());
}

double MapNode::CalculateHScore(MapNode * destination)
{
	return abs(destination->GetX() - GetX()) + abs(destination->GetY() - GetY());
}

double MapNode::CalculateFScore()
{
	return _gScore + _hScore;
}

void MapNode::ComputeScores(MapNode * destination)
{
	_gScore = CalculateGScore(_parent);
	_hScore = CalculateHScore(destination);
}