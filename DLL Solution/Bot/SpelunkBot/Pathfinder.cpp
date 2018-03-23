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
	InitializeVariables();
}

Pathfinder::~Pathfinder()
{
	for (int i = 0; i < X_NODES; i++)
	{
		for (int j = 0; j < Y_NODES; j++)
		{
			delete _grid[i][j];
		}
	}

	//pathlist contains pointers to nodes from _grid, which we already deleted
	//for (int i = 0; i < _pathList.size(); i++)
	//{
	//	delete (_pathList[i]);
	//}
	_pathList.clear();
}

void Pathfinder::InitializeVariables()
{
	//initialize the grid
	for (int i = 0; i < X_NODES; i++)
	{
		for (int j = 0; j < Y_NODES; j++)
		{
			_grid[i][j] = new MapSearchNode();
			_grid[i][j]->_x = i;
			_grid[i][j]->_y = j;
		}
	}
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


void Pathfinder::AddNeighboursLR(int x, int y, bool right, std::vector<Node> *neighbours)
{
	int MAX = 7; //maximum travel distance
	int borderY = 0;
	int okDist = 0;
	bool ok = true;

	for (int i = 1; i < MAX; i++)
	{
		if (i <= 2) borderY = 0;
		if (i > 2 && i <= 4) borderY = 1;
		if (i > 4) borderY = 2;

		for (int j = 0; j <= borderY; j++)
		{
			if (right)
			{
				if (Pelna(x + i, y - j)) ok = false;
			}
			else
				if (Pelna(x - i, y - j)) ok = false;
		}

		if (!ok) break;

		okDist = i;
	}

	int candidateX;

	for (int i = 0; i <= okDist; i++)
	{
		MOVEMENTACTION action;
		right ? candidateX = i + 1 : candidateX = -i - 1;

		if (abs(candidateX) != 1 && Ladder(x + candidateX, y))
		{
			MVSTATE mvState;
			if (IsJumpWithRunning(candidateX, 0, LADDER)) 
				mvState = mvCLIMBING_WITH_MOMENTUM;
			else 
				mvState = mvCLIMBING;

			neighbours->push_back(Node{ x + candidateX, y, JUMP, LADDER, mvState });
		}

		if (Pelna(x + candidateX, y + 1) && Pusta(x + candidateX, y))
		{
			if (abs(candidateX) == 1)
			{
				neighbours->push_back(Node{ x + candidateX, y, WALK });
			}
			else if (abs(candidateX) == 2)
			{
				neighbours->push_back(Node{ x + candidateX, y, WALK });
			}
			else
			{
				if (abs(candidateX) == 7)
					neighbours->push_back(Node{ x + candidateX, y, JUMP, LEDGE, mvHANGING});
				else
					neighbours->push_back(Node{ x + candidateX, y, JUMP, GROUND });
			}
			break;
		}
	}
}

vector<Node> Pathfinder::CalculateNeighboursHanging(Node node)
{
	vector<Node> neighbours;
	int x = node.GetX();
	int y = node.GetY();
	int MAX;

	//drop
	MAX = 8; //max distance to ground tile
	for (int i = 1; i <= MAX; i++)
	{
		if (i > 1 && Ladder(x, y + i - 1) && !Pelna(x, y + i))
		{
			neighbours.push_back(Node{ x, y + i - 1, DROP, LADDER, mvCLIMBING });
		}

		if (Pelna(x, y + i) && y + i < Y_NODES)
		{
			neighbours.push_back(Node{ x, y + i - 1, DROP });
			break;
		}
	}

	//climb
	if (Pelna(x + 1, y) && Pusta(x + 1, y - 1)) neighbours.push_back(Node{ x + 1, y - 1, CLIMBFROMHANG });
	if (Pelna(x - 1, y) && Pusta(x - 1, y - 1)) neighbours.push_back(Node{ x - 1, y - 1, CLIMBFROMHANG });


	return neighbours;
}

vector<Node> Pathfinder::CalculateNeighboursClimbing(Node node)
{
	vector<Node> neighbours;
	int x = node.GetX();
	int y = node.GetY();
	int MAX, MAXx, MAXy;

	//climb
	if (Ladder(x, y - 1))
		neighbours.push_back(Node{ x, y - 1, CLIMB, LADDER, mvCLIMBING });
	if (Ladder(x, y + 1))
	{
		if (Pelna(x, y + 2))
			neighbours.push_back(Node{ x, y + 1, CLIMB, GROUND });
		else
			neighbours.push_back(Node{ x, y + 1, CLIMB, LADDER, mvCLIMBING });
	}

	//drop
	MAX = 8; //max distance to ground tile
	for (int i = 1; i <= MAX; i++)
	{
		if (Pelna(x, y + i) && y + i < Y_NODES)
		{
			neighbours.push_back(Node{ x, y + i - 1, DROP });
			break;
		}
	}

	//jump y=-2
	//right
	if (Pusta(x, y - 1) && Pusta(x, y - 2) && Pusta(x + 1, y - 2) && Pelna(x + 1, y - 1))
		neighbours.push_back(Node{ x, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });
	if (Pusta(x, y - 1) && Pusta(x + 1, y) && Pusta(x + 1, y - 1) && Pusta(x + 1, y - 2) &&
		Pusta(x + 2, y - 2) && Pelna(x + 2, y - 1))
		neighbours.push_back(Node{ x + 1, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });

	//left
	if (Pusta(x, y - 1) && Pusta(x, y - 2) && Pusta(x - 1, y - 2) && Pelna(x - 1, y - 1))
		neighbours.push_back(Node{ x, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });
	if (Pusta(x, y - 1) && Pusta(x - 1, y) && Pusta(x - 1, y - 1) && Pusta(x - 1, y - 2) &&
		Pusta(x - 2, y - 2) && Pelna(x - 2, y - 1))
		neighbours.push_back(Node{ x - 1, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });

	//jump y=-1
	//right
	int safedist = 3;
	for (int i = 0; i <= 3; i++)
	{
		if (Pelna(x + i, y - 1))
		{
			safedist = i - 1;
			break;
		}
	}

	for (int i = 1; i <= safedist; i++)
	{
		if (Pelna(x + i, y) && Pusta(x + i, y - 1))
		{
			if (i == 1 || i == 2) neighbours.push_back(Node{ x + i, y - 1, JUMPFROMLADDER, GROUND });
			if (i == 3) neighbours.push_back(Node{ x + i - 1, y, JUMPFROMLADDER, LEDGE, mvHANGING });
			break;
		}
		else if (Ladder(x + i, y - 1) && i != 3)
		{
			neighbours.push_back(Node{ x + i, y - 1, JUMPFROMLADDER, LADDER, mvCLIMBING });
		}

	}

	//jump y=-1
	//left
	safedist = 3;
	for (int i = 0; i <= 3; i++)
	{
		if (Pelna(x - i, y - 1))
		{
			safedist = i - 1;
			break;
		}
	}

	for (int i = 1; i <= safedist; i++)
	{
		if (Pelna(x - i, y) && Pusta(x - i, y - 1))
		{
			if (i == 1 || i == 2) neighbours.push_back(Node{ x - i, y - 1, JUMPFROMLADDER, GROUND });
			if (i == 3) neighbours.push_back(Node{ x - i + 1, y, JUMPFROMLADDER, LEDGE, mvHANGING });
			break;
		}
		else if (Ladder(x - i, y - 1) && i != 3)
		{
			neighbours.push_back(Node{ x - i, y - 1, JUMPFROMLADDER, LADDER, mvCLIMBING });
		}

	}


	MAXx = 3;
	MAXy = 5;
	//jump y=0..5
	//right
	for (int i = 1; i <= MAXx; i++)
	{
		for (int j = 0; j <= MAXy; j++)
		{
			if (Pelna(x + i, y + j + 1) && Pusta(x + i, y + j) && DownJumpPathClear(x, y, x + i, y + j, xRIGHT))
				neighbours.push_back(Node{ x + i, y + j, JUMPFROMLADDER, GROUND });
			else if (Ladder(x + i, y + j) && DownJumpPathClear(x, y, x + i, y + j, xRIGHT))
				neighbours.push_back(Node{ x + i, y + j, JUMPFROMLADDER, LADDER, mvCLIMBING });
		}

	}

	//jump y=0..5
	//left
	for (int i = 1; i <= MAXx; i++)
		for (int j = 0; j <= MAXy; j++)
		{
			if (Pelna(x - i, y + j + 1) && Pusta(x - i, y + j) && DownJumpPathClear(x, y, x - i, y + j, xLEFT))
				neighbours.push_back(Node{ x - i, y + j, JUMPFROMLADDER, GROUND });
			else if (Ladder(x - i, y + j) && DownJumpPathClear(x, y, x - i, y + j, xLEFT))
				neighbours.push_back(Node{ x - i, y + j, JUMPFROMLADDER, LADDER, mvCLIMBING });
		}


	return neighbours;
}

std::vector<Node> Pathfinder::CalculateNeighboursClimbingWithMomentum(Node node)
{
	std::vector<Node> neighbours;
	int x = node.GetX();
	int y = node.GetY();
	int MAX, MAXx, MAXy;

	//climb
	if (Ladder(x, y - 1))
		neighbours.push_back(Node{ x, y - 1, CLIMB, LADDER, mvCLIMBING_WITH_MOMENTUM });
	if (Ladder(x, y + 1))
	{
		if (Pelna(x, y + 2))
			neighbours.push_back(Node{ x, y + 1, CLIMB, GROUND });
		else
			neighbours.push_back(Node{ x, y + 1, CLIMB, LADDER, mvCLIMBING_WITH_MOMENTUM });
	}
	

	//drop
	MAX = 8; //max distance to ground tile
	for (int i = 1; i <= MAX; i++)
	{
		if (Pelna(x, y + i) && y + i < Y_NODES)
		{
			neighbours.push_back(Node{ x, y + i - 1, DROP });
			break;
		}
	}


	//jump y=-2
	//right
	if (Pusta(x, y - 1) && Pusta(x, y - 2) && Pusta(x + 1, y - 2) && Pelna(x + 1, y - 1))
		neighbours.push_back(Node{ x, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });
	if (Pusta(x, y - 1) && Pusta(x + 1, y) && Pusta(x + 1, y - 1) && Pusta(x + 1, y - 2) &&
		Pusta(x + 2, y - 2) && Pelna(x + 2, y - 1))
		neighbours.push_back(Node{ x + 1, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });
	if (Pusta(x, y - 1) && Pusta(x + 1, y) && Pusta(x + 1, y - 1) && Pusta(x + 2, y - 1) &&
		Pusta(x + 2, y - 2) && Pusta(x + 3, y - 2) && Pelna(x + 3, y - 1))
		neighbours.push_back(Node{ x + 2, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });

	//left
	if (Pusta(x, y - 1) && Pusta(x, y - 2) && Pusta(x - 1, y - 2) && Pelna(x - 1, y - 1))
		neighbours.push_back(Node{ x, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });
	if (Pusta(x, y - 1) && Pusta(x - 1, y) && Pusta(x - 1, y - 1) && Pusta(x - 1, y - 2) &&
		Pusta(x - 2, y - 2) && Pelna(x - 2, y - 1))
		neighbours.push_back(Node{ x - 1, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });
	if (Pusta(x, y - 1) && Pusta(x - 1, y) && Pusta(x - 1, y - 1) && Pusta(x - 2, y - 1) &&
		Pusta(x - 2, y - 2) && Pusta(x - 3, y - 2) && Pelna(x - 3, y - 1))
		neighbours.push_back(Node{ x - 2, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });


	//jump y=-1
	MAX = 5;

	//right
	int safedist = MAX;
	for (int i = 0; i <= MAX; i++)
	{
		if (Pelna(x + i, y - 1))
		{
			safedist = i - 1;
			break;
		}
	}

	bool roof = false;
	for (int i = 1; i < MAX; i++)
		if (Pelna(x + i, y - 2))
		{
			roof = true;
			break;
		}

	for (int i = 1; i <= safedist; i++)
	{
		if (Pelna(x + i, y))
		{
			if (i == 5 || (i == 4 && roof))
				neighbours.push_back(Node{ x + i - 1, y, JUMPFROMLADDER, LEDGE, mvHANGING });
			else 
				neighbours.push_back(Node{ x + i, y - 1, JUMPFROMLADDER, GROUND });

			break;
		}
		else if (Ladder(x + i, y - 1))
		{
			if (i == 5 || (i == 4 && roof)) 
				continue;
			else
				neighbours.push_back(Node{ x + i, y - 1, JUMPFROMLADDER, LADDER, mvCLIMBING_WITH_MOMENTUM });
		}
	}

	//left
	safedist = MAX;
	for (int i = 0; i <= MAX; i++)
	{
		if (Pelna(x - i, y - 1))
		{
			safedist = i - 1;
			break;
		}
	}

	roof = false;
	for (int i = 1; i < MAX; i++)
		if (Pelna(x - i, y - 2))
		{
			roof = true;
			break;
		}

	for (int i = 1; i <= safedist; i++)
	{
		if (Pelna(x - i, y))
		{
			if (i == 5 || (i == 4 && roof))
				neighbours.push_back(Node{ x - i - 1, y, JUMPFROMLADDER, LEDGE, mvHANGING });
			else
				neighbours.push_back(Node{ x - i, y - 1, JUMPFROMLADDER, GROUND });

			break;
		}
		else if (Ladder(x - i, y - 1))
		{
			if (i == 5 || (i == 4 && roof))
				continue;
			else
				neighbours.push_back(Node{ x - i, y - 1, JUMPFROMLADDER, LADDER, mvCLIMBING_WITH_MOMENTUM });
		}
	}


	//DOWN jump
	//y=0..5
	MAXx = 5; //6 possible, but we take 5 just to be sure
	MAXy = 5; //only 5 because we would take damage if we jumped further
	MOVEMENTACTION action;

	//right
	for (int i = 1; i <= MAXx; i++)
		for (int j = 0; j <= MAXy; j++)
		{
			if (Pelna(x + i, y + j + 1) && Pusta(x + i, y + j) && DownJumpPathClear(x, y, x + i, y + j, xRIGHT))
				neighbours.push_back(Node{ x + i, y + j, JUMPFROMLADDER, GROUND });
			else if (Ladder(x + i, y + j) && DownJumpPathClear(x, y, x + i, y + j, xRIGHT))
				neighbours.push_back(Node{ x + i, y + j, JUMPFROMLADDER, LADDER, mvCLIMBING_WITH_MOMENTUM });
		}

	//left
	for (int i = 1; i <= MAXx; i++)
		for (int j = 0; j <= MAXy; j++)
		{
			if (Pelna(x - i, y + j + 1) && Pusta(x - i, y + j) && DownJumpPathClear(x, y, x - i, y + j, xLEFT))
				neighbours.push_back(Node{ x - i, y + j, JUMPFROMLADDER, GROUND });
			else if (Ladder(x - i, y + j) && DownJumpPathClear(x, y, x - i, y + j, xLEFT))
				neighbours.push_back(Node{ x - i, y + j, JUMPFROMLADDER, LADDER, mvCLIMBING_WITH_MOMENTUM });
		}


	return neighbours;
}

vector<Node> Pathfinder::CalculateNeighboursStanding(Node node)
{
	vector<Node> neighbours;
	int x = node.GetX();
	int y = node.GetY();
	int MAX;


	/*
	* UP I
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

	//LADDER UP
	if (Ladder(x, y - 1))
	{
		if (Ladder(x, y))
			neighbours.push_back(Node{ x, y - 1, CLIMB, LADDER, mvCLIMBING });
		else
			neighbours.push_back(Node{ x, y - 1, JUMP, LADDER, mvCLIMBING });
	}
	else if (Ladder(x, y - 2))
		neighbours.push_back(Node{ x, y - 2, JUMP, LADDER, mvCLIMBING });


	/*
	* UP III (max=5 - possible, max=4 - safe)
	*/
	MAX = 6; //max travel distance (counting to ground not to ledge)

	//right
	int maxUpIII = MAX - 1;
	for (int i = 0; i <= maxUpIII; i++)
	{
		if (Pusta(x + i, y) && Pusta(x + i, y - 1) && Pusta(x + i + 1, y - 2))
		{
			//if (i != 0) 
			//{
			//	if (Ladder(x + i, y - 1))
			//		neighbours.push_back(Node{ x + i, y - 1, JUMPUPRIGHT_LADDER });
			//	else if (Ladder(x + i, y - 2))
			//		neighbours.push_back(Node{ x + i, y - 2, JUMPUPRIGHT_LADDER });
			//}

			if (i != 0 && Ladder(x + i, y - 2))
			{
				MVSTATE mvState;
				if (IsJumpWithRunning(i, -2, LADDER))
					mvState = mvCLIMBING_WITH_MOMENTUM;
				else
					mvState = mvCLIMBING;

				neighbours.push_back(Node{ x + i, y - 2, JUMP, LADDER, mvState });
			}

			if (Pelna(x + i + 1, y - 1))
			{
				if (i == 0)
				{
					if (Pusta(x, y - 2)) //neighbours.push_back(Node{ x + 1, y - 2, JUMPUPRIGHT });
						neighbours.push_back(Node{ x + i, y - 1, JUMP, LEDGE, mvHANGING });
				}
				else
					//neighbours.push_back(Node{ x + i + 1, y - 2, JUMPUPRIGHT });
					neighbours.push_back(Node{ x + i, y - 1, JUMP, LEDGE, mvHANGING });

				break;
			}

		}
		else break;
	}
	//left
	for (int i = 0; i >= -maxUpIII; i--)
	{
		if (Pusta(x + i, y) && Pusta(x + i, y - 1) && Pusta(x + i - 1, y - 2))
		{
			//if (i != 0)
			//{
			//	if (Ladder(x + i, y - 1))
			//		neighbours.push_back(Node{ x + i, y - 1, JUMPUPLEFT_LADDER });
			//	else if (Ladder(x + i, y - 2))
			//		neighbours.push_back(Node{ x + i, y - 2, JUMPUPLEFT_LADDER });
			//}

			if (i != 0 && Ladder(x + i, y - 2))
			{
				MVSTATE mvState;
				if (IsJumpWithRunning(i, -2, LADDER))
					mvState = mvCLIMBING_WITH_MOMENTUM;
				else
					mvState = mvCLIMBING;

				neighbours.push_back(Node{ x + i, y - 2, JUMP, LADDER, mvState });
			}

			if (Pelna(x + i - 1, y - 1))
			{
				if (i == 0)
				{
					if (Pusta(x, y - 2)) //neighbours.push_back(Node{ x - 1, y - 2, JUMPUPLEFT });
						neighbours.push_back(Node{ x, y - 1, JUMP, LEDGE, mvHANGING });
				}
				else
					//neighbours.push_back(Node{ x + i - 1, y - 2, JUMPUPLEFT });
					neighbours.push_back(Node{ x + i, y - 1, JUMP, LEDGE, mvHANGING });
				break;
			}
		}
		else break;
	}


	/*
	* UP IV  (max=5 - possible, max=4 - safe)
	*/
	MAX = 6; //max travel distance

	//right
	int maxUpVI = MAX-1;
	for (int i = 0; i <= maxUpVI; i++)
	{
		if (Pusta(x + i, y) && Pusta(x + i, y - 1) && Pusta(x + i + 1, y - 1) && (abs(i) <= 1 || Pusta(x + i, y - 2)))
		{
			if (Ladder(x + i + 1, y - 1))
			{
				MVSTATE mvState;
				if (IsJumpWithRunning(i + 1, -1, LADDER))
					mvState = mvCLIMBING_WITH_MOMENTUM;
				else
					mvState = mvCLIMBING;

				neighbours.push_back(Node{ x + i + 1, y - 1, JUMP, LADDER, mvState });
			}

			if (Pelna(x + i + 1, y))
			{
				if (i == 0)
					neighbours.push_back(Node{ x + i + 1, y - 1, WALKUP});
				else if (i == MAX && Pusta(x + i, y + 1))
					neighbours.push_back(Node{ x + i, y, JUMP, LEDGE, mvHANGING });
				else
					neighbours.push_back(Node{ x + i + 1, y - 1, JUMP });
				break;
			}
		}
		else break;
	}

	//left
	for (int i = 0; i >= -maxUpVI; i--)
	{
		if (Pusta(x + i, y) && Pusta(x + i, y - 1) && Pusta(x + i - 1, y - 1) && (abs(i) <= 1 || Pusta(x + i, y - 2)))
		{
			if (Ladder(x + i - 1, y - 1))
			{
				MVSTATE mvState;
				if (IsJumpWithRunning(i + 1, -1, LADDER))
					mvState = mvCLIMBING_WITH_MOMENTUM;
				else
					mvState = mvCLIMBING;

				neighbours.push_back(Node{ x + i - 1, y - 1, JUMP, LADDER, mvState });
			}

			if (Pelna(x + i - 1, y))
			{
				if (i == 0)
					neighbours.push_back(Node{ x + i - 1, y - 1, WALKUP });
				else if (i == -MAX && Pusta(x + i, y + 1))
					neighbours.push_back(Node{ x + i, y, JUMP, LEDGE, mvHANGING });
				else
					neighbours.push_back(Node{ x + i - 1, y - 1, JUMP });
				break;
			}
		}
		else break;
	}


	/*
	* L/R I i II
	*/

	AddNeighboursLR(x, y, true, &neighbours);
	AddNeighboursLR(x, y, false, &neighbours);


	/*
	* DOWN climb
	*/

	if (Ladder(x, y + 1))
		neighbours.push_back(Node{ x, y + 1, CLIMB, LADDER, mvCLIMBING });


	/*
	* DOWN hang
	*/
	//right
	if (Pusta(x + 1, y) && Pusta(x + 1, y + 1))
	{
		if (Pelna(x + 1, y + 2)) neighbours.push_back(Node{ x + 1, y + 1, HANGDROP });
		else
		{
			if (Ladder(x + 1, y + 1))
				neighbours.push_back(Node{ x + 1, y + 1, HANG, LADDER, mvCLIMBING });
			else
				neighbours.push_back(Node{ x + 1, y + 1, HANG, LEDGE, mvHANGING });
		}
	}

	//left
	if (Pusta(x - 1, y) && Pusta(x - 1, y + 1))
	{
		if (Pelna(x - 1, y + 2)) neighbours.push_back(Node{ x - 1, y + 1, HANGDROP });
		else
		{
			if (Ladder(x - 1, y + 1))
				neighbours.push_back(Node{ x - 1, y + 1, HANG, LADDER, mvCLIMBING });
			else
				neighbours.push_back(Node{ x - 1, y + 1, HANG, LEDGE, mvHANGING });
		}
	}


	/*
	* DOWN jump
	*/
	int MAXx = 8; //max travel distance
	int MAXy = 5; //only 5 because we would take damage if we jumped further

	//right
	if (Pusta(x + 1, y + 1))
	{
		for (int i = 1; i <= MAXx; i++)
			for (int j = 1; j <= MAXy; j++)
			{
				//awkward blocked jump; bot will HangDrop instead
				if (i == 1 && Pelna(x, y - 1))
					continue;

				if (Pelna(x + i, y + j + 1) && Pusta(x + i, y + j) && DownJumpPathClear(x, y, x + i, y + j, xRIGHT))
					neighbours.push_back(Node{ x + i, y + j, JUMP });

				if (Ladder(x + i, y + j) && DownJumpPathClear(x, y, x + i, y + j, xRIGHT))
				{
					MVSTATE mvState;
					if (IsJumpWithRunning(i, j, LADDER))
						mvState = mvCLIMBING_WITH_MOMENTUM;
					else
						mvState = mvCLIMBING;

					neighbours.push_back(Node{ x + i, y + j, JUMP, LADDER, mvState });
				}
			}
				
	}

	//left
	if (Pusta(x - 1, y + 1))
	{
		for (int i = 1; i <= MAXx; i++)
			for (int j = 1; j <= MAXy; j++)
			{
				//awkward blocked jump; bot will HangDrop instead
				if (i == 1 && Pelna(x, y - 1))
					continue;

				if (Pelna(x - i, y + j + 1) && Pusta(x - i, y + j) && DownJumpPathClear(x, y, x - i, y + j, xLEFT))
					neighbours.push_back(Node{ x - i, y + j, JUMP });

				if (Ladder(x - i, y + j) && DownJumpPathClear(x, y, x - i, y + j, xLEFT))
				{
					MVSTATE mvState;
					if (IsJumpWithRunning(-i, j, LADDER))
						mvState = mvCLIMBING_WITH_MOMENTUM;
					else
						mvState = mvCLIMBING;

					neighbours.push_back(Node{ x - i, y + j, JUMP, LADDER, mvState });
				}
			}
				
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
				{
					if (Pelna(x + i, y + j + 1) && Pusta(x + i, y + j) && WalkOffLedgePathClear(x, y, x + i, y + j, xRIGHT))
						neighbours.push_back(Node{ x + i, y + j, WALKOFFLEDGE });
					//na razie mi siê nie chce robiæ walkoffledge to ladder
					//jak ju¿ zrobiê to trzeba te¿ rozró¿niæ które walkofy s¹ z bieganiem i zrobiæ wtedy climbing with momentum
					//if (Ladder(x + i, y + j) && WalkOffLedgePathClear(x, y, x + i, y + j, xRIGHT))
					//	neighbours.push_back(Node{ x + i, y + j, WALKOFFLEDGE, LADDER, mvCLIMBING });
				}
					
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
				{
					if (Pelna(x - i, y + j + 1) && Pusta(x - i, y + j) && WalkOffLedgePathClear(x, y, x - i, y + j, xLEFT))
						neighbours.push_back(Node{ x - i, y + j, WALKOFFLEDGE });
					//if (Ladder(x - i, y + j) && WalkOffLedgePathClear(x, y, x - i, y + j, xLEFT))
					//	neighbours.push_back(Node{ x - i, y + j, WALKOFFLEDGE, LADDER, mvCLIMBING });
				}
					
			}

	}

	return neighbours;
}


