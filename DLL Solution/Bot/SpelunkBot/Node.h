#pragma once

class Node
{
private:
	int _x;
	int _y;

	MOVEMENTACTION _actionToReach;
	JUMP_TARGET _jumpTarget;

public:
	Node(int x, int y, MOVEMENTACTION actionToReach, JUMP_TARGET jumpTarget);
	Node(int x, int y, MOVEMENTACTION actionToReach);
	Node(int x, int y);
	Node();

	int GetX();
	int GetY();
	MOVEMENTACTION GetActionToReach();
	JUMP_TARGET GetJumpTarget();

};