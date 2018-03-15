#pragma once

//forward declaration
class Node;
enum DIRECTIONX;


bool CloseToZero(double x);

//make them return values maybe
//void ConvertToNodeCoordinates(double &x);
void ConvertToNodeCoordinates(double &x, double &y);
void ConvertToNodeCoordinates(double &x1, double &y1, double &x2, double &y2);

double ConvertToNodeCoordinates(double x);
int ConvertToPixelCoordinates(double x);

void GetIntPartFractPart(double value, double &intpart, double &fractpart);

bool IsBotAgainstTheWall(int playerPositionX, bool goingRight);
bool closeToTarget(int playerPositionX, int targetPositionX);
bool closeToTargetFall(int playerPosX, int targetPosX, bool running, int distY);

bool closeToTargetFallToLadder(int playerPositionX, int targetPositionX, bool running, int distX, int distY);



bool WentThrough(int x1, int x2, DIRECTIONX direction);



//struct Node
//{
//	int x;
//	int y;
//
//	int actionToReach;
//
//};

Node CalculateTargetNode(int playerPosXNode, int playerPosYNode, int distX, int distY);

int MiddleXPixel(Node node);
int MiddleYPixel(Node node);
int EdgeOfNodeX(int nrOfNode, DIRECTIONX direction);
int CenterOfNode(int nrOfNode);

bool WithinRangeFromTarget(int posPixel, int targetPixel, int pixelRange);