MVSTATE Pathfinder::ToMvState(SpState spstate)
{
	switch (spstate)
	{
	case spSTANDING:
		return mvSTANDING;
	case spCLIMBING:
		return mvCLIMBING;
	case spHANGING:
		return mvHANGING;
	default:
		return mvSTANDING;
	}
}

vector<Node> Pathfinder::CalculateNeighboursList(Node node, MVSTATE mvstate)
{
	vector<Node> neighbours;
	
	switch (mvstate)
	{
	case mvHANGING:
		neighbours = CalculateNeighboursHanging(node);
		break;
	case mvCLIMBING:
		neighbours = CalculateNeighboursClimbing(node);
		break;
	case mvCLIMBING_WITH_MOMENTUM:
		neighbours = CalculateNeighboursClimbingWithMomentum(node);
		break;
	case mvSTANDING:
	default:
		neighbours = CalculateNeighboursStanding(node);
		break;
	}

	//delete any accidental out of bounds neighbours or neighbours in fog
	for (int i = 0; i < neighbours.size(); i++)
	{
		if (neighbours[i].GetX() <= 0 || neighbours[i].GetX() >= X_NODES-1 ||
			neighbours[i].GetY() <= 0 || neighbours[i].GetY() >= Y_NODES-1 ||
			_bot->GetFogState(neighbours[i].GetX(), neighbours[i].GetY(), NODE_COORDS) == 1 ||
			_bot->GetFogState(neighbours[i].GetX(), neighbours[i].GetY() + 1, NODE_COORDS) == 1)
		{
			neighbours.erase(neighbours.begin()+i);
			i--;
		}
	}


	return neighbours;
}



