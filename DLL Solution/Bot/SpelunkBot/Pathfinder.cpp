#include "stdafx.h"
#include "Pathfinder.h"


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
			_grid[i][j] = new MapNode(i,j);

	_exit = Coords(-1, -1);
	_exitFound = false;
}

#pragma endregion

#pragma region Simplicity/node state methods


bool Pathfinder::Impassable(int x, int y)
{
	return _bot->IsNodePassable(x, y, NODE_COORDS);
}

bool Pathfinder::Impassable(Coords c)
{
	return Impassable(c.GetX(), c.GetY());
}

bool Pathfinder::Passable(int x, int y)
{
	return !Impassable(x,y);
}

bool Pathfinder::Passable(Coords c)
{
	return Passable(c.GetX(), c.GetY());
}

bool Pathfinder::Ladder(int x, int y)
{
	return (_bot->GetNodeState(x, y, NODE_COORDS) == spLadder ||
		_bot->GetNodeState(x, y, NODE_COORDS) == spLadderTop); //a laddertop is a special kind of ladder
}

bool Pathfinder::Ladder(Coords c)
{
	return Ladder(c.GetX(), c.GetY());
}

bool Pathfinder::LadderTop(int x, int y)
{
	return _bot->GetNodeState(x, y, NODE_COORDS) == spLadderTop;
}

bool Pathfinder::LadderTop(Coords c)
{
	return LadderTop(c.GetX(), c.GetY());
}

bool Pathfinder::Spikes(int x, int y)
{
	return _bot->GetNodeState(x, y, NODE_COORDS) == spSpike;
}

bool Pathfinder::Spikes(Coords c)
{
	return false;
}

bool Pathfinder::ArrowTrapR(int x, int y)
{
	return _bot->GetNodeState(x, y, NODE_COORDS) == spArrowTrapRight;
}

bool Pathfinder::ArrowTrapR(Coords c)
{
	return false;
}

bool Pathfinder::ArrowTrapL(int x, int y)
{
	return _bot->GetNodeState(x, y, NODE_COORDS) == spArrowTrapLeft;
}

bool Pathfinder::ArrowTrapL(Coords c)
{
	return false;
}

bool Pathfinder::SpearTrap(int x, int y)
{
	return _bot->GetNodeState(x, y, NODE_COORDS) == spSpearTrap;
}

bool Pathfinder::SpearTrap(Coords c)
{
	return false;
}

std::vector<Coords> Pathfinder::GetPathBetweenNodes(Coords start, Coords end, MOVEMENTACTION used_action)
{
	std::vector<Coords> path;

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
		path.push_back(Coords(endX, endY - 1));
		path.push_back(end);
		break;
	case CLIMBFROMHANG:
		path.push_back(Coords(startX, startY - 1));
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
				path.push_back(Coords(endX, y));
			}
		}

		break;
	case JUMPABOVERIGHT:
		path.push_back(Coords(startX + 1, startY));
		path.push_back(Coords(startX + 1, startY - 1));
		path.push_back(Coords(startX + 1, startY - 2));
		path.push_back(end);
		break;
	case JUMPABOVELEFT:
		path.push_back(Coords(startX - 1, startY));
		path.push_back(Coords(startX - 1, startY - 1));
		path.push_back(Coords(startX - 1, startY - 2));
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
				path.push_back(Coords(x, y));
			}
		}
		else
		{
			x = dirX == xRIGHT ? startX + 1 : startX - 1;
			y = startY - 1;
			path.push_back(Coords(x, y));

			if (x != endX)
			{
				dirX == xRIGHT ? x++ : x--;
				y--;
				path.push_back(Coords(x, y));

				tick = false;
				while (x != endX)
				{
					dirX == xRIGHT ? x++ : x--;
					if (tick) y++;
					path.push_back(Coords(x, y));

					tick = tick ? false : true;
				}
			}

			while (y != endY)
			{
				y < endY ? y++ : y--;
				path.push_back(Coords(x, y));
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
				path.push_back(Coords(x, y));
			}
		}
		else
		{
			x = dirX == xRIGHT ? startX + 1 : startX - 1;
			y = startY;
			path.push_back(Coords(x, y));

			tick = true;
			while (x != endX)
			{
				dirX == xRIGHT ? x++ : x--;
				if (tick) y++;
				path.push_back(Coords(x, y));

				tick = tick ? false : true;
			}

			while (y != endY)
			{
				y < endY ? y++ : y--;
				path.push_back(Coords(x, y));
			}
		}
		break;
	default:
		path.push_back(end);
		break;
	}

	return path;
}

bool Pathfinder::IsOutOfBounds(Coords c)
{
	if (c.GetX() < 0 || c.GetX() >= X_NODES ||
		c.GetY() < 0 || c.GetY() >= Y_NODES)
		return true;
	else
		return false;
}

bool Pathfinder::CanStandInNode(Coords c)
{
	return !IsInFog(c) && !IsInFog(c.OffsetY(1)) && Impassable(c) && 
		(Passable(c.OffsetY(1)) || LadderTop(c.OffsetY(1)));
		//(!_bot->IsNodePassable(x, y + 1, NODE_COORDS) || LadderTop(x, y + 1));

}

//bool Pathfinder::CanStandInNode(MapNode* n)
//{
//	return CanStandInNode(n->GetX(), n->GetY());
//}

bool Pathfinder::IsNodeValidBotPosition(Coords c)
{
	return CanStandInNode(c) || Ladder(c) || CanHangInNode(c);
}

bool Pathfinder::CanHangInNode(Coords c)
{
	return !IsInFog(c) && Impassable(c) && Impassable(c.OffsetY(-1)) &&
		((Passable(c.OffsetX(1)) && Impassable(c.Offset(1, -1))) || (Passable(c.OffsetX(-1)) && Impassable(c.Offset(-1, -1))));
}



//bool Pathfinder::IsInFog(MapSearchNode * n)
//{
//	return IsInFog(n->_x, n->_y);
//}

bool Pathfinder::IsFogOnMap()
{
	for (int i = 0; i < X_NODES; i++)
		for (int j = 0; j < Y_NODES; j++)
			if (_bot->GetFogState(i, j, NODE_COORDS))
				return true;
	return false;
}

Coords Pathfinder::GetBotCoords()
{
	return Coords(_bot->GetPlayerPositionXNode(), _bot->GetPlayerPositionYNode(), NODE);
}

//int Pathfinder::CalculateDistance(MapSearchNode * n, MapSearchNode * m)
//{
//	return abs(n->_x - m->_x) + abs(n->_y - m->_y);
//}

int Pathfinder::CalculateDistance(Coords c1, Coords c2)
{
	return abs(c1.GetX() - c2.GetX()) + abs(c1.GetY() - c2.GetY());
}

bool Pathfinder::IsInFog(Coords c)
{
	return _bot->GetFogState(c.GetX(), c.GetY(), NODE_COORDS);
}

