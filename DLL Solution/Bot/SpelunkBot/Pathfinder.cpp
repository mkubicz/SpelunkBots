#include "stdafx.h"
#include "Pathfinder.h"


using namespace std;

#pragma region Constructors and initialization

Pathfinder::Pathfinder(std::shared_ptr<IBot> bot, std::shared_ptr<MovementSimulator> msim)
	: _bot(bot), _moveSim(msim)
{
	InitializeGrid();
	NewLevel();
}

void Pathfinder::InitializeGrid()
{
	for (int i = 0; i < X_NODES; i++)
	{
		_grid.push_back(std::vector<MapNode*>());

		for (int j = 0; j < Y_NODES; j++)
			_grid[i].push_back(new MapNode(i, j));
	}
}

void Pathfinder::NewLevel()
{
	//for (int i = 0; i < X_NODES; i++)
	//	for (int j = 0; j < Y_NODES; j++)
	//		_grid[i][j] = new MapNode(i, j);
	for (int i = 0; i < X_NODES; i++)
		for (int j = 0; j < Y_NODES; j++)
			_grid[i][j]->Reset();

	_pathList.clear();
	_exit = Coords(-1, -1);
	_explorationTarget = Coords(-1, -1);
	_nextCCTarget = Coords(-1, -1);
	_exitFound = false;
}



Pathfinder::~Pathfinder()
{
	for (int i = 0; i < X_NODES; i++)
	{
		for (int j = 0; j < Y_NODES; j++)
		{
			delete _grid[i][j];
		}
		_grid[i].clear();
	}

	_grid.clear();
	

	////pathlist contains pointers to nodes from _grid, which we already deleted
	////for (int i = 0; i < _pathList.size(); i++)
	////	delete (_pathList[i]);
	//_pathList.clear();
}

//void Pathfinder::InitializeGrid()
//{
//	for (int i = 0; i < X_NODES; i++)
//		for (int j = 0; j < Y_NODES; j++)
//			_grid[i][j] = new MapNode(i,j);
//
//	_exit = Coords(-1, -1);
//	_exitFound = false;
//}

#pragma endregion

#pragma region Simplicity/node state methods


bool Pathfinder::Passable(int x, int y)
{
	return _bot->IsNodePassable(x, y, NODE_COORDS);
}

bool Pathfinder::Passable(Coords c)
{
	return Passable(c.GetX(), c.GetY());
}

bool Pathfinder::Impassable(int x, int y)
{
	return !_bot->IsNodePassable(x, y, NODE_COORDS);
}

bool Pathfinder::Impassable(Coords c)
{
	return Impassable(c.GetX(), c.GetY());
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
	return Spikes(c.GetX(), c.GetY());
}

bool Pathfinder::ArrowTrapR(int x, int y)
{
	return _bot->GetNodeState(x, y, NODE_COORDS) == spArrowTrapRight;
}

bool Pathfinder::ArrowTrapR(Coords c)
{
	return ArrowTrapR(c.GetX(), c.GetY());
}

bool Pathfinder::ArrowTrapL(int x, int y)
{
	return _bot->GetNodeState(x, y, NODE_COORDS) == spArrowTrapLeft;
}

bool Pathfinder::ArrowTrapL(Coords c)
{
	return ArrowTrapL(c.GetX(), c.GetY());
}

bool Pathfinder::SpearTrap(int x, int y)
{
	return _bot->GetNodeState(x, y, NODE_COORDS) == spSpearTrap;
}

bool Pathfinder::SpearTrap(Coords c)
{
	return SpearTrap(c.GetX(), c.GetY());
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
	case HANGDROP:
		path.push_back(Coords(endX, endY - 1));
		path.push_back(end);
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
			//x = dirX == xRIGHT ? startX + 1 : startX - 1;
			//y = startY - 1;
			//path.push_back(Coords(x, y));
			
			x = startX;
			y = startY - 1;
			path.push_back(Coords(x, y));
			dirX == xRIGHT ? x++ : x--;
			path.push_back(Coords(x, y));

			if (x != endX)
			{
				//dirX == xRIGHT ? x++ : x--;
				//y--;
				//path.push_back(Coords(x, y));

				y--;
				path.push_back(Coords(x, y));
				dirX == xRIGHT ? x++ : x--;
				path.push_back(Coords(x, y));

				tick = false;
				while (x != endX)
				{
					//dirX == xRIGHT ? x++ : x--;
					//if (tick) y++;
					//path.push_back(Coords(x, y));

					if (tick)
					{
						y++;
						path.push_back(Coords(x, y));
					}
					dirX == xRIGHT ? x++ : x--;
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
				//dirX == xRIGHT ? x++ : x--;
				//if (tick) y++;
				//path.push_back(Coords(x, y));

				if (tick)
				{
					y++;
					path.push_back(Coords(x, y));
				}
				dirX == xRIGHT ? x++ : x--;
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
	return !IsInFog(c) && !IsInFog(c.OffsetY(1)) && Passable(c) && 
		(Impassable(c.OffsetY(1)) || LadderTop(c.OffsetY(1)));
		//(!_bot->IsNodePassable(x, y + 1, NODE_COORDS) || LadderTop(x, y + 1));

}

bool Pathfinder::IsNodeValidBotPosition(Coords c)
{
	return CanStandInNode(c) || Ladder(c) || CanHangInNode(c);
}

bool Pathfinder::CanHangInNode(Coords c)
{
	return !IsInFog(c) && Passable(c) && Passable(c.OffsetY(-1)) &&
		((Impassable(c.OffsetX(1)) && Passable(c.Offset(1, -1))) || (Impassable(c.OffsetX(-1)) && Passable(c.Offset(-1, -1))));
}


bool Pathfinder::IsFogOnMap()
{
	for (int i = 0; i < X_NODES; i++)
		for (int j = 0; j < Y_NODES; j++)
			if (_bot->GetFogState(i, j, NODE_COORDS))
				return true;
	return false;
}

bool Pathfinder::NoArrowTrapsOnPath(std::vector<MapNode*> path)
{
	for (auto node : path)
		if (node->IsArrowTrapOnWay())
			return false;

	return true;
}

directions Pathfinder::CalculateDirections(Coords from, Coords to)
{
	return directions{ CalculateDirX(from, to), CalculateDirY(from, to) };
}

DIRECTIONX Pathfinder::CalculateDirX(Coords from, Coords to)
{
	DIRECTIONX dirX;

	if (to.GetX() > from.GetX()) dirX = xRIGHT;
	else if (to.GetX() == from.GetX()) dirX = xNONE;
	else dirX = xLEFT;

	return dirX;
}

DIRECTIONY Pathfinder::CalculateDirY(Coords from, Coords to)
{
	DIRECTIONY dirY;

	if (to.GetY() > from.GetY()) dirY = yDOWN;
	else if (to.GetY() == from.GetY()) dirY = yNONE;
	else dirY = yUP;

	return dirY;
}

Coords Pathfinder::GetBotCoords()
{
	return Coords(_bot->GetPlayerPositionXNode(), _bot->GetPlayerPositionYNode(), NODE);
}

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
				if (Impassable(x + i, y - j))
				{
					ok = false;
					break;
				}
			}
			else
			{
				if (Impassable(x - i, y - j))
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

				if (Impassable(i, j)) return false;
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

				if (Impassable(i, j)) return false;
			}

			if (startX != x2) startX -= 1;
		}

		return true;
	}
	else
		return false;
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
				if (Impassable(i, j)) return false;

				//falling through narrow spaces is hard; walkoffledge cant do that or it is likely that it will get stuck.
				//until we rework it, we remove the neighbours getting to which requires flying througha narrow space.
				if (startX == x2)
				{
					if (Impassable(i - 1, j) || Impassable(i + 1, j)) return false;
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
				if (Impassable(i, j)) return false;

				//falling through narrow spaces is hard; walkoffledge cant do that or it is likely that it will get stuck.
				//until we rework it, we remove the neighbours getting to which requires flying througha narrow space.
				if (startX == x2)
				{
					if (Impassable(i - 1, j) || Impassable(i + 1, j)) return false;
				}
			}

			if (startX != x2) startX -= 1;
		}

		return true;
	}
	else
		return false;
}

