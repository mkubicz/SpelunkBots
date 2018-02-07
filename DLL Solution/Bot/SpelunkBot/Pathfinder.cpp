#include "stdafx.h"
#include "Pathfinder.h"
#include <vector>
#include <list>
#include <algorithm>
#include <fstream>
#include <ostream>

using namespace std;

Pathfinder::Pathfinder(IBot* bot)
{
	_bot = bot;
}

Pathfinder::~Pathfinder()
{
}


bool Pathfinder::Pusta(int x, int y)
{
	return _bot->IsNodePassable(x, y, NODE_COORDS);
}


bool Pathfinder::Pelna(int x, int y)
{
	return !Pusta(x,y);
}

bool Pathfinder::Ladder(int x, int y)
{
	return _bot->GetNodeState(x, y, NODE_COORDS) == spLadder;
}



vector<Node> Pathfinder::CalculateNeighboursList(Node node)
{
	int x = node.x;
	int y = node.y;
	vector<Node> neighbours;
	int MAX;

	/*
	* UP I
	*/
	/*
	if (Pelna(x, y - 1) && Pusta(x, y - 2) && ((Pusta(x - 1, y) && Pusta(x - 1, y - 1) && Pusta(x - 1, y - 2)) || (Pusta(x + 1, y) && Pusta(x + 1, y - 1) && Pusta(x + 1, y - 2))))
		neighbours.push_back(Node {x,y-2});
	*/
	if (Pelna(x, y - 1) && Pusta(x, y - 2))
	{
		//right
		if (Pusta(x + 1, y) && Pusta(x + 1, y - 1) && Pusta(x + 1, y - 2))
			neighbours.push_back(Node{ x, y - 2, JUMPABOVERIGHT });
		//left
		if (Pusta(x - 1, y) && Pusta(x - 1, y - 1) && Pusta(x - 1, y - 2)) 
			neighbours.push_back(Node{ x, y - 2, JUMPABOVELEFT });
	}

	/*
	* UP II
	*/
	if (Pusta(x, y - 1) && Pusta(x, y - 2))
	{
		//right
		if (Pusta(x + 1, y - 2) && Pelna(x + 1, y - 1))
		{
			if (Pusta(x + 1, y)) 
				neighbours.push_back(Node{ x + 1, y - 2, JUMPUPRIGHT });
			else
				neighbours.push_back(Node{ x + 1, y - 2, WALKUPRIGHT });
		}
		//left
		if (Pusta(x - 1, y - 2) && Pelna(x - 1, y - 1))
		{
			if (Pusta(x - 1, y))
				neighbours.push_back(Node{ x - 1, y - 2, JUMPUPLEFT });
			else
				neighbours.push_back(Node{ x - 1, y - 2, WALKUPLEFT });
		}
	}


	/*
	* UP III (max=4 - possible, max=3 - safe)
	*/
	MAX = 3; //safe option for now

	//right
	for (int i = 0; i <= MAX; i++)
	{
		if (Pusta(x + i, y) && Pusta(x + i, y - 1) && Pusta(x + i + 1, y - 2))
		{
			if (Pelna(x + i + 1, y - 1))
			{
				if (i != 0) //i==0 case is handled by UP II
					neighbours.push_back(Node { x + i + 1, y - 2, JUMPUPRIGHT });

				break;
			}
		}
		else break;
	}
	//left
	for (int i = 0; i >= -MAX; i--)
	{
		if (Pusta(x + i, y) && Pusta(x + i, y - 1) && Pusta(x + i - 1, y - 2))
		{
			if (Pelna(x + i - 1, y - 1))
			{
				if (i != 0) //i==0 case is handled by UP II
					neighbours.push_back(Node { x + i - 1, y - 2, JUMPUPLEFT });

				break;
			}
		}
		else break;
	}


	/*
	* UP IV  (max=5 - possible, max=4 - safe)
	*/
	MAX = 4;
	//right
	for (int i = 0; i <= MAX; i++)
	{
		if (Pusta(x + i, y) && Pusta(x + i, y - 1) && Pusta(x + i + 1, y - 1) && (i <= 1 || Pusta(x + i, y - 2)))
		{
			if (Pelna(x + i + 1, y))
			{
				if (i == 0)
					neighbours.push_back(Node{ x + i + 1, y - 1, WALKUPRIGHT });
				else
					neighbours.push_back(Node{ x + i + 1, y - 1, JUMPUPRIGHT });
				break;
			}
		}
		else break;
	}

	//left
	for (int i = 0; i >= -MAX; i--)
	{
		if (Pusta(x + i, y) && Pusta(x + i, y - 1) && Pusta(x + i - 1, y - 1) && (i <= 1 || Pusta(x + i, y - 2)))
		{
			if (Pelna(x + i - 1, y))
			{
				if (i == 0)
					neighbours.push_back(Node{ x + i - 1, y - 1, WALKUPLEFT });
				else
					neighbours.push_back(Node{ x + i - 1, y - 1, JUMPUPLEFT });
				break;
			}
		}
		else break;
	}


	/*
	* L/R I i II
	*/
	MAX = 6;

	//right
	int candidateXright = 0;

	for (int i = 1; i <= MAX; i++)
		if (Pelna(x + i, y + 1) && Pusta(x + i, y))
		{
			candidateXright = i;
			break;
		}

	if (candidateXright != 0)
	{
		int borderY = 0;
		if (candidateXright <= 2) borderY = 0;
		if (candidateXright > 2 && candidateXright <= 4) borderY = 1;
		if (candidateXright > 4) borderY = 2;

		bool ok = true;
		for (int i = 1; i < candidateXright; i++)
		{
			for (int j = 0; j <= borderY; j++)
			{
				if (Pelna(x + i, y - j)) ok = false;
			}
		}
		if (ok)
		{
			if (candidateXright == 1)
				neighbours.push_back(Node{ x + candidateXright, y, WALKRIGHT });
			else if (candidateXright == 2)
				neighbours.push_back(Node{ x + candidateXright, y, RUNRIGHT });
			else
				neighbours.push_back(Node{ x + candidateXright, y, JUMPUPRIGHT });
		}
			
	}


	//left
	int candidateXleft = 0;

	for (int i = 1; i <= MAX; i++)
		if (Pelna(x - i, y + 1) && Pusta(x - i, y))
		{
			candidateXleft = i;
			break;
		}

	if (candidateXleft != 0)
	{
		int borderY = 0;
		if (candidateXleft <= 2) borderY = 0;
		if (candidateXleft > 2 && candidateXleft <= 4) borderY = 1;
		if (candidateXleft > 4) borderY = 2;

		bool ok = true;
		for (int i = 1; i < candidateXleft; i++)
		{
			for (int j = 0; j <= borderY; j++)
			{
				if (Pelna(x - i, y - j)) ok = false;
			}
		}
		if (ok)
		{
			if (candidateXleft == 1)
				neighbours.push_back(Node{ x - candidateXleft, y, WALKLEFT });
			else if (candidateXleft == 2)
				neighbours.push_back(Node{ x - candidateXleft, y, RUNLEFT });
			else
				neighbours.push_back(Node{ x - candidateXleft, y, JUMPUPLEFT });
		}
	}


	/*
	* DOWN hangdrop
	*/
	MAX = 9; //max distance to ground tile
	//right
	if (Pusta(x + 1, y) && Pusta(x + 1, y + 1))
	{
		for (int j = 2; j <= MAX; j++)
		{
			if (Pelna(x + 1, y + j) && y + j < Y_NODES)
			{
				neighbours.push_back(Node { x + 1, y + j - 1, HANGDROPRIGHT });
				break;
			}
		}
	}

	//left
	if (Pusta(x - 1, y) && Pusta(x - 1, y + 1))
	{
		for (int j = 2; j <= MAX; j++)
		{
			if (Pelna(x - 1, y + j) && y + j < Y_NODES)
			{
				neighbours.push_back(Node { x - 1, y + j - 1, HANGDROPLEFT });
				break;
			}
		}
	}


	/*
	* DOWN jump
	*/
	int MAXx = 7;
	int MAXy = 5; //only 5 because we would take damage if we jumped further

	//right
	if (Pusta(x + 1, y + 1))
	{
		for (int i = 2; i <= MAXx; i++)
			for (int j = 1; j <= MAXy; j++)
				if (Pelna(x + i, y + j + 1) && Pusta(x + i, y + j) && DownJumpPathClear(x, y, x + i, y + j, RIGHT))
					neighbours.push_back(Node { x + i, y + j, JUMPDOWNRIGHT });
	}

	//left
	if (Pusta(x - 1, y + 1))
	{
		for (int i = 2; i <= MAXx; i++)
			for (int j = 1; j <= MAXy; j++)
				if (Pelna(x - i, y + j + 1) && Pusta(x - i, y + j) && DownJumpPathClear(x, y, x - i, y + j, LEFT))
					neighbours.push_back(Node { x - i, y + j, JUMPDOWNLEFT });
	}


	/*
	* DOWN walkOffLedge
	*/
	MAXx = 7;
	MAXy = 7;

	//right
	if (Pusta(x + 1, y + 1))
	{
		for (int i = 2; i <= MAXx; i++)
			for (int j = 1; j <= MAXy; j++)
			{
				if (i >= j + 3) continue; //fall is too steep
				else
					if (Pelna(x + i, y + j + 1) && Pusta(x + i, y + j) && WalkOffLedgePathClear(x, y, x + i, y + j, RIGHT))
						neighbours.push_back(Node{ x + i, y + j, WALKOFFLEDGERIGHT });
			}
				
	}

	//left
	if (Pusta(x - 1, y + 1))
	{
		for (int i = 2; i <= MAXx; i++)
			for (int j = 1; j <= MAXy; j++)
			{
				if (i >= j + 3) continue; //fall is too steep
				else
					if (Pelna(x - i, y + j + 1) && Pusta(x - i, y + j) && WalkOffLedgePathClear(x, y, x - i, y + j, LEFT))
						neighbours.push_back(Node{ x - i, y + j, WALKOFFLEDGELEFT });
			}

	}
	

	//delete any accidental out of bounds neighbours or neighbours in fog
	for (int i = 0; i < neighbours.size(); i++)
	{
		if (neighbours[i].x <= 0 || neighbours[i].x >= X_NODES-1 ||
			neighbours[i].y <= 0 || neighbours[i].y >= Y_NODES-1 ||
			_bot->GetFogState(neighbours[i].x, neighbours[i].y, NODE_COORDS) == 1 ||
			_bot->GetFogState(neighbours[i].x, neighbours[i].y+1, NODE_COORDS) == 1)
		{
			neighbours.erase(neighbours.begin()+i);
			i--;
		}
	}


	return neighbours;
}


