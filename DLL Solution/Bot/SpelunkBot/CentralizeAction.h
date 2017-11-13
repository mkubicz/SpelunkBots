#pragma once
#include "stdafx.h"
#include "IMovementAction.h"

class CentralizeAction : public IMovementAction
{
public:
	CentralizeAction(IBot *bot);
	~CentralizeAction() {};

	ordersStruct GetOrders() override;
};