vector<MapSearchNode*> Pathfinder::CalculateNeighboursList(MapSearchNode* node, MVSTATE mvstate)
{
	vector<MapSearchNode*> neighbours;

	vector<Node> n = CalculateNeighboursList(Node{ node->_x, node->_y }, mvstate);
	for (int i = 0; i < n.size(); i++)
	{
		_grid[n[i].GetX()][n[i].GetY()]->_actionToReachCandidate = n[i].GetActionToReach();
		_grid[n[i].GetX()][n[i].GetY()]->_actionTargetCandidate = n[i].GetActionTarget();
		_grid[n[i].GetX()][n[i].GetY()]->_mvStateCandidate = n[i].GetMvState();
		neighbours.push_back(_grid[n[i].GetX()][n[i].GetY()]);
	}

	return neighbours;
}

//std::vector<Node> Pathfinder::CalculateNeighboursList(MapSearchNode* node)
//{
//	MVSTATE mvstate = GetCurrentMvState(node->GetX(), node->GetY(), node->_actionToReach, node->_parent);
//
//	vector<Node> neighbours = CalculateNeighboursList(Node{ node->_x, node->_y }, mvstate);
//
//	return neighbours;
//}

//vector<Node> Pathfinder::CalculateNeighboursList(Node node)
//{
//	MVSTATE mvstate = GetCurrentMvState(node.x, node.y);
//
//	return CalculateNeighboursList(node, mvstate);
//}


