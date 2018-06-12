#pragma once
#include "IMovementAction.h"

class DropAction : public IMovementAction
{
public:
	DropAction(std::shared_ptr<IBot> const& bot, ACTION_TARGET actionTarget, int distY);
	DropAction(std::shared_ptr<IBot> const& bot, int distY);
	~DropAction() {};

	ordersStruct GetOrders() override;

private:
	ACTION_TARGET _actionTarget;
	enum STATE { DROPPING, FALLING, CLIMBING };
	STATE _state;
	int _distY;
};