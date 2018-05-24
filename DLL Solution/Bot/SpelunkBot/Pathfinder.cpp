#include "stdafx.h"
#include "Pathfinder.h"
#include <list>
#include <algorithm>
#include <fstream>
#include <ostream>

using namespace std;

#pragma region Constructors and initialization

Pathfinder::Pathfinder(IBot* bot)
{
	_bot = bot;
	InitializeGrid();
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
	//	delete (_pathList[i]);
	_pathList.clear();
}

void Pathfinder::InitializeGrid()
{
	for (int i = 0; i < X_NODES; i++)
		for (int j = 0; j < Y_NODES; j++)
			_grid[i][j] = new MapSearchNode(i,j);

	_exit = Node(-1, -1);
	_exitFound = false;
}

#pragma endregion

#pragma region Simplicity/node state methods


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
	return (_bot->GetNodeState(x, y, NODE_COORDS) == spLadder ||
		_bot->GetNodeState(x, y, NODE_COORDS) == spLadderTop); //a laddertop is a special kind of ladder
}

bool Pathfinder::Ladder(MapSearchNode *n)
{
	return Ladder(n->_x, n->_y);
}

bool Pathfinder::LadderTop(int x, int y)
{
	return _bot->GetNodeState(x, y, NODE_COORDS) == spLadderTop;
}

bool Pathfinder::Spikes(int x, int y)
{
	return _bot->GetNodeState(x, y, NODE_COORDS) == spSpike;
}

bool Pathfinder::ArrowTrapR(int x, int y)
{
	return _bot->GetNodeState(x, y, NODE_COORDS) == spArrowTrapRight;
}

bool Pathfinder::ArrowTrapL(int x, int y)
{
	return _bot->GetNodeState(x, y, NODE_COORDS) == spArrowTrapLeft;
}

bool Pathfinder::TikiTrap(int x, int y)
{
	return _bot->GetNodeState(x, y, NODE_COORDS) == spSpearTrap;
}

std::vector<Node> Pathfinder::GetPathBetweenNodes(Node start, Node end, MOVEMENTACTION used_action)
{
	std::vector<Node> path;

	int startX, startY, endX, endY, x ,y;
	startX = start.GetX();
	startY = start.GetY();
	endX = end.GetX();
	endY = end.GetY();
	DIRECTIONX dirX;
	DIRECTIONY dirY;
	bool tick;

	switch (used_action)
	{
	case IDLE:
	case CENTRALIZE:
	case WALK:
	case CLIMB:
		path.push_back(end);
		break;
	case HANG:
		path.push_back(Node(endX, endY - 1));
		path.push_back(end);
		break;
	case CLIMBFROMHANG:
		path.push_back(Node(startX, startY - 1));
		path.push_back(end);
		break;

	case DROP:
		if (startY > endY) cout << "Error in GetPathBetweenNodes: Upwards drop.";
		else
		{
			y = startY;
			while (y != endY)
			{
				y++;
				path.push_back(Node(endX, y));
			}
		}

		break;
	case JUMPABOVERIGHT:
		path.push_back(Node(startX + 1, startY));
		path.push_back(Node(startX + 1, startY - 1));
		path.push_back(Node(startX + 1, startY - 2));
		path.push_back(end);
		break;
	case JUMPABOVELEFT:
		path.push_back(Node(startX - 1, startY));
		path.push_back(Node(startX - 1, startY - 1));
		path.push_back(Node(startX - 1, startY - 2));
		path.push_back(end);
		break;

	case JUMP:
	case JUMPFROMLADDER:
		if (endX - startX > 0) dirX = xRIGHT;
		else if (endX == startX) dirX = xNONE;
		else dirX = xLEFT;

		if (endY - startY > 0) dirY = yDOWN;
		else if (endX == startX) dirY = yNONE;
		else dirY = yUP;

		if (dirX == xNONE)
		{
			x = startX;
			y = startY;
			while (y != endY)
			{
				y < endY ? y++ : y--;
				path.push_back(Node(x, y));
			}
		}
		else
		{
			x = dirX == xRIGHT ? startX + 1 : startX - 1;
			y = startY - 1;
			path.push_back(Node(x, y));

			if (x != endX)
			{
				dirX == xRIGHT ? x++ : x--;
				y--;
				path.push_back(Node(x, y));

				tick = false;
				while (x != endX)
				{
					dirX == xRIGHT ? x++ : x--;
					if (tick) y++;
					path.push_back(Node(x, y));

					tick = tick ? false : true;
				}
			}

			while (y != endY)
			{
				y < endY ? y++ : y--;
				path.push_back(Node(x, y));
			}
		}
		break;
	case WALKOFFLEDGE:
		if (endX - startX > 0) dirX = xRIGHT;
		else if (endX == startX) dirX = xNONE;
		else dirX = xLEFT;

		if (dirX == xNONE)
		{
			y = startY;
			while (y != endY)
			{
				y < endY ? y++ : y--;
				path.push_back(Node(x, y));
			}
		}
		else
		{
			x = dirX == xRIGHT ? startX + 1 : startX - 1;
			y = startY;
			path.push_back(Node(x, y));

			tick = true;
			while (x != endX)
			{
				dirX == xRIGHT ? x++ : x--;
				if (tick) y++;
				path.push_back(Node(x, y));

				tick = tick ? false : true;
			}

			while (y != endY)
			{
				y < endY ? y++ : y--;
				path.push_back(Node(x, y));
			}
		}
		break;
	default:
		path.push_back(end);
		break;
	}

	return path;
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
	return !IsInFog(x, y) && !IsInFog(x, y + 1) &&
		_bot->IsNodePassable(x, y, NODE_COORDS) &&
		(!_bot->IsNodePassable(x, y + 1, NODE_COORDS) || LadderTop(x, y + 1));

}

