#pragma once
#include "IMovementAction.h"

class WalkAction : public IMovementAction
{
private:
	double _distance;
	bool _usingPixelCoords;

public:
	WalkAction(IBot *bot, bool goingRight, double distance, bool usingPixelCoords);
	~WalkAction() {};

	ordersStruct GetOrders() override;
};