#include "stdafx.h"
#include "Node.h"
#include "Utilities.h"

void ConvertToNodeCoordinates(int &x, int &y)
{
	x /= PIXELS_IN_NODE;
	y /= PIXELS_IN_NODE;
}

void ConvertToNodeCoordinates(int &x1, int &y1, int &x2, int &y2)
{
	ConvertToNodeCoordinates(x1, y1);
	ConvertToNodeCoordinates(x2, y2);
}

double ConvertToNodeCoordinates(int x)
{
	return x / PIXELS_IN_NODE;
}

int ConvertToPixelCoordinates(double x)
{
	return x*= PIXELS_IN_NODE;
}

MVSTATE ToMvState(SpState spstate)
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

bool IsBotAgainstTheWall(int playerPositionX, bool goingRight)
{
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


bool WentThrough(int x1, int x2, DIRECTIONX direction)
{
	//changed from >= to >
	if (direction == xRIGHT) return x1 > x2;
	else return x2 > x1;
}


int MiddleXPixel(Node node)
{
	return ConvertToPixelCoordinates(node.GetX()) + 8;
}

int MiddleYPixel(Node node)
{
	return ConvertToPixelCoordinates(node.GetY()) + 8;
}

int MiddlePixelOfNode(int nodenr)
{
	return ConvertToPixelCoordinates(nodenr + 0.5);
}

int EdgeOfNodeX(int nodenr, DIRECTIONX direction)
{
	if (direction == xRIGHT)
		return (nodenr * PIXELS_IN_NODE) + PIXELS_IN_NODE;
	else
		return (nodenr * PIXELS_IN_NODE);
}


bool WithinRangeFromTarget(int posPixel, int targetPixel, int pixelRange)
{
	if (abs(targetPixel - posPixel) <= pixelRange) return true;
	else return false;
}

bool IsJumpWithRunning(int distX, int distY, ACTION_TARGET jumpTarget)
{
	bool withRunning = false;

	if (distY >= 0)
	{
		if (abs(distX) > 4)
			withRunning = true;
		else
			withRunning = false;
	}
	else
	{
		if (abs(distX) > 3)
			withRunning = true;
		else
			withRunning = false;
	}

	if (abs(distX) == 3 && distY == -1 && jumpTarget == LEDGE)
		withRunning = true;

	if (abs(distX) == 4 && distY == 1)
		withRunning = true;

	return withRunning;
}