#pragma region CalculateNeighbours with ladders
//
//vector<Node> Pathfinder::CalculateNeighboursList(Node node)
//{
//	int x = node.x;
//	int y = node.y;
//	vector<Node> neighbours;
//	int MAX;
//
//	/*
//	* UP I
//	*/
//	/*
//	if (Pelna(x, y - 1) && Pusta(x, y - 2) && ((Pusta(x - 1, y) && Pusta(x - 1, y - 1) && Pusta(x - 1, y - 2)) || (Pusta(x + 1, y) && Pusta(x + 1, y - 1) && Pusta(x + 1, y - 2))))
//	neighbours.push_back(Node {x,y-2});
//	*/
//	if (Pelna(x, y - 1) && Pusta(x, y - 2))
//	{
//		//right
//		if (Pusta(x + 1, y) && Pusta(x + 1, y - 1) && Pusta(x + 1, y - 2))
//			neighbours.push_back(Node{ x, y - 2, JUMPABOVERIGHT });
//		//left
//		if (Pusta(x - 1, y) && Pusta(x - 1, y - 1) && Pusta(x - 1, y - 2))
//			neighbours.push_back(Node{ x, y - 2, JUMPABOVELEFT });
//	}
//
//	//LADDER UP
//	if (Ladder(x, y - 1))
//	{
//		if (Ladder(x, y))
//			neighbours.push_back(Node{ x, y - 1, CLIMBUP });
//		else
//			neighbours.push_back(Node{ x, y - 1, JUMPUP_LADDER });
//	}
//	else if (Ladder(x, y - 2))
//		neighbours.push_back(Node{ x, y - 2, JUMPUP_LADDER });
//
//
//	/*
//	* UP II
//	*/
//	if (Pusta(x, y - 1) && Pusta(x, y - 2))
//	{
//		//right
//		if (Pusta(x + 1, y - 2) && Pelna(x + 1, y - 1))
//		{
//			if (Pusta(x + 1, y))
//				neighbours.push_back(Node{ x + 1, y - 2, JUMPUPRIGHT });
//			else
//				neighbours.push_back(Node{ x + 1, y - 2, WALKUPRIGHT });
//		}
//		//left
//		if (Pusta(x - 1, y - 2) && Pelna(x - 1, y - 1))
//		{
//			if (Pusta(x - 1, y))
//				neighbours.push_back(Node{ x - 1, y - 2, JUMPUPLEFT });
//			else
//				neighbours.push_back(Node{ x - 1, y - 2, WALKUPLEFT });
//		}
//
//
//		if (Ladder(x + 1, y - 2) && Ladder(x + 1, y - 1))
//			if (Pusta(x + 1, y))
//				neighbours.push_back(Node{ x + 1, y - 2, JUMPUPRIGHT_LADDER });
//
//		if (Ladder(x - 1, y - 2) && Ladder(x - 1, y - 1))
//			if (Pusta(x - 1, y))
//				neighbours.push_back(Node{ x - 1, y - 2, JUMPUPLEFT_LADDER });
//	}
//
//
//	/*
//	* UP III (max=4 - possible, max=3 - safe)
//	*/
//	MAX = 3; //safe option for now
//
//	//right
//	for (int i = 0; i <= MAX; i++)
//	{
//		if (Pusta(x + i, y) && Pusta(x + i, y - 1) && Pusta(x + i + 1, y - 2))
//		{
//			if (Pelna(x + i + 1, y - 1))
//			{
//				if (i != 0) //i==0 case is handled by UP II
//					neighbours.push_back(Node{ x + i + 1, y - 2, JUMPUPRIGHT });
//				break;
//			}
//
//			if (Ladder(x + i + 1, y - 1) && Ladder(x + i + 1, y - 2))
//			{
//				if (i != 0) //i==0 case is handled by UP II
//					neighbours.push_back(Node{ x + i + 1, y - 2, JUMPUPRIGHT_LADDER });
//				break;
//			}
//
//		}
//		else break;
//	}
//	//left
//	for (int i = 0; i >= -MAX; i--)
//	{
//		if (Pusta(x + i, y) && Pusta(x + i, y - 1) && Pusta(x + i - 1, y - 2))
//		{
//			if (Pelna(x + i - 1, y - 1))
//			{
//				if (i != 0) //i==0 case is handled by UP II
//					neighbours.push_back(Node{ x + i - 1, y - 2, JUMPUPLEFT });
//				break;
//			}
//
//			if (Ladder(x + i - 1, y - 1) && Ladder(x + i - 1, y - 2))
//			{
//				if (i != 0) //i==0 case is handled by UP II
//					neighbours.push_back(Node{ x + i - 1, y - 2, JUMPUPLEFT_LADDER });
//				break;
//			}
//		}
//		else break;
//	}
//
//
//	/*
//	* UP IV  (max=5 - possible, max=4 - safe)
//	*/
//	MAX = 4;
//	//right
//	for (int i = 0; i <= MAX; i++)
//	{
//		if (Pusta(x + i, y) && Pusta(x + i, y - 1) && Pusta(x + i + 1, y - 1) && (i <= 1 || Pusta(x + i, y - 2)))
//		{
//			if (Pelna(x + i + 1, y))
//			{
//				if (i == 0)
//					neighbours.push_back(Node{ x + i + 1, y - 1, WALKUPRIGHT });
//				else
//					neighbours.push_back(Node{ x + i + 1, y - 1, JUMPUPRIGHT });
//				break;
//			}
//
//			if (Ladder(x + i + 1, y) && Ladder(x + i + 1, y - 1))
//			{
//				neighbours.push_back(Node{ x + i + 1, y - 1, JUMPUPRIGHT_LADDER });
//				break;
//			}
//
//		}
//		else break;
//	}
//
//	//left
//	for (int i = 0; i >= -MAX; i--)
//	{
//		if (Pusta(x + i, y) && Pusta(x + i, y - 1) && Pusta(x + i - 1, y - 1) && (i <= 1 || Pusta(x + i, y - 2)))
//		{
//			if (Pelna(x + i - 1, y))
//			{
//				if (i == 0)
//					neighbours.push_back(Node{ x + i - 1, y - 1, WALKUPLEFT });
//				else
//					neighbours.push_back(Node{ x + i - 1, y - 1, JUMPUPLEFT });
//				break;
//			}
//
//			if (Ladder(x + i - 1, y) && Ladder(x + i - 1, y - 1))
//			{
//				neighbours.push_back(Node{ x + i - 1, y - 1, JUMPUPLEFT_LADDER });
//				break;
//			}
//
//		}
//		else break;
//	}
//
//
//	/*
//	* L/R I i II
//	*/
//	MAX = 6;
//
//	//right
//	int candidateDist = 0;
//	int candidateDistLadder = 0;
//
//	for (int i = 1; i <= MAX; i++)
//	{
//		if (Pelna(x + i, y + 1) && Pusta(x + i, y))
//		{
//			candidateDist = i;
//			break;
//		}
//
//		if (Ladder(x + i, y + 1) && Ladder(x + i, y))
//		{
//			candidateDistLadder = i;
//			break;
//		}
//	}
//
//	if (candidateDist != 0)
//	{
//		if (HorizontalJumpPathClear(x, y, candidateDist, RIGHT))
//		{
//			if (candidateDist == 1)
//				neighbours.push_back(Node{ x + candidateDist, y, WALKRIGHT });
//			else if (candidateDist == 2)
//				neighbours.push_back(Node{ x + candidateDist, y, RUNRIGHT });
//			else
//				neighbours.push_back(Node{ x + candidateDist, y, JUMPUPRIGHT });
//		}
//
//		if (HorizontalJumpPathClear(x, y, candidateDistLadder, RIGHT))
//		{
//			if (candidateDist >= 2)
//				neighbours.push_back(Node{ x + candidateDistLadder, y, JUMPUPRIGHT_LADDER });
//		}
//		
//	}
//
//
//	//left
//	candidateDist = 0;
//	candidateDistLadder = 0;
//
//	for (int i = 1; i <= MAX; i++)
//	{
//		if (Pelna(x - i, y + 1) && Pusta(x - i, y))
//		{
//			candidateDist = i;
//			break;
//		}
//
//		if (Ladder(x - i, y + 1) && Ladder(x - i, y))
//		{
//			candidateDistLadder = i;
//			break;
//		}
//	}
//
//	if (candidateDist != 0)
//	{
//		if (HorizontalJumpPathClear(x, y, candidateDistLadder, LEFT))
//		{
//			if (candidateDist == 1)
//				neighbours.push_back(Node{ x - candidateDist, y, WALKLEFT });
//			else if (candidateDist == 2)
//				neighbours.push_back(Node{ x - candidateDist, y, RUNLEFT });
//			else
//				neighbours.push_back(Node{ x - candidateDist, y, JUMPUPLEFT });
//		}
//
//		if (HorizontalJumpPathClear(x, y, candidateDistLadder, LEFT))
//		{
//			if (candidateDist >= 2)
//				neighbours.push_back(Node{ x + candidateDistLadder, y, JUMPUPLEFT_LADDER });
//		}
//	}
//
//
//	/*
//	* DOWN hangdrop
//	*/
//	MAX = 9; //max distance to ground tile
//			 //right
//	if (Pusta(x + 1, y) && Pusta(x + 1, y + 1))
//	{
//		for (int j = 2; j <= MAX; j++)
//		{
//			if (Pelna(x + 1, y + j) && y + j < Y_NODES)
//			{
//				neighbours.push_back(Node{ x + 1, y + j - 1, HANGDROPRIGHT });
//				break;
//			}
//		}
//	}
//
//	//left
//	if (Pusta(x - 1, y) && Pusta(x - 1, y + 1))
//	{
//		for (int j = 2; j <= MAX; j++)
//		{
//			if (Pelna(x - 1, y + j) && y + j < Y_NODES)
//			{
//				neighbours.push_back(Node{ x - 1, y + j - 1, HANGDROPLEFT });
//				break;
//			}
//		}
//	}
//
//
//	/*
//	* DOWN jump
//	*/
//	int MAXx = 7;
//	int MAXy = 5; //only 5 because we would take damage if we jumped further
//
//	//right
//	if (Pusta(x + 1, y + 1))
//	{
//		for (int i = 2; i <= MAXx; i++)
//			for (int j = 1; j <= MAXy; j++)
//			{
//				if (Pelna(x + i, y + j + 1) && Pusta(x + i, y + j) && DownJumpPathClear(x, y, x + i, y + j, RIGHT))
//					neighbours.push_back(Node{ x + i, y + j, JUMPDOWNRIGHT });
//				if (Ladder(x + i, y + j + 1) && Ladder(x + i, y + j) && DownJumpPathClear(x, y, x + i, y + j, RIGHT))
//					neighbours.push_back(Node{ x + i, y + j, JUMPDOWNRIGHT_LADDER });
//			}
//				
//	}
//
//	//left
//	if (Pusta(x - 1, y + 1))
//	{
//		for (int i = 2; i <= MAXx; i++)
//			for (int j = 1; j <= MAXy; j++)
//			{
//				if (Pelna(x - i, y + j + 1) && Pusta(x - i, y + j) && DownJumpPathClear(x, y, x - i, y + j, LEFT))
//					neighbours.push_back(Node{ x - i, y + j, JUMPDOWNLEFT });
//				if (Ladder(x - i, y + j + 1) && Ladder(x - i, y + j) && DownJumpPathClear(x, y, x - i, y + j, LEFT))
//					neighbours.push_back(Node{ x - i, y + j, JUMPDOWNLEFT_LADDER });
//			}
//				
//	}
//
//
//	/*
//	* DOWN walkOffLedge
//	*/
//	MAXx = 7;
//	MAXy = 7;
//
//	//right
//	if (Pusta(x + 1, y + 1))
//	{
//		for (int i = 2; i <= MAXx; i++)
//			for (int j = 1; j <= MAXy; j++)
//			{
//				if (i >= j + 3) continue; //fall is too steep
//				else
//				{
//					if (Pelna(x + i, y + j + 1) && Pusta(x + i, y + j) && WalkOffLedgePathClear(x, y, x + i, y + j, RIGHT))
//						neighbours.push_back(Node{ x + i, y + j, WALKOFFLEDGERIGHT });
//					if (Ladder(x + i, y + j + 1) && Ladder(x + i, y + j) && WalkOffLedgePathClear(x, y, x + i, y + j, RIGHT))
//						neighbours.push_back(Node{ x + i, y + j, WALKOFFLEDGERIGHT_LADDER });
//				}
//					
//			}
//
//	}
//
//	//left
//	if (Pusta(x - 1, y + 1))
//	{
//		for (int i = 2; i <= MAXx; i++)
//			for (int j = 1; j <= MAXy; j++)
//			{
//				if (i >= j + 3) continue; //fall is too steep
//				else
//				{
//					if (Pelna(x - i, y + j + 1) && Pusta(x - i, y + j) && WalkOffLedgePathClear(x, y, x - i, y + j, LEFT))
//						neighbours.push_back(Node{ x - i, y + j, WALKOFFLEDGELEFT });
//					if (Ladder(x - i, y + j + 1) && Ladder(x - i, y + j) && WalkOffLedgePathClear(x, y, x - i, y + j, LEFT))
//						neighbours.push_back(Node{ x - i, y + j, WALKOFFLEDGELEFT_LADDER });
//				}
//					
//			}
//
//	}
//
//
//	//delete any out of bounds neighbours
//	for (int i = 0; i < neighbours.size(); i++)
//	{
//		if (neighbours[i].x <= 0 || neighbours[i].x >= X_NODES ||
//			neighbours[i].y <= 0 || neighbours[i].y >= Y_NODES)
//		{
//			neighbours.erase(neighbours.begin() + i);
//			i--;
//		}
//	}
//
//
//	return neighbours;
//}