bool Pathfinder::CanStandInNode(MapSearchNode* n)
{
	return CanStandInNode(n->_x, n->_y);
}

bool Pathfinder::CanHangInNode(MapSearchNode * n)
{
	return !IsInFog(n) && Pusta(n->_x, n->_y) && Pusta(n->_x, n->_y - 1) &&
		((Pelna(n->_x + 1, n->_y) && Pusta(n->_x + 1, n->_y - 1)) || (Pelna(n->_x - 1, n->_y) && Pusta(n->_x - 1, n->_y - 1)));
}

bool Pathfinder::IsNodeValidBotPosition(MapSearchNode * n)
{
	return CanStandInNode(n) || Ladder(n) || CanHangInNode(n);
}

bool Pathfinder::IsInFog(MapSearchNode * n)
{
	return IsInFog(n->_x, n->_y);
}

bool Pathfinder::IsFogOnMap()
{
	for (int i = 0; i < X_NODES; i++)
		for (int j = 0; j < Y_NODES; j++)
			if (_bot->GetFogState(i, j, NODE_COORDS))
				return true;
	return false;
}

int Pathfinder::CalculateDistance(MapSearchNode * n, MapSearchNode * m)
{
	return abs(n->_x - m->_x) + abs(n->_y - m->_y);
}

int Pathfinder::CalculateDistance(Node * n, Node * m)
{
	return abs(n->GetX() - m->GetX()) + abs(n->GetX() - m->GetY());
}

bool Pathfinder::IsInFog(int x, int y)
{
	return _bot->GetFogState(x, y, NODE_COORDS);
}

bool Pathfinder::isCloseToFog(int x, int y, int closeness)
{
	for (int i = 1; i <= closeness; i++)
		if ((!IsOutOfBounds(x + i, y) && _bot->GetFogState(x + i, y, NODE_COORDS) == 1) ||
			(!IsOutOfBounds(x, y + i) && _bot->GetFogState(x, y + i, NODE_COORDS) == 1) ||
			(!IsOutOfBounds(x - i, y) && _bot->GetFogState(x - i, y, NODE_COORDS) == 1) ||
			(!IsOutOfBounds(x, y - i) && _bot->GetFogState(x, y - i, NODE_COORDS) == 1))
			return true;

	return false;


	////shorter check; might not be enough
	//if ((!IsOutOfBounds(x + closeness, y) && _bot->GetFogState(x + closeness, y, NODE_COORDS) == 1) ||
	//	(!IsOutOfBounds(x, y + closeness) && _bot->GetFogState(x, y + closeness, NODE_COORDS) == 1) ||
	//	(!IsOutOfBounds(x - closeness, y) && _bot->GetFogState(x - closeness, y, NODE_COORDS) == 1) ||
	//	(!IsOutOfBounds(x, y - closeness) && _bot->GetFogState(x, y - closeness, NODE_COORDS) == 1))
	//	return true;

	//return false;

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

bool Pathfinder::HorizontalJumpPathClear(int x, int y, int dist, DIRECTIONX directionx)
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

			if (directionx == xRIGHT)
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
				if (j == startY + 1)
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
}

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

				//falling through narrow spaces is hard; walkoffledge cant do that or it is likely that it will get stuck.
				//until we rework it, we remove the neighbours getting to which requires flying througha narrow space.
				if (startX == x2)
				{
					if (Pelna(i - 1, j) || Pelna(i + 1, j)) return false;
				}

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

				//falling through narrow spaces is hard; walkoffledge cant do that or it is likely that it will get stuck.
				//until we rework it, we remove the neighbours getting to which requires flying througha narrow space.
				if (startX == x2)
				{
					if (Pelna(i - 1, j) || Pelna(i + 1, j)) return false;
				}
			}

			if (startX != x2) startX -= 1;
		}

		return true;
	}
}

#pragma endregion

#pragma region Traps

void Pathfinder::DeleteUnsafeNeighbours(Node origin, std::vector<Node> &neighbours)
{
	DeleteUnsafeNeighboursSpikes(origin, neighbours);
	DeleteUnsafeNeighboursTikiTrap(origin, neighbours);
	DeleteUnsafeNeighboursArrowTrap(origin, neighbours);
}

