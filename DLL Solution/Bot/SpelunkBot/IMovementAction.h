#pragma once
#include "stdafx.h"
#include "IBot.h"

struct ordersStruct
{
	bool goRight = false;
	bool goLeft = false;
	bool lookUp = false;
	bool duck = false;
	bool jump = false;
	bool attack = false;
	bool ropep = false;
	bool bombp = false;
};

class IMovementAction
{
protected:
	IBot* _bot;
	//DebugBot bot;

	MOVEMENTACTION _actionType;
	bool _actionDone;

public:
	IMovementAction(IBot* bot);
	~IMovementAction();

	MOVEMENTACTION ActionType();
	bool ActionDone();

	
	virtual ordersStruct GetOrders() =0;

};
