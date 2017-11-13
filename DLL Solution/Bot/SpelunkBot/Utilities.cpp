#include "stdafx.h"
#include "Utilities.h"

bool CloseToZero(double x)
{
	if (x < 0.1 && x > -0.1) return true;
	else return false;
}

void ConvertToNodeCoordinates(double &x)
{
	x /= 16;
}

void ConvertToNodeCoordinates(double &x, double &y)
{
	x /= 16;
	y /= 16;
}