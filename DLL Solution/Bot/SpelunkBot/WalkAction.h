#pragma once
#include "IMovementAction.h"

class WalkAction : public IMovementAction
{
private:
	int _distance;
	bool _usingPixelCoords;
	bool _running;

public:
	WalkAction(IBot *bot, int dist);
	WalkAction(IBot *bot, int dist, bool run);
	void AddDistance(int distance);
	~WalkAction() {};

	ordersStruct GetOrders() override;
};