#pragma endregion


bool Pathfinder::HorizontalJumpPathClear(int x, int y, int dist, bool right)
{
	int borderY = 0;
	if (dist <= 2) borderY = 0;
	if (dist > 2 && dist <= 4) borderY = 1;
	if (dist > 4) borderY = 2;

	bool ok = true;

	for (int i = 1; i < dist; i++)
	{
		for (int j = 0; j <= borderY; j++)
		{

			if (right)
			{
				if (Pelna(x + i, y - j))
				{
					ok = false;
					break;
				}
			}
			else
			{
				if (Pelna(x - i, y - j))
				{
					ok = false;
					break;
				}
			}

		}

		if (!ok) break;
	}

	return ok;
}


vector<MapSearchNode*> Pathfinder::CalculateNeighboursList(MapSearchNode* node, std::map<int, std::map<int, MapSearchNode*>> grid)
{
	vector<MapSearchNode*> neighbours;

	vector<Node> n = CalculateNeighboursList(Node{ node->_x, node->_y });
	for (int i = 0; i < n.size(); i++)
	{
		neighbours.push_back(grid[n[i].x][n[i].y]);
	}

	return neighbours;
}

#pragma region CalculateNeighboursList on MapSearchNodes
//vector<MapSearchNode*> Pathfinder::CalculateNeighboursList(MapSearchNode* node, std::map<int, std::map<int, MapSearchNode*>> grid)
//{
//	int x = node->_x;
//	int y = node->_y;
//	vector<MapSearchNode*> neighbours;
//	int MAX;
//
//	/*
//	 * UP I
//	 */
//	if (Pelna(x, y - 1) && Pusta(x, y - 2) && ((Pusta(x - 1, y) && Pusta(x - 1, y - 1) && Pusta(x - 1, y - 2)) || (Pusta(x + 1, y) && Pusta(x + 1, y - 1) && Pusta(x + 1, y - 2))))
//		neighbours.push_back(grid[x][y - 2]);
//
//
//	/*
//	 * UP II
//	 */
//	if (Pusta(x, y - 1) && Pusta(x, y - 2))
//	{
//		//left
//		if (Pusta(x - 1, y - 2) && Pelna(x - 1, y - 1))
//			neighbours.push_back(grid[x - 1][y - 2]);
//		//right
//		if (Pusta(x + 1, y - 2) && Pelna(x + 1, y - 1))
//			neighbours.push_back(grid[x + 1][y - 2]);
//	}
//
//
//	/*
//	 * UP III (max=4 - possible, max=3 - safe)
//	 */
//	MAX = 3; //safe option for now
//	//right
//	for (int i = 0; i <= MAX; i++)
//	{
//		if (Pusta(x + i, y) && Pusta(x + i, y - 1) && Pusta(x + i + 1, y - 2))
//		{
//			if (Pelna(x + i + 1, y - 1))
//			{
//				neighbours.push_back(grid[x + i + 1][y - 2]);
//				break;
//			}
//		}
//		else break;
//	}
//	//left
//	for (int i = 0; i >= -MAX; i--)
//	{
//		if (Pusta(x + i, y) && Pusta(x + i, y - 1) && Pusta(x + i - 1, y - 2))
//		{
//			if (Pelna(x + i - 1, y - 1))
//			{
//				neighbours.push_back(grid[x + i - 1][y - 2]);
//				break;
//			}
//		}
//		else break;
//	}
//	
//
//	/*
//	 * UP IV  (max=5 - possible, max=4 - safe)
//	 */
//	MAX = 4;
//	//right
//	for (int i = 0; i <= MAX; i++)
//	{
//		if (Pusta(x + i, y) && Pusta(x + i, y - 1) && Pusta(x + i + 1, y - 1) && (i <= 1 || Pusta(x + i, y - 2)))
//		{
//			if (Pelna(x + i + 1, y))
//			{
//				neighbours.push_back(grid[x + i + 1][y-1]);
//				break;
//			}
//		}
//		else break;
//	}
//
//	//left
//	for (int i = 0; i >= -MAX; i--)
//	{
//		if (Pusta(x + i, y) && Pusta(x + i, y - 1) && Pusta(x + i - 1, y - 1) && (i <= 1 || Pusta(x + i, y - 2)))
//		{
//			if (Pelna(x + i - 1, y))
//			{
//				neighbours.push_back(grid[x + i - 1][y-1]);
//				break;
//			}
//		}
//		else break;
//	}
//
//
//	/*
//	 * L/R I i II
//	 */
//	MAX = 6;
//
//	//right
//	int candidateXright = 0;
//
//	for (int i = 1; i <= MAX; i++)
//		if (Pelna(x + i, y + 1) && Pusta(x + i, y))
//		{
//			candidateXright = i;
//			break;
//		}
//
//	if (candidateXright != 0)
//	{
//		int borderY = 0;
//		if (candidateXright <= 2) borderY = 0;
//		if (candidateXright > 2 && candidateXright <= 4) borderY = 1;
//		if (candidateXright > 4) borderY = 2;
//
//		bool ok = true;
//		for (int i = 1; i < candidateXright; i++)
//		{
//			for (int j = 0; j <= borderY; j++)
//			{
//				if (Pelna(x + i, y - j)) ok = false;
//			}
//		}
//		if (ok) neighbours.push_back(grid[x + candidateXright][y]);
//	}
//
//	
//	//left
//	int candidateXleft = 0;
//
//	for (int i = 1; i <= MAX; i++)
//		if (Pelna(x - i, y + 1) && Pusta(x - i, y))
//		{
//			candidateXleft = i;
//			break;
//		}
//
//	if (candidateXleft != 0)
//	{
//		int borderY = 0;
//		if (candidateXleft <= 2) borderY = 0;
//		if (candidateXleft > 2 && candidateXleft <= 4) borderY = 1;
//		if (candidateXleft > 4) borderY = 2;
//
//		bool ok = true;
//		for (int i = 1; i < candidateXleft; i++)
//		{
//			for (int j = 0; j <= borderY; j++)
//			{
//				if (Pelna(x - i, y - j)) ok = false;
//			}
//		}
//		if (ok) neighbours.push_back(grid[x - candidateXleft][y]);
//	}
//
//
//	/*
//	 * DOWN hangdrop
//	 */
//	MAX = 9; //max distance to ground tile
//	//right
//	if (Pusta(x + 1, y) && Pusta(x + 1, y + 1))
//	{
//		for (int j = 2; j <= MAX; j++)
//		{
//			if (Pelna(x + 1, y + j))
//			{
//				neighbours.push_back(grid[x + 1][y + j - 1]);
//				break;
//			}
//		}
//	}
//
//	//left
//	if (Pusta(x - 1, y) && Pusta(x - 1, y + 1))
//	{
//		for (int j = 2; j <= MAX; j++)
//		{
//			if (Pelna(x - 1, y + j))
//			{
//				neighbours.push_back(grid[x - 1][y + j - 1]);
//				break;
//			}
//		}
//	}
//
//
//	/*
//	* DOWN jump
//	*/
//	int MAXx = 7;
//	int MAXy = 5;
//
//	//right
//	if (Pusta(x + 1, y + 1))
//	{
//		for (int i = 2; i <= MAXx; i++)
//			for (int j = 1; j <= MAXy; j++)
//				if (Pelna(x + i, y + j + 1) && Pusta(x + i, y + j) && DownJumpPathClear(x, y, x + i, y + j, RIGHT))
//					neighbours.push_back(grid[x + i][y + j]);
//	}
//
//	//left
//	if (Pusta(x - 1, y + 1))
//	{
//		for (int i = 2; i <= MAXx; i++)
//			for (int j = 1; j <= MAXy; j++)
//				if (Pelna(x - i, y + j + 1) && Pusta(x - i, y + j) && DownJumpPathClear(x, y, x - i, y + j, LEFT))
//					neighbours.push_back(grid[x - i][y + j]);
//	}
//
//
//
//	return neighbours;
//}

