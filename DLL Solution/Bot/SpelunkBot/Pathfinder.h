#pragma once
#include "IBot.h"
#include "MapSearchNode.h"
#include "Node.h"
#include <map>

class Pathfinder
{
public:
	Pathfinder(IBot* bot);
	~Pathfinder();

	std::vector<Node> CalculateNeighboursList(Node node, MvState mvstate);
	std::vector<MapSearchNode*> CalculateNeighboursList(MapSearchNode* node, MvState mvstate);
	//std::vector<MapSearchNode*> CalculateNeighboursList(MapSearchNode* node);
	//std::vector<Node> CalculateNeighboursList(MapSearchNode* node);
	bool HorizontalJumpPathClear(int x, int y, int dist, bool right);
	bool isCloseToFog(int x, int y, int closeness);
	bool isCloseToFog(MapSearchNode *n, int closeness);
	Node toNode(MapSearchNode *n);
	std::vector<Node> FindExplorationTargets(double x1, double y1, double usingPixelCoords);
	bool CalculatePath(double x1, double y1, double x2, double y2, double usingPixelCoords);
	void NeighboursDebug(int x, int y);
	std::vector<MapSearchNode*> GetPathList();
	std::vector<Node> GetPathListNode();
	bool IsOutOfBounds(int x, int y);

	bool CanStandInNode(int x, int y);
	MvState ToMvState(SpState spstate);
	MvState GetCurrentMvState(Node *currentNode, Node *parentNode);
	JUMP_TARGET GetCurrentJumpTarget(Node *currentNode);

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

	MvState GetCurrentMvState2(MapSearchNode * currentNode);

	MvState GetCurrentMvState(MapSearchNode *currentNode);

	//for simplicity
	bool Pusta(int x, int y); //the node is passable
	bool Pelna(int x, int y); //the node is impassable
	bool Ladder(int x, int y);
	void AddNeighboursLR(int x, int y, bool right, std::vector<Node>* neighbours);
	//void AddNeighboursLRleft(int x, int y, std::vector<Node>* neighbours);
};