void Pathfinder::DeleteUnsafeNeighboursSpikes(Node origin, std::vector<Node> &neighbours)
{
	int x, y, distX, distY;
	ACTION_TARGET target;
	MOVEMENTACTION action;
	bool unsafe;

	std::vector<Node>::iterator it = neighbours.begin();
	while (it != neighbours.end())
	{
		x = it->GetX();
		y = it->GetY();
		distX = x - origin.GetX();
		distY = y - origin.GetY();
		target = it->GetActionTarget();
		action = it->GetActionToReach();
		unsafe = false;

		//WALKOFFLEDGE is super unsafe because it hasn't been reworked
		if (action == WALKOFFLEDGE && (Spikes(x - 1, y) || Spikes(x, y) || Spikes(x + 1, y)))
			unsafe = true;

		//drop
		if (action == DROP && distY > 0 && Spikes(x, y) && target == GROUND)
			unsafe = true;

		//jump
		if ((action == JUMP || action == JUMPFROMLADDER) && target == GROUND)
		{
			if (Spikes(x, y)) unsafe = true;

			//far jumps can catch nearby spikes sometimes
			if (distY == 1 && (distX == 7 || distX == 8) && Spikes(x - 1, y))
				unsafe = true;
			if (distY == 1 && (distX == -7 || distX == -8) && Spikes(x + 1, y))
				unsafe = true;

			if (distY == 2 && distX == 8 && Spikes(x - 1, y))
				unsafe = true;
			if (distY == 2 && distX == -8 && Spikes(x + 1, y))
				unsafe = true;
		}


		if (unsafe) it = neighbours.erase(it);
		else it++;
	}
}

void Pathfinder::DeleteUnsafeNeighboursTikiTrap(Node origin, std::vector<Node> &neighbours)
{
	int x, y, distX, distY;
	ACTION_TARGET target;
	MOVEMENTACTION action;
	bool unsafe;

	std::vector<Node>::iterator it = neighbours.begin();
	while (it != neighbours.end())
	{
		x = it->GetX();
		y = it->GetY();
		distX = x - origin.GetX();
		distY = y - origin.GetY();
		target = it->GetActionTarget();
		action = it->GetActionToReach();
		unsafe = false;

		if (TikiTrap(x + 1, y) || TikiTrap(x - 1, y)) unsafe = true;

		if (unsafe) it = neighbours.erase(it);
		else it++;
	}
}

void Pathfinder::DeleteUnsafeNeighboursArrowTrap(Node origin, std::vector<Node> &neighbours)
{
	int x, y, distX, distY;
	ACTION_TARGET target;
	MOVEMENTACTION action;
	bool unsafe;

	std::vector<Node>::iterator it = neighbours.begin();
	while (it != neighbours.end())
	{
		x = it->GetX();
		y = it->GetY();
		distX = x - origin.GetX();
		distY = y - origin.GetY();
		target = it->GetActionTarget();
		action = it->GetActionToReach();
		unsafe = false;

		std::vector<Node> path = GetPathBetweenNodes(origin, *it, action);


		for (auto n : path)
		{
			for (int i = 1; i <= 4; i++)
			{
				if (ArrowTrapR(n.GetX() - i, n.GetY()) && !_bot->IsArrowTrapDisarmed(n.GetX() - i, n.GetY()))
				{
					unsafe = true;
					break;
				}
				else if (Pusta(x - i, y))
					continue;
				else break;
			}

			for (int i = 1; i <= 4; i++)
			{
				if (ArrowTrapL(n.GetX() + i, n.GetY()) && !_bot->IsArrowTrapDisarmed(n.GetX() + i, n.GetY()))
				{
					unsafe = true;
					break;
				}
				else if (Pusta(x + i, y))
					continue;
				else break;
			}

			if (unsafe) break;
		}


		if (unsafe) it = neighbours.erase(it);
		else it++;
	}
}

#pragma endregion

#pragma region Calculating neighbours

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
				if (Pelna(x + i, y - j) || LadderTop(x + i, y - j)) ok = false;
			}
			else
				if (Pelna(x - i, y - j) || LadderTop(x - i, y - j)) ok = false;
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
			if (abs(candidateX) == 1 || abs(candidateX) == 2)
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

		if (LadderTop(x + candidateX, y + 1) && Pusta(x + candidateX, y))
		{
			if (abs(candidateX) == 1)
			{
				neighbours->push_back(Node{ x + candidateX, y, WALK, LADDERTOP });
			}
			else
			{
				if (abs(candidateX) < 7)
					neighbours->push_back(Node{ x + candidateX, y, JUMP, LADDERTOP });
			}
			break;
		}
	}
}

