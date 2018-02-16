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

double MapSearchNode::GetGScore(MapSearchNode *parent)
{
	//return p->_gScore + ((_x == p->_x || _y == p->_y) ? 1 : 1.5);

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

void MapSearchNode::ComputeScores(MapSearchNode * end)
{
	_gScore = GetGScore(_parent);
	_hScore = GetHScore(end);
	//_fScore = _gScore + _hScore;
}
