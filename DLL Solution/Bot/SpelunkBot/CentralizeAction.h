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
	CentralizeAction(IBot *bot, int centralizingPoint);
	CentralizeAction(IBot *bot);
	~CentralizeAction() {};

	ordersStruct GetOrders() override;
};