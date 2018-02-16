#pragma once
#include "IMovementAction.h"

class HangAction : public IMovementAction
{
public:
	HangAction(IBot* bot, bool goingRight);
	~HangAction() {};
	ordersStruct GetOrders() override;

private:

};