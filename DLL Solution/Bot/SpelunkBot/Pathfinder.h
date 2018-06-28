#pragma once
#include "IBot.h"
#include "MapNode.h"
#include "Item.h"
#include "MovementSimulator.h"
#include <map>
#include <stack>
#include <functional>
#include <list>
#include <algorithm>
#include <fstream>
#include <ostream>



class Pathfinder
{

public:
	//Pathfinder(IBot* bot);
	Pathfinder(std::shared_ptr<IBot> bot, std::shared_ptr<MovementSimulator> msim);
	~Pathfinder();

	void NewLevel();
	void InitializeGrid();

	std::vector<MapNode> CalculateNeighbours(MapNode node, MVSTATE mvstate, ACTION_TARGET target);
	std::vector<MapNode*> CalculateNeighboursInGrid(MapNode* node, MVSTATE mvstate, ACTION_TARGET target);

	MapNode* GetNodeFromGrid(Coords c);

	//bool TryToCalculatePath(Coords c1, Coords c2);
	//bool TryToCalculatePathNoGrid(Coords c1, Coords c2);
	bool TryToCalculatePath(Coords c1, Coords c2, PATHFINDING_AT_MODE atmode,
		std::vector<Coords> allowedArrowTraps = std::vector<Coords>());
	std::vector<MapNode*> GetPathFromGrid();
	std::vector<MapNode> GetPathCopy();
	std::vector<MapNode> GetPath2();
	int GetPathLength(std::vector<MapNode*> path);
	int GetPathLength();

	bool TryToFindExplorationTarget(Coords startc, PATHFINDING_AT_MODE atmode,
		std::vector<Coords> allowedArrowTraps = std::vector<Coords>());
	Coords GetExplorationTarget();

	bool TryToFindTargetInNextCC(Coords startc, PATHFINDING_AT_MODE atmode,
		std::vector<Coords> allowedArrowTraps = std::vector<Coords>());
	Coords GetTargetInNextCC();

	void TryToFindExit();
	Coords GetExit();
	bool IsExitFound();

	void CalculateConnectedComponents(PATHFINDING_AT_MODE atmode,
		std::vector<Coords> allowedArrowTraps = std::vector<Coords>());
	std::vector<MapNode*> GetAllNodesFromCC(int ccnr);
	int GetCCnr(Coords coords);

	//void Dijkstra(Coords startc, bool stopOnArrowTraps = false);
	void Dijkstra(Coords startc, PATHFINDING_AT_MODE pfMode, std::vector<Coords> allowedArrowTraps = std::vector<Coords>());
	std::vector<MapNode*> GetDijPath(Coords targetC);
	int GetDijDist(Coords c);

	MapNode GetSafeNodeToDisarmAT(MapNode candidate, Coords arrowTrapToDisarm);

	bool isCloseToFog(Coords c, int closeness);
	bool IsOutOfBounds(Coords c);
	bool CanStandInNode(Coords c);
	bool CanHangInNode(Coords c);
	bool IsNodeValidBotPosition(Coords c);
	bool IsInFog(Coords c);
	int CalculateDistance(Coords c1, Coords c2);
	bool IsFogOnMap();
	bool NoArrowTrapsOnPath(std::vector<MapNode*> path);
	directions CalculateDirections(Coords from, Coords to);
	DIRECTIONX CalculateDirX(Coords from, Coords to);
	DIRECTIONY CalculateDirY(Coords from, Coords to);

	Coords GetBotCoords();

	void NeighboursDebug(Coords c, bool hasMomentum);
	void SCCDebug();
	void DijkstraDebug(bool exact);

