#include "stdafx.h"
#include "MapNode.h"

MapNode::MapNode()
	: MapNode(0, 0, IDLE, GROUND, mvSTANDING)
{
}

int MapNode::GetX()
{
	return _coords.GetX();
}

int MapNode::GetY()
{
	return _coords.GetY();
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