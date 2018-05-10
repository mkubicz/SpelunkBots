#pragma once
#include "IBot.h"
#include "MapSearchNode.h"
#include "Node.h"
#include <map>
#include <stack>

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

	bool isCloseToFog(int x, int y, int closeness);
	bool isCloseToFog(MapSearchNode *n, int closeness);
	bool IsOutOfBounds(int x, int y);
	bool CanStandInNode(int x, int y);
	bool CanStandInNode(MapSearchNode * n);
	bool IsInFog(int x, int y);
	bool IsInFog(MapSearchNode * n);
	bool IsFogOnMap();

	void NeighboursDebug(int x, int y, bool hasMomentum);
	void SCCDebug();

private:	
	IBot* _bot;
	std::map<int, std::map<int, MapSearchNode*> > _grid;

	std::vector<MapSearchNode*> _pathList;
	Node _explorationTarget;

	bool _exitFound = false;
	Node _exit;

	bool DownJumpPathClear(int x1, int y1, int x2, int y2, DIRECTIONX direction);
	bool WalkOffLedgePathClear(int x1, int y1, int x2, int y2, DIRECTIONX direction);
	bool HorizontalJumpPathClear(int x, int y, int dist, DIRECTIONX direction);

	std::vector<Node> CalculateNeighboursHanging(Node node);
	std::vector<Node> CalculateNeighboursClimbing(Node node);
	std::vector<Node> CalculateNeighboursStanding(Node node);
	std::vector<Node> CalculateNeighboursStandingLT(Node node);
	std::vector<Node> CalculateNeighboursClimbingWithMomentum(Node node);

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

	//for simplicity
	bool Pusta(int x, int y); //node is passable
	bool Pelna(int x, int y); //node is impassable
	bool Ladder(int x, int y); //node contains a ladder
	bool Ladder(MapSearchNode* n); //node contains a ladder
	bool LadderTop(int x, int y); //node contains a laddertop
	void AddNeighboursLR(int x, int y, bool right, std::vector<Node>* neighbours);
};