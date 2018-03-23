#include "stdafx.h"
#include "Node.h"

Node::Node()
	: Node(0, 0, IDLE, GROUND, mvSTANDING)
{
}

Node::Node(int x, int y)
	: Node(x, y, IDLE, GROUND, mvSTANDING)
{
}

Node::Node(int x, int y, MOVEMENTACTION actionToReach)
	: Node(x, y, actionToReach, GROUND, mvSTANDING)
{
}

Node::Node(int x, int y, MOVEMENTACTION actionToReach, ACTION_TARGET jumpTarget)
	: Node(x, y, actionToReach, jumpTarget, mvSTANDING)
{
}

Node::Node(int x, int y, MOVEMENTACTION actionToReach, MVSTATE mvState)
	: Node(x, y, actionToReach, GROUND, mvState)
{
}

Node::Node(int x, int y, MOVEMENTACTION actionToReach, ACTION_TARGET jumpTarget, MVSTATE mvState)
{
	_x = x;
	_y = y;
	_actionToReach = actionToReach;
	_actionTarget = jumpTarget;
	_mvState = mvState;
}

int Node::GetX()
{
	return _x;
}

int Node::GetY()
{
	return _y;
}

MOVEMENTACTION Node::GetActionToReach()
{
	return _actionToReach;
}

ACTION_TARGET Node::GetActionTarget()
{
	return _actionTarget;
}

MVSTATE Node::GetMvState()
{
	return _mvState;
}
