#pragma once
#include "stdafx.h"
#include "MovementSimulator.h"

MovementSimulator::MovementSimulator(std::shared_ptr<IBot> bot/*, std::shared_ptr<Pathfinder> pathfinder*/)
	: _bot(bot)//, _pathfinder(pathfinder)
{
	_xAcc = 0;
	_yAcc = 1;
	_VxMax = 16;
	_VyMax = 16;
	_VxMin = 0;
	_VyMin = 0;
}

bool MovementSimulator::IsPointBelowLine(Coords point, std::pair<float, float> line)
{
	if (line.first * point.GetXpixel() + line.second > point.GetYpixel())
		return true;
	else
		return false;
}

std::pair<float, float> MovementSimulator::CalculateLineFactors(int X, int Y, int Xold, int Yold)
{
	std::pair<float, float> line;

	line.first = (Y - Yold) / (X - Xold);
	line.second = Yold - (((Y - Yold) / (X - Xold)) * Xold);

	return line;
}

directions MovementSimulator::CalculateCollisionDirection(int X, int Y, int Xold, int Yold, directions dir)
{
	directions colDir;

	//if (dir.x == xRIGHT && dir.y == yDOWN)
	//{
	//	Coords c = Coords(X, Y, PIXEL);
	//	if (_pathfinder->Impassable(c.OffsetX(-1)) && _pathfinder->Impassable(c.OffsetY(-1)))
	//		colDir.y = yDOWN;
	//	else if (_pathfinder->Passable(c.OffsetX(-1)) && _pathfinder->Impassable(c.OffsetY(-1)))
	//		colDir.x = xRIGHT;
	//	else if (_pathfinder->Impassable(c.OffsetX(-1)) && _pathfinder->Passable(c.OffsetY(-1)))
	//		colDir.y = yDOWN;
	//	else if (_pathfinder->Passable(c.OffsetX(-1)) && _pathfinder->Impassable(c.OffsetY(-1)))
	//	{
	//		//jak róg kratki do której wpad³o (X,Y) jest poni¿ej prostej wyznaczonej przez (X,Y) i (Xold,Yold)
	//		//to colDir.y = yDOWN; wpp colDir.x = xRIGHT;
	//		
	//		std::pair<float, float> line = CalculateLineFactors(X, Y, Xold, Yold);
	//		Coords corner = Coords(c.GetX(), c.GetY(), NODE_ROUNDDOWN);
	//		if (IsPointBelowLine(corner, line))
	//			colDir.y = yDOWN;
	//		else
	//			colDir.x = xRIGHT;
	//	}
	//}
	int ox, oy;
	if (dir.x == xRIGHT)
		ox = -1;
	else if (dir.x == xLEFT)
		ox = 1;

	if (dir.y == yDOWN)
		oy = -1;
	else if (dir.y == yUP)
		oy = 1;
	
	//if the moving object hit a wall on its right side, then its a RIGHT collision. If it hit a ceiling, its an UP collision.
	Coords c = Coords(X, Y, PIXEL);
	//if (_pathfinder->Impassable(c.OffsetX(ox)) && _pathfinder->Impassable(c.OffsetY(oy)))
	//{
	//	colDir.x = dir.x;
	//	colDir.y = dir.y;
	//}
	//else if (_pathfinder->Passable(c.OffsetX(ox)) && _pathfinder->Impassable(c.OffsetY(oy)))
	//	colDir.x = dir.x;
	//else if (_pathfinder->Impassable(c.OffsetX(ox)) && _pathfinder->Passable(c.OffsetY(oy)))
	//	colDir.y = dir.y;
	//else if (_pathfinder->Passable(c.OffsetX(ox)) && _pathfinder->Passable(c.OffsetY(oy)))
	if (!_bot->IsNodePassable(X+(ox*16), Y, PIXEL_COORDS) && !_bot->IsNodePassable(X, Y + (oy * 16), PIXEL_COORDS))
	{
		colDir.x = dir.x;
		colDir.y = dir.y;
	}
	else if (_bot->IsNodePassable(X + (ox * 16), Y, PIXEL_COORDS) && !_bot->IsNodePassable(X, Y + (oy * 16), PIXEL_COORDS))
		colDir.x = dir.x;
	else if (!_bot->IsNodePassable(X + (ox * 16), Y, PIXEL_COORDS) && _bot->IsNodePassable(X, Y + (oy * 16), PIXEL_COORDS))
		colDir.y = dir.y;
	else if (_bot->IsNodePassable(X + (ox * 16), Y, PIXEL_COORDS) && _bot->IsNodePassable(X, Y + (oy * 16), PIXEL_COORDS))
	{
		//jak ruch jest DOWN RIGHT i róg kratki do której wpad³o (X,Y) jest poni¿ej prostej wyznaczonej przez (X,Y) i (Xold,Yold)
		//to colDir.y = yDOWN; wpp colDir.x = xRIGHT;
			
		std::pair<float, float> line = CalculateLineFactors(X, Y, Xold, Yold);
		Coords corner = Coords(c.GetX(), c.GetY(), NODE_ROUNDDOWN);
		if (IsPointBelowLine(corner, line))
			colDir.y = dir.y;
		else
			colDir.x = dir.x;
	}
	

	return colDir;
}

