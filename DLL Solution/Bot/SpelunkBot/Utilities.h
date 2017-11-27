#pragma once

bool CloseToZero(double x);

//make them return values maybe
void ConvertToNodeCoordinates(double &x);
int ConvertToPixelCoordinates(double x);
void ConvertToNodeCoordinates(double &x, double &y);

void GetIntPartFractPart(double value, double &intpart, double &fractpart);

bool IsBotAgainstTheWall(int playerPositionX, bool goingRight);
bool closeToTarget(int playerPositionX, int targetPositionX);
bool closeToTargetFall(int playerPositionX, int targetPositionX, bool running);

bool WentThrough(bool goingRight, int x1, int x2);