#pragma once
#include "IBot.h"
#include "Coords.h"
#include <fstream>
#include <ostream>

class MovementSimulator
{
//Velocity always in pixels/frame

public:
	MovementSimulator(std::shared_ptr<IBot> bot/*, std::shared_ptr<Pathfinder> pathfinder*/);


	
	std::vector<Coords> SimulatePath(int Xo, int Yo, int Vox, int Voy, directions dir);

private:
	std::shared_ptr<IBot> _bot;
	//std::shared_ptr<Pathfinder> _pathfinder;

	bool IsPointBelowLine(Coords point, std::pair<float, float> line);
	std::pair<float, float> CalculateLineFactors(int X, int Y, int Xold, int Yold);
	directions CalculateCollisionDirection(int X, int Y, int Xold, int Yold, directions dir);

	int _xAcc;
	int _yAcc;
	int _VxMax, _VyMax, _VxMin, _VyMin;


};