bool Pathfinder::isCloseToFog(Coords c, int closeness)
{
	int x = c.GetX();
	int y = c.GetY();

	for (int i = 1; i <= closeness; i++)
		if ((!IsOutOfBounds(c.OffsetX(i)) && IsInFog(c.OffsetX(i))) ||
			(!IsOutOfBounds(c.OffsetY(i)) && IsInFog(c.OffsetY(i))) ||
			(!IsOutOfBounds(c.OffsetX(-i)) && IsInFog(c.OffsetX(-i))) ||
			(!IsOutOfBounds(c.OffsetY(-i)) && IsInFog(c.OffsetY(-i))))
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

//bool Pathfinder::isCloseToFog(Coords c, int closeness)
//{
//	return isCloseToFog(c.GetX(), c.GetY(), closeness);
//}

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
				if (Passable(x + i, y - j))
				{
					ok = false;
					break;
				}
			}
			else
			{
				if (Passable(x - i, y - j))
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

void Pathfinder::NewLevel()
{
	InitializeGrid();
}

bool Pathfinder::DownJumpPathClear(int x1, int y1, int x2, int y2, DIRECTIONX direction, bool fromLadder)
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

					//if jump is very short dont mind node which is in front and up from you, unless jump is from ladder
					if (distX == 1 && i == x1 + 1 && !fromLadder) continue;

				}

				if (Passable(i, j)) return false;
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
					if (distX == 1 && i == x1 - 1 && !fromLadder) continue;
				}

				if (Passable(i, j)) return false;
			}

			if (startX != x2) startX -= 1;
		}

		return true;
	}
}

bool Pathfinder::DownJumpPathClear(int x1, int y1, int x2, int y2, DIRECTIONX direction)
{
	return DownJumpPathClear(x1, y1, x2, y2, direction, false);
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
				if (Passable(i, j)) return false;

				//falling through narrow spaces is hard; walkoffledge cant do that or it is likely that it will get stuck.
				//until we rework it, we remove the neighbours getting to which requires flying througha narrow space.
				if (startX == x2)
				{
					if (Passable(i - 1, j) || Passable(i + 1, j)) return false;
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
				if (Passable(i, j)) return false;

				//falling through narrow spaces is hard; walkoffledge cant do that or it is likely that it will get stuck.
				//until we rework it, we remove the neighbours getting to which requires flying througha narrow space.
				if (startX == x2)
				{
					if (Passable(i - 1, j) || Passable(i + 1, j)) return false;
				}
			}

			if (startX != x2) startX -= 1;
		}

		return true;
	}
}

#pragma endregion

#pragma region Traps

void Pathfinder::DeleteUnsafeNeighbours(MapNode origin, std::vector<MapNode> &neighbours)
{
	DeleteUnsafeNeighboursSpikes(origin, neighbours);
	DeleteUnsafeNeighboursSpearTrap(origin, neighbours);
	DeleteUnsafeNeighboursArrowTrap(origin, neighbours);
}

