#pragma once

class MapSearchNode
{

friend class Pathfinder;

private:
	int _x;
	int _y;
	int _gScore;
	int _hScore;
	//int _fScore;
	MapSearchNode* _parent;

	MOVEMENTACTION _actionToReach;
	MOVEMENTACTION _actionToReachCandidate;

	ACTION_TARGET _actionTarget;
	ACTION_TARGET _actionTargetCandidate;

	MVSTATE _mvState;
	MVSTATE _mvStateCandidate;

	bool _opened;
	bool _closed;

	//used in FindExplorationTargets
	bool _visited = false;

	//result of tarjan's algorithm
	int _CCnr;

	//result of djikstra's algorithm
	int _dij_dist;
	MapSearchNode *_dij_prev;

public:
	MapSearchNode();
	MapSearchNode(int x, int y);
	int GetX();
	int GetY();
	double CalculateGScore(MapSearchNode *parentCandidate);
	double CalculateHScore(MapSearchNode *destination);
	double CalculateFScore();
	MOVEMENTACTION GetActionToReach();
	ACTION_TARGET GetActionTarget();
	MVSTATE GetMvState();

	void ComputeScores(MapSearchNode * destination);
}; 
