#pragma once
#include "stdafx.h"
#include "IMovementAction.h"

class CentralizeAction : public IMovementAction
{
private:
	int _centralizingPoint;
	int _centralizeMoveTimer = 0;
	int _centralizeBreakTimer = 0;
	DIRECTIONX _centralizeDir;

public:
	CentralizeAction(std::shared_ptr<IBot> const& bot, int centralizingPoint);
	CentralizeAction(std::shared_ptr<IBot> const& bot);
	~CentralizeAction() {};

	ordersStruct GetOrders() override;
};