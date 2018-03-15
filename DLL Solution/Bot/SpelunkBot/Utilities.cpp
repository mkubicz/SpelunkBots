#include "stdafx.h"
#include "Node.h"
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

bool closeToTargetFallToLadder(int playerPositionX, int targetPositionX, bool running, int distX, int distY)
{
	if (distY <= 0) //jumpup or jumpflat
	{
		if (running) return abs(playerPositionX - targetPositionX) <= PIXELS_IN_NODE * 1.5;
		else return closeToTarget(playerPositionX, targetPositionX);
	}
	else
	{ //jumpdown
		if (running)
		{
			if ((distX == 4) && (distY == 5))
				return abs(playerPositionX - targetPositionX) <= 28;

			if (distX == 5 && (distY == 2 || distY == 3))
				return abs(playerPositionX - targetPositionX) <= 24;

			if ((distX == 6) && (distY == 5))
				return abs(playerPositionX - targetPositionX) <= 26;

			if ((distX == 7 || distX == 6) && (distY == 2 || distY == 3))
				return abs(playerPositionX - targetPositionX) <= 20;
			else
				if (running) return abs(playerPositionX - targetPositionX) <= 28;
		}
		else
		{
			//return abs(playerPositionX - targetPositionX) <= 12;
			if (distY > distX)
				return abs(playerPositionX - targetPositionX) <= 14;
			else
				return abs(playerPositionX - targetPositionX) <= 12;
		}
	}

}



bool WentThrough(int x1, int x2, DIRECTIONX direction)
{
	//changed from >= to >
	if (direction == xRIGHT) return x1 > x2;
	else return x2 > x1;
}

Node CalculateTargetNode(int playerPosXNode, int playerPosYNode, int distX, int distY)
{
	Node targetNode(playerPosXNode + distX, playerPosYNode + distY);

	return targetNode;
}

int MiddleXPixel(Node node)
{
	return ConvertToPixelCoordinates(node.GetX()) + 8;
}

int MiddleYPixel(Node node)
{
	return ConvertToPixelCoordinates(node.GetY()) + 8;
}

int EdgeOfNodeX(int nrOfNode, DIRECTIONX direction)
{
	if (direction == xRIGHT)
		return (nrOfNode * PIXELS_IN_NODE) + PIXELS_IN_NODE;
	else
		return (nrOfNode * PIXELS_IN_NODE);
}

int CenterOfNode(int nrOfNode)
{
	return (nrOfNode + 0.5) * 16;
}

bool WithinRangeFromTarget(int posPixel, int targetPixel, int pixelRange)
{
	if (abs(targetPixel - posPixel) <= pixelRange) return true;
	else return false;
}