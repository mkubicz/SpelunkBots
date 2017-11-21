#pragma once
#include "IMovementAction.h"

class JumpAboveAction : public IMovementAction
{
private:
	bool _jumped;
	int _targetXside;
	int _startingPosY;
	enum STATES {WALKING, JUMPING, HANGING};
public:
	JumpAboveAction(IBot *bot, bool goingRight);
	~JumpAboveAction() {};

	ordersStruct GetOrders() override;
}; 