#pragma once
#include "IMovementAction.h"

class HangDropAction : public IMovementAction
{
private:
	bool _hanging;
	int _previousPosY;
	int _targetYHang;

	int _targetXHang;
	enum STATE { WALKING, HANGING, FALLING };
	STATE _state;
public:
	HangDropAction(std::shared_ptr<IBot> const&, DIRECTIONX directionX);
	~HangDropAction() {};

	ordersStruct GetOrders() override;
};