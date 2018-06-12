#pragma once
#include "IMovementAction.h"

class HangAction : public IMovementAction
{
public:
	HangAction(std::shared_ptr<IBot> const& bot, DIRECTIONX directionX);

	~HangAction() {};
	ordersStruct GetOrders() override;

private:
	ACTION_TARGET _actionTarget;

};