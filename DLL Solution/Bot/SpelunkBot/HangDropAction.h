#pragma once
#include "IMovementAction.h"

class HangDropAction : public IMovementAction
{
private:
	bool _lookDown;
	bool _hanging;
	int _lookDownTimer;
	int _previousPosY;
	int _targetYHang;
	enum STATE { WALKING, HANGING, FALLING };
	STATE _state;
public:
	HangDropAction(IBot *bot, bool goingRight, bool lookDown);
	~HangDropAction() {};

	ordersStruct GetOrders() override;
};