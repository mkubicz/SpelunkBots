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

	std::vector<MapSearchNode*> CalculateNeighboursList(MapSearchNode* node, std::map<int, std::map<int, MapSearchNode*>> grid);
	bool CalculatePath(double x1, double y1, double x2, double y2, double usingPixelCoords);
	void NeighboursDebug(int x, int y);
	std::vector<MapSearchNode*> GetPathList();

	bool CanStandInNode(int x, int y);

private:	
	IBot* _bot; 
	std::vector<MapSearchNode*> _pathList;

	bool DownJumpPathClear(int x1, int y1, int x2, int y2, bool right);

	//for simplicity
	bool Pusta(int x, int y); //the node is passable
	bool Pelna(int x, int y); //the node is impassable
};