std::vector<Node> Pathfinder::CalculateNeighboursHanging(Node node)
{
	std::vector<Node> neighbours;
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

		if (LadderTop(x, y + i) && y + i < Y_NODES)
		{
			neighbours.push_back(Node{ x, y + i - 1, DROP, LADDERTOP });
			break;
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

std::vector<Node> Pathfinder::CalculateNeighboursClimbing(Node node)
{
	std::vector<Node> neighbours;
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
		if (LadderTop(x, y + i) && y + i < Y_NODES)
		{
			neighbours.push_back(Node{ x, y + i - 1, DROP, LADDERTOP });
			break;
		}

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
			if (DownJumpPathClear(x, y, x + i, y + j, xRIGHT))
			{
				if (Pelna(x + i, y + j + 1) && Pusta(x + i, y + j))
					neighbours.push_back(Node{ x + i, y + j, JUMPFROMLADDER, GROUND });
				else if (LadderTop(x + i, y + j + 1) && Pusta(x + i, y + j))
					neighbours.push_back(Node{ x + i, y + j, JUMPFROMLADDER, LADDERTOP });
				else if (Ladder(x + i, y + j))
					neighbours.push_back(Node{ x + i, y + j, JUMPFROMLADDER, LADDER, mvCLIMBING });
			}

		}

	}

	//jump y=0..5
	//left
	for (int i = 1; i <= MAXx; i++)
		for (int j = 0; j <= MAXy; j++)
		{
			if (DownJumpPathClear(x, y, x - i, y + j, xLEFT))
			{
				if (Pelna(x - i, y + j + 1) && Pusta(x - i, y + j))
					neighbours.push_back(Node{ x - i, y + j, JUMPFROMLADDER, GROUND });
				else if (LadderTop(x - i, y + j + 1) && Pusta(x - i, y + j))
					neighbours.push_back(Node{ x + i, y + j, JUMPFROMLADDER, LADDERTOP });
				else if (Ladder(x - i, y + j))
					neighbours.push_back(Node{ x - i, y + j, JUMPFROMLADDER, LADDER, mvCLIMBING });
			}
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
		if (LadderTop(x, y + i) && y + i < Y_NODES)
		{
			neighbours.push_back(Node{ x, y + i - 1, DROP, LADDERTOP });
			break;
		}

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
			if (DownJumpPathClear(x, y, x + i, y + j, xRIGHT))
			{
				if (Pelna(x + i, y + j + 1) && Pusta(x + i, y + j))
					neighbours.push_back(Node{ x + i, y + j, JUMPFROMLADDER, GROUND });
				else if (LadderTop(x + i, y + j + 1) && Pusta(x + i, y + j))
					neighbours.push_back(Node{ x + i, y + j, JUMPFROMLADDER, LADDERTOP });
				else if (Ladder(x + i, y + j))
					neighbours.push_back(Node{ x + i, y + j, JUMPFROMLADDER, LADDER, mvCLIMBING });
			}

		}

	//left
	for (int i = 1; i <= MAXx; i++)
		for (int j = 0; j <= MAXy; j++)
		{
			if (DownJumpPathClear(x, y, x - i, y + j, xLEFT))
			{
				if (Pelna(x - i, y + j + 1) && Pusta(x - i, y + j))
					neighbours.push_back(Node{ x - i, y + j, JUMPFROMLADDER, GROUND });
				else if (LadderTop(x - i, y + j + 1) && Pusta(x - i, y + j))
					neighbours.push_back(Node{ x + i, y + j, JUMPFROMLADDER, LADDERTOP });
				else if (Ladder(x - i, y + j))
					neighbours.push_back(Node{ x - i, y + j, JUMPFROMLADDER, LADDER, mvCLIMBING });
			}
		}


	return neighbours;
}