#pragma endregion

#pragma region Traps

void Pathfinder::HandleUnsafeNeighbours(MapNode origin, std::vector<MapNode> &neighbours)
{
	DeleteUnsafeNeighboursSpikes(origin, neighbours);
	HandleUnsafeNeighboursSpearTrap(origin, neighbours);
	//HandleUnsafeNeighboursArrowTrap(origin, neighbours);
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
		target = it->GetPrevActionTarget();
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

void Pathfinder::HandleUnsafeNeighboursSpearTrap(MapNode origin, std::vector<MapNode> &neighbours)
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
		target = it->GetPrevActionTarget();
		action = it->GetActionToReach();
		unsafe = false;

		if (SpearTrap(x + 1, y) || SpearTrap(x - 1, y)) unsafe = true;

		if (unsafe) it = neighbours.erase(it);
		else it++;
	}
}

void Pathfinder::HandleUnsafeNeighboursArrowTrap(MapNode origin, std::vector<MapNode> &neighbours, AT_MODE atmode,
	std::vector<Coords> allowedArrowTraps)
{
	int x, y, distX, distY;
	ACTION_TARGET target;
	MOVEMENTACTION action;
	bool unsafe;
	Coords coords;

	std::vector<MapNode>::iterator neighbour = neighbours.begin();
	while (neighbour != neighbours.end())
	{
		//walking to arrowtrap horizontally is safe
		if (neighbour->GetActionToReach() == WALK)
		{
			neighbour++;
			continue;
		}

		x = neighbour->GetX();
		y = neighbour->GetY();
		distX = x - origin.GetX();
		distY = y - origin.GetY();
		target = neighbour->GetPrevActionTarget();
		action = neighbour->GetActionToReach();
		unsafe = false;
		

		std::vector<Coords> path = GetPathBetweenNodes(origin.GetCoords(), (*neighbour).GetCoords(), action);


		for (auto n : path)
		{
			//arrowTrapR has shorter range then arrowTrapL
			for (int i = 1; i <= 6; i++)
			{
				if (ArrowTrapR(n.GetX() - i, n.GetY()) && !_bot->IsArrowTrapDisarmed(n.GetX() - i, n.GetY()))
				{
					//HANG and HANGDROP is technically safe and can disarm a trap, but I don't have the infrastructure
					//to use it yet and it only causes problems
					//if (i == 1 && (action == HANG || action == HANGDROP) && origin.GetCoords() == n.Offset(-1, -1))
					//	continue;

					unsafe = true;
					coords = Coords(n.GetX() - i, n.GetY());
					break;
				}
				else if (Passable(n.GetX() - i, n.GetY()))
					continue;
				else break;
			}

			for (int i = 1; i <= 7; i++)
			{
				//if (i == 1 && (action == HANG || action == HANGDROP) && origin.GetCoords() == n.Offset(1, -1))
				//	continue;

				if (ArrowTrapL(n.GetX() + i, n.GetY()) && !_bot->IsArrowTrapDisarmed(n.GetX() + i, n.GetY()))
				{
					unsafe = true;
					coords = Coords(n.GetX() + i, n.GetY());
					break;
				}
				else if (Passable(n.GetX() + i, n.GetY()))
					continue;
				else break;
			}

			if (unsafe) break;
		}

		//czy jak n wyjdzie ze scope to coords siê zr¹bie?
		//nie

		if (unsafe)
		{
			neighbour->_arrowTrapOnWay = true;
			neighbour->_arrowTrapCoords = coords;

			switch (atmode)
			{
			case STOP_ON_ARROWTRAPS:
				neighbour = neighbours.erase(neighbour);
				break;
			case ALLOW_ARROWTRAPS:
				neighbour++;
				break;
			case ALLOW_SELECTED:
			{
				bool allowed = false;

				for (auto c : allowedArrowTraps)
					if (coords == c)
					{
						allowed = true;
						neighbour++;
						break;
					}

				if (!allowed)
					neighbour = neighbours.erase(neighbour);

				break;
			}
			default:
				break;
			}
		}
		else 
			neighbour++;
	}
}

