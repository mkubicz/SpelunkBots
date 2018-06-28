#pragma once
#include "stdafx.h"
#include "IMovementAction.h"

#include <chrono>
using Clock = std::chrono::steady_clock;
using std::chrono::time_point;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

class WaitAction : public IMovementAction
{
private:
	bool(*_func)(int, int);
	int _arg1, _arg2;
	bool _conditionSet;
	bool _timeConditionSet;
	int _timeInMs;
	std::chrono::time_point<Clock> _start;

public:
	WaitAction(std::shared_ptr<IBot> const& bot);
	WaitAction(std::shared_ptr<IBot> const& bot, int timeInMs);
	void SetCondition(bool(*func)(int, int), int arg1, int arg2);
	void SetTimeCondition(int timeInMs);

	ordersStruct GetOrders() override;
};