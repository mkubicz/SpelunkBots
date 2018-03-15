#include "stdafx.h"
#include "Node.h"

Node::Node()
	: Node(0, 0, IDLE, GROUND)
{
}

Node::Node(int x, int y)
	: Node(x, y, IDLE, GROUND)
{
}

Node::Node(int x, int y, MOVEMENTACTION actionToReach)
	: Node(x, y, actionToReach, GROUND)
{
}

Node::Node(int x, int y, MOVEMENTACTION actionToReach, JUMP_TARGET jumpTarget)
{
	_x = x;
	_y = y;
	_actionToReach = actionToReach;
	_jumpTarget = jumpTarget;
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

JUMP_TARGET Node::GetJumpTarget()
{
	return _jumpTarget;
}
