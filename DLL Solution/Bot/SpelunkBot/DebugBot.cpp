#include "stdafx.h"
#include "DebugBot.h"
#include "CentralizeAction.h"
#include "WalkAction.h"


DebugBot::DebugBot()
{
	IMovementAction* a;
	a = new WalkAction(this, LEFT, 4.65, NODE_COORDS);
	_actionsQ.push(a);
	a = new CentralizeAction(this);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new CentralizeAction(this);
	_actionsQ.push(a);	
	a = new WalkAction(this, LEFT, 2.15, NODE_COORDS);
	_actionsQ.push(a);
	a = new CentralizeAction(this);
	_actionsQ.push(a);

	/*
	IMovementAction* a = new CentralizeAction(this);
	std::queue<IMovementAction*> Q1;
	Q1.push(a);

	CentralizeAction a2(this);
	Q1.push(&a2); 
	//& przed zmienn¹ - pobranie adresu
	//* przed wskaŸnikiem - wy³uskanie wartoœci
	*/
}

DebugBot::~DebugBot()
{
	while (!_actionsQ.empty())
	{
		delete _actionsQ.front();
		_actionsQ.pop();
	}
}

void DebugBot::ExecuteOrders(ordersStruct orders)
{
	_goRight = orders.goRight;
	_goLeft = orders.goLeft;
	_jump = orders.jump;
	_attack = orders.attack;
	_lookUp = orders.lookUp;
	_duck = orders.duck;
	_ropep = orders.ropep;
	//_bombp = orders.bombp; TODO
}

void DebugBot::ClearOrders()
{
	_goRight = false;
	_goLeft = false;
	_jump = false;
	_attack = false;
	_lookUp = false;
	_duck = false;
	_ropep = false;
	//_bombp = false;
}

void DebugBot::Update()
{
	if (!_actionsQ.empty())
	{
		ordersStruct orders = (_actionsQ.front())->GetOrders();
		ExecuteOrders(orders);

		if ((_actionsQ.front())->ActionDone())
		{
			delete _actionsQ.front();
			_actionsQ.pop();
			ClearOrders();
		}
	}
	else
	{
		ClearOrders();
	}


}
