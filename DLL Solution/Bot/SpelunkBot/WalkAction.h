#pragma once
#include "IMovementAction.h"

class WalkAction : public IMovementAction
{
private:
	double _distance;
	bool _usingPixelCoords;
	bool _running;

public:
	//WalkAction(IBot *bot, bool goingRight, double distance, bool usingPixelCoords);
	//WalkAction(IBot *bot, bool goingRight, bool pressRun, double distance, bool usingPixelCoords);
	WalkAction(IBot *bot, int dist);
	WalkAction(IBot *bot, int dist, bool run);
	void AddDistance(int distance);
	~WalkAction() {};

	ordersStruct GetOrders() override;
};