//MVSTATE Pathfinder::GetCurrentMvState(MapSearchNode *currentNode)
//{
//	int distToParentX, distToParentY;
//	int x = currentNode->GetX();
//	int y = currentNode->GetY();
//
//
//	if (!_bot->IsNodePassable(x, y + 1, NODE_COORDS))
//		return mvSTANDING;
//	else
//	{
//		if (Ladder(x, y))
//		{
//			if (currentNode->_parent != NULL)
//			{
//				distToParentX = x - currentNode->_parent->_x;
//				distToParentY = y - currentNode->_parent->_y;
//
//				//if previous action that ended on a ladder was made using running, then bot has momentum while climbing
//				if (currentNode->_actionToReach == JUMP &&
//					(abs(distToParentX) > 3 || (abs(distToParentX) == 3 && distToParentY == -2))
//					)
//					return mvCLIMBING_WITH_MOMENTUM;
//				else
//					return mvCLIMBING;
//			}
//
//			else
//				return mvCLIMBING;
//		}
//		else 
//			return mvHANGING;
//	}
//		
//
//	return mvSTANDING;
//}

MVSTATE Pathfinder::GetCurrentMvState(MapSearchNode *currentNode)
{
	int distToParentX, distToParentY;
	int x = currentNode->GetX();
	int y = currentNode->GetY();
	
	if (x == (int)_bot->GetPlayerPositionXNode() &&
		y == (int)_bot->GetPlayerPositionYNode())
	{

	}


	switch (currentNode->GetActionTarget())
	{
	case GROUND:
		return mvSTANDING;
	case LEDGE:
		return mvHANGING;
	case LADDER:
		if (currentNode->GetActionToReach() == JUMP)
		{
			if (currentNode->_parent != NULL)
			{
				distToParentX = x - currentNode->_parent->_x;
				distToParentY = y - currentNode->_parent->_y;

				if (IsJumpWithRunning(distToParentX, distToParentY, LADDER))
					return mvCLIMBING_WITH_MOMENTUM;
				else
					return mvCLIMBING;
			}
		}

		//TODO: we need to save mvstate in nodes
		//if (currentNode->GetActionToReach() == JUMPFROMLADDER && currentNode->_parent->GetMvState() == mvCLIMBING_WITH_MOMENTUM)
		//	return mvCLIMBING_WITH_MOMENTUM;

		return mvCLIMBING;
	default:
		break;
	}
}

