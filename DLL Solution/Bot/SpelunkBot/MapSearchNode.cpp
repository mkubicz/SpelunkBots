#include "stdafx.h"
#include "MapSearchNode.h"


MapSearchNode::MapSearchNode()
	: MapSearchNode(0,0)
{
}

MapSearchNode::MapSearchNode(int x, int y)
{
	_x = x;
	_y = y;
	_gScore = 0;
	_hScore = 0;
	_parent = NULL;

	_actionToReach = IDLE;
	_actionToReachCandidate = IDLE;

	_actionTarget = GROUND;
	_actionTargetCandidate = GROUND;

	_mvState = mvSTANDING;
	_mvStateCandidate = mvSTANDING;

	_opened = false;
	_closed = false;
	_visited = false;

	_CCnr = 0;
}

int MapSearchNode::GetX()
{
	return _x;
}

int MapSearchNode::GetY()
{
	return _y;
}

double MapSearchNode::GetGScore(MapSearchNode *parent)
{
	//manhattan
	return parent->_gScore + abs(parent->_x - _x) + abs(parent->_y - _y);
}

double MapSearchNode::GetHScore(MapSearchNode *destination)
{
	/*
	float xDist = ((x) - (p->x));
	float yDist = ((y) - (p->y));
	float distance = sqrt((xDist * xDist) + (yDist * yDist));
	hScore = (double) distance;*/
	return abs(destination->_x - _x) + abs(destination->_y - _y);
}

double MapSearchNode::GetFScore()
{
	//return _fScore;
	return _gScore + _hScore;
}

MOVEMENTACTION MapSearchNode::GetActionToReach()
{
	return _actionToReach;
}

ACTION_TARGET MapSearchNode::GetActionTarget()
{
	return _actionTarget;
}

MVSTATE MapSearchNode::GetMvState()
{
	return _mvState;
}

void MapSearchNode::ComputeScores(MapSearchNode * end)
{
	_gScore = GetGScore(_parent);
	_hScore = GetHScore(end);
	//_fScore = _gScore + _hScore;
}
