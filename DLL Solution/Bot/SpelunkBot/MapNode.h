#pragma once
#include "Coords.h"

class MapNode
{
	friend class Pathfinder;

private:
	Coords _coords;
	int _gScore;
	int _hScore;
	MapNode* _parent;

	MOVEMENTACTION _actionToReach;
	MOVEMENTACTION _actionToReachCandidate;
	ACTION_TARGET _actionToReachTarget;
	ACTION_TARGET _actionToReachTargetCandidate;
	MVSTATE _mvState;
	MVSTATE _mvStateCandidate;

	//used in A*
	bool _opened;
	bool _closed;

	//used in FindExplorationTargets
	bool _visited;

	//result of tarjan's algorithm
	int _CCnr;

	//result of djikstra's algorithm
	int _dij_dist;
	MapNode *_dij_prev;

public:
	MapNode(int x, int y, MOVEMENTACTION actionToReach, ACTION_TARGET jumpTarget, MVSTATE mvState);
	MapNode(int x, int y, MOVEMENTACTION actionToReach, MVSTATE mvState);
	MapNode(int x, int y, MOVEMENTACTION actionToReach, ACTION_TARGET jumpTarget);
	MapNode(int x, int y, MOVEMENTACTION actionToReach);
	MapNode(int x, int y);
	MapNode();

	int GetX();
	int GetY();
	Coords GetCoords();
	MOVEMENTACTION GetActionToReach();
	ACTION_TARGET GetActionTarget();
	MVSTATE GetMvState();

	//used in A*
	double CalculateGScore(MapNode *parentCandidate);
	double CalculateHScore(MapNode *destination);
	double CalculateFScore();
	void ComputeScores(MapNode * destination);

};