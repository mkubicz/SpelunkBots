#pragma once
#include "stdafx.h"
#include "IBot.h"
#include "MapSearchNode.h"
#include <map>

class Pathfinder
{
public:
	Pathfinder(IBot* bot);
	~Pathfinder();

	std::vector<MapSearchNode*> CalculateNeighboursList(MapSearchNode* node, SpState spstate);
	std::vector<Node> CalculateNeighboursList(Node node, SpState spstate);
	std::vector<MapSearchNode*> CalculateNeighboursList(MapSearchNode* node);
	std::vector<Node> CalculateNeighboursList(Node node);
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

private:	
	IBot* _bot; 
	std::vector<MapSearchNode*> _pathList;
	std::map<int, std::map<int, MapSearchNode*> > _grid;

	bool DownJumpPathClear(int x1, int y1, int x2, int y2, bool right);
	bool WalkOffLedgePathClear(int x1, int y1, int x2, int y2, bool right);

	std::vector<Node> CalculateNeighboursHanging(Node node);
	std::vector<Node> CalculateNeighboursClimbing(Node node);
	std::vector<Node> CalculateNeighboursStanding(Node node);

	SpState PredictCurrentState(int x, int y);

	//for simplicity
	bool Pusta(int x, int y); //the node is passable
	bool Pelna(int x, int y); //the node is impassable
	bool Ladder(int x, int y);
};