#pragma once
#include "IMovementAction.h"

class ThrowRopeAction : public IMovementAction
{
private:

public:
	ThrowRopeAction(std::shared_ptr<IBot> const& bot);

	ordersStruct GetOrders() override;
};
