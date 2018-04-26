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

	void InitializeVariables();

	std::vector<Node> CalculateNeighboursList(Node node, MVSTATE mvstate);
	std::vector<MapSearchNode*> CalculateNeighboursList(MapSearchNode* node, MVSTATE mvstate);
	//std::vector<MapSearchNode*> CalculateNeighboursList(MapSearchNode* node);
	//std::vector<Node> CalculateNeighboursList(MapSearchNode* node);
	MapSearchNode* GetNodeFromGrid(int x, int y);
	bool HorizontalJumpPathClear(int x, int y, int dist, bool right);
	bool isCloseToFog(int x, int y, int closeness);
	bool isCloseToFog(MapSearchNode *n, int closeness);
	Node toNode(MapSearchNode *n);
	std::vector<Node> FindExplorationTargets(double x1, double y1, double usingPixelCoords);
	bool TryToCalculatePath(double x1, double y1, double x2, double y2, double usingPixelCoords);
	void NeighboursDebug(int x, int y);
	void SCCDebug();
	std::vector<MapSearchNode*> GetPathList();
	std::vector<Node> GetPathListNode();
	int GetPathLength();
	int GetPathLength(std::vector<Node> path);
	bool IsOutOfBounds(int x, int y);

	bool CanStandInNode(int x, int y);
	bool CanStandInNode(MapSearchNode * n);
	bool IsInFog(int x, int y);
	bool IsInFog(MapSearchNode * n);
	MVSTATE ToMvState(SpState spstate);
	MVSTATE GetCurrentMvState(Node *currentNode, Node *parentNode);
	ACTION_TARGET GetCurrentJumpTarget(Node *currentNode);

	void CalculateConnectedComponents();

	//bool PathReady();
	//std::vector<Node>* GetNextPath();

private:	
	IBot* _bot; 
	std::vector<MapSearchNode*> _pathList;
	std::map<int, std::map<int, MapSearchNode*> > _grid;

	bool DownJumpPathClear(int x1, int y1, int x2, int y2, DIRECTIONX direction);
	bool WalkOffLedgePathClear(int x1, int y1, int x2, int y2, DIRECTIONX direction);

	std::vector<Node> CalculateNeighboursHanging(Node node);
	std::vector<Node> CalculateNeighboursClimbing(Node node);
	std::vector<Node> CalculateNeighboursStanding(Node node);
	std::vector<Node> CalculateNeighboursClimbingWithMomentum(Node node);

	MVSTATE GetCurrentMvState(MapSearchNode *currentNode);

	//tarjan's algorithm
	int _tar_cvn;
	int _tar_CCnr;
	int _tar_VN[X_NODES][Y_NODES];
	int _tar_VLow[X_NODES][Y_NODES];
	bool _tar_VS[X_NODES][Y_NODES];
	std::stack<MapSearchNode*> _tar_S;
	std::vector<std::vector<MapSearchNode*>> _tar_Lscc;
	void TarjanDFS(MapSearchNode* n);

	//for simplicity
	bool Pusta(int x, int y); //node is passable
	bool Pelna(int x, int y); //node is impassable
	bool Ladder(int x, int y); //node contains a ladder
	bool Ladder(MapSearchNode* n); //node contains a ladder
	void AddNeighboursLR(int x, int y, bool right, std::vector<Node>* neighbours);
};