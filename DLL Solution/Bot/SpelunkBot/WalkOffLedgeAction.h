#pragma once
#include "IMovementAction.h"

class WalkOffLedgeAction : public IMovementAction
{
private:
	int _distX;
	int _distY;

	int _startPosXside;
	int _startPosXmid;
	int _targetXSide;
	//int _previousPosX;
	//int _previousPosY;

	bool _running;
	enum STATE { WALKING, FALLING, FREEFALLING, LANDED };
	STATE _state;

public:
	ordersStruct GetOrders();
	WalkOffLedgeAction(IBot * bot, int distX, int distY);
};