#pragma endregion

#pragma region Calculating neighbours

void Pathfinder::AddNeighboursLR(int x, int y, bool right, std::vector<MapNode> *neighbours)
{
	//old; in case of problems - use CalculatePathBeetweenNodes

	int MAX = 7; //maximum travel distance
	int borderY = 0;
	int okDist = 0;
	bool ok = true;

	for (int i = 1; i < MAX; i++)
	{
		if (i <= 1) borderY = 0;
		if (i > 1 && i <= 3) borderY = 1;
		if (i > 3) borderY = 2;

		//int n = right ? 1 : -1;
		//if (i == 3)
		//	for (int k = 0; k < 3; k++)
		//		if (Impassable(x + k*n, y - 1) || LadderTop(x + k*n, y - 1)) ok = false;

		//if (i == 5)
		//{
		//	for (int k = 0; k < 5; k++)
		//		if (Impassable(x + k*n, y - 1) || LadderTop(x + k*n, y - 1)) ok = false;
		//	for (int k = 1; k < 5; k++)
		//		if (Impassable(x + k*n, y - 2) || LadderTop(x + k*n, y - 2)) ok = false;
		//}
		//	

		for (int j = 0; j <= borderY; j++)
		{
			if (right)
			{
				if (Impassable(x + i, y - j) || LadderTop(x + i, y - j)) ok = false;
			}
			else
				if (Impassable(x - i, y - j) || LadderTop(x - i, y - j)) ok = false;
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

		if (Impassable(x + candidateX, y + 1) && Passable(x + candidateX, y))
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

		if (LadderTop(x + candidateX, y + 1) && Passable(x + candidateX, y))
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

void Pathfinder::RemoveDuplicateNeighbours(std::vector<MapNode> &neighbours)
{
	bool n1_erased;
	std::vector<MapNode>::iterator n1, n2; 
	n1 = neighbours.begin();

	while (n1 != neighbours.end())
	{
		n1_erased = false;
		n2 = n1 + 1;

		while (n2 != neighbours.end())
		{
			//if neighbours pointed by n1, n2 are equal (have the same coordinates)
			if (*n1 == *n2)
			{
				if (n2->GetPenalty() < n1->GetPenalty())
				{
					//erase n1
					n1 = neighbours.erase(n1);
					//if n1 points to the same element as n2 move n2
					if (n1 == n2) n2++;
					n1_erased = true;
					break;
				}
				else
				{
					//erase n2
					n2 = neighbours.erase(n2);
				}
			}
			else n2++;
		}

		if (!n1_erased) n1++;
	}


	//for (auto n = neighbours.begin(); n != neighbours.end(); n++)
	//{
	//	for (auto n = neighbours.begin(); n != neighbours.end(); n++)
	//		if (*nt == *n)
	//		{
	//			if (nt->GetPenalty() < n->GetPenalty())
	//			{
	//				neighbours.erase(n);
	//				neighbours.push_back(*nt);
	//			}
	//			dupe_found = true;
	//			break;
	//		}

	//	if (!dupe_found)
	//	{
	//		neighbours.push_back(*nt);
	//	}
	//}

}

//std::vector<MapNode> Pathfinder::RemoveNeighboursWithArrowTraps(std::vector<MapNode> &neighboursTemp, 
//	AT_MODE atmode, 
//	std::vector<Coords> allowedArrowTraps)
//{
//	std::vector<MapNode> neighbours;
//
//	for (MapNode n : neighboursTemp)
//	{
//		switch (atmode)
//		{
//		case STOP_ON_ARROWTRAPS:
//			if (!n._arrowTrapOnWay) 
//				neighbours.push_back(n);
//			break;
//		case ALLOW_ARROWTRAPS:
//			neighbours.push_back(n);
//			break;
//		case ALLOW_SELECTED:
//			if (!n._arrowTrapOnWay) 
//				neighbours.push_back(n);
//			else
//			{
//				for (auto c : allowedArrowTraps)
//					if (n.GetArrowTrapCoords() == c)
//					{
//						neighbours.push_back(n);
//						break;
//					}
//			}
//			break;
//		default:
//			break;
//		}
//
//	}
//
//	
//	return neighbours;
//}

std::vector<MapNode*> Pathfinder::GetNodesFromGrid(std::vector<MapNode> &n)
{
	std::vector<MapNode*> neighboursGrid;

	for (int i = 0; i < n.size(); i++)
		neighboursGrid.push_back(_grid[n[i].GetX()][n[i].GetY()]);

	return neighboursGrid;
}

void Pathfinder::UpdateMovementInfoInGrid(std::vector<MapNode>& neighbours)
{
	for (int i = 0; i < neighbours.size(); i++)
	{
		_grid[neighbours[i].GetX()][neighbours[i].GetY()]->_actionToReachCandidate = neighbours[i].GetActionToReach();
		_grid[neighbours[i].GetX()][neighbours[i].GetY()]->_actionToReachTargetCandidate = neighbours[i].GetPrevActionTarget();
		_grid[neighbours[i].GetX()][neighbours[i].GetY()]->_mvStateCandidate = neighbours[i].GetMvState();
		_grid[neighbours[i].GetX()][neighbours[i].GetY()]->_arrowTrapOnWayCandidate = neighbours[i]._arrowTrapOnWay;
		_grid[neighbours[i].GetX()][neighbours[i].GetY()]->_arrowTrapCoordsCandidate = neighbours[i]._arrowTrapCoords;
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
		if (i > 1 && Ladder(x, y + i - 1) && !Impassable(x, y + i))
		{
			neighbours.push_back(MapNode{ x, y + i - 1, DROP, LADDER, mvCLIMBING });
		}

		if (LadderTop(x, y + i) && y + i < Y_NODES)
		{
			neighbours.push_back(MapNode{ x, y + i - 1, DROP, LADDERTOP });
			break;
		}

		if (Impassable(x, y + i) && y + i < Y_NODES)
		{
			neighbours.push_back(MapNode{ x, y + i - 1, DROP });
			break;
		}
	}

	//climb
	if (Impassable(x + 1, y) && Passable(x + 1, y - 1)) neighbours.push_back(MapNode{ x + 1, y - 1, CLIMBFROMHANG });
	if (Impassable(x - 1, y) && Passable(x - 1, y - 1)) neighbours.push_back(MapNode{ x - 1, y - 1, CLIMBFROMHANG });


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
		if (Impassable(x, y + 2))
			neighbours.push_back(MapNode{ x, y + 1, CLIMB, GROUND });
		else
			neighbours.push_back(MapNode{ x, y + 1, CLIMB, LADDER, mvCLIMBING });
	}

	//drop
	MAX = 8; //max distance to ground tile
	for (int i = 1; i <= MAX; i++)
	{
		if (i > 1 && Ladder(x, y + i - 1) && !Impassable(x, y + i))
		{
			neighbours.push_back(MapNode{ x, y + i - 1, DROP, LADDER, mvCLIMBING });
		}

		if (LadderTop(x, y + i) && y + i < Y_NODES)
		{
			neighbours.push_back(MapNode{ x, y + i - 1, DROP, LADDERTOP });
			break;
		}

		if (Impassable(x, y + i) && y + i < Y_NODES)
		{
			neighbours.push_back(MapNode{ x, y + i - 1, DROP });
			break;
		}
	}

	//jump y=-2
	//right
	if (Passable(x, y - 1) && Passable(x, y - 2) && Passable(x + 1, y - 2) && Impassable(x + 1, y - 1))
		neighbours.push_back(MapNode{ x, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });
	if (Passable(x, y - 1) && Passable(x + 1, y) && Passable(x + 1, y - 1) && Passable(x + 1, y - 2) &&
		Passable(x + 2, y - 2) && Impassable(x + 2, y - 1))
		neighbours.push_back(MapNode{ x + 1, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });

	//left
	if (Passable(x, y - 1) && Passable(x, y - 2) && Passable(x - 1, y - 2) && Impassable(x - 1, y - 1))
		neighbours.push_back(MapNode{ x, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });
	if (Passable(x, y - 1) && Passable(x - 1, y) && Passable(x - 1, y - 1) && Passable(x - 1, y - 2) &&
		Passable(x - 2, y - 2) && Impassable(x - 2, y - 1))
		neighbours.push_back(MapNode{ x - 1, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });

	//jump y=-1
	//right
	int safedist = 3;
	for (int i = 0; i <= 3; i++)
	{
		if (Impassable(x + i, y - 1))
		{
			safedist = i - 1;
			break;
		}
	}

	for (int i = 1; i <= safedist; i++)
	{
		if (Impassable(x + i, y) && Passable(x + i, y - 1))
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
		if (Impassable(x - i, y - 1))
		{
			safedist = i - 1;
			break;
		}
	}

	for (int i = 1; i <= safedist; i++)
	{
		if (Impassable(x - i, y) && Passable(x - i, y - 1))
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
				if (Impassable(x + i, y + j + 1) && Passable(x + i, y + j))
					neighbours.push_back(MapNode{ x + i, y + j, JUMPFROMLADDER, GROUND });
				else if (LadderTop(x + i, y + j + 1) && Passable(x + i, y + j))
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
				if (Impassable(x - i, y + j + 1) && Passable(x - i, y + j))
					neighbours.push_back(MapNode{ x - i, y + j, JUMPFROMLADDER, GROUND });
				else if (LadderTop(x - i, y + j + 1) && Passable(x - i, y + j))
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
		if (Impassable(x, y + 2))
			neighbours.push_back(MapNode{ x, y + 1, CLIMB, GROUND });
		else
			neighbours.push_back(MapNode{ x, y + 1, CLIMB, LADDER, mvCLIMBING_WITH_MOMENTUM });
	}
	

	//drop
	MAX = 8; //max distance to ground tile
	for (int i = 1; i <= MAX; i++)
	{
		if (i > 1 && Ladder(x, y + i - 1) && !Impassable(x, y + i))
		{
			neighbours.push_back(MapNode{ x, y + i - 1, DROP, LADDER, mvCLIMBING_WITH_MOMENTUM });
		}

		if (LadderTop(x, y + i) && y + i < Y_NODES)
		{
			neighbours.push_back(MapNode{ x, y + i - 1, DROP, LADDERTOP });
			break;
		}

		if (Impassable(x, y + i) && y + i < Y_NODES)
		{
			neighbours.push_back(MapNode{ x, y + i - 1, DROP });
			break;
		}
	}


	//jump y=-2
	//right
	if (Passable(x, y - 1) && Passable(x, y - 2) && Passable(x + 1, y - 2) && Impassable(x + 1, y - 1))
		neighbours.push_back(MapNode{ x, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });
	if (Passable(x, y - 1) && Passable(x + 1, y) && Passable(x + 1, y - 1) && Passable(x + 1, y - 2) &&
		Passable(x + 2, y - 2) && Impassable(x + 2, y - 1))
		neighbours.push_back(MapNode{ x + 1, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });
	if (Passable(x, y - 1) && Passable(x + 1, y) && Passable(x + 1, y - 1) && Passable(x + 2, y - 1) &&
		Passable(x + 2, y - 2) && Passable(x + 3, y - 2) && Impassable(x + 3, y - 1))
		neighbours.push_back(MapNode{ x + 2, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });

	//left
	if (Passable(x, y - 1) && Passable(x, y - 2) && Passable(x - 1, y - 2) && Impassable(x - 1, y - 1))
		neighbours.push_back(MapNode{ x, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });
	if (Passable(x, y - 1) && Passable(x - 1, y) && Passable(x - 1, y - 1) && Passable(x - 1, y - 2) &&
		Passable(x - 2, y - 2) && Impassable(x - 2, y - 1))
		neighbours.push_back(MapNode{ x - 1, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });
	if (Passable(x, y - 1) && Passable(x - 1, y) && Passable(x - 1, y - 1) && Passable(x - 2, y - 1) &&
		Passable(x - 2, y - 2) && Passable(x - 3, y - 2) && Impassable(x - 3, y - 1))
		neighbours.push_back(MapNode{ x - 2, y - 1, JUMPFROMLADDER, LEDGE, mvHANGING });


	//jump y=-1
	MAX = 5;

	//right
	int safedist = MAX;
	for (int i = 0; i <= MAX; i++)
	{
		if (Impassable(x + i, y - 1))
		{
			safedist = i - 1;
			break;
		}
	}

	bool roof = false;
	for (int i = 1; i < MAX; i++)
		if (Impassable(x + i, y - 2))
		{
			roof = true;
			break;
		}

	for (int i = 1; i <= safedist; i++)
	{
		if (Impassable(x + i, y))
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
		if (Impassable(x - i, y - 1))
		{
			safedist = i - 1;
			break;
		}
	}

	roof = false;
	for (int i = 1; i < MAX; i++)
		if (Impassable(x - i, y - 2))
		{
			roof = true;
			break;
		}

	for (int i = 1; i <= safedist; i++)
	{
		if (Impassable(x - i, y))
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
				if (Impassable(x + i, y + j + 1) && Passable(x + i, y + j))
					neighbours.push_back(MapNode{ x + i, y + j, JUMPFROMLADDER, GROUND });
				else if (LadderTop(x + i, y + j + 1) && Passable(x + i, y + j))
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
				if (Impassable(x - i, y + j + 1) && Passable(x - i, y + j))
					neighbours.push_back(MapNode{ x - i, y + j, JUMPFROMLADDER, GROUND });
				else if (LadderTop(x - i, y + j + 1) && Passable(x - i, y + j))
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

	if (Impassable(x, y - 1) && Passable(x, y - 2))
	{
		//right
		if (Passable(x + 1, y) && Passable(x + 1, y - 1) && Passable(x + 1, y - 2))
			neighbours.push_back(MapNode{ x, y - 2, JUMPABOVERIGHT });
		//left
		if (Passable(x - 1, y) && Passable(x - 1, y - 1) && Passable(x - 1, y - 2))
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
	else if (Ladder(x, y - 2) && Passable(x, y - 1))
		neighbours.push_back(MapNode{ x, y - 2, JUMP, LADDER, mvCLIMBING });


	/*
	* UP III (max=5 - possible, max=4 - safe)
	*/
	MAX = 6; //max travel distance (counting to ground not to ledge)

	//right
	int maxUpIII = MAX - 1;
	for (int i = 0; i <= maxUpIII; i++)
	{
		if (Passable(x + i, y) && Passable(x + i, y - 1) && Passable(x + i + 1, y - 2))
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

			if (Impassable(x + i + 1, y - 1))
			{
				if (i == 0)
				{
					if (Passable(x, y - 2))
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
		if (Passable(x + i, y) && Passable(x + i, y - 1) && Passable(x + i - 1, y - 2))
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

			if (Impassable(x + i - 1, y - 1))
			{
				if (i == 0)
				{
					if (Passable(x, y - 2))
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
		if (Passable(x + i, y) && Passable(x + i, y - 1) && Passable(x + i + 1, y - 1) && (abs(i) <= 1 || Passable(x + i, y - 2)))
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

			if (Impassable(x + i + 1, y))
			{
				if (i == 0)
					neighbours.push_back(MapNode{ x + i + 1, y - 1, JUMP});
				else if (i == MAX && Passable(x + i, y + 1))
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
		if (Passable(x + i, y) && Passable(x + i, y - 1) && Passable(x + i - 1, y - 1) && (abs(i) <= 1 || Passable(x + i, y - 2)))
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

			if (Impassable(x + i - 1, y))
			{
				if (i == 0)
					neighbours.push_back(MapNode{ x + i - 1, y - 1, JUMP });
				else if (i == -MAX && Passable(x + i, y + 1))
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
	if (Passable(x + 1, y) && Passable(x + 1, y + 1))
	{
		//if there are problems with hanging on a ladder with ground just below, restore hangdrop action
		if (Impassable(x + 1, y + 2)) neighbours.push_back(MapNode{ x + 1, y + 1, HANGDROP });
		else
		{
			if (Ladder(x + 1, y + 1) || LadderTop(x + 1, y + 1))
				neighbours.push_back(MapNode{ x + 1, y + 1, HANG, LADDER, mvCLIMBING });
			else
				neighbours.push_back(MapNode{ x + 1, y + 1, HANG, LEDGE, mvHANGING });
		}
	}

	//left
	if (Passable(x - 1, y) && Passable(x - 1, y + 1))
	{
		if (Impassable(x - 1, y + 2)) neighbours.push_back(MapNode{ x - 1, y + 1, HANGDROP });
		else
		{
			if (Ladder(x - 1, y + 1) || LadderTop(x - 1, y + 1))
				neighbours.push_back(MapNode{ x - 1, y + 1, HANG, LADDER, mvCLIMBING });
			else
				neighbours.push_back(MapNode{ x - 1, y + 1, HANG, LEDGE, mvHANGING });
		}
	}


	/*
	* DOWN jump
	*/
	int MAXx = 8; //max travel distance
	int MAXy = 5; //only 5 because we would take damage if we jumped further

	//right
	bool canHang = false;
	if (Passable(x + 1, y + 1))
	{
		for (int i = 1; i <= MAXx; i++)
			for (int j = 1; j <= MAXy; j++)
			{
				//awkward blocked jump; bot will HangDrop instead
				if (i == 1 && (Impassable(x, y - 1) || Impassable(x + 1, y - 1)))
					continue;

				if (DownJumpPathClear(x, y, x + i, y + j, xRIGHT))
				{
					if (Impassable(x + i, y + j + 1) && Passable(x + i, y + j))
						neighbours.push_back(MapNode{ x + i, y + j, JUMP });

					if (LadderTop(x + i, y + j + 1) && Passable(x + i, y + j))
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
	if (Passable(x - 1, y + 1))
	{
		for (int i = 1; i <= MAXx; i++)
			for (int j = 1; j <= MAXy; j++)
			{
				//awkward blocked jump; bot will HangDrop instead
				if (i == 1 && (Impassable(x, y - 1) || Impassable(x - 1, y - 1)))
					continue;

				if (DownJumpPathClear(x, y, x - i, y + j, xLEFT))
				{
					if (Impassable(x - i, y + j + 1) && Passable(x - i, y + j))
						neighbours.push_back(MapNode{ x - i, y + j, JUMP });

					if (LadderTop(x - i, y + j + 1) && Passable(x + i, y + j))
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
	if (Passable(x + 1, y + 1))
	{
		for (int i = 2; i <= MAXx; i++)
			for (int j = 1; j <= MAXy; j++)
			{
				if (i >= j + 3) continue; //fall is too steep
				else
				{
					if (WalkOffLedgePathClear(x, y, x + i, y + j, xRIGHT))
					{
						if (Impassable(x + i, y + j + 1) && Passable(x + i, y + j))
							neighbours.push_back(MapNode{ x + i, y + j, WALKOFFLEDGE });
						if (LadderTop(x + i, y + j + 1) && Passable(x + i, y + j))
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
	if (Passable(x - 1, y + 1))
	{
		for (int i = 2; i <= MAXx; i++)
			for (int j = 1; j <= MAXy; j++)
			{
				if (i >= j + 3) continue; //fall is too steep
				else
				{
					if (WalkOffLedgePathClear(x, y, x - i, y + j, xLEFT))
					{
						if (Impassable(x - i, y + j + 1) && Passable(x - i, y + j))
							neighbours.push_back(MapNode{ x - i, y + j, WALKOFFLEDGE });
						if (LadderTop(x - i, y + j + 1) && Passable(x - i, y + j))
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
		if (neighbours[i].GetPrevActionTarget() == HANG)
			neighbours.erase(neighbours.begin() + i);
		else
			i++;
	}

	int MAX = 8; //max distance to ground tile
	for (int i = 1; i <= MAX; i++)
	{
		if (Impassable(x, y + i + 1) && y + i + 1 < Y_NODES)
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

std::vector<MapNode> Pathfinder::CalculateNeighbours(MapNode node, bool allow_duplicates, AT_MODE atmode,
	std::vector<Coords> allowedArrowTraps)
{
	std::vector<MapNode> neighbours;
	
	switch (node.GetMvState())
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
		if (node.GetPrevActionTarget() == LADDERTOP)
			neighbours = CalculateNeighboursStandingLT(node);
		else
			neighbours = CalculateNeighboursStanding(node);
		break;
	default:
		neighbours = CalculateNeighboursStanding(node);
		break;
	}

	//HandleUnsafeNeighbours(node, neighbours);
	DeleteUnsafeNeighboursSpikes(node, neighbours);
	HandleUnsafeNeighboursSpearTrap(node, neighbours);
	HandleUnsafeNeighboursArrowTrap(node, neighbours, atmode, allowedArrowTraps);
	
	if (!allow_duplicates) RemoveDuplicateNeighbours(neighbours);
	//RemoveNeighboursWithArrowTraps(neighbours, atmode, allowedArrowTraps);

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

//std::vector<MapNode*> Pathfinder::CalculateNeighboursInGrid(MapNode * node, MVSTATE mvstate, ACTION_TARGET target)
//{
//	std::vector<MapNode*> neighbours;
//	
//	std::vector<MapNode> n = CalculateNeighbours(*node, mvstate, target);
//	for (int i = 0; i < n.size(); i++)
//	{
//		_grid[n[i].GetX()][n[i].GetY()]->_actionToReachCandidate = n[i].GetActionToReach();
//		_grid[n[i].GetX()][n[i].GetY()]->_actionToReachTargetCandidate = n[i].GetPrevActionTarget();
//		_grid[n[i].GetX()][n[i].GetY()]->_mvStateCandidate = n[i].GetMvState();
//		_grid[n[i].GetX()][n[i].GetY()]->_arrowTrapOnWayCandidate = n[i]._arrowTrapOnWay;
//		_grid[n[i].GetX()][n[i].GetY()]->_arrowTrapCoordsCandidate = n[i]._arrowTrapCoords;
//		neighbours.push_back(_grid[n[i].GetX()][n[i].GetY()]);
//	}
//	
//	return neighbours;
//}

#pragma endregion

#pragma region Calculating strong connected components

void Pathfinder::TarjanDFS(MapNode* n, AT_MODE atmode, std::vector<Coords> allowedArrowTraps)
{
	_tar_cvn++;
	_tar_VN[n->GetX()][n->GetY()] = _tar_cvn;
	_tar_VLow[n->GetX()][n->GetY()] = _tar_cvn;
	_tar_S.push(n);
	_tar_VS[n->GetX()][n->GetY()] = true;

	std::vector<MapNode> neighboursTemp = CalculateNeighbours(*n, false, atmode, allowedArrowTraps);
	UpdateMovementInfoInGrid(neighboursTemp);
	std::vector<MapNode*> neighbours = GetNodesFromGrid(neighboursTemp);

	for each (MapNode * m in neighbours)
	{
		//m not visited - visit it recursively
		if (_tar_VN[m->GetX()][m->GetY()] == 0)
		{
			m->SolidifyCandidates();

			TarjanDFS(m, atmode, allowedArrowTraps);
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

void Pathfinder::CalculateConnectedComponents(AT_MODE atmode, std::vector<Coords> allowedArrowTraps)
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
				TarjanDFS(_grid[i][j], atmode, allowedArrowTraps);
		}
}

#pragma endregion

#pragma region Getter methods

int Pathfinder::GetCCnr(Coords c)
{
	return _grid[c.GetX()][c.GetY()]->_CCnr;
}

std::vector<MapNode*> Pathfinder::GetAllNodesFromCC(int ccnr)
{
	return _tar_CCmap[ccnr];
}

MapNode* Pathfinder::GetNodeFromGrid(Coords c)
{
	return _grid[c.GetX()][c.GetY()];
}

std::vector<MapNode*> Pathfinder::GetPathFromGrid()
{
	return _pathList;
}

std::vector<MapNode> Pathfinder::GetPathCopy()
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

void Pathfinder::Dijkstra(Coords startc, AT_MODE atmode, std::vector<Coords> allowedArrowTraps)
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

	while (!_dij_pL.empty())
		_dij_pL.clear();
	

	_grid[x][y]->_dij_dist = 0;
	_dij_visited[x][y] = true;

	_dij_pL.push_front(_grid[x][y]);

	//instead of a priority Queue we have a list and we keep it ordered
	//we can't have a priority queue because we are using nodes from grid, and
	//when you update a node that is already in a queue the queue is ruined
	while (!_dij_pL.empty())
	{
		MapNode *n = _dij_pL.front();
		_dij_pL.pop_front();

		_dij_visited[n->GetX()][n->GetY()] = true;

		std::vector<MapNode> neighboursTemp = CalculateNeighbours(*n, false, atmode, allowedArrowTraps);
		UpdateMovementInfoInGrid(neighboursTemp);
		std::vector<MapNode*> neighbours = GetNodesFromGrid(neighboursTemp);

		for (MapNode* m : neighbours)
		{
			if (!_dij_visited[m->GetX()][m->GetY()])
				if (n->_dij_dist + CalculateDistance(n->GetCoords(), m->GetCoords()) + m->GetPenalty() < m->_dij_dist)
				{
					m->_dij_dist = n->_dij_dist + CalculateDistance(n->GetCoords(), m->GetCoords()) + m->GetPenalty();
					m->_dij_prev = n;
					m->SolidifyCandidates();


					auto i = _dij_pL.begin();
					while (i != _dij_pL.end())
					{
						if (*i == m)
							i = _dij_pL.erase(i);
						else
							i++;
					}

					//update priority list
					if (_dij_pL.empty())
					{
						_dij_pL.push_front(m);
					}
					else
					{
						//podzielmy to gówno na 2 pêtle bo rzal
						for (auto i = _dij_pL.begin(); i != _dij_pL.end(); i++)
						{

							if ((*i)->_dij_dist > m->_dij_dist)
							{
								//wstawiamy przed aktualn¹ pozycjê "i"...
								_dij_pL.insert(i, m);
								break;
							}
							if (*i == _dij_pL.back())
							{
								//...lub na koniec
								_dij_pL.push_back(m);
								break;
							}
						
						}
					}
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

int Pathfinder::GetDijDist(Coords c)
{
	return _grid[c.GetX()][c.GetY()]->_dij_dist;
}

#pragma endregion

#pragma region Other

MapNode Pathfinder::GetSafeNodeToDisarmAT(MapNode candidate, Coords arrowTrap)
{
	int Xo, Yo;

	Xo = arrowTrap.GetXpixel() + (ArrowTrapL(arrowTrap) ? -1 : 16);
	Yo = arrowTrap.GetYpixel();
	directions dirs = directions(ArrowTrapL(arrowTrap) ? xLEFT : xRIGHT, yDOWN);

	vector<Coords> path = _moveSim->SimulatePath(Xo, Yo, 8, 0, dirs);

	bool safe = false;

	//zrób jak¹œ kolejkê do której wrzuæ candidate
	//potem w pêtli bierz pierwszego z kolejki i sprawdŸ czy safe, jak tak to gut
	//jak nie to wyszukaj jego neighboursów i wsadŸ do kolejki na koniec i continue

	std::queue<MapNode> candidates;
	MapNode curr;
	candidates.push(candidate);
	
	while (!safe)
	{
		curr = candidates.front();
		candidates.pop();

		safe = true;
		for (Coords c : path)
		{
			if (c.SameNode(curr.GetCoords()))
			{
				safe = false;
				break;
			}
		}

		if (!safe)
		{	
			std::vector<MapNode> neighbours = CalculateNeighbours(curr, false, AT_MODE::STOP_ON_ARROWTRAPS);

			for (int i = 0; i < neighbours.size(); i++)
				candidates.push(neighbours[i]);
		}

	}

	return curr;
}
directions Pathfinder::SetThrowDirections(Coords beforeAT, Coords afterAT, Coords arrowTrapToDisarm)
{
	directions dirToAT, dirToAfterAT, throwDir;
	dirToAT = CalculateDirections(beforeAT, arrowTrapToDisarm);
	dirToAfterAT = CalculateDirections(beforeAT, afterAT);
	throwDir = dirToAT.x == dirToAfterAT.x ? dirToAT : dirToAfterAT;

	return throwDir;
}

void Pathfinder::ModifyThrowDownLaddertop(MapNode &beforeAT, directions & throwDir)
{
	if (beforeAT.GetPrevActionTarget() == LADDERTOP)
	{
		MapNode beforeATold = beforeAT;

		if (beforeAT.GetActionToReach() == WALK)
			beforeAT = *beforeAT.GetDijParent();
		else
		{
			if (CanStandInNode(Coords(beforeAT.GetX() - 1, beforeAT.GetY())))
				beforeAT = *GetNodeFromGrid(Coords(beforeAT.GetX() - 1, beforeAT.GetY()));
			else if (CanStandInNode(Coords(beforeAT.GetX() + 1, beforeAT.GetY())))
				beforeAT = *GetNodeFromGrid(Coords(beforeAT.GetX() + 1, beforeAT.GetY()));
			else
			{
				//ultra rare case - can stay as-is for now
			}
		}

		if (beforeATold.GetX() > beforeAT.GetX()) throwDir.x = xRIGHT;
		if (beforeATold.GetX() < beforeAT.GetX()) throwDir.x = xLEFT;
	}

	if (beforeAT.GetPrevActionTarget() == LADDER && beforeAT.GetActionToReach() == HANG)
	{
		MapNode beforeATold = beforeAT;
		beforeAT = *beforeAT.GetDijParent();

		if (beforeATold.GetX() > beforeAT.GetX()) throwDir.x = xRIGHT;
		if (beforeATold.GetX() < beforeAT.GetX()) throwDir.x = xLEFT;
	}
}

#pragma endregion

#pragma region Pathfinding methods

bool Pathfinder::TryToCalculatePath(Coords c1, Coords c2, AT_MODE atmode, std::vector<Coords> allowedArrowTraps)
{
	bool DEBUG = true;

	bool path_found = false;

	_pathList.clear();
	for (int i = 0; i < X_NODES; i++)
		for (int j = 0; j < Y_NODES; j++)
		{
			_grid[i][j]->_arrowTrapOnWay = false;
			_grid[i][j]->_arrowTrapOnWayCandidate = false;
			_grid[i][j]->_arrowTrapCoords = Coords();
			_grid[i][j]->_arrowTrapCoordsCandidate = Coords();
		}

	if (c1.GetX() != c2.GetX() || c1.GetY() != c2.GetY())
	{

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

			std::vector<MapNode> neighboursTemp = CalculateNeighbours(*current, false, atmode, allowedArrowTraps);
			UpdateMovementInfoInGrid(neighboursTemp);
			std::vector<MapNode*> neighbours = GetNodesFromGrid(neighboursTemp);

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
					//we can compare the G score, because F = G+H, and H is just our distance to target prediction, 
					//which is going to be the same because its the same node reached in 2 different ways
					if (child->_gScore > child->CalculateGScore(current))
					{
						child->_parent = current;
						child->ComputeScores(end);
						child->SolidifyCandidates();
					}
				}
				else
				{
					openList.push_back(child);
					child->_opened = true;

					// COMPUTE THE G
					child->_parent = current;
					child->ComputeScores(end);
					child->SolidifyCandidates();
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
				fileStream << " G: ";
				fileStream << _pathList[i]->_gScore;
				fileStream << " H: ";
				fileStream << _pathList[i]->_hScore;
				fileStream << "\n";
			}
			fileStream.close();
		}

		// Reset
		for (i = openList.begin(); i != openList.end(); i++)
		{
			(*i)->_opened = false;
			(*i)->_hScore = 0;
			(*i)->_gScore = 0;

			(*i)->_parent = NULL;
			//(*i)->_arrowTrapOnWay = false;
		}
		for (i = closedList.begin(); i != closedList.end(); i++)
		{
			(*i)->_closed = false;
			(*i)->_hScore = 0;
			(*i)->_gScore = 0;

			(*i)->_parent = NULL;
			//(*i)->_arrowTrapOnWay = false;
		}

		return path_found;


	}
	else //we are standing on the exit
		return true;
}



bool Pathfinder::TryToFindExplorationTarget(Coords startc, AT_MODE atmode, std::vector<Coords> allowedArrowTraps)
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

		std::vector<MapNode> neighboursTemp = CalculateNeighbours(*current, false, atmode, allowedArrowTraps);
		UpdateMovementInfoInGrid(neighboursTemp);
		std::vector<MapNode*> neighbours = GetNodesFromGrid(neighboursTemp);

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
					current->SolidifyCandidates();

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


bool Pathfinder::TryToFindTargetInNextCC(Coords startc, AT_MODE atmode, std::vector<Coords> allowedArrowTraps)
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
		if (current->_CCnr != start->_CCnr)
		{
			targetFound = true;
			_nextCCTarget = current->GetCoords();
			break;
		}

		std::vector<MapNode> neighboursTemp = CalculateNeighbours(*current, false, atmode, allowedArrowTraps);
		UpdateMovementInfoInGrid(neighboursTemp);
		std::vector<MapNode*> neighbours = GetNodesFromGrid(neighboursTemp);

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
					current->SolidifyCandidates();

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

Coords Pathfinder::GetTargetInNextCC()
{
	return _nextCCTarget;
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
//		vector<MapSearchNode*> neighbours = CalculateNeighboursInGrid(current, current->GetMvState(), current->GetPrevActionTarget());
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
	if (Passable(x, y) && Impassable(x, y + 1)) target = GROUND;
	else if (Ladder(x, y) && !LadderTop(x, y)) target = LADDER;
	else if (Passable(x, y) && LadderTop(x, y + 1)) target = LADDERTOP;
	else target = LEDGE;

	std::vector<MapNode> neighbours = CalculateNeighbours(MapNode(x, y, IDLE, target, mvState), true, AT_MODE::ALLOW_ARROWTRAPS);

	for (int i = 0; i < neighbours.size(); i++)
	{
		fileStream << " X: " << neighbours[i].GetX();
		fileStream << " Y: " << neighbours[i].GetY();
		fileStream << " " << MVactionStrings[neighbours[i].GetActionToReach()];
		fileStream << " " << ActionTargetStrings[neighbours[i].GetPrevActionTarget()];
		fileStream << " arrowtr=" << neighbours[i].IsArrowTrapOnWay();
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