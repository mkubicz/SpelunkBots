#pragma once
#include "IMovementAction.h"

class WalkAction : public IMovementAction
{
private:
	double _distance;
	bool _usingPixelCoords;
	bool _pressRun;

public:
	WalkAction(IBot *bot, bool goingRight, double distance, bool usingPixelCoords);
	WalkAction(IBot *bot, bool goingRight, bool pressRun, double distance, bool usingPixelCoords);
	~WalkAction() {};

	ordersStruct GetOrders() override;
};