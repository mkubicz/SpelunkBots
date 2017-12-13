#include "stdafx.h"
#include "Pathfinder.h"
#include <vector>
#include <list>
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


vector<MapSearchNode*> Pathfinder::GetNeighboursList(MapSearchNode* node, std::map<int, std::map<int, MapSearchNode*>> grid)
{
	int x = node->_x;
	int y = node->_y;
	vector<MapSearchNode*> neighbours;
	int MAX;

	/*
	 * UP I
	 */
	if (Pelna(x, y - 1) && Pusta(x, y - 2) && ((Pusta(x - 1, y) && Pusta(x - 1, y - 1) && Pusta(x - 1, y - 2)) || (Pusta(x + 1, y) && Pusta(x + 1, y - 1) && Pusta(x + 1, y - 2))))
		neighbours.push_back(grid[x][y - 2]);


	/*
	 * UP II
	 */
	if (Pusta(x, y - 1) && Pusta(x, y - 2))
	{
		//left
		if (Pusta(x - 1, y - 2) && Pelna(x - 1, y - 1))
			neighbours.push_back(grid[x - 1][y - 2]);
		//right
		if (Pusta(x + 1, y - 2) && Pelna(x + 1, y - 1))
			neighbours.push_back(grid[x + 1][y - 2]);
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
				neighbours.push_back(grid[x + i + 1][y - 2]);
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
				neighbours.push_back(grid[x + i - 1][y - 2]);
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
				neighbours.push_back(grid[x + i + 1][y-1]);
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
				neighbours.push_back(grid[x + i - 1][y]);
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
		if (ok) neighbours.push_back(grid[x + candidateXright][y]);
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
		if (ok) neighbours.push_back(grid[x - candidateXleft][y]);
	}


	/*
	 * DOWN hangdrop
	 */
	
	//right
	if (Pusta(x + 1, y) && Pusta(x + 1, y + 1))
	{
		for (int j = 2; j <= 8; j++)
		{
			if (Pelna(x + 1, y + j))
			{
				neighbours.push_back(grid[x + 1][y + j - 1]);
				break;
			}
		}
	}

	//left
	if (Pusta(x - 1, y) && Pusta(x - 1, y + 1))
	{
		for (int j = 2; j <= 8; j++)
		{
			if (Pelna(x - 1, y + j))
			{
				neighbours.push_back(grid[x - 1][y + j - 1]);
				break;
			}
		}
	}


	/*
	* DOWN jump
	*/
	int MAXx = 7;
	int MAXy = 7;

	//right
	if (Pusta(x + 1, y + 1))
	{
		for (int i = 2; i <= MAXx; i++)
			for (int j = 1; j <= MAXy; j++)
				if (Pelna(x + i, y + j + 1) && Pusta(x + i, y + j) && DownJumpPathClear(x, y, x + i, y + j, RIGHT))
					neighbours.push_back(grid[x + i][y + j]);
	}

	//left
	if (Pusta(x - 1, y + 1))
	{
		for (int i = 2; i <= MAXx; i++)
			for (int j = 1; j <= MAXy; j++)
				if (Pelna(x - i, y + j + 1) && Pusta(x - i, y + j) && DownJumpPathClear(x, y, x - i, y + j, LEFT))
					neighbours.push_back(grid[x - i][y + j]);
	}


	return neighbours;
}

//crude; to be used by GetNeighboursList
bool Pathfinder::DownJumpPathClear(int x1, int y1, int x2, int y2, bool right)
{
	
	int startX;
	
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
}


void Pathfinder::NeighboursDebug(int x, int y)
{

	ofstream fileStream;
	fileStream.open(".\\Pathfinder\\neighbours.txt");

	MapSearchNode* node = new MapSearchNode();
	node->_x = x;
	node->_y = y;

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

	std::vector<MapSearchNode*> neighbours = GetNeighboursList(node, grid);
	

	for (int i = 0; i < neighbours.size(); i++)
	{
		fileStream << " X: " << neighbours[i]->_x;
		fileStream << " Y: " << neighbours[i]->_y;
		fileStream << endl;
	}
	fileStream.close();

}

/*
double Pathfinder::CalculatePath(double x1, double y1, double x2, double y2, double usingPixelCoords)
{
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
		
		while (n == 0 || (current != end && n < 50))
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
				break;
			}

			// Remove the current point from the open list
			openList.remove(current);
			current->_opened = false;

			// Add the current point to the closed list
			closedList.push_back(current);
			current->_closed = true;


			//Get reachable neighbours
			//std::vector<MapSearchNode*> neighbours = GetNeighboursList(current, grid);



			// Get all the current adjacent walkable points
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					if (x == 0 && y == 0)
					{
						// ignore current node, pass
						continue;
					}

					if (x == 0 || y == 0)
					{

						child = grid[current->_x + x][current->_y + y];

						// if it's closed or not walkable then pass
						if (child->_closed || !_bot->IsNodePassable(child->_x, child->_y, NODE_COORDS))
							//(spmap[child->_x][child->_y] != 0 && spmap[child->_x][child->_y] != 3 && spmap[child->_x][child->_y] != 4 && spmap[child->_x][child->_y] != 2 && spmap[child->_x][child->_y] != 9))
						{
							fileStream << "\n";
							fileStream << "closed or not walkable";
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
			fileStream << "X ";
			fileStream << current->_x;
			fileStream << " Y ";
			fileStream << current->_y;
			fileStream << "\n";
			_pathList.push_back(current);
			current = current->_parent;
			n++;
		}
		fileStream.close();
		return 0;

		
	}
	return 0;
}
*/




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