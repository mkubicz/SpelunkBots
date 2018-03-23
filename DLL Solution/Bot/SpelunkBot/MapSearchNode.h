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

	MVSTATE _mvState = mvSTANDING;
	MVSTATE _mvStateCandidate = mvSTANDING;

	bool _opened;
	bool _closed;

	//used in FindExplorationTargets
	bool _visited = false;

public:
	int GetX();
	int GetY();
	double GetGScore(MapSearchNode *p);
	double GetHScore(MapSearchNode *p);
	double GetFScore();
	MOVEMENTACTION GetActionToReach();
	ACTION_TARGET GetActionTarget();
	MVSTATE GetMvState();

	void ComputeScores(MapSearchNode * end);
}; 
