#pragma once

//forward declarations
//class Node;
enum DIRECTIONX;
enum ACTION_TARGET;
enum MVSTATE;
enum SpState;

void ConvertToNodeCoordinates(int &x, int &y);
void ConvertToNodeCoordinates(int &x1, int &y1, int &x2, int &y2);
double ConvertToNodeCoordinates(int x);
int ConvertToPixelCoordinates(double x);

MVSTATE ToMvState(SpState spstate);

bool closeToTarget(int playerPositionX, int targetPositionX);
bool WentThrough(int x1, int x2, DIRECTIONX direction);
bool WithinRangeFromTarget(int posPixel, int targetPixel, int pixelRange);

//int MiddleXPixel(Node node);
//int MiddleYPixel(Node node);
int EdgeOfNodeX(int nodenr, DIRECTIONX direction);
//int MiddlePixelOfNode(int nodenr);

bool IsBotAgainstTheWall(int playerPositionX, bool goingRight);
bool IsJumpWithRunning(int distX, int distY, ACTION_TARGET jumpTarget);