#pragma once
#include "IBot.h"
#include "IMovementAction.h"


class DebugBot : public IBot
{
public:
	DebugBot();// { }
	~DebugBot();// { }

	std::queue<IMovementAction*> _actionsQ;
	bool _actionDone = false;

	void ExecuteOrders(ordersStruct orders);
	void ClearOrders();
	void Update() override;
};