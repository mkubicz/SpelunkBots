#pragma once
#include "IMovementAction.h"

class WalkAction : public IMovementAction
{
private:
	int _distance;
	bool _usingPixelCoords;
	bool _running;

public:
	WalkAction(std::shared_ptr<IBot> const& bot, int dist);
	WalkAction(std::shared_ptr<IBot> const& bot, int dist, bool run);
	void AddDistance(int distance);
	~WalkAction() {};

	ordersStruct GetOrders() override;
};