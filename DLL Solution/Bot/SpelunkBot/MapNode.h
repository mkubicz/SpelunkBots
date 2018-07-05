#pragma once
#include "Coords.h"

class MapNode
{
	friend class Pathfinder;

private:
	Coords _coords;
	double _gScore;
	double _hScore;
	MapNode* _parent;

	MOVEMENTACTION _actionToReach;
	MOVEMENTACTION _actionToReachCandidate;
	ACTION_TARGET _actionToReachTarget;
	ACTION_TARGET _actionToReachTargetCandidate;
	MVSTATE _mvState;
	MVSTATE _mvStateCandidate;
	bool _arrowTrapOnWay;
	bool _arrowTrapOnWayCandidate;
	Coords _arrowTrapCoords;
	Coords _arrowTrapCoordsCandidate;

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

	void Reset();

	int GetX();
	int GetY();
	Coords GetCoords();
	MOVEMENTACTION GetActionToReach();
	ACTION_TARGET GetPrevActionTarget();
	MVSTATE GetMvState();
	bool IsArrowTrapOnWay();
	int GetPenalty();
	Coords GetArrowTrapCoords();
	MapNode *GetDijParent();

	//void CopyInfo(MapNode n);
	void SolidifyCandidates();

	//used in A*
	double CalculateGScore(MapNode *parentCandidate);
	double CalculateHScore(MapNode *destination);
	double CalculateFScore();
	void ComputeScores(MapNode * destination);
	
	inline bool operator==(const MapNode& n)
	{
		return
			this->_coords == n._coords;// &&
			//this->_actionToReach == n._actionToReach &&
			//this->_actionToReachTarget == n._actionToReachTarget &&
			//this->_mvState == n._mvState;
	};

	inline bool operator!=(const MapNode& n)
	{
		return !(*this == n);
	};
};