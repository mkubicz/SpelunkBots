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

int ConvertToPixelCoordinates(double x)
{
	x *= 16;
	return x;
}

void ConvertToNodeCoordinates(double &x, double &y)
{
	x /= 16;
	y /= 16;
}

void GetIntPartFractPart(double value, double &intpart, double &fractpart)
{
	fractpart = modf(value, &intpart);
}

bool IsBotAgainstTheWall(int playerPositionX, bool goingRight)
{//TODO: allow use of node coords
	if (goingRight)
	{
		if ((int)playerPositionX % 16 == 11) return true;
		else return false;
	}
	else
	{
		if ((int)playerPositionX % 16 == 5) return true;
		else return false;
	}

}

bool closeToTarget(int playerPositionX, int targetPositionX)
{
	return abs(playerPositionX - targetPositionX) <= 6;  //6 - magic number which minimises walking error
}