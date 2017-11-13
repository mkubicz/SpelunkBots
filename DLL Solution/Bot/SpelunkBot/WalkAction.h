#pragma once
#include "IMovementAction.h"

class WalkAction : public IMovementAction
{
private:
	double _targetPositionXNode;
	double _distance;
	bool _actionInProgress;
	bool _usingPixelCoords;
	bool _goingRight;

public:
	WalkAction(IBot *bot, bool goingRight, double distance, bool usingPixelCoords);
	~WalkAction() {};

	ordersStruct GetOrders() override;
};