std::vector<Node> Pathfinder::CalculateNeighboursStanding(Node node)
{
	std::vector<Node> neighbours;
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
	else if (Ladder(x, y - 2) && Pusta(x, y - 1))
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
					if (Pusta(x, y - 2))
						neighbours.push_back(Node{ x + i, y - 1, JUMP, LEDGE, mvHANGING });
				}
				else
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
					if (Pusta(x, y - 2))
						neighbours.push_back(Node{ x, y - 1, JUMP, LEDGE, mvHANGING });
				}
				else
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
					neighbours.push_back(Node{ x + i + 1, y - 1, JUMP});
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
					neighbours.push_back(Node{ x + i - 1, y - 1, JUMP });
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
	* DOWN hang
	*/
	//right
	if (Pusta(x + 1, y) && Pusta(x + 1, y + 1))
	{
		//if there are problems with hanging on a ladder with ground just below, restore hangdrop action
		//if (Pelna(x + 1, y + 2)) neighbours.push_back(Node{ x + 1, y + 1, HANGDROP });
		//else
		//{
			if (Ladder(x + 1, y + 1) || LadderTop(x + 1, y + 1))
				neighbours.push_back(Node{ x + 1, y + 1, HANG, LADDER, mvCLIMBING });
			else
				neighbours.push_back(Node{ x + 1, y + 1, HANG, LEDGE, mvHANGING });
		//}
	}

	//left
	if (Pusta(x - 1, y) && Pusta(x - 1, y + 1))
	{
		//if (Pelna(x - 1, y + 2)) neighbours.push_back(Node{ x - 1, y + 1, HANGDROP });
		//else
		//{
			if (Ladder(x - 1, y + 1) || LadderTop(x - 1, y + 1))
				neighbours.push_back(Node{ x - 1, y + 1, HANG, LADDER, mvCLIMBING });
			else
				neighbours.push_back(Node{ x - 1, y + 1, HANG, LEDGE, mvHANGING });
		//}
	}


	/*
	* DOWN jump
	*/
	int MAXx = 8; //max travel distance
	int MAXy = 5; //only 5 because we would take damage if we jumped further

	//right
	bool canHang = false;
	if (Pusta(x + 1, y + 1))
	{
		for (int i = 1; i <= MAXx; i++)
			for (int j = 1; j <= MAXy; j++)
			{
				//awkward blocked jump; bot will HangDrop instead
				if (i == 1 && (Pelna(x, y - 1) || Pelna(x + 1, y - 1)))
					continue;

				if (DownJumpPathClear(x, y, x + i, y + j, xRIGHT))
				{
					if (Pelna(x + i, y + j + 1) && Pusta(x + i, y + j))
						neighbours.push_back(Node{ x + i, y + j, JUMP });

					if (LadderTop(x + i, y + j + 1) && Pusta(x + i, y + j))
						neighbours.push_back(Node{ x + i, y + j, JUMP, LADDERTOP });

					if (Ladder(x + i, y + j))
					{
						if (i == 1 && j == 1)
							canHang = true;

						MVSTATE mvState;
						if (IsJumpWithRunning(i, j, LADDER))
							mvState = mvCLIMBING_WITH_MOMENTUM;
						else
							mvState = mvCLIMBING;

						if (canHang)
						{
							//if ladder is close, block jumping to it; Spelunker will have to hang to it
							if (i != 1) neighbours.push_back(Node{ x + i, y + j, JUMP, LADDER, mvState });
						}
						else
						{
							neighbours.push_back(Node{ x + i, y + j, JUMP, LADDER, mvState });
						}
					}
				}

			}
				
	}

	//left
	canHang = false;
	if (Pusta(x - 1, y + 1))
	{
		for (int i = 1; i <= MAXx; i++)
			for (int j = 1; j <= MAXy; j++)
			{
				//awkward blocked jump; bot will HangDrop instead
				if (i == 1 && (Pelna(x, y - 1) || Pelna(x - 1, y - 1)))
					continue;

				if (DownJumpPathClear(x, y, x - i, y + j, xLEFT))
				{
					if (Pelna(x - i, y + j + 1) && Pusta(x - i, y + j))
						neighbours.push_back(Node{ x - i, y + j, JUMP });

					if (LadderTop(x - i, y + j + 1) && Pusta(x + i, y + j))
						neighbours.push_back(Node{ x - i, y + j, JUMP, LADDERTOP });

					if (Ladder(x - i, y + j))
					{
						if (i == 1 && j == 1)
							canHang = true;

						MVSTATE mvState;
						if (IsJumpWithRunning(-i, j, LADDER))
							mvState = mvCLIMBING_WITH_MOMENTUM;
						else
							mvState = mvCLIMBING;
						if (canHang)
						{
							//if ladder is close, block jumping to it; Spelunker will have to hang to it
							if (i != 1) neighbours.push_back(Node{ x - i, y + j, JUMP, LADDER, mvState });
						}
						else
						{
							neighbours.push_back(Node{ x - i, y + j, JUMP, LADDER, mvState });
						}
					}
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
					if (WalkOffLedgePathClear(x, y, x + i, y + j, xRIGHT))
					{
						if (Pelna(x + i, y + j + 1) && Pusta(x + i, y + j))
							neighbours.push_back(Node{ x + i, y + j, WALKOFFLEDGE });
						if (LadderTop(x + i, y + j + 1) && Pusta(x + i, y + j))
							neighbours.push_back(Node{ x + i, y + j, WALKOFFLEDGE, LADDERTOP });
					}
					//TODO: walkoffledge to ladder
					//we need to also differentiate which walkoffs are with running and which arent, 
					//and set mvState to climbing or climbing with momentum
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
					if (WalkOffLedgePathClear(x, y, x - i, y + j, xLEFT))
					{
						if (Pelna(x - i, y + j + 1) && Pusta(x - i, y + j))
							neighbours.push_back(Node{ x - i, y + j, WALKOFFLEDGE });
						if (LadderTop(x - i, y + j + 1) && Pusta(x - i, y + j))
							neighbours.push_back(Node{ x - i, y + j, WALKOFFLEDGE, LADDERTOP });
					}
					//TODO: walkoffledge to ladder
					//we need to also differentiate which walkoffs are with running and which arent, 
					//and set mvState to climbing or climbing with momentum
					//if (Ladder(x - i, y + j) && WalkOffLedgePathClear(x, y, x - i, y + j, xLEFT))
					//	neighbours.push_back(Node{ x - i, y + j, WALKOFFLEDGE, LADDER, mvCLIMBING });
				}
					
			}

	}

	return neighbours;
}

std::vector<Node> Pathfinder::CalculateNeighboursStandingLT(Node node)
{
	std::vector<Node> neighbours;
	int x = node.GetX();
	int y = node.GetY();

	neighbours = CalculateNeighboursStanding(node);

	int i = 0;
	while (i < neighbours.size())
	{
		if (neighbours[i].GetActionTarget() == HANG)
			neighbours.erase(neighbours.begin() + i);
		else
			i++;
	}

	int MAX = 8; //max distance to ground tile
	for (int i = 1; i <= MAX; i++)
	{
		if (Pelna(x, y + i + 1) && y + i + 1 < Y_NODES)
		{
			neighbours.push_back(Node{ x, y + i, DROP });
			break;
		}

		if (LadderTop(x, y + i + 1) && y + i + 1 < Y_NODES)
		{
			neighbours.push_back(Node{ x, y + i, DROP, LADDERTOP });
		}

		if (Ladder(x, y + i + 1) && i > 1)
		{
			neighbours.push_back(Node{ x, y + i, DROP, LADDER, mvCLIMBING });
		}
	}

	return neighbours;
}

