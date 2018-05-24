#pragma once
#include "IBot.h"
#include "MapSearchNode.h"
#include "Node.h"
#include <map>
#include <stack>
#include <functional>

class Pathfinder
{
public:
	Pathfinder(IBot* bot);
	~Pathfinder();

	void InitializeGrid();

	std::vector<MapSearchNode*> CalculateNeighboursList(MapSearchNode* node, MVSTATE mvstate, ACTION_TARGET target);
	std::vector<Node> CalculateNeighboursList(Node node, MVSTATE mvstate, ACTION_TARGET target);

	MapSearchNode* GetNodeFromGrid(int x, int y);
	
	bool TryToCalculatePath(int x1, int y1, int x2, int y2);
	std::vector<MapSearchNode*> GetPathList();
	std::vector<Node> GetPathListNode();
	int GetPathLength(std::vector<Node> path);
	int GetPathLength();

	bool TryToFindExplorationTarget(int x, int y);
	Node GetExplorationTarget();
	std::vector<Node> FindExplorationTargets(int x1, int y1);

	void TryToFindExit();
	Node GetExit();
	bool IsExitFound();

	void CalculateConnectedComponents();
	std::vector<MapSearchNode*> GetAllNodesFromCC(int ccnr);
	int GetCCnr(int nodeX, int nodeY);

	void Dijkstra(int x, int y);
	int GetDijDist(int x, int y);

	bool isCloseToFog(int x, int y, int closeness);
	bool isCloseToFog(MapSearchNode *n, int closeness);
	bool IsOutOfBounds(int x, int y);
	bool CanStandInNode(int x, int y);
	bool CanStandInNode(MapSearchNode * n);
	bool CanHangInNode(MapSearchNode * n);
	bool IsNodeValidBotPosition(MapSearchNode * n);
	bool IsInFog(int x, int y);
	bool IsInFog(MapSearchNode * n);
	bool IsFogOnMap();
	int CalculateDistance(MapSearchNode *n, MapSearchNode *m);
	int CalculateDistance(Node * n, Node * m);

	void NeighboursDebug(int x, int y, bool hasMomentum);
	void SCCDebug();
	void DijkstraDebug();

private:	
	IBot* _bot;
	std::map<int, std::map<int, MapSearchNode*> > _grid;

	std::vector<MapSearchNode*> _pathList;
	Node _explorationTarget;

	bool _exitFound = false;
	Node _exit;

	//used by neighbour searching
	bool DownJumpPathClear(int x1, int y1, int x2, int y2, DIRECTIONX direction);
	bool WalkOffLedgePathClear(int x1, int y1, int x2, int y2, DIRECTIONX direction);
	bool HorizontalJumpPathClear(int x, int y, int dist, DIRECTIONX direction);

	//neighbours
	std::vector<Node> CalculateNeighboursHanging(Node node);
	std::vector<Node> CalculateNeighboursClimbing(Node node);
	std::vector<Node> CalculateNeighboursClimbingWithMomentum(Node node);
	std::vector<Node> CalculateNeighboursStanding(Node node);
	std::vector<Node> CalculateNeighboursStandingLT(Node node);
	void AddNeighboursLR(int x, int y, bool right, std::vector<Node>* neighbours);

	//traps
	void DeleteUnsafeNeighbours(Node origin, std::vector<Node> &neighbours);
	void DeleteUnsafeNeighboursSpikes(Node origin, std::vector<Node>& neighbours);
	void DeleteUnsafeNeighboursTikiTrap(Node origin, std::vector<Node>& neighbours);
	void DeleteUnsafeNeighboursArrowTrap(Node origin, std::vector<Node>& neighbours);

	//tarjan's algorithm
	int _tar_cvn;
	int _tar_CCnr;
	int _tar_VN[X_NODES][Y_NODES];
	int _tar_VLow[X_NODES][Y_NODES];
	bool _tar_VS[X_NODES][Y_NODES];
	std::stack<MapSearchNode*> _tar_S;
	std::vector<std::vector<MapSearchNode*>> _tar_CClist;
	std::map<int, std::vector<MapSearchNode*>> _tar_CCmap;
	void TarjanDFS(MapSearchNode* n);

	//dijkstra's algorithm
	struct _dij_MSNodeCmp
	{
		bool operator() (MapSearchNode* n1, MapSearchNode* n2)
		{
			return n1->_dist > n2->_dist;
			//return _dij_dists[n1->_x][n1->_y] < _dij_dists[n1->_x][n1->_y];
		}
	};
	std::priority_queue<MapSearchNode*, std::vector<MapSearchNode*>, _dij_MSNodeCmp> _dij_pQ;
	//static int _dij_dists[X_NODES][Y_NODES];
	bool _dij_visited[X_NODES][Y_NODES];
	static int _dij_prev[X_NODES][Y_NODES];


	//for simplicity
	bool Pusta(int x, int y); //node is passable
	bool Pelna(int x, int y); //node is impassable
	bool Ladder(int x, int y); //node contains a ladder
	bool Ladder(MapSearchNode* n); //node contains a ladder
	bool LadderTop(int x, int y); //node contains a laddertop
	bool Spikes(int x, int y);
	bool ArrowTrapR(int x, int y);
	bool ArrowTrapL(int x, int y);
	bool TikiTrap(int x, int y);

	std::vector<Node> GetPathBetweenNodes(Node start, Node end, MOVEMENTACTION used_action);
};