void Pathfinder::DeleteUnsafeNeighboursSpikes(MapNode origin, std::vector<MapNode> &neighbours)
{
	int x, y, distX, distY;
	ACTION_TARGET target;
	MOVEMENTACTION action;
	bool unsafe;

	std::vector<MapNode>::iterator it = neighbours.begin();
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

void Pathfinder::DeleteUnsafeNeighboursSpearTrap(MapNode origin, std::vector<MapNode> &neighbours)
{
	int x, y, distX, distY;
	ACTION_TARGET target;
	MOVEMENTACTION action;
	bool unsafe;

	std::vector<MapNode>::iterator it = neighbours.begin();
	while (it != neighbours.end())
	{
		x = it->GetX();
		y = it->GetY();
		distX = x - origin.GetX();
		distY = y - origin.GetY();
		target = it->GetActionTarget();
		action = it->GetActionToReach();
		unsafe = false;

		if (SpearTrap(x + 1, y) || SpearTrap(x - 1, y)) unsafe = true;

		if (unsafe) it = neighbours.erase(it);
		else it++;
	}
}

void Pathfinder::DeleteUnsafeNeighboursArrowTrap(MapNode origin, std::vector<MapNode> &neighbours)
{
	int x, y, distX, distY;
	ACTION_TARGET target;
	MOVEMENTACTION action;
	bool unsafe;

	std::vector<MapNode>::iterator it = neighbours.begin();
	while (it != neighbours.end())
	{
		//walking to arrowtrap horizontally is safe
		if (it->GetActionToReach() == WALK)
		{
			it++;
			continue;
		}

		x = it->GetX();
		y = it->GetY();
		distX = x - origin.GetX();
		distY = y - origin.GetY();
		target = it->GetActionTarget();
		action = it->GetActionToReach();
		unsafe = false;

		std::vector<Coords> path = GetPathBetweenNodes(origin.GetCoords(), (*it).GetCoords(), action);


		for (auto n : path)
		{
			for (int i = 1; i <= 7; i++)
			{
				if (ArrowTrapR(n.GetX() - i, n.GetY()) && !_bot->IsArrowTrapDisarmed(n.GetX() - i, n.GetY()))
				{
					unsafe = true;
					break;
				}
				else if (Impassable(x - i, y))
					continue;
				else break;
			}

			for (int i = 1; i <= 7; i++)
			{
				if (ArrowTrapL(n.GetX() + i, n.GetY()) && !_bot->IsArrowTrapDisarmed(n.GetX() + i, n.GetY()))
				{
					unsafe = true;
					break;
				}
				else if (Impassable(x + i, y))
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

void Pathfinder::AddNeighboursLR(int x, int y, bool right, std::vector<MapNode> *neighbours)
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
				if (Passable(x + i, y - j) || LadderTop(x + i, y - j)) ok = false;
			}
			else
				if (Passable(x - i, y - j) || LadderTop(x - i, y - j)) ok = false;
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

			neighbours->push_back(MapNode{ x + candidateX, y, JUMP, LADDER, mvState });
		}

		if (Passable(x + candidateX, y + 1) && Impassable(x + candidateX, y))
		{
			if (abs(candidateX) == 1 || abs(candidateX) == 2)
			{
				neighbours->push_back(MapNode{ x + candidateX, y, WALK });
			}
			else
			{
				if (abs(candidateX) == 7)
					neighbours->push_back(MapNode{ x + candidateX, y, JUMP, LEDGE, mvHANGING});
				else
					neighbours->push_back(MapNode{ x + candidateX, y, JUMP, GROUND });
			}
			break;
		}

		if (LadderTop(x + candidateX, y + 1) && Impassable(x + candidateX, y))
		{
			if (abs(candidateX) == 1)
			{
				neighbours->push_back(MapNode{ x + candidateX, y, WALK, LADDERTOP });
			}
			else
			{
				if (abs(candidateX) < 7)
					neighbours->push_back(MapNode{ x + candidateX, y, JUMP, LADDERTOP });
			}
			break;
		}
	}
}

std::vector<MapNode> Pathfinder::CalculateNeighboursHanging(MapNode node)
{
	std::vector<MapNode> neighbours;
	int x = node.GetX();
	int y = node.GetY();
	int MAX;

	//drop
	MAX = 8; //max distance to ground tile
	for (int i = 1; i <= MAX; i++)
	{
		if (i > 1 && Ladder(x, y + i - 1) && !Passable(x, y + i))
		{
			neighbours.push_back(MapNode{ x, y + i - 1, DROP, LADDER, mvCLIMBING });
		}

		if (LadderTop(x, y + i) && y + i < Y_NODES)
		{
			neighbours.push_back(MapNode{ x, y + i - 1, DROP, LADDERTOP });
			break;
		}

		if (Passable(x, y + i) && y + i < Y_NODES)
		{
			neighbours.push_back(MapNode{ x, y + i - 1, DROP });
			break;
		}
	}

	//climb
	if (Passable(x + 1, y) && Impassable(x + 1, y - 1)) neighbours.push_back(MapNode{ x + 1, y - 1, CLIMBFROMHANG });
	if (Passable(x - 1, y) && Impassable(x - 1, y - 1)) neighbours.push_back(MapNode{ x - 1, y - 1, CLIMBFROMHANG });


	return neighbours;
}

std::vector<MapNode> Pathfinder::CalculateNeighboursClimbing(MapNode node)
{
	std::vector<MapNode> neighbours;
	int x = node.GetX();
	int y = node.GetY();
	int MAX, MAXx, MAXy;

	//climb
	if (Ladder(x, y - 1))
		neighbours.push_back(MapNode{ x, y - 1, CLIMB, LADDER, mvCLIMBING });
	if (Ladder(x, y + 1))
	{
		if (Passable(x, y + 2))
			neighbours.push_back(MapNode{ x, y + 1, CLIMB, GROUND });
		else
			neighbours.push_back(MapNode{ x, y + 1, CLIMB, LADDER, mvCLIMBING });
	}

	//drop
	MAX = 8; //max distance to ground tile
	for (int i = 1; i <= MAX; i++)
	{
		if (i > 1 && Ladder(x, y + i - 1) && !Passable(x, y + i))
		{
			neighbours.push_back(MapNode{ x, y + i - 1, DROP, LADDER, mvCLIMBING });
		}

		if (LadderTop(x, y + i) && y + i < Y_NODES)
		{
			neighbours.push_back(MapNode{ x, y + i - 1, DROP, LADDERTOP });
			break;
		}

		if (Passable(x, y + i) && y + i < Y_NODES)
		{
			neighbours.push_back(MapNode{ x, y + i - 1, DROP });
			break;
		}
	}

	//jump y=-2
	//right
	if (Impassable(x, y - 1) && Impassable(x, y - 2) && Impassable(x + 1, y - 2) && Passable(x + 1, y - 1))
		neighbours.push_back(MapNode{ x, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });
	if (Impassable(x, y - 1) && Impassable(x + 1, y) && Impassable(x + 1, y - 1) && Impassable(x + 1, y - 2) &&
		Impassable(x + 2, y - 2) && Passable(x + 2, y - 1))
		neighbours.push_back(MapNode{ x + 1, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });

	//left
	if (Impassable(x, y - 1) && Impassable(x, y - 2) && Impassable(x - 1, y - 2) && Passable(x - 1, y - 1))
		neighbours.push_back(MapNode{ x, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });
	if (Impassable(x, y - 1) && Impassable(x - 1, y) && Impassable(x - 1, y - 1) && Impassable(x - 1, y - 2) &&
		Impassable(x - 2, y - 2) && Passable(x - 2, y - 1))
		neighbours.push_back(MapNode{ x - 1, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });

	//jump y=-1
	//right
	int safedist = 3;
	for (int i = 0; i <= 3; i++)
	{
		if (Passable(x + i, y - 1))
		{
			safedist = i - 1;
			break;
		}
	}

	for (int i = 1; i <= safedist; i++)
	{
		if (Passable(x + i, y) && Impassable(x + i, y - 1))
		{
			if (i == 1 || i == 2) neighbours.push_back(MapNode{ x + i, y - 1, JUMPFROMLADDER, GROUND });
			if (i == 3) neighbours.push_back(MapNode{ x + i - 1, y, JUMPFROMLADDER, LEDGE, mvHANGING });
			break;
		}
		else if (Ladder(x + i, y - 1) && i != 3)
		{
			neighbours.push_back(MapNode{ x + i, y - 1, JUMPFROMLADDER, LADDER, mvCLIMBING });
		}

	}

	//jump y=-1
	//left
	safedist = 3;
	for (int i = 0; i <= 3; i++)
	{
		if (Passable(x - i, y - 1))
		{
			safedist = i - 1;
			break;
		}
	}

	for (int i = 1; i <= safedist; i++)
	{
		if (Passable(x - i, y) && Impassable(x - i, y - 1))
		{
			if (i == 1 || i == 2) neighbours.push_back(MapNode{ x - i, y - 1, JUMPFROMLADDER, GROUND });
			if (i == 3) neighbours.push_back(MapNode{ x - i + 1, y, JUMPFROMLADDER, LEDGE, mvHANGING });
			break;
		}
		else if (Ladder(x - i, y - 1) && i != 3)
		{
			neighbours.push_back(MapNode{ x - i, y - 1, JUMPFROMLADDER, LADDER, mvCLIMBING });
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
			if (DownJumpPathClear(x, y, x + i, y + j, xRIGHT, true))
			{
				if (Passable(x + i, y + j + 1) && Impassable(x + i, y + j))
					neighbours.push_back(MapNode{ x + i, y + j, JUMPFROMLADDER, GROUND });
				else if (LadderTop(x + i, y + j + 1) && Impassable(x + i, y + j))
					neighbours.push_back(MapNode{ x + i, y + j, JUMPFROMLADDER, LADDERTOP });
				else if (Ladder(x + i, y + j))
					neighbours.push_back(MapNode{ x + i, y + j, JUMPFROMLADDER, LADDER, mvCLIMBING });
			}

		}

	}

	//jump y=0..5
	//left
	for (int i = 1; i <= MAXx; i++)
		for (int j = 0; j <= MAXy; j++)
		{
			if (DownJumpPathClear(x, y, x - i, y + j, xLEFT, true))
			{
				if (Passable(x - i, y + j + 1) && Impassable(x - i, y + j))
					neighbours.push_back(MapNode{ x - i, y + j, JUMPFROMLADDER, GROUND });
				else if (LadderTop(x - i, y + j + 1) && Impassable(x - i, y + j))
					neighbours.push_back(MapNode{ x + i, y + j, JUMPFROMLADDER, LADDERTOP });
				else if (Ladder(x - i, y + j))
					neighbours.push_back(MapNode{ x - i, y + j, JUMPFROMLADDER, LADDER, mvCLIMBING });
			}
		}


	return neighbours;
}

std::vector<MapNode> Pathfinder::CalculateNeighboursClimbingWithMomentum(MapNode node)
{
	std::vector<MapNode> neighbours;
	int x = node.GetX();
	int y = node.GetY();
	int MAX, MAXx, MAXy;

	//climb
	if (Ladder(x, y - 1))
		neighbours.push_back(MapNode{ x, y - 1, CLIMB, LADDER, mvCLIMBING_WITH_MOMENTUM });
	if (Ladder(x, y + 1))
	{
		if (Passable(x, y + 2))
			neighbours.push_back(MapNode{ x, y + 1, CLIMB, GROUND });
		else
			neighbours.push_back(MapNode{ x, y + 1, CLIMB, LADDER, mvCLIMBING_WITH_MOMENTUM });
	}
	

	//drop
	MAX = 8; //max distance to ground tile
	for (int i = 1; i <= MAX; i++)
	{
		if (i > 1 && Ladder(x, y + i - 1) && !Passable(x, y + i))
		{
			neighbours.push_back(MapNode{ x, y + i - 1, DROP, LADDER, mvCLIMBING_WITH_MOMENTUM });
		}

		if (LadderTop(x, y + i) && y + i < Y_NODES)
		{
			neighbours.push_back(MapNode{ x, y + i - 1, DROP, LADDERTOP });
			break;
		}

		if (Passable(x, y + i) && y + i < Y_NODES)
		{
			neighbours.push_back(MapNode{ x, y + i - 1, DROP });
			break;
		}
	}


	//jump y=-2
	//right
	if (Impassable(x, y - 1) && Impassable(x, y - 2) && Impassable(x + 1, y - 2) && Passable(x + 1, y - 1))
		neighbours.push_back(MapNode{ x, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });
	if (Impassable(x, y - 1) && Impassable(x + 1, y) && Impassable(x + 1, y - 1) && Impassable(x + 1, y - 2) &&
		Impassable(x + 2, y - 2) && Passable(x + 2, y - 1))
		neighbours.push_back(MapNode{ x + 1, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });
	if (Impassable(x, y - 1) && Impassable(x + 1, y) && Impassable(x + 1, y - 1) && Impassable(x + 2, y - 1) &&
		Impassable(x + 2, y - 2) && Impassable(x + 3, y - 2) && Passable(x + 3, y - 1))
		neighbours.push_back(MapNode{ x + 2, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });

	//left
	if (Impassable(x, y - 1) && Impassable(x, y - 2) && Impassable(x - 1, y - 2) && Passable(x - 1, y - 1))
		neighbours.push_back(MapNode{ x, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });
	if (Impassable(x, y - 1) && Impassable(x - 1, y) && Impassable(x - 1, y - 1) && Impassable(x - 1, y - 2) &&
		Impassable(x - 2, y - 2) && Passable(x - 2, y - 1))
		neighbours.push_back(MapNode{ x - 1, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });
	if (Impassable(x, y - 1) && Impassable(x - 1, y) && Impassable(x - 1, y - 1) && Impassable(x - 2, y - 1) &&
		Impassable(x - 2, y - 2) && Impassable(x - 3, y - 2) && Passable(x - 3, y - 1))
		neighbours.push_back(MapNode{ x - 2, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });


	//jump y=-1
	MAX = 5;

	//right
	int safedist = MAX;
	for (int i = 0; i <= MAX; i++)
	{
		if (Passable(x + i, y - 1))
		{
			safedist = i - 1;
			break;
		}
	}

	bool roof = false;
	for (int i = 1; i < MAX; i++)
		if (Passable(x + i, y - 2))
		{
			roof = true;
			break;
		}

	for (int i = 1; i <= safedist; i++)
	{
		if (Passable(x + i, y))
		{
			if (i == 5 || (i == 4 && roof))
				neighbours.push_back(MapNode{ x + i - 1, y, JUMPFROMLADDER, LEDGE, mvHANGING });
			else 
				neighbours.push_back(MapNode{ x + i, y - 1, JUMPFROMLADDER, GROUND });

			break;
		}
		else if (Ladder(x + i, y - 1))
		{
			if (i == 5 || (i == 4 && roof)) 
				continue;
			else
				neighbours.push_back(MapNode{ x + i, y - 1, JUMPFROMLADDER, LADDER, mvCLIMBING_WITH_MOMENTUM });
		}
	}

	//left
	safedist = MAX;
	for (int i = 0; i <= MAX; i++)
	{
		if (Passable(x - i, y - 1))
		{
			safedist = i - 1;
			break;
		}
	}

	roof = false;
	for (int i = 1; i < MAX; i++)
		if (Passable(x - i, y - 2))
		{
			roof = true;
			break;
		}

	for (int i = 1; i <= safedist; i++)
	{
		if (Passable(x - i, y))
		{
			if (i == 5 || (i == 4 && roof))
				neighbours.push_back(MapNode{ x - i - 1, y, JUMPFROMLADDER, LEDGE, mvHANGING });
			else
				neighbours.push_back(MapNode{ x - i, y - 1, JUMPFROMLADDER, GROUND });

			break;
		}
		else if (Ladder(x - i, y - 1))
		{
			if (i == 5 || (i == 4 && roof))
				continue;
			else
				neighbours.push_back(MapNode{ x - i, y - 1, JUMPFROMLADDER, LADDER, mvCLIMBING_WITH_MOMENTUM });
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
			if (DownJumpPathClear(x, y, x + i, y + j, xRIGHT, true))
			{
				if (Passable(x + i, y + j + 1) && Impassable(x + i, y + j))
					neighbours.push_back(MapNode{ x + i, y + j, JUMPFROMLADDER, GROUND });
				else if (LadderTop(x + i, y + j + 1) && Impassable(x + i, y + j))
					neighbours.push_back(MapNode{ x + i, y + j, JUMPFROMLADDER, LADDERTOP });
				else if (Ladder(x + i, y + j))
					neighbours.push_back(MapNode{ x + i, y + j, JUMPFROMLADDER, LADDER, mvCLIMBING });
			}

		}

	//left
	for (int i = 1; i <= MAXx; i++)
		for (int j = 0; j <= MAXy; j++)
		{
			if (DownJumpPathClear(x, y, x - i, y + j, xLEFT, true))
			{
				if (Passable(x - i, y + j + 1) && Impassable(x - i, y + j))
					neighbours.push_back(MapNode{ x - i, y + j, JUMPFROMLADDER, GROUND });
				else if (LadderTop(x - i, y + j + 1) && Impassable(x - i, y + j))
					neighbours.push_back(MapNode{ x + i, y + j, JUMPFROMLADDER, LADDERTOP });
				else if (Ladder(x - i, y + j))
					neighbours.push_back(MapNode{ x - i, y + j, JUMPFROMLADDER, LADDER, mvCLIMBING });
			}
		}


	return neighbours;
}

std::vector<MapNode> Pathfinder::CalculateNeighboursStanding(MapNode node)
{
	std::vector<MapNode> neighbours;
	int x = node.GetX();
	int y = node.GetY();
	int MAX;


	/*
	* UP I
	*/

	if (Passable(x, y - 1) && Impassable(x, y - 2))
	{
		//right
		if (Impassable(x + 1, y) && Impassable(x + 1, y - 1) && Impassable(x + 1, y - 2))
			neighbours.push_back(MapNode{ x, y - 2, JUMPABOVERIGHT });
		//left
		if (Impassable(x - 1, y) && Impassable(x - 1, y - 1) && Impassable(x - 1, y - 2))
			neighbours.push_back(MapNode{ x, y - 2, JUMPABOVELEFT });
	}

	//LADDER UP
	if (Ladder(x, y - 1))
	{
		if (Ladder(x, y))
			neighbours.push_back(MapNode{ x, y - 1, CLIMB, LADDER, mvCLIMBING });
		else
			neighbours.push_back(MapNode{ x, y - 1, JUMP, LADDER, mvCLIMBING });
	}
	else if (Ladder(x, y - 2) && Impassable(x, y - 1))
		neighbours.push_back(MapNode{ x, y - 2, JUMP, LADDER, mvCLIMBING });


	/*
	* UP III (max=5 - possible, max=4 - safe)
	*/
	MAX = 6; //max travel distance (counting to ground not to ledge)

	//right
	int maxUpIII = MAX - 1;
	for (int i = 0; i <= maxUpIII; i++)
	{
		if (Impassable(x + i, y) && Impassable(x + i, y - 1) && Impassable(x + i + 1, y - 2))
		{
			if (i != 0 && Ladder(x + i, y - 2))
			{
				MVSTATE mvState;
				if (IsJumpWithRunning(i, -2, LADDER))
					mvState = mvCLIMBING_WITH_MOMENTUM;
				else
					mvState = mvCLIMBING;

				neighbours.push_back(MapNode{ x + i, y - 2, JUMP, LADDER, mvState });
			}

			if (Passable(x + i + 1, y - 1))
			{
				if (i == 0)
				{
					if (Impassable(x, y - 2))
						neighbours.push_back(MapNode{ x + i, y - 1, JUMP, LEDGE, mvHANGING });
				}
				else
					neighbours.push_back(MapNode{ x + i, y - 1, JUMP, LEDGE, mvHANGING });

				break;
			}

		}
		else break;
	}
	//left
	for (int i = 0; i >= -maxUpIII; i--)
	{
		if (Impassable(x + i, y) && Impassable(x + i, y - 1) && Impassable(x + i - 1, y - 2))
		{
			if (i != 0 && Ladder(x + i, y - 2))
			{
				MVSTATE mvState;
				if (IsJumpWithRunning(i, -2, LADDER))
					mvState = mvCLIMBING_WITH_MOMENTUM;
				else
					mvState = mvCLIMBING;

				neighbours.push_back(MapNode{ x + i, y - 2, JUMP, LADDER, mvState });
			}

			if (Passable(x + i - 1, y - 1))
			{
				if (i == 0)
				{
					if (Impassable(x, y - 2))
						neighbours.push_back(MapNode{ x, y - 1, JUMP, LEDGE, mvHANGING });
				}
				else
					neighbours.push_back(MapNode{ x + i, y - 1, JUMP, LEDGE, mvHANGING });
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
		if (Impassable(x + i, y) && Impassable(x + i, y - 1) && Impassable(x + i + 1, y - 1) && (abs(i) <= 1 || Impassable(x + i, y - 2)))
		{
			if (Ladder(x + i + 1, y - 1))
			{
				MVSTATE mvState;
				if (IsJumpWithRunning(i + 1, -1, LADDER))
					mvState = mvCLIMBING_WITH_MOMENTUM;
				else
					mvState = mvCLIMBING;

				neighbours.push_back(MapNode{ x + i + 1, y - 1, JUMP, LADDER, mvState });
			}

			if (Passable(x + i + 1, y))
			{
				if (i == 0)
					neighbours.push_back(MapNode{ x + i + 1, y - 1, JUMP});
				else if (i == MAX && Impassable(x + i, y + 1))
					neighbours.push_back(MapNode{ x + i, y, JUMP, LEDGE, mvHANGING });
				else
					neighbours.push_back(MapNode{ x + i + 1, y - 1, JUMP });
				break;
			}
		}
		else break;
	}

	//left
	for (int i = 0; i >= -maxUpVI; i--)
	{
		if (Impassable(x + i, y) && Impassable(x + i, y - 1) && Impassable(x + i - 1, y - 1) && (abs(i) <= 1 || Impassable(x + i, y - 2)))
		{
			if (Ladder(x + i - 1, y - 1))
			{
				MVSTATE mvState;
				if (IsJumpWithRunning(i + 1, -1, LADDER))
					mvState = mvCLIMBING_WITH_MOMENTUM;
				else
					mvState = mvCLIMBING;

				neighbours.push_back(MapNode{ x + i - 1, y - 1, JUMP, LADDER, mvState });
			}

			if (Passable(x + i - 1, y))
			{
				if (i == 0)
					neighbours.push_back(MapNode{ x + i - 1, y - 1, JUMP });
				else if (i == -MAX && Impassable(x + i, y + 1))
					neighbours.push_back(MapNode{ x + i, y, JUMP, LEDGE, mvHANGING });
				else
					neighbours.push_back(MapNode{ x + i - 1, y - 1, JUMP });
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
	if (Impassable(x + 1, y) && Impassable(x + 1, y + 1))
	{
		//if there are problems with hanging on a ladder with ground just below, restore hangdrop action
		//if (Passable(x + 1, y + 2)) neighbours.push_back(MapNode{ x + 1, y + 1, HANGDROP });
		//else
		//{
			if (Ladder(x + 1, y + 1) || LadderTop(x + 1, y + 1))
				neighbours.push_back(MapNode{ x + 1, y + 1, HANG, LADDER, mvCLIMBING });
			else
				neighbours.push_back(MapNode{ x + 1, y + 1, HANG, LEDGE, mvHANGING });
		//}
	}

	//left
	if (Impassable(x - 1, y) && Impassable(x - 1, y + 1))
	{
		//if (Passable(x - 1, y + 2)) neighbours.push_back(MapNode{ x - 1, y + 1, HANGDROP });
		//else
		//{
			if (Ladder(x - 1, y + 1) || LadderTop(x - 1, y + 1))
				neighbours.push_back(MapNode{ x - 1, y + 1, HANG, LADDER, mvCLIMBING });
			else
				neighbours.push_back(MapNode{ x - 1, y + 1, HANG, LEDGE, mvHANGING });
		//}
	}


	/*
	* DOWN jump
	*/
	int MAXx = 8; //max travel distance
	int MAXy = 5; //only 5 because we would take damage if we jumped further

	//right
	bool canHang = false;
	if (Impassable(x + 1, y + 1))
	{
		for (int i = 1; i <= MAXx; i++)
			for (int j = 1; j <= MAXy; j++)
			{
				//awkward blocked jump; bot will HangDrop instead
				if (i == 1 && (Passable(x, y - 1) || Passable(x + 1, y - 1)))
					continue;

				if (DownJumpPathClear(x, y, x + i, y + j, xRIGHT))
				{
					if (Passable(x + i, y + j + 1) && Impassable(x + i, y + j))
						neighbours.push_back(MapNode{ x + i, y + j, JUMP });

					if (LadderTop(x + i, y + j + 1) && Impassable(x + i, y + j))
						neighbours.push_back(MapNode{ x + i, y + j, JUMP, LADDERTOP });

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
							if (i != 1) neighbours.push_back(MapNode{ x + i, y + j, JUMP, LADDER, mvState });
						}
						else
						{
							neighbours.push_back(MapNode{ x + i, y + j, JUMP, LADDER, mvState });
						}
					}
				}

			}
				
	}

	//left
	canHang = false;
	if (Impassable(x - 1, y + 1))
	{
		for (int i = 1; i <= MAXx; i++)
			for (int j = 1; j <= MAXy; j++)
			{
				//awkward blocked jump; bot will HangDrop instead
				if (i == 1 && (Passable(x, y - 1) || Passable(x - 1, y - 1)))
					continue;

				if (DownJumpPathClear(x, y, x - i, y + j, xLEFT))
				{
					if (Passable(x - i, y + j + 1) && Impassable(x - i, y + j))
						neighbours.push_back(MapNode{ x - i, y + j, JUMP });

					if (LadderTop(x - i, y + j + 1) && Impassable(x + i, y + j))
						neighbours.push_back(MapNode{ x - i, y + j, JUMP, LADDERTOP });

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
							if (i != 1) neighbours.push_back(MapNode{ x - i, y + j, JUMP, LADDER, mvState });
						}
						else
						{
							neighbours.push_back(MapNode{ x - i, y + j, JUMP, LADDER, mvState });
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
	if (Impassable(x + 1, y + 1))
	{
		for (int i = 2; i <= MAXx; i++)
			for (int j = 1; j <= MAXy; j++)
			{
				if (i >= j + 3) continue; //fall is too steep
				else
				{
					if (WalkOffLedgePathClear(x, y, x + i, y + j, xRIGHT))
					{
						if (Passable(x + i, y + j + 1) && Impassable(x + i, y + j))
							neighbours.push_back(MapNode{ x + i, y + j, WALKOFFLEDGE });
						if (LadderTop(x + i, y + j + 1) && Impassable(x + i, y + j))
							neighbours.push_back(MapNode{ x + i, y + j, WALKOFFLEDGE, LADDERTOP });
					}
					//TODO: walkoffledge to ladder
					//we need to also differentiate which walkoffs are with running and which arent, 
					//and set mvState to climbing or climbing with momentum
					//if (Ladder(x + i, y + j) && WalkOffLedgePathClear(x, y, x + i, y + j, xRIGHT))
					//	neighbours.push_back(MapNode{ x + i, y + j, WALKOFFLEDGE, LADDER, mvCLIMBING });
				}
					
			}

	}

	//left
	if (Impassable(x - 1, y + 1))
	{
		for (int i = 2; i <= MAXx; i++)
			for (int j = 1; j <= MAXy; j++)
			{
				if (i >= j + 3) continue; //fall is too steep
				else
				{
					if (WalkOffLedgePathClear(x, y, x - i, y + j, xLEFT))
					{
						if (Passable(x - i, y + j + 1) && Impassable(x - i, y + j))
							neighbours.push_back(MapNode{ x - i, y + j, WALKOFFLEDGE });
						if (LadderTop(x - i, y + j + 1) && Impassable(x - i, y + j))
							neighbours.push_back(MapNode{ x - i, y + j, WALKOFFLEDGE, LADDERTOP });
					}
					//TODO: walkoffledge to ladder
					//we need to also differentiate which walkoffs are with running and which arent, 
					//and set mvState to climbing or climbing with momentum
					//if (Ladder(x - i, y + j) && WalkOffLedgePathClear(x, y, x - i, y + j, xLEFT))
					//	neighbours.push_back(MapNode{ x - i, y + j, WALKOFFLEDGE, LADDER, mvCLIMBING });
				}
					
			}

	}

	return neighbours;
}

std::vector<MapNode> Pathfinder::CalculateNeighboursStandingLT(MapNode node)
{
	std::vector<MapNode> neighbours;
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
		if (Passable(x, y + i + 1) && y + i + 1 < Y_NODES)
		{
			neighbours.push_back(MapNode{ x, y + i, DROP });
			break;
		}

		if (LadderTop(x, y + i + 1) && y + i + 1 < Y_NODES)
		{
			neighbours.push_back(MapNode{ x, y + i, DROP, LADDERTOP });
		}

		if (Ladder(x, y + i + 1) && i > 1)
		{
			neighbours.push_back(MapNode{ x, y + i, DROP, LADDER, mvCLIMBING });
		}
	}

	return neighbours;
}

std::vector<MapNode> Pathfinder::CalculateNeighbours(MapNode node, MVSTATE mvstate, ACTION_TARGET prevActionTarget)
{
	std::vector<MapNode> neighbours;
	
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

std::vector<MapNode*> Pathfinder::CalculateNeighboursInGrid(MapNode * node, MVSTATE mvstate, ACTION_TARGET target)
{
	std::vector<MapNode*> neighbours;
	
	std::vector<MapNode> n = CalculateNeighbours(*node, mvstate, target);
	for (int i = 0; i < n.size(); i++)
	{
		_grid[n[i].GetX()][n[i].GetY()]->_actionToReachCandidate = n[i].GetActionToReach();
		_grid[n[i].GetX()][n[i].GetY()]->_actionToReachTargetCandidate = n[i].GetActionTarget();
		_grid[n[i].GetX()][n[i].GetY()]->_mvStateCandidate = n[i].GetMvState();
		neighbours.push_back(_grid[n[i].GetX()][n[i].GetY()]);
	}
	
	return neighbours;
}

//std::vector<MapSearchNode*> Pathfinder::CalculateNeighboursInGrid(MapSearchNode* node, MVSTATE mvstate, ACTION_TARGET target)
//{
//	std::vector<MapSearchNode*> neighbours;
//
//	std::vector<Node> n = CalculateNeighboursInGrid(Node{ node->_x, node->_y }, mvstate, target);
//	for (int i = 0; i < n.size(); i++)
//	{
//		_grid[n[i].GetX()][n[i].GetY()]->_actionToReachCandidate = n[i].GetActionToReach();
//		_grid[n[i].GetX()][n[i].GetY()]->_actionToReachTargetCandidate = n[i].GetActionTarget();
//		_grid[n[i].GetX()][n[i].GetY()]->_mvStateCandidate = n[i].GetMvState();
//		neighbours.push_back(_grid[n[i].GetX()][n[i].GetY()]);
//	}
//
//	return neighbours;
//}

#pragma endregion

#pragma region Calculating strong connected components

void Pathfinder::TarjanDFS(MapNode* n)
{
	_tar_cvn++;
	_tar_VN[n->GetX()][n->GetY()] = _tar_cvn;
	_tar_VLow[n->GetX()][n->GetY()] = _tar_cvn;
	_tar_S.push(n);
	_tar_VS[n->GetX()][n->GetY()] = true;

	//will GetMvState work? IF not, we can estimate it --it seems to work for now
	for each (MapNode * m in CalculateNeighboursInGrid(n, n->GetMvState(), n->GetActionTarget()))
	{
		//m not visited - visit it recursively
		if (_tar_VN[m->GetX()][m->GetY()] == 0)
		{
			//we need to save mvstate which is saved as candidate during neighbour-finding
			m->_mvState = m->_mvStateCandidate;
			m->_actionToReachTarget = m->_actionToReachTargetCandidate;

			TarjanDFS(m);
			_tar_VLow[n->GetX()][n->GetY()] = min(_tar_VLow[n->GetX()][n->GetY()], _tar_VLow[m->GetX()][m->GetY()]);
		}
		else
		{
			//if node m is on the stack, a path m -> n exists
			//and we know that m is n's neighbour, so path n -> m exists
			//so n and m are in one connected component
			if (_tar_VS[m->GetX()][m->GetY()]) _tar_VLow[n->GetX()][n->GetY()] = min(_tar_VLow[n->GetX()][n->GetY()], _tar_VLow[m->GetX()][m->GetY()]);
		}
	}

	//continue only if you are at the root of the connected component
	if (_tar_VLow[n->GetX()][n->GetY()] != _tar_VN[n->GetX()][n->GetY()]) return;

	_tar_CCnr++;
	MapNode* m;
	vector<MapNode*> sccp;
	do
	{
		m = _tar_S.top();
		_tar_S.pop();
		_tar_VS[m->GetX()][m->GetY()] = false;
		m->_CCnr = _tar_CCnr;
		sccp.push_back(m);
	} while (m->GetX() != n->GetX() || m->GetY() != n->GetY());

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
			if (_tar_VN[i][j] == 0 && !IsInFog(_grid[i][j]->GetCoords()) &&
				(CanStandInNode(_grid[i][j]->GetCoords()) || Ladder(_grid[i][j]->GetCoords())))
				TarjanDFS(_grid[i][j]);
		}
}

#pragma endregion

#pragma region Getter methods

//int Pathfinder::GetCCnr(int nodeX, int nodeY)
//{
//	return _grid[nodeX][nodeY]->_CCnr;
//}
//
//int Pathfinder::GetCCnr(Node n)
//{
//	return _grid[n.GetX()][n.GetY()]->_CCnr;
//}
//
//int Pathfinder::GetCCnr(MapSearchNode n)
//{
//	return _grid[n.GetX()][n.GetY()]->_CCnr;
//}
//
//int Pathfinder::GetCCnr(Item item)
//{
//	return _grid[item.GetX()][item.GetY()]->_CCnr;
//}

int Pathfinder::GetCCnr(Coords c)
{
	return _grid[c.GetX()][c.GetY()]->_CCnr;
}

std::vector<MapNode*> Pathfinder::GetAllNodesFromCC(int ccnr)
{
	return _tar_CCmap[ccnr];
}

//MapNode* Pathfinder::GetNodeFromGrid(int x, int y)
//{
//	return _grid[x][y];
//}

MapNode* Pathfinder::GetNodeFromGrid(Coords c)
{
	return _grid[c.GetX()][c.GetY()];
}

//Node Pathfinder::ToNode(MapSearchNode * n)
//{
//	return Node(n->_x, n->_y, n->_actionToReach, n->_actionToReachTarget, n->GetMvState());
//}

std::vector<MapNode*> Pathfinder::GetPathFromGrid()
{
	return _pathList;
}

std::vector<MapNode> Pathfinder::GetPath()
{
	std::vector<MapNode> pathCopy;

	for (int i = 0; i < _pathList.size(); i++)
	{
		//dzia³a dobrze, mo¿e byæ problem z rzeczami wykorzystuj¹cymi dynamiczn¹ pamiêæ, ale ja i tak ich nie u¿ywam
		MapNode n = MapNode(*_pathList[i]);
		pathCopy.push_back(n);
	}

	return pathCopy;
}

//std::vector<Node> Pathfinder::GetPathListNode()
//{
//	std::vector<Node> pathListNode;
//	for (int i = 0; i < _pathList.size(); i++)
//		pathListNode.push_back(Node{ _pathList[i]->_x, _pathList[i]->_y, _pathList[i]->_actionToReach, _pathList[i]->_actionToReachTarget, _pathList[i]->GetMvState() });
//
//	return pathListNode;
//}

Coords Pathfinder::GetExplorationTarget()
{
	return _explorationTarget;
}

int Pathfinder::GetPathLength()
{
	return GetPathLength(GetPathFromGrid());
}

int Pathfinder::GetPathLength(std::vector<MapNode*> path)
{
	int dist = 0;
	if (!path.empty())
	{
		auto current = path[0];

		for (int i = 1; i < path.size(); i++)
		{
			dist += abs(current->GetX() - path[i]->GetX()) + abs(current->GetY() - path[i]->GetY());
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
				_exit = Coords(nodeX, nodeY);
				_exitFound = true;
				return;
			}
		}
	}
}

Coords Pathfinder::GetExit()
{
	return _exit;
}

bool Pathfinder::IsExitFound()
{
	return _exitFound;
}

#pragma endregion

#pragma region Dijkstra

void Pathfinder::Dijkstra(Coords startc)
{
	int x, y;
	x = startc.GetX();
	y = startc.GetY();

	for (int i = 0; i < X_NODES; i++)
		for (int j = 0; j < Y_NODES; j++)
		{
			_grid[i][j]->_dij_dist = INT_MAX;
			_grid[i][j]->_dij_prev = NULL;
			_dij_visited[i][j] = false;
		}


	_grid[x][y]->_dij_dist = 0;
	_dij_visited[x][y] = true;

	while (!_dij_pQ.empty())
		_dij_pQ.pop();

	_dij_pQ.push(_grid[x][y]);

	while (!_dij_pQ.empty())
	{
		MapNode *n = _dij_pQ.top();
		_dij_pQ.pop();

		//break if you reach unreachable nodes
		if (n->_dij_dist == INT_MAX) break;

		vector<MapNode*> neighbours = CalculateNeighboursInGrid(n, n->GetMvState(), n->GetActionTarget());

		for (MapNode* m : neighbours)
		{
			if (!_dij_visited[m->GetX()][m->GetY()])
				if (n->_dij_dist + CalculateDistance(n->GetCoords(), m->GetCoords()) < m->_dij_dist)
				{
					m->_dij_dist = n->_dij_dist + CalculateDistance(n->GetCoords(), m->GetCoords());
					m->_dij_prev = n;
					m->_mvState = m->_mvStateCandidate;
					m->_actionToReachTarget = m->_actionToReachTargetCandidate;
					m->_actionToReach = m->_actionToReachCandidate;
					_dij_pQ.push(m);
					_dij_visited[m->GetX()][m->GetY()] = true;
				}
		}

	}
}

std::vector<MapNode*> Pathfinder::GetDijPath(Coords targetC)
{
	std::vector<MapNode*> vect;
	MapNode *current = _grid[targetC.GetX()][targetC.GetY()];

	while (current != NULL)
	{
		vect.push_back(current);
		current = current->_dij_prev;
	}

	reverse(vect.begin(), vect.end());

	return vect;
}

//std::vector<Node> Pathfinder::GetDijPathNode(int targetX, int targetY)
//{
//	std::vector<MapSearchNode*> vect = GetDijPath(targetX, targetY);
//	std::vector<Node> vectNode;
//	for (int i = 0; i < vect.size(); i++)
//		vectNode.push_back(ToNode(vect[i]));
//	
//	return vectNode;
//}



//int Pathfinder::GetDijDist(int x, int y)
//{
//	return _grid[x][y]->_dij_dist;
//}
//
//int Pathfinder::GetDijDist(Item i)
//{
//	return _grid[i.GetX()][i.GetY()]->_dij_dist;
//}

int Pathfinder::GetDijDist(Coords c)
{
	return _grid[c.GetX()][c.GetY()]->_dij_dist;
}

#pragma endregion

#pragma region Pathfinding methods

bool Pathfinder::TryToCalculatePath(Coords c1, Coords c2)
{
	bool DEBUG = false;

	bool path_found = false;

	if (c1.GetX() != c2.GetX() || c1.GetY() != c2.GetY())
	{
		_pathList.clear();

		ofstream fileStream;
		if (DEBUG) fileStream.open(".\\Pathfinder\\level_paths.txt");

		//we have to take start from grid to be able to use info about previous action
		MapNode* start = GetNodeFromGrid(c1);
		MapNode* end = GetNodeFromGrid(c2);

		if (DEBUG)
		{
			fileStream << "START:" << endl;
			fileStream << "X: ";
			fileStream << start->GetX();
			fileStream << " Y: ";
			fileStream << start->GetY();
			fileStream << endl;

			fileStream << "END" << endl;
			fileStream << "X: ";
			fileStream << end->GetX();
			fileStream << " Y: ";
			fileStream << end->GetY();
			fileStream << endl;
		}

		MapNode* current = new MapNode();
		MapNode* child = new MapNode();

		std::list<MapNode*> openList;
		std::list<MapNode*> closedList;
		list<MapNode*>::iterator i;

		unsigned int n = 0;

		openList.push_back(start);
		start->_opened = true;

		while (n == 0 || (current != end && n < 500))
		{
			// Look for the smallest f value in the openList
			for (i = openList.begin(); i != openList.end(); i++)
			{
				if (i == openList.begin() || (*i)->CalculateFScore() <= current->CalculateFScore())
				{
					current = (*i);
				}
			}

			if (DEBUG)
			{
				fileStream << "searching";
				fileStream << " Current X: ";
				fileStream << current->GetX();
				fileStream << " Current Y: ";
				fileStream << current->GetY();
			}

			// Stop if we've reached the end
			if (current->GetX() == end->GetX() && current->GetY() == end->GetY())
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

			std::vector<MapNode*> neighbours = CalculateNeighboursInGrid(current, current->GetMvState(), current->GetActionTarget());

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
					if (child->_gScore > child->CalculateGScore(current))
					{
						child->_parent = current;
						child->_actionToReach = child->_actionToReachCandidate;
						child->_actionToReachTarget = child->_actionToReachTargetCandidate;
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
					child->_actionToReachTarget = child->_actionToReachTargetCandidate;
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
				fileStream << _pathList[i]->GetX();
				fileStream << " Y ";
				fileStream << _pathList[i]->GetY();
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

bool Pathfinder::TryToFindExplorationTarget(Coords startc)
{
	bool targetFound = false;
	std::list<MapNode*> visitedList;
	list<MapNode*>::iterator i;

	_pathList.clear();
	
	MapNode* start = GetNodeFromGrid(startc);
	start->_visited = true;
	visitedList.push_back(start);

	MapNode* current = start;

	unsigned int n = 0;

	while (n < 1000)
	{
		if (isCloseToFog(current->GetCoords(), 5))
		{
			targetFound = true;
			_explorationTarget = current->GetCoords();
			break;
		}

		vector<MapNode*> neighbours = CalculateNeighboursInGrid(current, current->GetMvState(), current->GetActionTarget());

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
					current->_actionToReachTarget = current->_actionToReachTargetCandidate;
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

//std::vector<Node> Pathfinder::FindExplorationTargets(int x1, int y1)
//{
//	std::vector<Node> explTargets;
//	std::list<MapSearchNode*> visitedList;
//	list<MapSearchNode*>::iterator i;
//
//	_pathList.clear();
//
//	MapSearchNode* start = _grid[x1][y1];
//	start->_visited = true;
//	visitedList.push_back(start);
//
//	MapSearchNode* current = start;
//	
//
//	unsigned int n = 0;
//
//	while (n < 1000)
//	{
//		if (isCloseToFog(current, 5))
//		{
//			explTargets.push_back(Node(current->_x, current->_y));
//		}
//
//		vector<MapSearchNode*> neighbours = CalculateNeighboursInGrid(current, current->GetMvState(), current->GetActionTarget());
//
//		//no neighbours - backtracking
//		if (neighbours.empty())
//		{
//			if (current->_parent != NULL)
//				current = current->_parent;
//			else
//				//current does not have a parent, we can't backtrack
//				break; 
//		}
//		else
//		{
//			//find first not visited neighbour
//			int i = 0;
//			while (i < neighbours.size())
//			{
//				if (!neighbours[i]->_visited)
//				{
//					//first neighbour becomes current
//					neighbours[i]->_parent = current;
//					current = neighbours[i];
//
//					current->_visited = true;
//					visitedList.push_back(current);
//					current->_actionToReach = current->_actionToReachCandidate;
//					current->_actionToReachTarget = current->_actionToReachTargetCandidate;
//					current->_mvState = current->_mvStateCandidate;
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
//					//current does not have a parent, we can't backtrack
//					break; 
//			}
//		}
//
//		n++;
//	}
//
//	//reset
//	for (i = visitedList.begin(); i != visitedList.end(); i++)
//	{
//		(*i)->_visited = false;
//		(*i)->_parent = NULL;
//	}
//
//	return explTargets;
//}

#pragma endregion

#pragma region Debug methods

void Pathfinder::NeighboursDebug(Coords c, bool hasMomentum)
{
	int x, y;
	x = c.GetX();
	y = c.GetY();
	ofstream fileStream;
	fileStream.open(".\\Pathfinder\\neighbours.txt");

	MVSTATE mvState = ToMvState(_bot->GetSpelunkerState());
	if (mvState == mvCLIMBING && hasMomentum)
		mvState = mvCLIMBING_WITH_MOMENTUM;

	ACTION_TARGET target;
	if (Impassable(x, y) && Passable(x, y + 1)) target = GROUND;
	else if (Ladder(x, y) && !LadderTop(x, y)) target = LADDER;
	else if (Impassable(x, y) && LadderTop(x, y + 1)) target = LADDERTOP;
	else target = LEDGE;

	std::vector<MapNode> neighbours = CalculateNeighbours(MapNode(x,y), mvState, target);

	for (int i = 0; i < neighbours.size(); i++)
	{
		fileStream << " X: " << neighbours[i].GetX();
		fileStream << " Y: " << neighbours[i].GetY();
		fileStream << " " << MVactionStrings[neighbours[i].GetActionToReach()];
		fileStream << " " << ActionTargetStrings[neighbours[i].GetActionTarget()];
		fileStream << endl;

		auto path = GetPathBetweenNodes(_grid[x][y]->GetCoords(), neighbours[i].GetCoords(), neighbours[i].GetActionToReach());
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

void Pathfinder::DijkstraDebug(bool exact)
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
			else if (IsNodeValidBotPosition(_grid[i][j]->GetCoords()))
			{
				if (_grid[i][j]->_dij_dist == INT_MAX) fileStream << 'M';
				else 
				{
					if (exact)
						fileStream << _grid[i][j]->_dij_dist;
					else
					{
						if (_grid[i][j]->_dij_dist == 0) fileStream << '*';
						else if (_grid[i][j]->_dij_dist <= 5) fileStream << 'a';
						else if (_grid[i][j]->_dij_dist <= 10) fileStream << 'b';
						else if (_grid[i][j]->_dij_dist <= 15) fileStream << 'c';
						else if (_grid[i][j]->_dij_dist <= 20) fileStream << 'd';
						else if (_grid[i][j]->_dij_dist <= 25) fileStream << 'e';
						else if (_grid[i][j]->_dij_dist <= 30) fileStream << 'f';
						else if (_grid[i][j]->_dij_dist <= 35) fileStream << 'g';
						else if (_grid[i][j]->_dij_dist <= 40) fileStream << 'h';
						else if (_grid[i][j]->_dij_dist <= 45) fileStream << 'i';
						else if (_grid[i][j]->_dij_dist <= 50) fileStream << 'j';
						else if (_grid[i][j]->_dij_dist <= 55) fileStream << 'k';
						else if (_grid[i][j]->_dij_dist <= 60) fileStream << 'l';
						else fileStream << 'x';
					}

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