#pragma once
#include "IMovementAction.h"

class WalkOffLedgeAction : public IMovementAction
{
private:
	int _distX;
	int _distY;
	bool _running;
	int _startPosXside;

	enum STATE { WALKING, FALLING, FREEFALLING, LANDED };
	STATE _state;

	bool closeToTargetFall(int playerPosX, int targetPosX, bool running, int distY);

public:
	ordersStruct GetOrders();
	WalkOffLedgeAction(IBot * bot, int distX, int distY);
};