#pragma endregion


//crude; to be used by GetNeighboursList
bool Pathfinder::DownJumpPathClear(int x1, int y1, int x2, int y2, bool right)
{
	int startX, startY;

	int distX = abs(x2 - x1);
	int distY = abs(y2 - y1);
	
	if (right)
	{
		//assumes that x2 > x1 && y2 > y1
		startX = x1 - 1;
		startY = y1 - 2;

		for (int j = startY; j <= y2; j++)
		{
			for (int i = startX; i <= x2; i++)
			{
				//in the row 2 tiles above spelunker
				if (j == startY)
				{
					//don't mind corner and close to corner tiles
					if (i == x1 - 1 || i == x1 || i == x2) continue;
					//if jump is close enough don't mind possible tiles above your head
					if ((distX - distY) <= 3) continue; 
				}

				if (Pelna(i, j)) return false;
			}

			if (startX != x2) startX += 1;
		}

		return true;
	}
	else
	{
		//assumes that x1 > x2 && y2 > y1
		startX = x1 + 1;
		startY = y1 - 2;

		for (int j = startY; j <= y2; j++)
		{
			for (int i = startX; i >= x2; i--)
			{
				//in the row 2 tiles above spelunker
				if (j == startY)
				{
					//don't mind corner and close to corner tiles
					if (i == x1 + 1 || i == x1 || i == x2) continue;
					//if jump is close enough don't mind possible tiles above your head
					if ((distX - distY) <= 3) continue;
				}

				if (Pelna(i, j)) return false;
			}

			if (startX != x2) startX -= 1;
		}

		return true;
	}

#pragma region old version
	/*
	if (right)
	{ 
		//assumes that x2 > x1 && y2 > y1
		startX = x1 - 1;

		for (int j = y1 - 2; j <= y2; j++)
		{
			for (int i = startX; i <= x2; i++)
			{
				if (j == y1 - 2 && (i == x1 - 1 || i == x1 || i == x2)) continue;
				if (Pelna(i, j)) return false;
			}

			if (startX != x2) startX += 1;
		}

		return true;
	}
	else
	{
		//assumes that x1 > x2 && y2 > y1
		startX = x1 + 1;

		for (int j = y1 - 2; j <= y2; j++)
		{
			for (int i = startX; i >= x2; i--)
			{
				if (j == y1 - 2 && (i == x1 + 1 || i == x1 || i == x2)) continue;
				if (Pelna(i, j)) return false;
			}

			if (startX != x2) startX -= 1;
		}

		return true;
	}
	*/
#pragma endregion 
}