	//for simplicity
	bool Passable(int x, int y);
	bool Passable(Coords c);
	bool Impassable(int x, int y);
	bool Impassable(Coords c);
	bool Ladder(int x, int y);
	bool Ladder(Coords c);
	bool LadderTop(int x, int y);
	bool LadderTop(Coords c);
	bool Spikes(int x, int y);
	bool Spikes(Coords c);
	bool ArrowTrapR(int x, int y);
	bool ArrowTrapR(Coords c);
	bool ArrowTrapL(int x, int y);
	bool ArrowTrapL(Coords c);
	bool SpearTrap(int x, int y);
	bool SpearTrap(Coords c);

private:	
	std::shared_ptr<IBot> _bot;
	std::shared_ptr<MovementSimulator> _moveSim;
	//std::map<int, std::map<int, MapNode*> > _grid;
	std::vector<std::vector<MapNode*> > _grid;

	std::vector<MapNode*> _pathList;
	std::vector<MapNode> _pathList2;
	Coords _explorationTarget;
	Coords _nextCCTarget;

	bool _exitFound = false;
	Coords _exit;

	//used by neighbour searching
	bool DownJumpPathClear(int x1, int y1, int x2, int y2, DIRECTIONX direction, bool fromLadder);
	bool DownJumpPathClear(int x1, int y1, int x2, int y2, DIRECTIONX direction);
	bool WalkOffLedgePathClear(int x1, int y1, int x2, int y2, DIRECTIONX direction);
	bool HorizontalJumpPathClear(int x, int y, int dist, DIRECTIONX direction);


	//neighbours
	std::vector<MapNode> CalculateNeighboursHanging(MapNode node);
	std::vector<MapNode> CalculateNeighboursClimbing(MapNode node);
	std::vector<MapNode> CalculateNeighboursClimbingWithMomentum(MapNode node);
	std::vector<MapNode> CalculateNeighboursStanding(MapNode node);
	std::vector<MapNode> CalculateNeighboursStandingLT(MapNode node);
	void AddNeighboursLR(int x, int y, bool right, std::vector<MapNode> *neighbours);
	std::vector<MapNode> RemoveDuplicateNeighbours(std::vector<MapNode> neighboursTemp);
	std::vector<MapNode> RemoveNeighboursWithArrowTraps(
		std::vector<MapNode> neighboursTemp, 
		PATHFINDING_AT_MODE atmode, 
		std::vector<Coords> allowedArrowTraps = std::vector<Coords>());
	std::vector<MapNode*> ConvertNeighboursToGrid(std::vector<MapNode> neighbours);

	//traps
	void HandleUnsafeNeighbours(MapNode origin, std::vector<MapNode> &neighbours);
	void DeleteUnsafeNeighboursSpikes(MapNode origin, std::vector<MapNode>& neighbours);
	void HandleUnsafeNeighboursSpearTrap(MapNode origin, std::vector<MapNode>& neighbours);
	void HandleUnsafeNeighboursArrowTrap(MapNode origin, std::vector<MapNode>& neighbours);

	//tarjan's algorithm
	int _tar_cvn;
	int _tar_CCnr;
	int _tar_VN[X_NODES][Y_NODES];
	int _tar_VLow[X_NODES][Y_NODES];
	bool _tar_VS[X_NODES][Y_NODES];
	std::stack<MapNode*> _tar_S;
	std::vector<std::vector<MapNode*>> _tar_CClist;
	std::map<int, std::vector<MapNode*>> _tar_CCmap;
	void TarjanDFS(MapNode* n, PATHFINDING_AT_MODE pfMode, std::vector<Coords> allowedArrowTraps = std::vector<Coords>());

	//dijkstra's algorithm
	struct _dij_MSNodeCmp
	{
		bool operator() (MapNode* n1, MapNode* n2)
		{
			return n1->_dij_dist > n2->_dij_dist;
			//return _dij_dists[n1->_x][n1->_y] < _dij_dists[n1->_x][n1->_y];
		}
	};
	std::priority_queue<MapNode*, std::vector<MapNode*>, _dij_MSNodeCmp> _dij_pQ;
	//static int _dij_dists[X_NODES][Y_NODES];
	bool _dij_visited[X_NODES][Y_NODES];
	static int _dij_prev[X_NODES][Y_NODES];




	std::vector<Coords> GetPathBetweenNodes(Coords start, Coords end, MOVEMENTACTION used_action);
};