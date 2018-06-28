#include "stdafx.h"
#include "WaitAction.h"


WaitAction::WaitAction(std::shared_ptr<IBot> const & bot)
	: IMovementAction(bot)
{
	_conditionSet = false;
	_timeConditionSet = false;
	_actionType = WAIT;
}

WaitAction::WaitAction(std::shared_ptr<IBot> const & bot, int timeInMs)
	: WaitAction(bot)
{
	//_conditionSet = false;
	SetTimeCondition(timeInMs);
}

void WaitAction::SetCondition(bool(*func)(int, int), int arg1, int arg2)
{
	_conditionSet = true;
	_func = func;
	_arg1 = arg1;
	_arg2 = arg2;
}

void WaitAction::SetTimeCondition(int timeInMs)
{
	_timeConditionSet = true;
	_timeInMs = timeInMs;
}

ordersStruct WaitAction::GetOrders()
{
	if (_conditionSet && _func(_arg1, _arg2))
		_actionDone = true;

	if (_timeConditionSet)
	{
		if (!_actionInProgress)
		{
			_start = Clock::now();
			_actionInProgress = true;
		}

		milliseconds diff = duration_cast<milliseconds>(Clock::now() - _start);
		if (diff.count() >= _timeInMs)
			_actionDone = true;
	}

	if (!_conditionSet && !_timeConditionSet)
		_actionDone = true;

	return ordersStruct();
}
