#include "stdafx.h"
#include "DebugBot.h"
#include "CentralizeAction.h"
#include "WalkAction.h"
#include "WalkUpAction.h"
#include "HangDropAction.h"
#include "JumpAboveAction.h"
#include "JumpAction.h"
#include "WalkOffLedgeAction.h"

DebugBot::DebugBot()
{
	//Node _current = Node{ (int)GetPlayerPositionXNode(), (int)GetPlayerPositionYNode()};
	_pathfinder = new Pathfinder(this);
	_objectManager = new ObjectManager(this);
	NewLevel();
}

void DebugBot::InitialiseHelperVariables()
{
	_state = SEARCHING_FOR_EXIT;
	//_state = DEBUG;
}

void DebugBot::NewLevel()
{
	_debugTimer = 30;



	InitialiseVariables();
	InitialiseHelperVariables();
	ClearOrders();

	while (!_actionsQ.empty())
	{
		delete _actionsQ.front();
		_actionsQ.pop();
	}

#pragma region Testy


	/*
	 * TESTING AND DEBUGGING
	 */
	IMovementAction* a;
	/*
	a = new WalkAction(this, RIGHT, false, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, DOWN, 2, 2);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, UP, 6, 0);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, false, 3, NODE_COORDS);
	_actionsQ.push(a);
	a = new WalkOffLedgeAction(this, RIGHT, 3, 1);
	_actionsQ.push(a);
	a = new HangDropAction(this, LEFT, false);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, false, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new WalkOffLedgeAction(this, LEFT, 4, 3);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, false, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new WalkOffLedgeAction(this, LEFT, 3, 1);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, false, 1, NODE_COORDS);
	_actionsQ.push(a);
	*/
	/*
	//JUMPTOLADDERTEST
	a = new WalkAction(this, RIGHT, false, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, UP, true, 3, 2);
	_actionsQ.push(a);
	*/

	//WALKOFFLEDGETEST
	/*
	a = new WalkAction(this, RIGHT, false, 2, NODE_COORDS);
	_actionsQ.push(a);
	for (int i = 1; i <= 7; i++)
	{
		a = new WalkOffLedgeAction(this, RIGHT, 2, i);
		_actionsQ.push(a); 
		a = new WalkAction(this, RIGHT, 1, NODE_COORDS);
		_actionsQ.push(a);
	}
	*/

	//WALKOFFLEDGETEST2
	/*
	a = new WalkAction(this, RIGHT, false, 2, NODE_COORDS);
	_actionsQ.push(a);
	for (int i = 1; i <= 7; i++)
	{
		a = new WalkOffLedgeAction(this, RIGHT, 3, i);
		_actionsQ.push(a);
		a = new WalkAction(this, RIGHT, 1, NODE_COORDS);
		_actionsQ.push(a);
	}
	*/

	//WALKOFFLEDGETEST3
	/*
	a = new WalkAction(this, RIGHT, false, 5, NODE_COORDS);
	_actionsQ.push(a);
	a = new	HangDropAction(this, RIGHT, false);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, false, 1, NODE_COORDS);
	_actionsQ.push(a);
	for (int i = 2; i <= 7; i++)
	{
	a = new WalkOffLedgeAction(this, RIGHT, 4, i);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, 1, NODE_COORDS);
	_actionsQ.push(a);
	}
	/*

	//WALKOFFLEDGETEST4
	/*
	a = new WalkAction(this, RIGHT, false, 2, NODE_COORDS);
	_actionsQ.push(a);
	for (int i = 3; i <= 7; i++)
	{
	a = new WalkOffLedgeAction(this, RIGHT, 5, i);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, 1, NODE_COORDS);
	_actionsQ.push(a);
	}
	*/

	

	//JUMPDOWNTEST2
	/*
	a = new WalkAction(this, RIGHT, false, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, DOWN, 2, 1);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, false, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, DOWN, 2, 2);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, false, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, DOWN, 2, 3);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, false, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, DOWN, 2, 4);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, false, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, DOWN, 2, 5);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, false, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, DOWN, 2, 6);
	_actionsQ.push(a);
	*/

	//JUMPDOWNTEST
	/*
	a = new WalkAction(this, RIGHT, false, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, DOWN, 2, 1);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, false, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, DOWN, 2, 2);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, false, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, DOWN, 2, 3);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, false, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, DOWN, 2, 4);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, false, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, DOWN, 2, 5);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, false, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, DOWN, 2, 6);
	_actionsQ.push(a);
	*/

	//SMALLJUMPTEST
	/*
	a = new WalkAction(this, RIGHT, false, 3, NODE_COORDS);
	_actionsQ.push(a);
	a = new WalkUpAction(this, RIGHT);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, UP, 1, 2);
	_actionsQ.push(a);
	*/

	//SMALLJUMPTEST2
	/*
	a = new WalkAction(this, LEFT, false, 6, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, UP, 1, 2);
	_actionsQ.push(a);
	*/

	//WALKTEST
	/*
	a = new WalkAction(this, RIGHT, true, 5, NODE_COORDS);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, true, 5, NODE_COORDS);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, true, 5, NODE_COORDS);
	_actionsQ.push(a);
	*/

	/*
	//JUMPABOVETEST
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
	*/

	
	//HANGDROPTEST
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

	//WALKUPTEST
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

	//JUMPTEST
	/*
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 3, 0);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 5, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 7, 0);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 5, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, false, 5, 4);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 3, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, false, 2, 5);
	_actionsQ.push(a);
	a = new JumpAction(this, LEFT, false, 3, 4);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, NORUN, 3, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, LEFT, true, 3, 1);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, NORUN, 4, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, LEFT, true, 2, 2);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, NORUN, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, LEFT, true, 4, 2);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, NORUN, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, LEFT, true, 6, 1);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, NORUN, 3, NODE_COORDS);
	_actionsQ.push(a);
	*/
	//JUMPTEST2
	/*
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a); 
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a); 
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a); 
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 4, 2);
	_actionsQ.push(a);
	*/
	
	//JUMPUPTEST
	/*
	a = new JumpAction(this, RIGHT, true, 2, 1);
	_actionsQ.push(a);	
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 3, 1);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 4, 1);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 5, 1);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	*/

	//JUMPUPTEST2
	/*
	a = new JumpAction(this, RIGHT, true, 2, 2);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 3, 2);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 4, 2);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	*/

	//JUMPFLATEST
	/*
	a = new JumpAction(this, RIGHT, true, 2, 0);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 3, 0);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 4, 0);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 5, 0);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 6, 0);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);	
	a = new JumpAction(this, RIGHT, true, 7, 0);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	*/

#pragma endregion 

}


