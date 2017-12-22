#include "stdafx.h"
#include "MapSearchNode.h"


int MapSearchNode::GetX()
{
	return _x;
}

int MapSearchNode::GetY()
{
	return _y;
}

double MapSearchNode::GetGScore(MapSearchNode *p)
{
	return p->_gScore + ((_x == p->_x || _y == p->_y) ? 1 : 1.5);
}

double MapSearchNode::GetHScore(MapSearchNode *p)
{
	/*
	float xDist = ((x) - (p->x));
	float yDist = ((y) - (p->y));
	float distance = sqrt((xDist * xDist) + (yDist * yDist));
	hScore = (double) distance;*/
	return (abs(p->_x - _x) + abs(p->_y - _y));
}

double MapSearchNode::GetFScore()
{
	return _fScore;
}

void MapSearchNode::ComputeScores(MapSearchNode * end)
{
	_gScore = GetGScore(_parent);
	_hScore = GetHScore(end);
	_fScore = _gScore + _hScore;
}
