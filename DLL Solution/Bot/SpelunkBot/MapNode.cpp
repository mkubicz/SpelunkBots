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

	_arrowTrapCoords = Coords();
	_arrowTrapCoordsCandidate = Coords();
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

	_arrowTrapCoords = Coords();
	_arrowTrapCoordsCandidate = Coords();
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

Coords MapNode::GetArrowTrapCoords()
{
	return _arrowTrapCoords;
}

MapNode * MapNode::GetDijParent()
{
	return _dij_prev;
}

void MapNode::CopyInfo(MapNode n)
{
	_actionToReach = n._actionToReach;
	_actionToReachTarget = n._actionToReachTarget;
	_mvState = n._mvState;

	_actionToReachCandidate = n._actionToReachCandidate;
	_actionToReachTargetCandidate = n._actionToReachTargetCandidate;
	_mvStateCandidate = n._mvStateCandidate;

	_gScore = n._gScore;
	_hScore = n._hScore;
	_parent = n._parent;

	_opened = n._opened;
	_closed = n._closed;
	_visited = n._visited;

	_CCnr = n._CCnr;
	_dij_dist = n._dij_dist;
	_dij_prev = n._dij_prev;

	_arrowTrapOnWay = n._arrowTrapOnWay;
	_arrowTrapOnWayCandidate = n._arrowTrapOnWayCandidate;

	_arrowTrapCoords = n._arrowTrapCoords;
	_arrowTrapCoordsCandidate = n._arrowTrapCoordsCandidate;
}

void MapNode::SolidifyCandidates()
{
	_actionToReach = _actionToReachCandidate;
	_actionToReachTarget = _actionToReachTargetCandidate;
	_mvState = _mvStateCandidate;

	_arrowTrapOnWay = _arrowTrapOnWayCandidate;
	_arrowTrapCoords = _arrowTrapCoordsCandidate;

	//if there is an arrowtrap on the way to node's parent, then there is an arrowtrap on the way to node itself

	if (_parent != NULL)
		if (_parent->_arrowTrapOnWay == true)
		{
			_arrowTrapOnWay = true;
			_arrowTrapCoords = _parent->_arrowTrapCoords;
		}
	if (_dij_prev != NULL)
		if (_dij_prev->_arrowTrapOnWay == true)
		{
			_arrowTrapOnWay = true;
			_arrowTrapCoords = _dij_prev->_arrowTrapCoords;
		}
}

int MapNode::GetPenalty()
{
	if (_actionToReach == WALK || _actionToReachCandidate == WALK) return 0;
	else if (_actionToReach == JUMP || _actionToReachCandidate == JUMP) return 5;
	else if (_actionToReach == WALKOFFLEDGE || _actionToReachCandidate == WALKOFFLEDGE) return 7;
	else return 1;
}

double MapNode::CalculateGScore(MapNode * parentCandidate)
{
	double dist = 0;

	//if (_actionToReachCandidate == WALK || _actionToReachCandidate == CLIMB)
	//	dist = 0.5; //walk and climb has lower cost because its safer
	//else
		dist = abs(parentCandidate->GetX() - GetX()) + abs(parentCandidate->GetY() - GetY());

	//if (_actionToReachCandidate == JUMP || _actionToReachCandidate == WALKOFFLEDGE)
	//	dist += 1000;

		dist += GetPenalty();

	//manhattan
	return parentCandidate->_gScore + dist;

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