DebugBot::~DebugBot()
{
	while (!_actionsQ.empty())
	{
		delete _actionsQ.front();
		_actionsQ.pop();
	}
	delete _pathfinder;
}

ObjectManager* DebugBot::GetObjectManager()
{
	return _objectManager;
}

SpState DebugBot::SpelunkerState()
{
	return (SpState)GetSpelunkerState();
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

#pragma region CreateCommands using actionToReach
//void DebugBot::CreateCommands(std::vector<Node> path)
//{
//	bool actionFound = false;
//	int currX, currY, targetX, targetY;
//	currX = (int)_playerPositionXNode;
//	currY = (int)_playerPositionYNode;
//	
//	for (int i = 0; i < path.size(); i++)
//	{
//		targetX = path[i].x;
//		targetY = path[i].y;
//
//		int distX = abs(targetX - currX);
//		int distY = abs(targetY - currY);
//
//		MOVEMENTACTION action = (MOVEMENTACTION)path[i].actionToReach;
//	
//		switch (action)
//		{
//		case IDLE:
//			break;
//		case CENTRALIZE:
//			_actionsQ.push(new CentralizeAction(this));
//			break;
//		case WALKRIGHT:
//			_actionsQ.push(new WalkAction(this, RIGHT, NORUN, distX, NODE_COORDS));
//			break;
//		case WALKLEFT:
//			_actionsQ.push(new WalkAction(this, LEFT, NORUN, distX, NODE_COORDS));
//			break;
//		case RUNRIGHT:
//			_actionsQ.push(new WalkAction(this, RIGHT, RUN, distX, NODE_COORDS));
//			break;
//		case RUNLEFT:
//			_actionsQ.push(new WalkAction(this, LEFT, RUN, distX, NODE_COORDS));
//			break;
//		case HANGDROPRIGHT:
//			_actionsQ.push(new HangDropAction(this, RIGHT, false));
//			break;
//		case HANGDROPLEFT:
//			_actionsQ.push(new HangDropAction(this, LEFT, false));
//			break;
//		case JUMPABOVERIGHT:
//			_actionsQ.push(new JumpAboveAction(this, RIGHT));
//			break;
//		case JUMPABOVELEFT:
//			_actionsQ.push(new JumpAboveAction(this, LEFT));
//			break;
//		case JUMPUPRIGHT:
//			_actionsQ.push(new JumpAction(this, RIGHT, UP, distX, distY));
//			break;
//		case JUMPUPLEFT:
//			_actionsQ.push(new JumpAction(this, LEFT, UP, distX, distY));
//			break;
//		case JUMPDOWNRIGHT:
//			_actionsQ.push(new JumpAction(this, RIGHT, DOWN, distX, distY));
//			break;
//		case JUMPDOWNLEFT:
//			_actionsQ.push(new JumpAction(this, LEFT, DOWN, distX, distY));
//			break;
//		case WALKOFFLEDGERIGHT:
//			_actionsQ.push(new WalkOffLedgeAction(this, RIGHT, distX, distY));
//			break;
//		case WALKOFFLEDGELEFT:
//			_actionsQ.push(new WalkOffLedgeAction(this, LEFT, distX, distY));
//			break;
//		case WALKUPRIGHT:
//			_actionsQ.push(new WalkUpAction(this, RIGHT));
//			break;
//		case WALKUPLEFT:
//			_actionsQ.push(new WalkUpAction(this, LEFT));
//			break;
//		default:
//			break;
//		}
//
//		currX = targetX;
//		currY = targetY;
//	}
//}

#pragma endregion

#pragma region old CreateCommands (without calculating neighbours)
//
//void DebugBot::CreateCommands(std::vector<Node> path)
//{
//
//	bool actionFound = false;
//	int currX, currY, targetX, targetY;
//	currX = (int)_playerPositionXNode;
//	currY = (int)_playerPositionYNode;
//
//	for (int i = 0; i < path.size(); i++)
//	{
//		targetX = path[i].x;
//		targetY = path[i].y;
//
//		int distX = abs(targetX - currX);
//		int distY = abs(targetY - currY);
//
//		//walk
//		if (distX == 1 && currY == targetY && !actionFound)
//		{
//			
//			if (targetX > currX)
//			{
//				if (!_actionsQ.empty() && _actionsQ.back()->ActionType() == WALKRIGHT)
//					//previous action is also a WalkRight, so we increase its distance instead of creating a new command (ActionBatching™)
//					dynamic_cast<WalkAction*>(_actionsQ.back())->AddDistance(distX);	
//				else
//					_actionsQ.push(new WalkAction(this, RIGHT, NORUN, distX, NODE_COORDS));
//			}
//			else
//			{
//				if (!_actionsQ.empty() && _actionsQ.back()->ActionType() == WALKLEFT)
//					//previous action is also a WalkLeft, so we increase its distance instead of creating a new command (ActionBatching™)
//					dynamic_cast<WalkAction*>(_actionsQ.back())->AddDistance(distX);
//				else
//					_actionsQ.push(new WalkAction(this, LEFT, NORUN, distX, NODE_COORDS));
//			}
//
//			
//			actionFound = true;
//		}
//
//		//run (only above 1-tile gaps)
//		if (distX == 2 && currY == targetY && !actionFound)
//		{
//			if (targetX > currX)
//			{
//				if (IsNodePassable(currX + 1, currY, NODE_COORDS))
//				{
//					_actionsQ.push(new WalkAction(this, RIGHT, RUN, distX, NODE_COORDS));
//					actionFound = true;
//				}
//			}
//			else
//			{
//				if (IsNodePassable(currX - 1, currY, NODE_COORDS))
//				{
//					_actionsQ.push(new WalkAction(this, LEFT, RUN, distX, NODE_COORDS));
//					actionFound = true;
//				}
//			}
//		}
//
//		//hangdrop
//		if (distX == 1 && targetY > currY && !actionFound)
//		{
//			if (targetX > currX)
//				_actionsQ.push(new HangDropAction(this, RIGHT, false));
//			else
//				_actionsQ.push(new HangDropAction(this, LEFT, false));
//			actionFound = true;
//		}
//
//		//walkup
//		if (distX == 1 && (distY == 1 || distY == 2) && !actionFound)
//		{
//			if (targetX > currX)
//			{
//				if (distY == 2 && IsNodePassable(targetX, targetY + 2, NODE_COORDS))
//				{
//					_actionsQ.push(new JumpAction(this, RIGHT, UP, distX, distY));
//					//_actionsQ.push(new CentralizeAction(this));
//				}
//				else
//					_actionsQ.push(new WalkUpAction(this, RIGHT));
//			}
//			else
//			{
//				if (distY == 2 && IsNodePassable(targetX, targetY + 2, NODE_COORDS))
//				{
//					_actionsQ.push(new JumpAction(this, LEFT, UP, distX, distY));
//					//_actionsQ.push(new CentralizeAction(this));
//				}
//				else
//					_actionsQ.push(new WalkUpAction(this, LEFT));
//
//			}
//			actionFound = true;
//		}
//
//		/*
//		//walkup
//		if (distX == 1 && (distY == 1 || distY == 2) && !actionFound)
//		{
//			if (targetX > currX)
//				_actionsQ.push(new WalkUpAction(this, RIGHT));
//			else
//				_actionsQ.push(new WalkUpAction(this, LEFT));
//			actionFound = true;
//		}
//		*/
//
//		//jumpabove
//		if (currX == targetX && !actionFound)
//		{
//			if (IsNodePassable(currX + 1, currY + 1, NODE_COORDS))
//				_actionsQ.push(new JumpAboveAction(this, RIGHT));
//			else
//				_actionsQ.push(new JumpAboveAction(this, LEFT));
//			actionFound = true;
//		}
//
//
//		//walkoffledge
//		if (distX > 1 && targetY > currY && distX < distY + 3 && !actionFound)
//		{
//			if (targetX > currX)
//			{
//				for (int i = currX; i <= targetX; i++)
//					if (!IsNodePassable(i, currY - 1, NODE_COORDS))
//					{
//						actionFound = true;
//						break;
//					}
//
//				for (int i = currX + 1; i < targetX; i++)
//					if (!IsNodePassable(i, currY - 2, NODE_COORDS))
//					{
//						actionFound = true;
//						break;
//					}
//
//				if (actionFound) 
//					_actionsQ.push(new WalkOffLedgeAction(this, RIGHT, distX, distY));
//			}
//			else
//			{
//				for (int i = currX; i >= targetX; i--)
//					if (!IsNodePassable(i, currY - 1, NODE_COORDS))
//					{
//						actionFound = true;
//						break;
//					}
//
//				for (int i = currX - 1; i > targetX; i--)
//					if (!IsNodePassable(i, currY - 2, NODE_COORDS))
//					{
//						actionFound = true;
//						break;
//					}
//
//				if (actionFound) 
//					_actionsQ.push(new WalkOffLedgeAction(this, LEFT, distX, distY));
//			}
//			//_actionsQ.push(new CentralizeAction(this));
//
//		}
//
//
//		//jump
//		if (distX > 1 && !actionFound)
//		{
//			if (targetX > currX)
//				if (targetY > currY)
//					_actionsQ.push(new JumpAction(this, RIGHT, DOWN, distX, distY));
//				else
//					_actionsQ.push(new JumpAction(this, RIGHT, UP, distX, distY));
//			else
//				if (targetY > currY)
//					_actionsQ.push(new JumpAction(this, LEFT, DOWN, distX, distY));
//				else
//					_actionsQ.push(new JumpAction(this, LEFT, UP, distX, distY));
//			//_actionsQ.push(new CentralizeAction(this));
//			actionFound = true;
//		}
//
//
//		currX = targetX;
//		currY = targetY;
//		actionFound = false;
//	}
//}
#pragma endregion

void DebugBot::CreateCommands(std::vector<Node> path)
{
	int currX, currY, targetX, targetY, distX, distY;
	currX = (int)_playerPositionXNode;
	currY = (int)_playerPositionYNode;

	for (int i = 0; i < path.size(); i++)
	{
		targetX = path[i].x;
		targetY = path[i].y;
		distX = abs(targetX - currX);
		distY = abs(targetY - currY);

		
		std::vector<Node> neighbours = _pathfinder->CalculateNeighboursList(Node{ currX, currY });
		

		std::vector<MOVEMENTACTION> actionsToReachTarget;

		for (int i = 0; i < neighbours.size(); i++)
			if (neighbours[i].x == targetX && neighbours[i].y == targetY)
				actionsToReachTarget.push_back((MOVEMENTACTION)neighbours[i].actionToReach);

		if (actionsToReachTarget.empty())
		{
			std::cout << "Error! Couldn't create commands.";
			return;
		}


		//take first action that allows to reach target
		MOVEMENTACTION action = actionsToReachTarget[0];

		AddActionToActionQueue(action, distX, distY);


		currX = targetX;
		currY = targetY;
	}
}

void DebugBot::AddActionToActionQueue(MOVEMENTACTION action, int distX, int distY)
{
	switch (action)
	{
	case IDLE:
		break;
	case CENTRALIZE:
		_actionsQ.push(new CentralizeAction(this));
		break;
	case WALKRIGHT:
		if (!_actionsQ.empty() && _actionsQ.back()->ActionType() == WALKRIGHT)
			//previous action is also a WalkRight, so we increase its distance instead of creating a new command (ActionBatching™)
			dynamic_cast<WalkAction*>(_actionsQ.back())->AddDistance(distX);	
		else
			_actionsQ.push(new WalkAction(this, RIGHT, NORUN, distX, NODE_COORDS));
		break;
	case WALKLEFT:
		if (!_actionsQ.empty() && _actionsQ.back()->ActionType() == WALKLEFT)
			//previous action is also a WalkLeft, so we increase its distance instead of creating a new command (ActionBatching™)
			dynamic_cast<WalkAction*>(_actionsQ.back())->AddDistance(distX);	
		else
			_actionsQ.push(new WalkAction(this, LEFT, NORUN, distX, NODE_COORDS));
		break;
	case RUNRIGHT:
		_actionsQ.push(new WalkAction(this, RIGHT, RUN, distX, NODE_COORDS));
		break;
	case RUNLEFT:
		_actionsQ.push(new WalkAction(this, LEFT, RUN, distX, NODE_COORDS));
		break;
	case HANGDROPRIGHT:
		_actionsQ.push(new HangDropAction(this, RIGHT, false));
		break;
	case HANGDROPLEFT:
		_actionsQ.push(new HangDropAction(this, LEFT, false));
		break;
	case JUMPABOVERIGHT:
		_actionsQ.push(new JumpAboveAction(this, RIGHT));
		break;
	case JUMPABOVELEFT:
		_actionsQ.push(new JumpAboveAction(this, LEFT));
		break;
	case JUMPUPRIGHT:
		_actionsQ.push(new JumpAction(this, RIGHT, UP, distX, distY));
		break;
	case JUMPUPLEFT:
		_actionsQ.push(new JumpAction(this, LEFT, UP, distX, distY));
		break;
	case JUMPDOWNRIGHT:
		_actionsQ.push(new JumpAction(this, RIGHT, DOWN, distX, distY));
		break;
	case JUMPDOWNLEFT:
		_actionsQ.push(new JumpAction(this, LEFT, DOWN, distX, distY));
		break;
	case WALKOFFLEDGERIGHT:
		_actionsQ.push(new WalkOffLedgeAction(this, RIGHT, distX, distY));
		break;
	case WALKOFFLEDGELEFT:
		_actionsQ.push(new WalkOffLedgeAction(this, LEFT, distX, distY));
		break;
	case WALKUPRIGHT:
		_actionsQ.push(new WalkUpAction(this, RIGHT));
		break;
	case WALKUPLEFT:
		_actionsQ.push(new WalkUpAction(this, LEFT));
		break;
		/*
		case CLIMBUP,
			case CLIMBDOWN,
			case JUMPUP_LADDER,
			case JUMPUPRIGHT_LADDER,
			case JUMPUPLEFT_LADDER,
			case JUMPDOWNRIGHT_LADDER,
			case JUMPDOWNLEFT_LADDER,
			case WALKOFFLEDGERIGHT_LADDER,
			case WALKOFFLEDGELEFT_LADDER,
	*/
	default:
		break;
	}
}



bool DebugBot::FindExit(int &x, int &y)
{
	for (int nodeX = 0; nodeX < X_NODES; nodeX += 1)
	{
		for (int nodeY = 0; nodeY < Y_NODES; nodeY += 1)
		{
			if (GetNodeState(nodeX, nodeY, NODE_COORDS) == spExit)
			{
				x = nodeX;
				y = nodeY;
				return true;
			}
		}
	}
	return false;
}


void DebugBot::Update()
{
	int exitX, exitY;
	int playerPosXNode = (int)_playerPositionXNode;
	int playerPosYNode = (int)_playerPositionYNode;

	
	if (_debugTimer < 0)
	{
		_pathfinder->NeighboursDebug((int)_playerPositionXNode, (int)_playerPositionYNode);
		_debugTimer = 10;
	}
	_debugTimer -= 1;

	//std::vector<enemyObject> enemies = _objectManager->GetVisibleEnemies();
	//collectableObject e = GetVisibleEnemies();


	_objectManager->UpdateGameObjectLists();


	std::cout << SpelunkerState() << std::endl;



	switch (_state)
	{
		case SEARCHING_FOR_EXIT:
			if (FindExit(exitX, exitY) && _pathfinder->CalculatePath(_playerPositionXNode, _playerPositionYNode, exitX, exitY, NODE_COORDS))
			{
				CreateCommands(_pathfinder->GetPathListNode());
				_hasGoal = true;
			}
			else
			{
				//dfs
				std::vector<Node> explTargets = _pathfinder->FindExplorationTargets(_playerPositionXNode, _playerPositionYNode, NODE_COORDS);

				if (explTargets.size() > 0)
				{
					Node target = explTargets[0];
					int dist = abs((int)_playerPositionXNode - target.x) + abs((int)_playerPositionYNode - target.y);

					for (int i = 1; i < explTargets.size(); i++)
					{
						int distNew = abs((int)_playerPositionXNode - explTargets[i].x) + abs((int)_playerPositionYNode - explTargets[i].y);
						if (distNew < dist)
						{
							target = explTargets[i];
							dist = distNew;
						}

					}
					
					_pathfinder->CalculatePath(_playerPositionXNode, _playerPositionYNode, target.x, target.y, NODE_COORDS);
					CreateCommands(_pathfinder->GetPathListNode());
				}
				else
				{
					//no exploration target - we can search again with looser criteria, or allow usage of ropes and bombs
				}
				

			}

			_state = EXECUTING_COMMANDS;
			break;

		case EXECUTING_COMMANDS:
			if (!_actionsQ.empty())
			{
				ordersStruct orders = (_actionsQ.front())->GetOrders();
				


				//naive fighting (mostly with snakes)
				if (_attackTimer == 0)
				{
					std::vector<enemyObject> enemies = _objectManager->GetVisibleEnemies();
					for (int i = 0; i < enemies.size(); i++)
					{
						if (enemies[i].y == _playerPositionYNode &&
							abs(enemies[i].x - _playerPositionXNode) < 2.5)
						{
							orders.attack = true;
							_attackTimer = 6;
							break;
						}
					}
				}
				else
					_attackTimer -= 1;



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
				//if (_hasGoal) _lookUp = true;
				if (GetNodeState(_playerPositionX, _playerPositionY, PIXEL_COORDS) == spExit)
					_lookUp = true;
				else 
					_state = SEARCHING_FOR_EXIT;
			}

			break;

		case DEBUG:
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
	}
	
	
}


/*
IMovementAction* a = new CentralizeAction(this);
std::queue<IMovementAction*> Q1;
Q1.push(a);

CentralizeAction a2(this);
Q1.push(&a2);
//& przed zmienn¹ - pobranie adresu
//* przed wskaŸnikiem - wy³uskanie wartoœci
*/

				//eksplorowanie mg³y
				//simple solution: find fog, find standable tile next to it, try to create path -> success: go -> fail: find another fog or standable tile
				/*
				int exploreX, exploreY;
				//find node with fog
				for (int nodeX = 0; nodeX < X_NODES; nodeX += 1)
				{
					for (int nodeY = 0; nodeY < Y_NODES; nodeY += 1)
					{
						
						if (GetNodeState(nodeX, nodeY, NODE_COORDS) == -1) //-1 means fog
						{
							//find standable node within radius 5 from node with fog
							for (int iX = -5; iX <= 5; iX++)
								for (int iY = -5; iY <= 5; iY++)
								{
									if (_pathfinder->CanStandInNode(nodeX + iX, nodeY + iY))
									{
										exploreX = nodeX + iX;
										exploreY = nodeY + iY;
										//if there is a path to it create commands and go
										if (_pathfinder->CalculatePath(_playerPositionX, _playerPositionY, exploreX, exploreY, PIXEL_COORDS))
										{
											CreateCommands(_pathfinder->GetPathList());
											_state = EXECUTING_COMMANDS;
											return; //get out of this costly loop
										}

										//early exit - this loop takes too long
										//TODO instead of early exit - make random possible move and after it search again
										_state = EXECUTING_COMMANDS;
										return;
									}
								}
						}
					}
				}
				*/



				//make a random move with priority
				//priority: going down, same as previous
				/*
				int currPosX = (int)_playerPositionXNode;
				int currPosY = (int)_playerPositionYNode;
				std::vector<Node> neighbours = _pathfinder->CalculateNeighboursList(Node{ currPosX, currPosY });

				int dirX, dirY;

				int i = 0;
				while (i < neighbours.size())
				{
				if (neighbours[i].y > currPosY)
				dirY = DOWN;
				else if (neighbours[i].y == currPosY)
				dirY = NONE;
				else
				dirY = UP;

				if (neighbours[i].x > currPosX)
				dirX = RIGHT;
				else if (neighbours[i].x == currPosX)
				dirX = NONE;
				else
				dirX = LEFT;

				if (dirY == DOWN) break;
				if (dirX == prevDirX) break;

				i++;
				}

				if (i == neighbours.size())
				{
				srand(time(NULL));
				i = rand() % neighbours.size();

				if (neighbours[i].y > currPosY)
				dirY = DOWN;
				else if (neighbours[i].y == currPosY)
				dirY = NONE;
				else
				dirY = UP;

				if (neighbours[i].x > currPosX)
				dirX = RIGHT;
				else if (neighbours[i].x == currPosX)
				dirX = NONE;
				else
				dirX = LEFT;
				}


				prevDirX = dirX;
				prevDirY = dirY;

				std::vector<Node> path;
				path.push_back(neighbours[i]);

				CreateCommands(path);
				*/

				//move to fog
				/*
				int targetX = _playerPositionXNode;
				while (targetX < X_NODES && GetFogState(targetX, _playerPositionYNode, NODE_COORDS) == 0)
				{
				targetX += 1;
				}

				std::vector<Node> neighbours = _pathfinder->CalculateNeighboursList(current);
				*/