#pragma once
#include "IMovementAction.h"

class JumpAboveAction : public IMovementAction
{
private:
	bool _jumped;
	int _targetXside;
	int _targetXmid;
	int _targetYmid;
	int _startingPosY;
	enum STATES {WALKING, JUMPING, HANGING};
public:
	JumpAboveAction(IBot *bot, DIRECTIONX directionX);
	~JumpAboveAction() {};

	ordersStruct GetOrders() override;
}; 
