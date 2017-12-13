#pragma once

bool CloseToZero(double x);

//make them return values maybe
void ConvertToNodeCoordinates(double &x);
void ConvertToNodeCoordinates(double &x, double &y);
void ConvertToNodeCoordinates(double &x1, double &y1, double &x2, double &y2);

int ConvertToPixelCoordinates(double x);

void GetIntPartFractPart(double value, double &intpart, double &fractpart);

bool IsBotAgainstTheWall(int playerPositionX, bool goingRight);
bool closeToTarget(int playerPositionX, int targetPositionX);
bool closeToTargetFall(int playerPositionX, int targetPositionX, bool running);

bool WentThrough(bool goingRight, int x1, int x2);