std::vector<Coords> MovementSimulator::SimulatePath(int Xo, int Yo, int Vxo, int Vyo, directions dir)
{
	std::vector<Coords> path;

	int X, Y, Xold, Yold, Vx, Vy;
	X = Xo; Y = Yo; Vx = Vxo, Vy = Vyo;
	Xold = X; Yold = Y;

	std::ofstream fileStream1, fileStream2;
	fileStream1.open(".\\MovementSimulation.txt");
	fileStream2.open(".\\MovementSimulationPix.txt");

	bool gravTick = false;
	bool stop = false;

	while (!stop)
	{
		//wind resistance
		Vx -= _xAcc;
		if (Vx < _VxMin) Vx = _VxMin;

		//gravity
		if (gravTick)
		{
			if (dir.y == yDOWN) Vy += _yAcc;
			else Vy -= _yAcc;
			if (Vy < _VyMin) Vy = _VyMin;
			if (Vy > _VyMax) Vy = _VyMax;
			gravTick = false;
		}
		else
			gravTick = true;


		Xold = X; Yold = Y;
		//move X
		if (dir.x == xRIGHT) X += Vx;
		if (dir.x == xLEFT) X -= Vx;
		//move Y
		if (dir.y == yDOWN) Y += Vy;
		if (dir.y == yUP) Y -= Vy;

		//collisions
		if (!_bot->IsNodePassable(X, Y, PIXEL_COORDS))
		{
			directions collisionDir = CalculateCollisionDirection(X, Y, Xold, Yold, dir);

			if (collisionDir.x == xRIGHT || collisionDir.x == xLEFT)
			{
				//collision in X axis - flip X direction
				dir.FlipX();
				X = EdgeOfNodeX((int)X / 16, dir.x);
			}
			else if (collisionDir.y == yDOWN)
			{
				stop = true;
			}
			else if (collisionDir.y == yUP)
			{
				dir.FlipY();
				Y = EdgeOfNodeY((int)Y / 16, dir.y);
			}
		}


		////collisions
		//if (!_bot->IsNodePassable(X, Yold, PIXEL_COORDS))
		//{
		//	//collision in X axis - flip X direction
		//	dir.FlipX();
		//	X = EdgeOfNodeX((int)X / 16, dir.x);
		//}
		//else if (!_bot->IsNodePassable(Xold, Y, PIXEL_COORDS))
		//{
		//	//collision in Y axis - stop the simulation (for now)
		//	stop = true;
		//}

		fileStream1 << (float)X / 16.0 << ", " << (float)-Y / 16.0 << std::endl;
		fileStream2 << X << ", " << Y << std::endl;

		path.push_back(Coords(X, Y, PIXEL));
	}


	fileStream1.close();
	fileStream2.close();

	return path;
}