std::vector<Node> Pathfinder::CalculateNeighboursList(Node node, MVSTATE mvstate, ACTION_TARGET prevActionTarget)
{
	std::vector<Node> neighbours;
	
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
		if (prevActionTarget == LADDERTOP)
			neighbours = CalculateNeighboursStandingLT(node);
		else
			neighbours = CalculateNeighboursStanding(node);
		break;
	default:
		neighbours = CalculateNeighboursStanding(node);
		break;
	}

	DeleteUnsafeNeighbours(node, neighbours);

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

std::vector<MapSearchNode*> Pathfinder::CalculateNeighboursList(MapSearchNode* node, MVSTATE mvstate, ACTION_TARGET target)
{
	std::vector<MapSearchNode*> neighbours;

	std::vector<Node> n = CalculateNeighboursList(Node{ node->_x, node->_y }, mvstate, target);
	for (int i = 0; i < n.size(); i++)
	{
		_grid[n[i].GetX()][n[i].GetY()]->_actionToReachCandidate = n[i].GetActionToReach();
		_grid[n[i].GetX()][n[i].GetY()]->_actionTargetCandidate = n[i].GetActionTarget();
		_grid[n[i].GetX()][n[i].GetY()]->_mvStateCandidate = n[i].GetMvState();
		neighbours.push_back(_grid[n[i].GetX()][n[i].GetY()]);
	}

	return neighbours;
}

#pragma endregion

#pragma region Calculating strong connected components

void Pathfinder::TarjanDFS(MapSearchNode* n)
{
	_tar_cvn++;
	_tar_VN[n->_x][n->_y] = _tar_cvn;
	_tar_VLow[n->_x][n->_y] = _tar_cvn;
	_tar_S.push(n);
	_tar_VS[n->_x][n->_y] = true;

	//will GetMvState work? IF not, we can estimate it --it seems to work for now
	for each (MapSearchNode * m in CalculateNeighboursList(n, n->GetMvState(), n->GetActionTarget()))
	{
		//m not visited - visit it recursively
		if (_tar_VN[m->_x][m->_y] == 0)
		{
			//we need to save mvstate which is saved as candidate during neighbour-finding
			m->_mvState = m->_mvStateCandidate;
			m->_actionTarget = m->_actionTargetCandidate;

			TarjanDFS(m);
			_tar_VLow[n->_x][n->_y] = min(_tar_VLow[n->_x][n->_y], _tar_VLow[m->_x][m->_y]);
		}
		else
		{
			//if node m is on the stack, a path m -> n exists
			//and we know that m is n's neighbour, so path n -> m exists
			//so n and m are in one connected component
			if (_tar_VS[m->_x][m->_y]) _tar_VLow[n->_x][n->_y] = min(_tar_VLow[n->_x][n->_y], _tar_VLow[m->_x][m->_y]);
		}
	}

	//continue only if you are at the root of the connected component
	if (_tar_VLow[n->_x][n->_y] != _tar_VN[n->_x][n->_y]) return;

	_tar_CCnr++;
	MapSearchNode* m;
	vector<MapSearchNode*> sccp;
	do
	{
		m = _tar_S.top();
		_tar_S.pop();
		_tar_VS[m->_x][m->_y] = false;
		m->_CCnr = _tar_CCnr;
		sccp.push_back(m);
	} while (m->_x != n->_x || m->_y != n->_y);

	_tar_CClist.push_back(sccp);
	_tar_CCmap[m->_CCnr] = sccp;
}

void Pathfinder::CalculateConnectedComponents()
{
	//initialize variables
	_tar_cvn = 0;
	_tar_CCnr = 0;
	for (int i = 0; i < X_NODES; i++)
		for (int j = 0; j < Y_NODES; j++)
		{
			_tar_VN[i][j] = 0;
			_tar_VS[i][j] = 0;
			_tar_VLow[i][j] = 0;
		}
	//clear the stack just in case
	while (!_tar_S.empty())
		_tar_S.pop();
	_tar_CClist.clear();

	//reset the component numbers
	for (int i = 0; i < X_NODES; i++)
	{
		for (int j = 0; j < Y_NODES; j++)
		{
			_grid[i][j]->_CCnr = 0;
		}
	}

	for (int i = 0; i < X_NODES; i++)
		for (int j = 0; j < Y_NODES; j++)
		{
			if (_tar_VN[i][j] == 0 && !IsInFog(_grid[i][j]) &&
				(CanStandInNode(_grid[i][j]) || Ladder(_grid[i][j])))
				TarjanDFS(_grid[i][j]);
		}
}

#pragma endregion

#pragma region Getter methods