//crude; to be used by GetNeighboursList
bool Pathfinder::WalkOffLedgePathClear(int x1, int y1, int x2, int y2, bool right)
{
	int startX, startY;

	int distX = abs(x2 - x1);
	int distY = abs(y2 - y1);

	if (right)
	{
		//assumes that x2 > x1 && y2 > y1
		//startX = x1 + 1;
		startX = x1;
		startY = y1;

		for (int j = startY; j <= y2; j++)
		{
			for (int i = startX; i <= x2; i++)
			{
				if (Pelna(i, j)) return false;
			}

			if (startX != x2) startX += 1;
		}

		return true;
	}
	else
	{
		//assumes that x1 > x2 && y2 > y1
		//startX = x1 - 1;
		startX = x1;
		startY = y1;

		for (int j = startY; j <= y2; j++)
		{
			for (int i = startX; i >= x2; i--)
			{
				if (Pelna(i, j)) return false;
			}

			if (startX != x2) startX -= 1;
		}

		return true;
	}
}


void Pathfinder::NeighboursDebug(int x, int y)
{

	ofstream fileStream;
	fileStream.open(".\\Pathfinder\\neighbours.txt");

	std::vector<Node> neighbours = CalculateNeighboursList(Node{x,y});
	

	for (int i = 0; i < neighbours.size(); i++)
	{
		fileStream << " X: " << neighbours[i].x;
		fileStream << " Y: " << neighbours[i].y;
		fileStream << " " << MVactionStrings[neighbours[i].actionToReach];
		fileStream << endl;
	}
	fileStream.close();

}

