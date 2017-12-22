#include "stdafx.h"
#include "Utilities.h"

bool CloseToZero(double x)
{
	if (x < 0.1 && x > -0.1) return true;
	else return false;
}

//void ConvertToNodeCoordinates(double &x)
//{
//	x /= 16;
//}

void ConvertToNodeCoordinates(double &x, double &y)
{
	x /= 16;
	y /= 16;
}

void ConvertToNodeCoordinates(double &x1, double &y1, double &x2, double &y2)
{
	ConvertToNodeCoordinates(x1, y1);
	ConvertToNodeCoordinates(x2, y2);
}

double ConvertToNodeCoordinates(double x)
{
	return x / 16;
}

int ConvertToPixelCoordinates(double x)
{
	x *= 16;
	return x;
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


bool closeToTargetFall(int playerPositionX, int targetPositionX, bool running, int distY)
{
	if (distY <= 0) //jumpup or jumpflat
	{
		if (running) return abs(playerPositionX - targetPositionX) <= PIXELS_IN_NODE * 1.5;
		else return closeToTarget(playerPositionX, targetPositionX);
	}
	else
	{ //jumpdown
		if (running) return abs(playerPositionX - targetPositionX) <= PIXELS_IN_NODE * 2;
		else return abs(playerPositionX - targetPositionX) <= 12;
	}

}


bool WentThrough(bool goingRight, int x1, int x2)
{
	if (goingRight) return x1 >= x2;
	else return x2 >= x1;
}