int Pathfinder::GetCCnr(int nodeX, int nodeY)
{
	return _grid[nodeX][nodeY]->_CCnr;
}

std::vector<MapSearchNode*> Pathfinder::GetAllNodesFromCC(int ccnr)
{
	return _tar_CCmap[ccnr];
}

MapSearchNode* Pathfinder::GetNodeFromGrid(int x, int y)
{
	return _grid[x][y];
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

Node Pathfinder::GetExplorationTarget()
{
	return _explorationTarget;
}

int Pathfinder::GetPathLength()
{
	return GetPathLength(GetPathListNode());
}

int Pathfinder::GetPathLength(std::vector<Node> path)
{
	int dist = 0;
	if (!path.empty())
	{
		auto current = path[0];

		for (int i = 1; i < path.size(); i++)
		{
			dist += abs(current.GetX() - path[i].GetX()) + abs(current.GetY() - path[i].GetY());
			current = path[i];
		}
	}

	return dist;
}

#pragma endregion

#pragma region Finding exit

void Pathfinder::TryToFindExit()
{
	for (int nodeX = 0; nodeX < X_NODES; nodeX += 1)
	{
		for (int nodeY = 0; nodeY < Y_NODES; nodeY += 1)
		{
			if (_bot->GetNodeState(nodeX, nodeY, NODE_COORDS) == spExit)
			{
				_exit = Node(nodeX, nodeY);
				_exitFound = true;
				return;
			}
		}
	}
}

Node Pathfinder::GetExit()
{
	return _exit;
}

bool Pathfinder::IsExitFound()
{
	return _exitFound;
}

#pragma endregion

#pragma region Dijkstra

void Pathfinder::Dijkstra(int x, int y)
{
	for (int i = 0; i < X_NODES; i++)
		for (int j = 0; j < Y_NODES; j++)
		{
			_grid[i][j]->_dist = INT_MAX;
			_dij_visited[i][j] = false;
		}


	_grid[x][y]->_dist = 0;
	_dij_visited[x][y] = true;

	while (!_dij_pQ.empty())
		_dij_pQ.pop();

	_dij_pQ.push(_grid[x][y]);

	while (!_dij_pQ.empty())
	{
		MapSearchNode *n = _dij_pQ.top();
		_dij_pQ.pop();

		//break if you reach unreachable nodes
		if (n->_dist == INT_MAX) break;

		vector<MapSearchNode*> neighbours = CalculateNeighboursList(n, n->GetMvState(), n->GetActionTarget());

		for (MapSearchNode* m : neighbours)
		{
			if (!_dij_visited[m->_x][m->_y])
				if (n->_dist + CalculateDistance(n, m) < m->_dist)
				{
					m->_dist = n->_dist + CalculateDistance(n, m);
					m->_mvState = m->_mvStateCandidate;
					m->_actionTarget = m->_actionTargetCandidate;
					_dij_pQ.push(m);
					_dij_visited[m->_x][m->_y] = true;
				}
		}

	}
}

int Pathfinder::GetDijDist(int x, int y)
{
	return _grid[x][y]->_dist;
}

#pragma endregion

#pragma region Pathfinding methods

bool Pathfinder::TryToCalculatePath(int x1, int y1, int x2, int y2)
{
	bool DEBUG = false;

	bool path_found = false;

	if (x1 != x2 || y1 != y2)
	{
		_pathList.clear();

		ofstream fileStream;
		if (DEBUG) fileStream.open(".\\Pathfinder\\level_paths.txt");

		//we have to take start from grid to be able to use info about previous action
		MapSearchNode* start = _grid[x1][y1];
		MapSearchNode* end = _grid[x2][y2];

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

			std::vector<MapSearchNode*> neighbours = CalculateNeighboursList(current, current->GetMvState(), current->GetActionTarget());

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

		if (path_found)
		{
			// resolve the path starting from the end point
			while (current->_parent && current != start)
			{
				_pathList.push_back(current);
				current = current->_parent;
			}
			_pathList.push_back(start);

			reverse(_pathList.begin(), _pathList.end());
		}

		// Reset
		for (i = openList.begin(); i != openList.end(); i++)
		{
			(*i)->_opened = false;
			(*i)->_hScore = 0;
			(*i)->_gScore = 0;

			(*i)->_parent = NULL;
		}
		for (i = closedList.begin(); i != closedList.end(); i++)
		{
			(*i)->_closed = false;
			(*i)->_hScore = 0;
			(*i)->_gScore = 0;

			(*i)->_parent = NULL;
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

bool Pathfinder::TryToFindExplorationTarget(int x, int y)
{
	bool targetFound = false;
	std::list<MapSearchNode*> visitedList;
	list<MapSearchNode*>::iterator i;

	_pathList.clear();

	MapSearchNode* start = _grid[x][y];
	start->_visited = true;
	visitedList.push_back(start);

	MapSearchNode* current = start;

	unsigned int n = 0;

	while (n < 1000)
	{
		if (isCloseToFog(current, 5))
		{
			targetFound = true;
			_explorationTarget = Node(current->_x, current->_y);
			break;
		}

		vector<MapSearchNode*> neighbours = CalculateNeighboursList(current, current->GetMvState(), current->GetActionTarget());

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

	return targetFound;
}

std::vector<Node> Pathfinder::FindExplorationTargets(int x1, int y1)
{
	std::vector<Node> explTargets;
	std::list<MapSearchNode*> visitedList;
	list<MapSearchNode*>::iterator i;

	_pathList.clear();

	MapSearchNode* start = _grid[x1][y1];
	start->_visited = true;
	visitedList.push_back(start);

	MapSearchNode* current = start;
	

	unsigned int n = 0;

	while (n < 1000)
	{
		if (isCloseToFog(current, 5))
		{
			explTargets.push_back(Node(current->_x, current->_y));
		}

		vector<MapSearchNode*> neighbours = CalculateNeighboursList(current, current->GetMvState(), current->GetActionTarget());

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

#pragma endregion

#pragma region Debug methods

void Pathfinder::NeighboursDebug(int x, int y, bool hasMomentum)
{
	ofstream fileStream;
	fileStream.open(".\\Pathfinder\\neighbours.txt");

	MVSTATE mvState = ToMvState(_bot->GetSpelunkerState());
	if (mvState == mvCLIMBING && hasMomentum)
		mvState = mvCLIMBING_WITH_MOMENTUM;

	ACTION_TARGET target;
	if (Pusta(x, y) && Pelna(x, y + 1)) target = GROUND;
	else if (Ladder(x, y) && !LadderTop(x, y)) target = LADDER;
	else if (Pusta(x, y) && LadderTop(x, y + 1)) target = LADDERTOP;
	else target = LEDGE;

	std::vector<Node> neighbours = CalculateNeighboursList(Node{ x,y }, mvState, target);

	for (int i = 0; i < neighbours.size(); i++)
	{
		fileStream << " X: " << neighbours[i].GetX();
		fileStream << " Y: " << neighbours[i].GetY();
		fileStream << " " << MVactionStrings[neighbours[i].GetActionToReach()];
		fileStream << " " << ActionTargetStrings[neighbours[i].GetActionTarget()];
		fileStream << endl;

		auto path = GetPathBetweenNodes(Node{ x,y }, neighbours[i], neighbours[i].GetActionToReach());
		fileStream << "Path: ";
		for (int j = 0; j < path.size(); j++)
		{
			fileStream << "(" << path[j].GetX()  << ", " << path[j].GetY() << ") ";
		}
		fileStream << endl;
	}

	fileStream.close();
}

void Pathfinder::SCCDebug()
{
	ofstream fileStream;
	fileStream.open(".\\Pathfinder\\level_layoutCC.txt");
	for (int j = 0; j < Y_NODES; j++)
	{
		for (int i = 0; i < X_NODES; i++)
		{
			int nodeState = _bot->GetNodeState(i, j, NODE_COORDS);
			if (nodeState != -1)
			{
				if (_grid[i][j]->_CCnr == 0)
					fileStream << nodeState;
				else
				{
					char int_to_char[] = { ' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
					if (_grid[i][j]->_CCnr < 26)
						fileStream << int_to_char[_grid[i][j]->_CCnr];
					else
						fileStream << '*';
				}
			}
			else
			{
				fileStream << '#'; //mapFog[j][i];
			}
			fileStream << " ";
		}
		fileStream << "\n";
	}
	fileStream.close();
}

void Pathfinder::DijkstraDebug()
{
	ofstream fileStream;
	fileStream.open(".\\Pathfinder\\level_layoutDijkstra.txt");
	for (int j = 0; j < Y_NODES; j++)
	{
		for (int i = 0; i < X_NODES; i++)
		{
			int nodeState = _bot->GetNodeState(i, j, NODE_COORDS);
			if (nodeState == -1)
			{
				fileStream << '#'; //mapFog[j][i];
			}
			else if (IsNodeValidBotPosition(_grid[i][j]))
			{
				if (_grid[i][j]->_dist == INT_MAX) fileStream << 'M';
				else 
				{
					if (_grid[i][j]->_dist == 0) fileStream << '*';
					else if (_grid[i][j]->_dist <= 5) fileStream << 'a';
					else if (_grid[i][j]->_dist <= 10) fileStream << 'b';
					else if (_grid[i][j]->_dist <= 15) fileStream << 'c';
					else if (_grid[i][j]->_dist <= 20) fileStream << 'd';
					else if (_grid[i][j]->_dist <= 25) fileStream << 'e';
					else if (_grid[i][j]->_dist <= 30) fileStream << 'f';
					else if (_grid[i][j]->_dist <= 35) fileStream << 'g';
					else if (_grid[i][j]->_dist <= 40) fileStream << 'h';
					else if (_grid[i][j]->_dist <= 45) fileStream << 'i';
					else if (_grid[i][j]->_dist <= 50) fileStream << 'j';
					else if (_grid[i][j]->_dist <= 55) fileStream << 'k';
					else if (_grid[i][j]->_dist <= 60) fileStream << 'l';
					else fileStream << 'x';
				}
			}
			else
				fileStream << nodeState;

			fileStream << " ";
		}
		fileStream << "\n";
	}
	fileStream.close();
}

#pragma endregion