std::vector<MapSearchNode*> Pathfinder::GetPathList()
{
	return _pathList;
}

std::vector<Node> Pathfinder::GetPathListNode()
{
	std::vector<Node> pathListNode;
	for (int i = 0; i < _pathList.size(); i++)
		pathListNode.push_back(Node{ _pathList[i]->_x, _pathList[i]->_y });

	return pathListNode;
}

bool Pathfinder::IsOutOfBounds(int x, int y)
{
	if (x < 0 || x > X_NODES ||
		y < 0 || y > Y_NODES) 
		return true;
	else 
		return false;
}

bool Pathfinder::CanStandInNode(int x, int y)
{
	return _bot->IsNodePassable(x, y, NODE_COORDS) && !_bot->IsNodePassable(x, y + 1, NODE_COORDS);
}

//int CalculateDistanceToFog(int x, int y)
//{
//	//x right, y down, both
//	for (int i = x + 1; i < X_NODES; i++)
//	{
//		for (int j = y + 1; j < Y_NODES; j++)
//		{
//
//		}
//	}
//
//	//x right
//	for (int i = x+1; i < X_NODES; i++)
//	{
//
//	}
//
//	//y left
//
//	//y right
//	return 0;
//}



bool Pathfinder::isCloseToFog(int x, int y, int closeness)
{
	//shorter check; might not be enough
	if ((!IsOutOfBounds(x + closeness, y) && _bot->GetFogState(x + closeness, y, NODE_COORDS) == 1) ||
		(!IsOutOfBounds(x, y + closeness) && _bot->GetFogState(x, y + closeness, NODE_COORDS) == 1) ||
		(!IsOutOfBounds(x - closeness, y) && _bot->GetFogState(x - closeness, y, NODE_COORDS) == 1) ||
		(!IsOutOfBounds(x, y - closeness) && _bot->GetFogState(x, y - closeness, NODE_COORDS) == 1))
		return true;

	return false;

	/*
	//x right, y down, both
	for (int xi = x + 1; xi < X_NODES && xi <= x + closeness; xi++)
	{
		//x right
		if (_bot->GetFogState(xi, y, NODE_COORDS) == 1) return true;

		for (int yi = y + 1; yi < Y_NODES && yi <= y + closeness; yi++)
		{
			//y down
			if (_bot->GetFogState(x, yi, NODE_COORDS) == 1) return true;
			//both
			if (_bot->GetFogState(xi, yi, NODE_COORDS) == 1) return true;
		}
	}

	//x left, y up, both
	for (int xi = x - 1; xi >= 0 && xi >= x - closeness; xi--)
	{
		//x left
		if (_bot->GetFogState(xi, y, NODE_COORDS) == 1) return true;

		for (int yi = y - 1; yi >= 0 && yi >= y - closeness; yi--)
		{
			//y up
			if (_bot->GetFogState(x, yi, NODE_COORDS) == 1) return true;
			//both
			if (_bot->GetFogState(xi, yi, NODE_COORDS) == 1) return true;
		}
	}
	
	return false;
	*/
}