MVSTATE Pathfinder::GetCurrentMvState(Node *currentNode, Node *parentNode)
{
	MapSearchNode current;
	current._x = currentNode->GetX();
	current._y = currentNode->GetY();
	current._actionToReach = currentNode->GetActionToReach();
	current._actionTarget = currentNode->GetActionTarget();

	if (parentNode != NULL)
	{
		MapSearchNode parent;
		parent._x = parentNode->GetX();
		parent._y = parentNode->GetY();
		parent._actionToReach = parentNode->GetActionToReach();
		parent._actionTarget = parentNode->GetActionTarget();

		current._parent = &parent;
	}
	else
		current._parent = NULL;

	return GetCurrentMvState(&current);
}

ACTION_TARGET Pathfinder::GetCurrentJumpTarget(Node *currentNode)
{
	if (CanStandInNode(currentNode->GetX(), currentNode->GetY())) return GROUND;
	else if (Ladder(currentNode->GetX(), currentNode->GetY())) return LADDER;
	else if (Pelna(currentNode->GetX()+1, currentNode->GetY()) || 
			 Pelna(currentNode->GetX()-1, currentNode->GetY())) return LEDGE;

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
//	int candidateX = 0;
//
//	for (int i = 1; i <= MAX; i++)
//		if (Pelna(x + i, y + 1) && Pusta(x + i, y))
//		{
//			candidateX = i;
//			break;
//		}
//
//	if (candidateX != 0)
//	{
//		int borderY = 0;
//		if (candidateX <= 2) borderY = 0;
//		if (candidateX > 2 && candidateX <= 4) borderY = 1;
//		if (candidateX > 4) borderY = 2;
//
//		bool ok = true;
//		for (int i = 1; i < candidateX; i++)
//		{
//			for (int j = 0; j <= borderY; j++)
//			{
//				if (Pelna(x + i, y - j)) ok = false;
//			}
//		}
//		if (ok) neighbours.push_back(grid[x + candidateX][y]);
//	}
//
//	
//	//left
//	int candidateX = 0;
//
//	for (int i = 1; i <= MAX; i++)
//		if (Pelna(x - i, y + 1) && Pusta(x - i, y))
//		{
//			candidateX = i;
//			break;
//		}
//
//	if (candidateX != 0)
//	{
//		int borderY = 0;
//		if (candidateX <= 2) borderY = 0;
//		if (candidateX > 2 && candidateX <= 4) borderY = 1;
//		if (candidateX > 4) borderY = 2;
//
//		bool ok = true;
//		for (int i = 1; i < candidateX; i++)
//		{
//			for (int j = 0; j <= borderY; j++)
//			{
//				if (Pelna(x - i, y - j)) ok = false;
//			}
//		}
//		if (ok) neighbours.push_back(grid[x - candidateX][y]);
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


MapSearchNode* Pathfinder::GetNodeFromGrid(int x, int y)
{
	return _grid[x][y];
}

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

//crude; to be used by GetNeighboursList
bool Pathfinder::DownJumpPathClear(int x1, int y1, int x2, int y2, DIRECTIONX direction)
{
	int startX, startY;

	int distX = abs(x2 - x1);
	int distY = abs(y2 - y1);
	
	if (direction == xRIGHT)
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

				//fresh change
				//in the row 1 tile above spelunker
				if (j == startY+1)
				{
					//don't mind corner straight above your head (check if bot can make jump if you ignore that tile)
					if (i == x1) continue;

					//if jump is very short dont mind node which is in front and up from you
					if (distX == 1 && i == x1 + 1) continue;

				}

				if (Pelna(i, j)) return false;
			}

			if (startX != x2) startX += 1;
		}

		return true;
	}
	else if (direction == xLEFT)
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

				//fresh change
				//in the row 1 tile above spelunker
				if (j == startY + 1)
				{
					//don't mid corner straight above your head (check if bot can make jump if you ignore that tile)
					if (i == x1) continue;

					//if jump is very short dont mind node which is in front and up from you
					if (distX == 1 && i == x1 - 1) continue;
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
bool Pathfinder::WalkOffLedgePathClear(int x1, int y1, int x2, int y2, DIRECTIONX direction)
{
	int startX, startY;

	int distX = abs(x2 - x1);
	int distY = abs(y2 - y1);

	if (direction == xRIGHT)
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
	else if (direction == xLEFT)
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

	MVSTATE mvState = ToMvState(_bot->GetSpelunkerState());
	if (mvState == mvCLIMBING && _bot->HasMomentum())
		mvState = mvCLIMBING_WITH_MOMENTUM;

	std::vector<Node> neighbours = CalculateNeighboursList(Node{x,y}, mvState);
	

	for (int i = 0; i < neighbours.size(); i++)
	{
		fileStream << " X: " << neighbours[i].GetX();
		fileStream << " Y: " << neighbours[i].GetY();
		fileStream << " " << MVactionStrings[neighbours[i].GetActionToReach()];
		fileStream << " " << ActionTargetStrings[neighbours[i].GetActionTarget()];
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
		pathListNode.push_back(Node{ _pathList[i]->_x, _pathList[i]->_y, _pathList[i]->_actionToReach, _pathList[i]->_actionTarget, _pathList[i]->GetMvState() });

	return pathListNode;
}

bool Pathfinder::IsOutOfBounds(int x, int y)
{
	if (x < 0 || x >= X_NODES ||
		y < 0 || y >= Y_NODES) 
		return true;
	else 
		return false;
}

bool Pathfinder::CanStandInNode(int x, int y)
{
	return _bot->IsNodePassable(x, y, NODE_COORDS) && !_bot->IsNodePassable(x, y + 1, NODE_COORDS);
}

bool Pathfinder::isCloseToFog(int x, int y, int closeness)
{
	//shorter check; might not be enough
	if ((!IsOutOfBounds(x + closeness, y) && _bot->GetFogState(x + closeness, y, NODE_COORDS) == 1) ||
		(!IsOutOfBounds(x, y + closeness) && _bot->GetFogState(x, y + closeness, NODE_COORDS) == 1) ||
		(!IsOutOfBounds(x - closeness, y) && _bot->GetFogState(x - closeness, y, NODE_COORDS) == 1) ||
		(!IsOutOfBounds(x, y - closeness) && _bot->GetFogState(x, y - closeness, NODE_COORDS) == 1))
		return true;

	return false;

#pragma region prev, slower version
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

#pragma endregion

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

	std::list<MapSearchNode*> visitedList;
	list<MapSearchNode*>::iterator i;

	if (usingPixelCoords)
		ConvertToNodeCoordinates(x1, y1);

	_pathList.clear();

	MapSearchNode* start = _grid[x1][y1];
	start->_visited = true;
	visitedList.push_back(start);

	MapSearchNode* current = start;
	

	unsigned int n = 0;

	//vector<MapSearchNode*> neighbours = CalculateNeighboursList(start, GetCurrentMvState(start));
	

	//if (!neighbours.empty())
	//{
	//	current = neighbours[0];
	//	current->_visited = true;
	//	visitedList.push_back(current);
	//	current->_parent = start;
	//}
	//else
	//{
	//	//no neighbours - we return empty target list
	//	return explTargets;
	//}

	while (n < 1000)
	{
		if (isCloseToFog(current, 5))
		{
			explTargets.push_back(toNode(current));
		}

		//vector<MapSearchNode*> neighbours = CalculateNeighboursList(current, GetCurrentMvState(current));
		vector<MapSearchNode*> neighbours = CalculateNeighboursList(current, current->GetMvState());

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
					//first neighbour becomes current
					neighbours[i]->_parent = current;
					current = neighbours[i];

					current->_visited = true;
					visitedList.push_back(current);
					current->_actionToReach = current->_actionToReachCandidate;
					current->_actionTarget = current->_actionTargetCandidate;
					current->_mvState = current->_mvStateCandidate;

					//neighbours[i]->_visited = true;
					//visitedList.push_back(neighbours[i]);
					//neighbours[i]->_parent = current;
					//current = neighbours[i];

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

	//reset
	for (i = visitedList.begin(); i != visitedList.end(); i++)
	{
		(*i)->_visited = false;
		(*i)->_parent = NULL;
	}

	return explTargets;

}


bool Pathfinder::CalculatePath(double x1, double y1, double x2, double y2, double usingPixelCoords)
{
	bool DEBUG = true;

	bool path_found = false;

	if (usingPixelCoords)
		ConvertToNodeCoordinates(x1, y1, x2, y2);

	if (x1 != x2 || y1 != y2)
	{
		_pathList.clear();


		ofstream fileStream;
		if (DEBUG) fileStream.open(".\\Pathfinder\\level_paths.txt");

		// define the new nodes
		//MapSearchNode* start = new MapSearchNode();
		//start->_x = x1;
		//start->_y = y1;

		//we have to take start from grid to be able to use info about previous action
		MapSearchNode* start = _grid[x1][y1];


		MapSearchNode* end = new MapSearchNode();
		end->_x = x2;
		end->_y = y2;

		if (DEBUG)
		{
			fileStream << "START:" << endl;
			fileStream << "X: ";
			fileStream << start->_x;
			fileStream << " Y: ";
			fileStream << start->_y;
			fileStream << endl;

			fileStream << "END" << endl;
			fileStream << "X: ";
			fileStream << end->_x;
			fileStream << " Y: ";
			fileStream << end->_y;
			fileStream << endl;
		}

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

			if (DEBUG)
			{
				fileStream << "searching";
				fileStream << " Current X: ";
				fileStream << current->_x;
				fileStream << " Current Y: ";
				fileStream << current->_y;
			}

			// Stop if we've reached the end
			if (current->_x == end->_x && current->_y == end->_y)
			{
				if (DEBUG) fileStream << endl << "end reached";
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
			//std::vector<MapSearchNode*> neighbours;

			//std::vector<Node> neighboursNode = CalculateNeighboursList(Node{current->_x, current->_y});
			////Convert Nodes to MapSearchNodes
			//for (int i = 0; i < neighboursNode.size(); i++)
			//	neighbours.push_back(_grid[neighboursNode[i].x][neighboursNode[i].y]);

			//std::vector<MapSearchNode*> neighbours = CalculateNeighboursList(current, GetCurrentMvState(current));

			std::vector<MapSearchNode*> neighbours = CalculateNeighboursList(current, current->GetMvState());

			for (int i = 0; i < neighbours.size(); i++)
			{
				child = neighbours[i];

				// if it's closed then pass
				if (child->_closed)
				{
					if (DEBUG)
					{
						fileStream << "\n";
						fileStream << "closed";
					}

					continue;
				}

				// IF AT A CORNER?

				// if it's already in the opened list
				if (child->_opened)
				{
					//we can compare the G score, because the F = G+H, and H is just our distance to target prediction, 
					//which is going to be the same because its the same node reached in 2 different ways
					if (child->_gScore > child->GetGScore(current))
					{
						child->_parent = current;
						child->_actionToReach = child->_actionToReachCandidate;
						child->_actionTarget = child->_actionTargetCandidate;
						child->_mvState = child->_mvStateCandidate;
						child->ComputeScores(end);
					}
				}
				else
				{
					openList.push_back(child);
					child->_opened = true;

					// COMPUTE THE G
					child->_parent = current;
					child->_actionToReach = child->_actionToReachCandidate;
					child->_actionTarget = child->_actionTargetCandidate;
					child->_mvState = child->_mvStateCandidate;
					child->ComputeScores(end);
				}
			}

			n++;
			if (DEBUG) fileStream << "\n";
		}

		if (DEBUG) fileStream.close();

		if (DEBUG) fileStream.open(".\\Pathfinder\\level_path.txt");

		// resolve the path starting from the end point
		while (current->_parent && current != start)
		{
			_pathList.push_back(current);
			current = current->_parent;
		}
		reverse(_pathList.begin(), _pathList.end());


		// Reset
		for (i = openList.begin(); i != openList.end(); i++)
		{
			(*i)->_opened = false;
			(*i)->_hScore = 0;
			(*i)->_gScore = 0;

			(*i)->_parent = NULL;
			//(*i)->_closed = false;
		}
		for (i = closedList.begin(); i != closedList.end(); i++)
		{
			(*i)->_closed = false;
			(*i)->_hScore = 0;
			(*i)->_gScore = 0;

			(*i)->_parent = NULL;
			//(*i)->_opened = false;
		}


		if (DEBUG)
		{
			for (int i = 0; i < _pathList.size(); i++)
			{

				fileStream << "X ";
				fileStream << _pathList[i]->_x;
				fileStream << " Y ";
				fileStream << _pathList[i]->_y;
				fileStream << " Akcja: ";
				fileStream << MVactionStrings[_pathList[i]->_actionToReach];
				fileStream << "\n";
			}
			fileStream.close();
		}



		return path_found;

		
	}
	else //we are standing on the exit
		return true;
}
