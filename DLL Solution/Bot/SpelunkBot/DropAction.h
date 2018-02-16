#pragma once
#include "IMovementAction.h"

class DropAction : public IMovementAction
{
public:
	DropAction(IBot* bot);
	~DropAction() {};

	ordersStruct GetOrders() override;
};