bool Pathfinder::isCloseToFog(MapSearchNode *n, int closeness)
{
	return isCloseToFog(n->_x, n->_y, closeness);
}


Node Pathfinder::toNode(MapSearchNode *n)
{
	return Node{ n->_x, n->_y };
}

#pragma region stare FindExplorationPath
//
//bool Pathfinder::FindExplorationPath(double x1, double y1, double usingPixelCoords)
//{
//	bool path_found = false;
//
//	if (usingPixelCoords)
//		ConvertToNodeCoordinates(x1, y1);
//
//	_pathList.clear();
//
//	std::map<int, std::map<int, MapSearchNode*> > grid;
//	for (int i = 0; i < X_NODES; i++)
//	{
//		for (int j = 0; j < Y_NODES; j++)
//		{
//			grid[i][j] = new MapSearchNode();
//			grid[i][j]->_x = i;
//			grid[i][j]->_y = j;
//		}
//	}
//
//	// define the new nodes
//	MapSearchNode* start = grid[x1][y1];
//	start->_visited = true;
//
//	MapSearchNode* current = new MapSearchNode();
//	MapSearchNode* child = new MapSearchNode();
//
//	unsigned int n = 0;
//
//	vector<MapSearchNode*> neighbours = CalculateNeighboursList(start, grid);
//	if (!neighbours.empty())
//	{
//		current = neighbours[0];
//		current->_visited = true;
//		current->_parent = start;
//	}
//	else
//	{
//		return false;
//	}
//
//	while (n < 1000) //if current close to fog (we can check in 8 directions if its close to fog)
//	{
//		if (isCloseToFog(current, 5))
//		{
//			path_found = true;
//			break;
//		}
//
//		vector<MapSearchNode*> neighbours = CalculateNeighboursList(current, grid);
//		
//		//if no neighbours - bot will walk to current (generally shouldn't happen)
//		if (neighbours.empty())
//		{
//			path_found = true;
//			break;
//		}
//		else
//		{
//			//find first not visited neighbour
//			int i = 0;
//			while (i < neighbours.size())
//			{
//				if (!neighbours[i]->_visited)
//				{
//					neighbours[i]->_visited = true;
//					neighbours[i]->_parent = current;
//					current = neighbours[i];
//					break;
//				}
//				i++;
//			}
//			//unvisited neighbour not found - backtracking
//			if (i == neighbours.size())
//			{
//				if (current->_parent != NULL)
//					current = current->_parent;
//				else
//					break; //current does not have a parent and all his neighbours are visited
//				//mo¿na dodaæ taki wierzcho³ek do listy potencjalnych celi eksploracji
//			}
//		}
//
//		
//		n++;
//	}
//
//
//	// resolve the path starting from the end point
//	while (current->_parent && current != start)
//	{
//		_pathList.push_back(current);
//		current = current->_parent;
//	}
//	reverse(_pathList.begin(), _pathList.end());
//
//	return path_found;
//
//}

#pragma endregion

std::vector<Node> Pathfinder::FindExplorationTargets(double x1, double y1, double usingPixelCoords)
{
	std::vector<Node> explTargets;

	if (usingPixelCoords)
		ConvertToNodeCoordinates(x1, y1);

	_pathList.clear();

	std::map<int, std::map<int, MapSearchNode*> > grid;
	for (int i = 0; i < X_NODES; i++)
	{
		for (int j = 0; j < Y_NODES; j++)
		{
			grid[i][j] = new MapSearchNode();
			grid[i][j]->_x = i;
			grid[i][j]->_y = j;
		}
	}

	MapSearchNode* start = grid[x1][y1];
	start->_visited = true;

	MapSearchNode* current = new MapSearchNode();
	MapSearchNode* child = new MapSearchNode();

	unsigned int n = 0;

	vector<MapSearchNode*> neighbours = CalculateNeighboursList(start, grid);
	if (!neighbours.empty())
	{
		current = neighbours[0];
		current->_visited = true;
		current->_parent = start;
	}
	else
	{
		//no neighbours - we return empty target list
		return explTargets;
	}

	while (n < 1000)
	{
		if (isCloseToFog(current, 5))
		{
			explTargets.push_back(toNode(current));
		}

		vector<MapSearchNode*> neighbours = CalculateNeighboursList(current, grid);
		
		//no neighbours - backtracking
		if (neighbours.empty())
		{
			if (current->_parent != NULL)
				current = current->_parent;
			else
				//current does not have a parent, we can't backtrack
				break; 
		}
		else
		{
			//find first not visited neighbour
			int i = 0;
			while (i < neighbours.size())
			{
				if (!neighbours[i]->_visited)
				{
					neighbours[i]->_visited = true;
					neighbours[i]->_parent = current;
					current = neighbours[i];
					break;
				}
				i++;
			}
			//unvisited neighbour not found - backtracking
			if (i == neighbours.size())
			{
				if (current->_parent != NULL)
					current = current->_parent;
				else
					//current does not have a parent, we can't backtrack
					break; 
			}
		}

		n++;
	}


	return explTargets;

}


