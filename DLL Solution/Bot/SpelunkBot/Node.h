#pragma once

class Node
{
private:
	int _x;
	int _y;

	MOVEMENTACTION _actionToReach;
	ACTION_TARGET _actionTarget;
	MVSTATE _mvState;

public:
	Node(int x, int y, MOVEMENTACTION actionToReach, ACTION_TARGET jumpTarget, MVSTATE mvState);
	Node(int x, int y, MOVEMENTACTION actionToReach, MVSTATE mvState);
	Node(int x, int y, MOVEMENTACTION actionToReach, ACTION_TARGET jumpTarget);
	Node(int x, int y, MOVEMENTACTION actionToReach);
	Node(int x, int y);
	Node();

	int GetX();
	int GetY();
	MOVEMENTACTION GetActionToReach();
	ACTION_TARGET GetActionTarget();
	MVSTATE GetMvState();

};