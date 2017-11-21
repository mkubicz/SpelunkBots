#include "stdafx.h"
#include "DebugBot.h"
#include "CentralizeAction.h"
#include "WalkAction.h"
#include "WalkUpAction.h"
#include "HangDropAction.h"
#include "JumpAboveAction.h"

DebugBot::DebugBot()
{

	IMovementAction* a;	
	a = new WalkAction(this, RIGHT, 5, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAboveAction(this, RIGHT);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAboveAction(this, LEFT);
	_actionsQ.push(a);	
	a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAboveAction(this, RIGHT);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAboveAction(this, LEFT);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAboveAction(this, LEFT);
	_actionsQ.push(a);
	a = new JumpAboveAction(this, LEFT);
	_actionsQ.push(a);
	a = new JumpAboveAction(this, LEFT);
	_actionsQ.push(a);


	/*
	a = new CentralizeAction(this);
	_actionsQ.push(a);
	a = new HangDropAction(this, RIGHT, false);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, 5, NODE_COORDS);
	_actionsQ.push(a);
	a = new HangDropAction(this, RIGHT, false);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, 5, NODE_COORDS);
	_actionsQ.push(a);
	a = new HangDropAction(this, LEFT, false);
	_actionsQ.push(a);
	*/
	
	/*
	a = new WalkAction(this, RIGHT, 5, NODE_COORDS);
	_actionsQ.push(a);
	a = new WalkUpAction(this, RIGHT);
	_actionsQ.push(a);	
	a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new WalkUpAction(this, RIGHT);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new WalkUpAction(this, RIGHT);
	_actionsQ.push(a);
	a = new WalkUpAction(this, RIGHT);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, 3, NODE_COORDS);
	_actionsQ.push(a);
	a = new WalkUpAction(this, RIGHT);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new WalkUpAction(this, RIGHT);
	_actionsQ.push(a);
	a = new WalkUpAction(this, RIGHT);
	_actionsQ.push(a);
	*/
	/*
	IMovementAction* a = new CentralizeAction(this);
	std::queue<IMovementAction*> Q1;
	Q1.push(a);

	CentralizeAction a2(this);
	Q1.push(&a2); 
	//& przed zmiennπ - pobranie adresu
	//* przed wskaünikiem - wy≥uskanie wartoúci
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
	_bombp = orders.bombp;
	_runp = orders.run;
	_payp = orders.pay;
	_itemp = orders.itemp;
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
	_bombp = false;
	_runp = false;
	_payp = false;
	_itemp = false;
}

void DebugBot::Update()
{
	if (_playerPositionX == 100)
	{
		std::cout << "jest" << std::endl;
	}


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
