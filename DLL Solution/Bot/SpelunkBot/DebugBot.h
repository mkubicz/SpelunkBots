#pragma once
#include "IBot.h"
#include "IMovementAction.h"
#include "Pathfinder.h"


class DebugBot : public IBot
{
public:
	DebugBot();// { }
	~DebugBot();// { }



	void ExecuteOrders(ordersStruct orders);
	void ClearOrders();
	void Update() override;
	void NewLevel() override;

private:
	std::queue<IMovementAction*> _actionsQ;
	Pathfinder* _pathfinder;

	int _debugTimer;
};