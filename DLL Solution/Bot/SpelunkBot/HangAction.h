#pragma once
#include "IMovementAction.h"

class HangAction : public IMovementAction
{
public:
	HangAction(IBot* bot, DIRECTIONX directionX);
	~HangAction() {};
	ordersStruct GetOrders() override;

private:

};