bool Pathfinder::CalculatePath(double x1, double y1, double x2, double y2, double usingPixelCoords)
{
	bool path_found = false;

	if (usingPixelCoords)
		ConvertToNodeCoordinates(x1, y1, x2, y2);

	if (x1 != x2 || y1 != y2)
	{
		_pathList.clear();

		std::map<int, std::map<int, MapSearchNode*> > grid;
		for (int i = 0; i < X_NODES; i++)
		{
			for (int j = 0; j < Y_NODES; j++)
			{
				grid[i][j] = new MapSearchNode();
				grid[i][j]->_x = i;
				grid[i][j]->_y = j;
			}
		}

		ofstream fileStream;
		fileStream.open(".\\Pathfinder\\level_paths.txt");


		// define the new nodes
		MapSearchNode* start = new MapSearchNode();
		start->_x = x1;
		start->_y = y1;

		fileStream << "START:" << endl;
		fileStream << "X: ";
		fileStream << start->_x;
		fileStream << " Y: ";
		fileStream << start->_y;
		fileStream << endl;

		MapSearchNode* end = new MapSearchNode();
		end->_x = x2;
		end->_y = y2;

		fileStream << "END" << endl;
		fileStream << "X: ";
		fileStream << end->_x;
		fileStream << " Y: ";
		fileStream << end->_y;
		fileStream << endl;

		MapSearchNode* current = new MapSearchNode();
		MapSearchNode* child = new MapSearchNode();

		std::list<MapSearchNode*> openList;
		std::list<MapSearchNode*> closedList;
		list<MapSearchNode*>::iterator i;

		unsigned int n = 0;

		openList.push_back(start);
		start->_opened = true;
		
		while (n == 0 || (current != end && n < 500))
		{
			// Look for the smallest f value in the openList
			for (i = openList.begin(); i != openList.end(); i++)
			{
				if (i == openList.begin() || (*i)->GetFScore() <= current->GetFScore())
				{
					current = (*i);
				}
			}

			fileStream << "searching";
			fileStream << " Current X: ";
			fileStream << current->_x;
			fileStream << " Current Y: ";
			fileStream << current->_y;

			// Stop if we've reached the end
			if (current->_x == end->_x && current->_y == end->_y)
			{
				fileStream << endl << "end reached";
				path_found = true;
				break;
			}

			// Remove the current point from the open list
			openList.remove(current);
			current->_opened = false;

			// Add the current point to the closed list
			closedList.push_back(current);
			current->_closed = true;


			//Get reachable neighbours
			std::vector<MapSearchNode*> neighbours;

			std::vector<Node> neighboursNode = CalculateNeighboursList(Node{current->_x, current->_y});
			//Convert Nodes to MapSearchNodes
			for (int i = 0; i < neighboursNode.size(); i++)
				neighbours.push_back(grid[neighboursNode[i].x][neighboursNode[i].y]);

			//std::vector<MapSearchNode*> neighbours = CalculateNeighboursList(current, grid);

			for (int i = 0; i < neighbours.size(); i++)
			{
				child = neighbours[i];

				// if it's closed then pass
				if (child->_closed)
				{
					fileStream << "\n";
					fileStream << "closed";
					continue;
				}

				// IF AT A CORNER?

				// if it's already in the opened list
				if (child->_opened)
				{
					if (child->_gScore > child->GetGScore(current))
					{
						child->_parent = current;
						child->ComputeScores(end);
					}
				}
				else
				{
					openList.push_back(child);
					child->_opened = true;

					// COMPUTE THE G
					child->_parent = current;
					child->ComputeScores(end);
				}
			}

			n++;
			fileStream << "\n";
		}

		// Reset
		for (i = openList.begin(); i != openList.end(); i++)
		{
			(*i)->_opened = false;
		}
		for (i = closedList.begin(); i != closedList.end(); i++)
		{
			(*i)->_closed = false;
		}
		fileStream.close();

		fileStream.open(".\\Pathfinder\\level_path.txt");
		// resolve the path starting from the end point
		while (current->_parent && current != start)
		{
			_pathList.push_back(current);
			current = current->_parent;
		}

		reverse(_pathList.begin(), _pathList.end());

		for (int i = 0; i < _pathList.size(); i++)
		{

			fileStream << "X ";
			fileStream << _pathList[i]->_x;
			fileStream << " Y ";
			fileStream << _pathList[i]->_y;
			fileStream << "\n";
		}

		fileStream.close();
		return path_found;

		
	}
	else //we are standing on the exit
		return true;
}





//find neighbours LR II pierwsza wersja
/*
switch (candidateX)
{
case 1:
neighbours.push_back(grid[x + candidateX][y]);
break;
case 2:
if (Pusta(x + 1, y)) neighbours.push_back(grid[x + candidateX][y]);
break;
case 3:
case 4:
bool ok = true;
for (int i = 1; i < candidateX; i++)
{
for (int j = -1; j <= 0; j--)
{
if (Pelna(x + i, y + j)) ok = false;
}
}
if (ok) neighbours.push_back(grid[x + candidateX][y]);
break;
case 5:
case 6:
bool ok = true;
for (int i = 1; i < candidateX; i++)
{
for (int j = -2; j <= 0; j--)
{
if (Pelna(x + i, y + j)) ok = false;
}
}
if (ok) neighbours.push_back(grid[x + candidateX][y]);
break;
}
*/