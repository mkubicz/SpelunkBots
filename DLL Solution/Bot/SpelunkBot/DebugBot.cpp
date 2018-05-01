#include "stdafx.h"
#include "DebugBot.h"
#include "CentralizeAction.h"
#include "WalkAction.h"
#include "WalkUpAction.h"
#include "HangDropAction.h"
#include "JumpAboveAction.h"
#include "JumpActionStary.h"
#include "WalkOffLedgeAction.h"
#include "ClimbFromHangAction.h"
#include "HangAction.h"
#include "DropAction.h"
#include "JumpAction.h"
#include "JumpFromLadderAction.h"
#include "JumpToLadderAction.h"
#include "ClimbAction.h"

#include <fstream>
#include <ostream>


DebugBot::DebugBot()
{
	_pathfinder = new Pathfinder(this);
	_objectManager = new ObjectManager(this);
	NewLevel();
}

void DebugBot::BotLogicStateDebug()
{
	std::ofstream fileStream;
	fileStream.open("botlogicstate.txt");

	fileStream << BotLogicStrings[_botLogicState] << std::endl;
	//fileStream << BotLogicStrings[_prevState] << std::endl;

	fileStream.close();
}

void DebugBot::InitialiseHelperVariables()
{
	//_state = SEARCHING_FOR_EXIT;
	//_state = DEBUG;

	//_primState = pIDLE;
	//_primState = SEARCHING_FOR_EXIT;
	//_secState = WAITING_FOR_PATH;
	//_secState = DEBUG;


	//_botLogicState = GATHERING;
	_botLogicState = GATHER_FROM_CC;
	_secState = WAITING_FOR_PATH;
	
	_hasMomentum = false;
}

void DebugBot::NewLevel()
{
	_debugTimer = 30;

	_botLogicState = EXIT;
	if (_botLogicThread.joinable())
		_botLogicThread.join();

	_botLogicState = GATHER_FROM_CC;
	_botLogicThread = std::thread(&DebugBot::BotLogic, this);

	_pathsQ.clear();

	InitialiseVariables();
	InitialiseHelperVariables();
	_pathfinder->InitializeVariables();
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

	//a = new HangAction(this, xRIGHT);
	//_actionsQ.push(a);
	//a = new DropAction(this, LADDER, 3);
	//_actionsQ.push(a);
	


	//NEWJUMPTEST1
	//y=-1 x=1..6 ground
	//int distY = -1;
	//int maxX = 6;
	//for (int distX = maxX; distX > 0; distX--)
	//{
	//	a = new JumpAction(this, GROUND, distX, distY);
	//	_actionsQ.push(a);
	//	a = new HangDropAction(this, xLEFT);
	//	_actionsQ.push(a);
	//	a = new WalkAction(this, -distX+2);
	//	_actionsQ.push(a);
	//}


	//y=-1 x=1..6 ladder
	//int distY = -1;
	//int maxX = 6;
	//for (int distX = maxX; distX > 0; distX--)
	//{
	//	a = new JumpAction(this, LADDER, distX, distY);
	//	_actionsQ.push(a);
	//	a = new DropAction(this);
	//	_actionsQ.push(a);
	//	a = new WalkAction(this, -distX+1);
	//	_actionsQ.push(a);
	//}

	//y=-1 x=7 ledge
	//a = new JumpAction(this, LEDGE, 6, 0);
	//_actionsQ.push(a);
	//a = new ClimbFromHangAction(this, xRIGHT);
	//_actionsQ.push(a);

	//y=-2 x=1..6 ledge
	//int distY = -1;
	//int maxX = 5;
	//for (int distX = maxX; distX >= 0; distX--)
	//{
	//	a = new JumpAction(this, LEDGE, distX, distY);
	//	_actionsQ.push(a);
	//	a = new ClimbFromHangAction(this, xRIGHT);
	//	_actionsQ.push(a);
	//	a = new HangDropAction(this, xLEFT);
	//	_actionsQ.push(a);
	//	a = new WalkAction(this, -distX+1);
	//	_actionsQ.push(a);
	//}



	//y=1..5 x=2..8 ladder
	//y=0 x=1..7 ladder
	//int distY = 5;
	//int maxX = 8;
	//DIRECTIONX dir;
	//if (distY % 2 == 1)
	//	dir = xRIGHT;
	//else
	//	dir = xLEFT;

	//for (int distX = 2; distX <= maxX; distX++)
	//{
	//	a = new JumpAction(this, LADDER, dir*distX, distY);
	//	_actionsQ.push(a);
	//	a = new DropAction(this);
	//	_actionsQ.push(a);
	//	a = new WalkAction(this, dir*(-distX-1));
	//	_actionsQ.push(a);
	//	a = new ClimbAction(this, -distY-2);
	//	_actionsQ.push(a);
	//	a = new JumpFromLadderAction(this, GROUND, false, dir*1, 0);
	//	_actionsQ.push(a);
	//}


	//y=1..5 x=2..8 ground
	//y=0 x=2..7 ground
	//int distY = 5;
	//int maxX = 8;
	//DIRECTIONX dir;
	//if (distY % 2 == 1)
	//	dir = xRIGHT;
	//else
	//	dir = xLEFT;

	//for (int distX = 2; distX <= maxX; distX++)
	//{
	//	a = new JumpAction(this, GROUND, dir*distX, distY);
	//	_actionsQ.push(a);
	//	a = new WalkAction(this, dir*(-distX + 2));
	//	_actionsQ.push(a);
	//	a = new HangDropAction(this, (DIRECTIONX)-dir);
	//	_actionsQ.push(a);
	//	a = new WalkAction(this, dir*(-2));
	//	_actionsQ.push(a);
	//	a = new ClimbAction(this, -distY - 2);
	//	_actionsQ.push(a);
	//	a = new JumpFromLadderAction(this, GROUND, false, dir * 1, 0);
	//	_actionsQ.push(a);
	//}


	//y=0 x=2..7 ground
	//int distY = 0;
	//int maxX = 7;

	//for (int distX = 2; distX <= maxX; distX++)
	//{
	//	a = new JumpAction(this, GROUND, distX, distY);
	//	_actionsQ.push(a);
	//	a = new WalkAction(this, -distX));
	//	_actionsQ.push(a);
	//}


	//JUMPFROMLADDERTEST3
	//jump without momentum test

	//ACTION_TARGET target = LADDER; //target should be GROUND or LADDER (LEDGE will set itself automatically)
	////int distY = -1;
	//int maxX = 3;

	//for (int distY = 0; distY <= 5; distY++)
	//{
	//	//do not change
	//	int climbDist = -distY - 3;

	//	for (int i = 1; i <= maxX; i++)
	//	{
	//		if (target == LADDER)
	//		{
	//			a = new WalkAction(this, 2, NORUN);
	//			_actionsQ.push(a);
	//			a = new ClimbAction(this, climbDist);
	//			_actionsQ.push(a);
	//			a = new JumpFromLadderAction(this, target, false, i, distY);
	//			_actionsQ.push(a);
	//			a = new DropAction(this);
	//			_actionsQ.push(a);
	//			a = new WalkAction(this, 1, NORUN);
	//			_actionsQ.push(a);
	//		}
	//		else if (target == GROUND)
	//		{
	//			if (distY == -2 || (distY == -1 && i == 3))
	//			{
	//				a = new WalkAction(this, 2, NORUN);
	//				_actionsQ.push(a);
	//				a = new ClimbAction(this, climbDist);
	//				_actionsQ.push(a);
	//				a = new JumpFromLadderAction(this, LEDGE, false, i - 1, distY + 1);
	//				_actionsQ.push(a);
	//				a = new ClimbFromHangAction(this, xRIGHT);
	//				_actionsQ.push(a);
	//				a = new HangDropAction(this, xRIGHT);
	//				_actionsQ.push(a);
	//			}
	//			else
	//			{
	//				a = new WalkAction(this, 2, NORUN);
	//				_actionsQ.push(a);
	//				a = new ClimbAction(this, climbDist);
	//				_actionsQ.push(a);
	//				a = new JumpFromLadderAction(this, GROUND, false, i, distY);
	//				_actionsQ.push(a);
	//				a = new HangDropAction(this, xRIGHT);
	//				_actionsQ.push(a);
	//			}
	//		}
	//	}

	//	a = new WalkAction(this, -15, NORUN);
	//	_actionsQ.push(a);
	//}



	//JUMPFROMLADDERTEST3
	//jump with momentum test
	//distY=-1..5
	//int distY = 3;
	//ACTION_TARGET target = LADDER;
	//int minX = 1;
	//int maxX = 5;

	//for (int distY = -1; distY <= 5; distY++)
	//{

	//	int climbDist = -distY - 3;

	//	if (target == LADDER)
	//	{
	//		if (distY == -1) maxX = 4;
	//		else maxX = 5;
	//	}


	//	if (target == LADDER)
	//	{
	//		for (int i = minX; i <= maxX; i++)
	//		{
	//			a = new WalkAction(this, 2, RUN);
	//			_actionsQ.push(a);
	//			a = new ClimbAction(this, climbDist);
	//			_actionsQ.push(a);
	//			a = new JumpFromLadderAction(this, target, true, i, distY);
	//			_actionsQ.push(a);
	//			a = new DropAction(this);
	//			_actionsQ.push(a);
	//			a = new WalkAction(this, 1, RUN);
	//			_actionsQ.push(a);
	//		}
	//	}
	//	else
	//	{
	//		for (int i = minX; i <= maxX; i++)
	//		{
	//			a = new WalkAction(this, 2, RUN);
	//			_actionsQ.push(a);
	//			a = new ClimbAction(this, climbDist);
	//			_actionsQ.push(a);
	//			if (distY == -1 && i == 5)
	//			{
	//				a = new JumpFromLadderAction(this, LEDGE, true, i - 1, distY + 1);
	//				_actionsQ.push(a);
	//				a = new ClimbFromHangAction(this, xRIGHT);
	//				_actionsQ.push(a);
	//			}
	//			else
	//			{
	//				a = new JumpFromLadderAction(this, target, true, i, distY);
	//				_actionsQ.push(a);
	//			}

	//			a = new HangDropAction(this, xRIGHT);
	//			_actionsQ.push(a);
	//		}
	//	}

	//	if (target == LADDER)
	//	{
	//		if (distY == -1)
	//		{
	//			a = new WalkAction(this, -22, NORUN);
	//			_actionsQ.push(a);
	//		}
	//		else
	//		{
	//			a = new WalkAction(this, -30, NORUN);
	//			_actionsQ.push(a);
	//		}
	//	}
	//	else
	//	{
	//		a = new WalkAction(this, -30, NORUN);
	//		_actionsQ.push(a);
	//	}
	//}

	//y=-2
	//int maxX = 2;
	//for (int distX = 0; distX <= 2; distX++)
	//{
	//	a = new WalkAction(this, 2, RUN);
	//	_actionsQ.push(a);
	//	a = new ClimbAction(this, -1);
	//	_actionsQ.push(a);
	//	a = new JumpFromLadderAction(this, LEDGE, true, distX, -1);
	//	_actionsQ.push(a);
	//	a = new ClimbFromHangAction(this, xRIGHT);
	//	_actionsQ.push(a);
	//	a = new HangDropAction(this, xRIGHT);
	//	_actionsQ.push(a);
	//}


	//a = new WalkAction(this, RIGHT, RUN, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new ClimbAction(this, -1);
	//_actionsQ.push(a);
	//a = new JumpFromLadderAction(this, GROUND, true, 1, -1);
	//_actionsQ.push(a);
	//a = new HangDropAction(this, RIGHT, false);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, RUN, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new ClimbAction(this, -1);
	//_actionsQ.push(a);
	//a = new JumpFromLadderAction(this, GROUND, true, 2, -1);
	//_actionsQ.push(a);
	//a = new HangDropAction(this, RIGHT, false);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, RUN, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new ClimbAction(this, -1);
	//_actionsQ.push(a);
	//a = new JumpFromLadderAction(this, GROUND, true, 3, -1);
	//_actionsQ.push(a);
	//a = new HangDropAction(this, RIGHT, false);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, RUN, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new ClimbAction(this, -1);
	//_actionsQ.push(a);
	//a = new JumpFromLadderAction(this, GROUND, true, 4, -1);
	//_actionsQ.push(a);
	//a = new HangDropAction(this, RIGHT, false);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, RUN, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new ClimbAction(this, -1);
	//_actionsQ.push(a);
	//a = new JumpFromLadderAction(this, LEDGE, true, 4, 0);
	//_actionsQ.push(a);
	//a = new ClimbFromHangAction(this, RIGHT);
	//_actionsQ.push(a);
	//a = new HangDropAction(this, RIGHT, false);
	//_actionsQ.push(a);





	//high no momentum jump
	//a = new WalkAction(this, RIGHT, NORUN, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new ClimbAction(this, -7);
	//_actionsQ.push(a);
	//a = new JumpFromLadderAction(this, GROUND, false, 1, 5);
	//_actionsQ.push(a);
	//a = new HangDropAction(this, RIGHT, false);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, NORUN, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new ClimbAction(this, -7);
	//_actionsQ.push(a);
	//a = new JumpFromLadderAction(this, GROUND, false, 2, 5);
	//_actionsQ.push(a);
	//a = new HangDropAction(this, RIGHT, false);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, NORUN, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new ClimbAction(this, -7);
	//_actionsQ.push(a);
	//a = new JumpFromLadderAction(this, GROUND, false, 3, 5);
	//_actionsQ.push(a);
	////a = new ClimbFromHangAction(this, RIGHT);
	////_actionsQ.push(a);
	//a = new HangDropAction(this, RIGHT, false);
	//_actionsQ.push(a);



	//JUMPFROMLADDERTEST2
	//a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 6, 0);
	//_actionsQ.push(a);
	//a = new ClimbAction(this, -2);
	//_actionsQ.push(a);
	//a = new JumpFromLadderAction(this, GROUND, 2, 0);
	//_actionsQ.push(a);

	//JUMPFROMLADDERTEST1
	//a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new ClimbAction(this, -2);
	//_actionsQ.push(a);
	//a = new JumpFromLadderAction(this, GROUND, 1, -1);
	//_actionsQ.push(a);
	//a = new HangDropAction(this, LEFT, false);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, 5, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new ClimbAction(this, -2);
	//_actionsQ.push(a);
	//a = new JumpFromLadderAction(this, GROUND, 2, -1);
	//_actionsQ.push(a);
	//a = new HangDropAction(this, LEFT, false);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, 4, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new ClimbAction(this, -2);
	//_actionsQ.push(a);
	//a = new JumpFromLadderAction(this, LEDGE, 2, 0);
	//_actionsQ.push(a);
	//a = new ClimbFromHangAction(this, RIGHT);
	//_actionsQ.push(a);
	//a = new HangDropAction(this, RIGHT, false);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, 1, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new ClimbAction(this, -2);
	//_actionsQ.push(a);
	//a = new JumpFromLadderAction(this, GROUND, -2, -1);
	//_actionsQ.push(a);
	//a = new HangDropAction(this, RIGHT, false);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, 1, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new ClimbAction(this, -2);
	//_actionsQ.push(a);
	//a = new JumpFromLadderAction(this, LADDER, 3, 0);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);

	//a = new WalkAction(this, LEFT, 20, NODE_COORDS);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new ClimbAction(this, -4);
	//_actionsQ.push(a);
	//a = new JumpFromLadderAction(this, GROUND, 1, 1);
	//_actionsQ.push(a);
	//a = new HangDropAction(this, LEFT, false);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, 5, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new ClimbAction(this, -5);
	//_actionsQ.push(a);
	//a = new JumpFromLadderAction(this, GROUND, 2, 2);
	//_actionsQ.push(a);
	//a = new HangDropAction(this, LEFT, false);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, 4, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new ClimbAction(this, -3);
	//_actionsQ.push(a);
	//a = new JumpFromLadderAction(this, GROUND, 3, 0);
	//_actionsQ.push(a);
	//a = new HangDropAction(this, RIGHT, false);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, 1, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new ClimbAction(this, -4);
	//_actionsQ.push(a);
	//a = new JumpFromLadderAction(this, GROUND, -2, 2);
	//_actionsQ.push(a);
	//a = new HangDropAction(this, RIGHT, false);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, 1, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new ClimbAction(this, -5);
	//_actionsQ.push(a);
	//a = new JumpFromLadderAction(this, LADDER, 3, 2);
	//_actionsQ.push(a);

	//JUMPONLADDERTEST3
	//a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 7, 2);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);
	//a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 2, 4);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);
	//a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 6, 5);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);
	//a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 3, 3);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);
	//a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 5, 2);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);
	//a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 2, 2);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);

	//a = new WalkAction(this, LEFT, 3, NODE_COORDS);
	//_actionsQ.push(a);

	//JUMPONLADDERTEST2
	
	//a = new WalkAction(this, RIGHT, 1, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 2, 1);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);
	//a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 2, 2);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);
	//a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 3, 0);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);
	//a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 6, 0);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);
	//a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 4, 5);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);
	

	////JUMPONLADDERTEST1
	//jumpdist = 3
	//a = new JumpToLadderAction(this, 3, -1);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);
	//a = new WalkAction(this, LEFT, 3, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 3, -2);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, 4, NODE_COORDS);
	//_actionsQ.push(a);

	//a = new JumpToLadderAction(this, 3, -1);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);
	//a = new WalkAction(this, LEFT, 3, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 3, -2);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, 3, NODE_COORDS);
	//_actionsQ.push(a);

	//a = new JumpToLadderAction(this, 3, -2);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);
	//a = new WalkAction(this, LEFT, 3, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 3, -2);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);

	//a = new WalkAction(this, LEFT, RUN, 16, NODE_COORDS);
	//_actionsQ.push(a);
	//
	////jumpdist = 2
	//a = new WalkAction(this, RIGHT, 1, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 2, -1);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);
	//a = new WalkAction(this, LEFT, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 2, -2);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, 5, NODE_COORDS);
	//_actionsQ.push(a);

	//a = new JumpToLadderAction(this, 2, -1);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);
	//a = new WalkAction(this, LEFT, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 2, -2);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, 4, NODE_COORDS);
	//_actionsQ.push(a);

	//a = new JumpToLadderAction(this, 2, -2);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);
	//a = new WalkAction(this, LEFT, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 2, -2);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);

	//a = new WalkAction(this, LEFT, RUN, 16, NODE_COORDS);
	//_actionsQ.push(a);


	////jumpdist = 1
	//a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 1, -1);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);
	//a = new WalkAction(this, LEFT, 1, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 1, -2);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, 6, NODE_COORDS);
	//_actionsQ.push(a);

	//a = new JumpToLadderAction(this, 1, -1);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);
	//a = new WalkAction(this, LEFT, 1, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 1, -2);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);

	//a = new WalkAction(this, RIGHT, 5, NODE_COORDS);
	//_actionsQ.push(a);

	//a = new JumpToLadderAction(this, 1, -2);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);
	//a = new WalkAction(this, LEFT, RUN, 1, NODE_COORDS);
	//_actionsQ.push(a);
	//a = new JumpToLadderAction(this, 1, -2);
	//_actionsQ.push(a);
	//a = new DropAction(this);
	//_actionsQ.push(a);

	//a = new WalkAction(this, LEFT, RUN, 16, NODE_COORDS);
	//_actionsQ.push(a);




	//CLIMBFROMHANGTEST
	/*
	a = new WalkAction(this, RIGHT, false, 3, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, UP, LEDGE, 2, 1);
	_actionsQ.push(a);
	a = new ClimbFromHangAction(this, RIGHT);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, false, 4, NODE_COORDS);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, false, 4, NODE_COORDS);
	_actionsQ.push(a);
	a = new HangAction(this, LEFT);
	_actionsQ.push(a); 
	a = new DropAction(this);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, false, 1, NODE_COORDS);
	_actionsQ.push(a);
	*/
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
	a = new JumpAction(this, LEFT, true, 3, 5);
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
	//if (_botLogicThread.joinable())
	//	_botLogicThread.join();

	_botLogicState = EXIT;
	_botLogicThread.join();

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

bool DebugBot::IsPathToTargetSheduled(int x, int y)
{
	bool ok = false;

	for (int j = 0; j < _pathsQ.size(); j++)
	{
		if (_pathsQ[j].back().GetX() == x &&
			_pathsQ[j].back().GetY() == y)
		{
			ok = true;
			break;
		}
	}

	return ok;
}

std::vector<std::pair<moveTarget, std::vector<Node>>> DebugBot::CalculatePathsToReachableTargets(std::vector<moveTarget> targets)
{
	std::vector<std::pair<moveTarget, std::vector<Node>>> paths;

	Node start;
	if (_pathsQ.empty())
		start = Node((int)_playerPositionXNode, (int)_playerPositionYNode);
	else
		start = Node(_pathsQ.back().back().GetX(), _pathsQ.back().back().GetY()); //target of most recently added path is our start node

	for (int i = 0; i < targets.size(); i++)
	{
		if (_pathfinder->TryToCalculatePath(start.GetX(), start.GetY(), (int)targets[i].x, (int)targets[i].y, NODE_COORDS))
		{
			paths.push_back(std::make_pair(targets[i], _pathfinder->GetPathListNode()));
		}
		else
		{
			/*we skip unreachable targets*/
			continue;
		}
	}

	return paths;
}

std::pair<moveTarget, std::vector<Node>> DebugBot::SelectShortestPath(std::vector<std::pair<moveTarget, std::vector<Node>>> paths)
{
	int shortestIndex = 0;
	int currentLength;

	if (paths.size() > 0)
	{
		int shortestLength = _pathfinder->GetPathLength(paths[0].second);

		for (int i = 1; i < paths.size(); i++)
		{
			currentLength = _pathfinder->GetPathLength(paths[i].second);
			if (currentLength < shortestLength)
			{
				shortestIndex = i;
				shortestLength = currentLength;
			}
		}

		return paths[shortestIndex];
	}
	else
	{
		std::cout << "Unexpected behaviour - calculating shortest of 0 paths.";
		return std::pair<moveTarget, std::vector<Node>>();
	}

}

Node DebugBot::GetStartNodeForNextPath()
{
	Node start;
	if (_pathsQ.empty())
		start = Node((int)_playerPositionXNode, (int)_playerPositionYNode);
	else
		start = Node(_pathsQ.back().back().GetX(), _pathsQ.back().back().GetY()); //target of most recently added path is our start node

	return start;
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


#pragma region heurestic CreateCommands (without calculating neighbours)
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

#pragma region CreateCommands (with calculating neighbours)
//
//void DebugBot::CreateCommands(std::vector<Node> path)
//{
//	int currX, currY, targetX, targetY, distX, distY;
//	MOVEMENTACTION prevAction = IDLE;
//	currX = (int)_playerPositionXNode;
//	currY = (int)_playerPositionYNode;
//	MVSTATE mvState;
//
//	for (int i = 0; i < path.size(); i++)
//	{
//		targetX = path[i].x;
//		targetY = path[i].y;
//		distX = abs(targetX - currX);
//		distY = abs(targetY - currY);
//
//		if (currX == (int)_playerPositionXNode && currY == (int)_playerPositionYNode)
//			mvState = _pathfinder->ToMvState(GetSpelunkerState());
//		else
//		{
//			if (prevAction == HANGRIGHT ||
//				prevAction == HANGLEFT ||
//				prevAction == JUMPUPRIGHT_LEDGE ||
//				prevAction == JUMPUPLEFT_LEDGE)
//				mvState = mvHANGING;
//			else
//				mvState = mvSTANDING;
//		}
//
//
//		std::vector<Node> neighbours = _pathfinder->CalculateNeighboursList(Node{ currX, currY }, mvState);
//		
//
//		std::vector<MOVEMENTACTION> actionsToReachTarget;
//
//		for (int i = 0; i < neighbours.size(); i++)
//			if (neighbours[i].x == targetX && neighbours[i].y == targetY)
//				actionsToReachTarget.push_back((MOVEMENTACTION)neighbours[i].actionToReach);
//
//		if (actionsToReachTarget.empty())
//		{
//			std::cout << "Error! Couldn't create commands.";
//			return;
//		}
//
//
//		//take first action that allows to reach target
//		MOVEMENTACTION action = actionsToReachTarget[0];
//
//		AddActionToActionQueue(action, distX, distY);
//
//
//		currX = targetX;
//		currY = targetY;
//		prevAction = action;
//	}
//}
#pragma endregion

void DebugBot::CreateCommands(std::vector<Node> path)
{
	int currX, currY, targetX, targetY, distX, distY;
	MOVEMENTACTION prevAction = MOVEMENTACTION::IDLE;
	currX = (int)_playerPositionXNode;
	currY = (int)_playerPositionYNode;
	//MapSearchNode *start = _pathfinder->GetNodeFromGrid(currX, currY);
	MVSTATE mvState;
	ACTION_TARGET jumpTarget;

	for (int i = 0; i < path.size(); i++)
	{
		//first node in a path is start node
		if (i == 0) continue;

		targetX = path[i].GetX();
		targetY = path[i].GetY();
		//distX = abs(targetX - currX);
		//distY = abs(targetY - currY);
		distX = targetX - currX;
		distY = targetY - currY;

		MOVEMENTACTION action = path[i].GetActionToReach();

		jumpTarget = path[i].GetActionTarget();
		
		//if (i == 0) mvState = start->GetMvState();
		//else mvState = path[i-1].GetMvState();
		mvState = path[i - 1].GetMvState();

		AddActionToActionQueue(action, jumpTarget, mvState, distX, distY);


		currX = targetX;
		currY = targetY;
		prevAction = action;
	}
}

void DebugBot::AddActionToActionQueue(MOVEMENTACTION action, ACTION_TARGET jumpTarget, MVSTATE mvState, int distX, int distY)
{
	DIRECTIONX directionX;
	DIRECTIONY directionY;

	if (distX > 0) directionX = xRIGHT;
	else if (distX == 0) directionX = xNONE;
	else directionX = xLEFT;

	if (distY > 0) directionY = yDOWN;
	else if (distY == 0) directionY = yNONE;
	else directionY = yUP;

	switch (action)
	{
	case IDLE:
		break;
	case CENTRALIZE:
		_actionsQ.push(new CentralizeAction(this));
		break;
	case WALK:
		//if (distX == 2), then bot is walking over a hole which should be runned
		if (abs(distX) == 2)
		{
			_actionsQ.push(new WalkAction(this, distX, RUN));
			break;
		}

		if (!_actionsQ.empty())
		{
			if (_actionsQ.back()->ActionType() == WALK &&
				((distX > 0 && _actionsQ.back()->GetDirectionX() == xRIGHT) ||
				(distX < 0 && _actionsQ.back()->GetDirectionX() == xLEFT)))
			{
				//previous action is also a WalkAction, and its going in the same direction,
				//so we increase its distance instead of creating a new command (ActionBatching™)
				dynamic_cast<WalkAction*>(_actionsQ.back())->AddDistance(distX);
				break;
			}
		}

		_actionsQ.push(new WalkAction(this, distX));

		break;
	case HANG:
		_actionsQ.push(new HangAction(this, directionX));
		break;
	case DROP:
		_actionsQ.push(new DropAction(this, jumpTarget, distY));
		break;
	case HANGDROP:
		_actionsQ.push(new HangDropAction(this, directionX));
		break;
	case WALKUP:
		_actionsQ.push(new WalkUpAction(this, directionX));
		break;
	case CLIMBFROMHANG:
		_actionsQ.push(new ClimbFromHangAction(this, directionX));
		break;
	case CLIMB:
		if (!_actionsQ.empty())
		{
			if (_actionsQ.back()->ActionType() == CLIMB &&
				((distY > 0 && _actionsQ.back()->GetDirectionY() == yDOWN) ||
				(distY < 0 && _actionsQ.back()->GetDirectionY() == yUP)))
			{
				//previous action is also a ClimbAction, and its going in the same direction,
				//so we increase its distance instead of creating a new command (ActionBatching™)
				dynamic_cast<ClimbAction*>(_actionsQ.back())->AddDistance(distY);
				break;
			}
		}

		_actionsQ.push(new ClimbAction(this, distY));
		break;
	case JUMPABOVERIGHT:
		_actionsQ.push(new JumpAboveAction(this, xRIGHT));
		break;
	case JUMPABOVELEFT:
		_actionsQ.push(new JumpAboveAction(this, xLEFT));
		break;
	case JUMP:
		_actionsQ.push(new JumpAction(this, jumpTarget, distX, distY));
		break;
	case JUMPFROMLADDER:
		if (mvState == mvCLIMBING_WITH_MOMENTUM)
			_actionsQ.push(new JumpFromLadderAction(this, jumpTarget, true, distX, distY));
		else if (mvState == mvCLIMBING)
			_actionsQ.push(new JumpFromLadderAction(this, jumpTarget, false, distX, distY));
		else
			std::cout << "Error: MVSTATE is not CLIMBING when creating JUMPFROMLADDER" << std::endl;
		break;
	case WALKOFFLEDGE:
		//_actionsQ.push(new WalkOffLedgeAction(this, distX, distY, jumpTarget));
		_actionsQ.push(new WalkOffLedgeAction(this, distX, distY));
		break;
	default:
		break;
	}
}



void DebugBot::TryToFindExit()
{
	for (int nodeX = 0; nodeX < X_NODES; nodeX += 1)
	{
		for (int nodeY = 0; nodeY < Y_NODES; nodeY += 1)
		{
			if (GetNodeState(nodeX, nodeY, NODE_COORDS) == spExit)
			{
				_exit = Node(nodeX, nodeY);
				_exitFound = true;
				return;
			}
		}
	}
}


//void DebugBot::BotLogic()
//{
//	_botLogicInProgress = true;
//
//	Sleep(3000);
//	std::cout << "BotLogic Done, id: " << _botLogicThread.get_id() << std::endl;
//
//	_botLogicInProgress = false;
//}

//void DebugBot::BotLogic2()
//{
//	_botLogicInProgress = true;
//
//	int exitX, exitY;
//	int playerPosXNode = (int)_playerPositionXNode;
//	int playerPosYNode = (int)_playerPositionYNode;
//	std::vector<Node> explTargets;
//	bool exploration;
//
//	switch (_primState)
//	{
//	case pIDLE:
//		//IF there is time THEN
//		//IF there is treasure THEN
//		//		_primState = GATHERING
//		//	ELSE 
//		//		_primState = EXPLORING
//		//ELSE
//		//	_primState = GOING_TO_EXIT
//
//		_primState = EXPLORING_GATHERING;
//
//		break;
//	case GATHERING:
//
//
//		if (_collectablesQ.empty())
//		{
//			auto collectablesOM = _objectManager->GetCollectables();
//
//			if (collectablesOM.empty())
//				exploration = true;
//			else
//			{
//				for (int i = 0; i < collectablesOM.size(); i++)
//					_collectablesQ.push(collectablesOM[i]);
//			}
//		}
//		break;
//	case EXPLORING_GATHERING:
//		bool exploration;
//		collectableObject collectCandidate;
//
//
//		exploration = false;
//
//		if (_collectablesQ.empty())
//		{
//			auto collectablesOM = _objectManager->GetCollectables();
//
//			if (collectablesOM.empty())
//				exploration = true;
//			else
//			{
//				for (int i = 0; i < collectablesOM.size(); i++)
//					_collectablesQ.push(collectablesOM[i]);
//			}
//		}
//		else
//		{
//			while (!_collectablesQ.empty())
//			{
//				collectCandidate = _collectablesQ.front();
//				_collectablesQ.pop();
//
//				//check if collect candidate is marked as unreachable
//				if (_unreachableCollectables.find(collectCandidate.id) != _unreachableCollectables.end()
//					&& _unreachableCollectables[collectCandidate.id] == true)
//				{
//					//if Q is empty, all collectables are unreachable. We should explore.
//					if (_collectablesQ.empty())
//						exploration = true;
//					else
//						continue;
//				}
//				else
//				{
//					//if candidate not marked as unreachable, try to calculate a path to it
//					if (_pathfinder->TryToCalculatePath(_playerPositionXNode, _playerPositionYNode, collectCandidate.x, collectCandidate.y, NODE_COORDS))
//					{
//						//if there is a path, execute it
//						//CreateCommands(_pathfinder->GetPathListNode());
//						//_secState = EXECUTING_COMMANDS;
//						//_secState = CREATING_COMMANDS;
//
//						//break the loop
//						break;
//					}
//					else
//					{
//						//if there is no path, mark the candidate as unreachable
//						_unreachableCollectables[collectCandidate.id] = true;
//					}
//				}
//			}
//		}
//
//
//		if (exploration)
//		{
//			explTargets = _pathfinder->FindExplorationTargets(playerPosXNode, playerPosYNode, NODE_COORDS);
//
//			if (explTargets.size() > 0)
//			{
//				_target = &explTargets[0];
//				int dist = abs((int)_playerPositionXNode - _target->GetX()) + abs((int)_playerPositionYNode - _target->GetY());
//
//				for (int i = 1; i < explTargets.size(); i++)
//				{
//					int distNew = abs((int)_playerPositionXNode - explTargets[i].GetX()) + abs((int)_playerPositionYNode - explTargets[i].GetY());
//					if (distNew < dist)
//					{
//						_target = &explTargets[i];
//						dist = distNew;
//					}
//
//				}
//
//				//_secState = NEW_TARGET;
//				if (_pathfinder->TryToCalculatePath(_playerPositionXNode, _playerPositionYNode, _target->GetX(), _target->GetY(), NODE_COORDS))
//				{
//					//CreateCommands(_pathfinder->GetPathListNode());
//					//_secState = EXECUTING_COMMANDS;
//					_secState = CREATING_COMMANDS;
//				}
//				else
//				{
//					std::cout << "Unexpected behaviour: Exploration target unreachable while Exploring-Gathering.";
//					_secState = UNREACHABLE_TARGET;
//				}
//
//				//delete unreachable markings - something may be reachable now
//				std::map<int, bool>::iterator uc = _unreachableCollectables.begin();
//				while (uc != _unreachableCollectables.end())
//				{
//					uc->second = false;
//					uc++;
//				}
//
//			}
//			else
//			{
//				//nothing to explore - go to exit
//				_primState = SEARCHING_FOR_EXIT;
//			}
//		}
//
//		break;
//	case SEARCHING_FOR_EXIT:
//		//1. search for exit among not-fogged nodes
//		//2. if you found exit, go to it.
//		//3. if you did not find exit, explore a node and goto 1.
//		//4. if there are no nodes to explore, Idle.
//
//
//		_target = TryToFindExit();
//		if (_target != NULL)
//		{
//			_primState = GOING_TO_EXIT;
//			break;
//		}
//
//		//dfs
//		explTargets = _pathfinder->FindExplorationTargets(playerPosXNode, playerPosYNode, NODE_COORDS);
//
//		if (explTargets.size() > 0)
//		{
//			_target = &explTargets[0];
//			int dist = abs((int)_playerPositionXNode - _target->GetX()) + abs((int)_playerPositionYNode - _target->GetY());
//
//			for (int i = 1; i < explTargets.size(); i++)
//			{
//				int distNew = abs((int)_playerPositionXNode - explTargets[i].GetX()) + abs((int)_playerPositionYNode - explTargets[i].GetY());
//				if (distNew < dist)
//				{
//					_target = &explTargets[i];
//					dist = distNew;
//				}
//
//			}
//
//			//_secState = NEW_TARGET;
//			if (_pathfinder->TryToCalculatePath(_playerPositionXNode, _playerPositionYNode, _target->GetX(), _target->GetY(), NODE_COORDS))
//			{
//				//CreateCommands(_pathfinder->GetPathListNode());
//				//_secState = EXECUTING_COMMANDS;
//				_secState = CREATING_COMMANDS;
//			}
//			else
//			{
//				std::cout << "Unexpected behaviour: Exploration target unreachable while Searching For Exit.";
//				_secState = UNREACHABLE_TARGET;
//			}
//		}
//		else
//		{
//			//no exploration target - we can search again with looser criteria, or allow usage of ropes and bombs
//			_primState = pIDLE;
//		}
//
//		break;
//	case GOING_TO_EXIT:
//		//1. go to exit.
//		//2. if exit is not yet found, search for it.
//
//		if (_target != NULL)
//		{
//			//_secState = NEW_TARGET;
//			if (_pathfinder->TryToCalculatePath(_playerPositionXNode, _playerPositionYNode, _target->GetX(), _target->GetY(), NODE_COORDS))
//			{
//				//CreateCommands(_pathfinder->GetPathListNode());
//				//_secState = EXECUTING_COMMANDS;
//				_secState = CREATING_COMMANDS;
//			}
//			else
//				_secState = UNREACHABLE_TARGET;
//		}
//		else
//		{
//			std::cout << "Unexpected behaviour: target is NULL when going to exit." << std::endl;
//			_primState = SEARCHING_FOR_EXIT;
//		}
//
//		break;
//	default:
//		break;
//	}
//
//
//	_botLogicInProgress = false;
//}

//void DebugBot::BotLogic2()
//{
//	using namespace std::chrono_literals;
//
//	while (true)
//	{
//		switch (_botLogicState)
//		{
//		case blIDLE:
//			std::this_thread::sleep_for(100ms);
//			break;
//		case GATHERING:
//		{
//
//			if (_waitTimer > 0)
//			{
//				std::this_thread::sleep_for(100ms);
//				_waitTimer -= 1;
//			}
//			else
//			{
//				_pathfinder->CalculateConnectedComponents();
//				_pathfinder->SCCDebug();
//
//				std::vector<collectableObject> collectablesList = _objectManager->GetCollectables();
//				std::vector<moveTarget> moveTargets;
//
//				for (int i = 0; i < collectablesList.size(); i++)
//				{
//					moveTarget t = moveTarget((int)collectablesList[i].x, (int)collectablesList[i].y);
//					if (!IsPathToTargetSheduled(t)) moveTargets.push_back(t);
//				}
//
//				std::vector<std::pair<moveTarget, std::vector<Node>>> paths = CalculatePathsToReachableTargets(moveTargets);
//
//
//				if (paths.size() > 0)
//				{
//					std::pair<moveTarget, std::vector<Node>> shortest = SelectShortestPath(paths);
//					_pathsQ.push_back(shortest);
//				}
//
//
//				_waitTimer = 10;
//			}
//			break;
//		}
//		case EXIT:
//			return;
//		}
//	}
//}

void DebugBot::BotLogicWaiting()
{
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(100ms);

	switch (_botLogicState)
	{
		//case DebugBot::IDLE:
		//	break;
		//case DebugBot::EXIT:
		//	break;
	case DebugBot::GATHER_FROM_CC:
		_waitTimer--;
		if (_waitTimer == 0 || _pathsQ.empty()) _botLogicWaiting = false;
		break;
	case DebugBot::EXPLORE_CC:
		_waitTimer--;
		if (_waitTimer == 0 || _pathsQ.empty()) _botLogicWaiting = false;
		break;
	case DebugBot::PICK_TARGET_IN_NEXT_CC:
		break;
	case DebugBot::MOVE_TO_NEXT_CC:
		//na razie bez timera, ale w sumie nie zaszkodzi³oby jakby by³
		if (_pathsQ.empty()) _botLogicWaiting = false;
		break;
	case DebugBot::SEARCH_FOR_EXIT:
		break;
	case DebugBot::GO_TO_EXIT:
		if ((int)_playerPositionXNode == _exit.GetX() &&
			(int)_playerPositionYNode == _exit.GetY())
			_botLogicWaiting = false;
		break;
		//case DebugBot::EXIT_REACHED:
		//	break;
		//case DebugBot::UNREACHABLE_EXIT:
		//	break;
		//case DebugBot::NO_EXIT_EXPLORE:
		//	break;
		//case DebugBot::NO_EXIT_ERROR:
		//	break;
	default:
		_waitTimer--;
		if (_waitTimer == 0) _botLogicWaiting = false;
		break;
	}


}

void DebugBot::BotLogic()
{
	using namespace std::chrono_literals;
	int currentCCnr;

	while (true)
	{
		BotLogicStateDebug();
		if (!_exitFound)
			TryToFindExit();


		if (_botLogicWaiting)
		{
			BotLogicWaiting();
			continue;
		}


		//czy to potrzebne? mo¿e jednak mo¿na liczyæ za ka¿dym razem?
		if (_botLogicState == GATHER_FROM_CC || _botLogicState == EXPLORE_CC)
		{
			_pathfinder->CalculateConnectedComponents();
			_pathfinder->SCCDebug();
			currentCCnr = _pathfinder->GetCCnr((int)_playerPositionXNode, (int)_playerPositionYNode);
		}


		switch (_botLogicState)
		{
		case IDLE:
			std::this_thread::sleep_for(100ms);
			break;
		case GATHER_FROM_CC:
		{
			std::vector<collectableObject> collectablesList = _objectManager->GetCollectables();
			collectableObject c;

			//mo¿na potem dodawaæ do kolejki zaczynaj¹c od najkrótszej œcie¿ki
			//tylko to wymaga przeliczania œcie¿ek do mo¿liwych celów za ka¿dy cel który chcemy dodaæ
			for (int i = 0; i < collectablesList.size(); i++)
			{
				c = collectablesList[i];
				if (_pathfinder->GetCCnr(c.x, c.y) == currentCCnr &&
					!IsPathToTargetSheduled(c.x, c.y))
				{
					Node start = GetStartNodeForNextPath();
					if (_pathfinder->TryToCalculatePath(start.GetX(), start.GetY(), c.x, c.y, NODE_COORDS))
						_pathsQ.push_back(_pathfinder->GetPathListNode());
				}
			}

			if (!_pathsQ.empty())
			{
				_waitTimer = 10;
				_botLogicWaiting = true;
			}
			else
				_botLogicState = EXPLORE_CC;

			break;
		}
		case EXPLORE_CC:
		{
			std::vector<collectableObject> collectablesList = _objectManager->GetCollectables();
			collectableObject c;
			bool collectableFound = false;
			for (int i = 0; i < collectablesList.size(); i++)
			{
				c = collectablesList[i];
				if (_pathfinder->GetCCnr(c.x, c.y) == currentCCnr &&
					!IsPathToTargetSheduled(c.x, c.y))
				{
					collectableFound = true;
					break;
				}
			}
			if (collectableFound)
			{
				//mo¿na tu dodaæ przerywanie eksploracji
				//na razie wydaje sie to problematyczne, a korzyœci daje znikome
				//_breakExploration = true;
				_botLogicState = GATHER_FROM_CC;
				break;
			}

			if (_pathsQ.empty())
			{
				std::vector<MapSearchNode*> nodes = _pathfinder->GetAllNodesFromCC(currentCCnr);
				MapSearchNode * e;
				bool exploratonTargetFound = false;
				for (int i = 0; i < nodes.size(); i++)
				{
					if (_pathfinder->isCloseToFog(nodes[i], 5))
					{
						e = nodes[i];
						exploratonTargetFound = true;
						break;
					}
				}

				if (exploratonTargetFound)
				{
					Node start = GetStartNodeForNextPath();
					if (_pathfinder->TryToCalculatePath(start.GetX(), start.GetY(), e->GetX(), e->GetY(), NODE_COORDS))
						_pathsQ.push_back(_pathfinder->GetPathListNode());
				}
				else
				{
					_botLogicState = PICK_TARGET_IN_NEXT_CC;
				}
			}
			else
			{
				_waitTimer = 10;
				_botLogicWaiting = true;
			}


			break;
		}
		case PICK_TARGET_IN_NEXT_CC:
		{
			bool targetFound = false;
			for each (collectableObject c in _objectManager->GetCollectables())
			{
				if (_pathfinder->TryToCalculatePath((int)_playerPositionXNode, (int)_playerPositionYNode, c.x, c.y, NODE_COORDS))
				{
					targetFound = true;
					_pathsQ.push_back(_pathfinder->GetPathListNode());
					_botLogicState = MOVE_TO_NEXT_CC;
					break;
				}
			}
			if (targetFound) break;

			Node e;
			if (_pathfinder->TryToFindExplorationTarget((int)_playerPositionXNode, (int)_playerPositionYNode))
			{
				e = _pathfinder->GetExplorationTarget();
				if (_pathfinder->TryToCalculatePath((int)_playerPositionXNode, (int)_playerPositionYNode, e.GetX(), e.GetY(), NODE_COORDS))
				{
					targetFound = true;
					_pathsQ.push_back(_pathfinder->GetPathListNode());
					_botLogicState = MOVE_TO_NEXT_CC;
					break;
				}
			}

			if (targetFound) break;

			_botLogicState = SEARCH_FOR_EXIT;

			break;
		}
		case MOVE_TO_NEXT_CC:
			if (_pathsQ.empty())
				_botLogicState = GATHER_FROM_CC;
			else
			{
				_botLogicWaiting = true;
			}
			break;
		case SEARCH_FOR_EXIT:
			if (_exitFound)
			{
				if (_pathfinder->TryToCalculatePath((int)_playerPositionXNode, (int)_playerPositionYNode, _exit.GetX(), _exit.GetY(), NODE_COORDS))
				{
					_pathsQ.push_back(_pathfinder->GetPathListNode());
					_botLogicState = GO_TO_EXIT;
				}
				else
					_botLogicState = UNREACHABLE_EXIT;
			}
			else
			{
				if (_pathfinder->IsFogOnMap())
					_botLogicState = NO_EXIT_EXPLORE;
				else
					_botLogicState = NO_EXIT_ERROR;
			}

			break;
		case GO_TO_EXIT:
			if ((int)_playerPositionXNode == _exit.GetX() &&
				(int)_playerPositionYNode == _exit.GetY())
				_botLogicState = EXIT_REACHED;
			else
			{
				_botLogicWaiting = true;
			}
			break;
		case EXIT_REACHED:
			//mo¿na wcisn¹æ strza³kê w górê albo coœ
			std::this_thread::sleep_for(100ms);
			break;
		case UNREACHABLE_EXIT:
			std::this_thread::sleep_for(100ms);
			break;
		case NO_EXIT_EXPLORE:
			std::this_thread::sleep_for(100ms);
			break;
		case NO_EXIT_ERROR:
			std::this_thread::sleep_for(100ms);
			break;
		case EXIT:
			return;
		}
	}
}

void DebugBot::Update()
{
	//UPDATING INFO ABOUT ENEMIES AND COLLECTABLES
	_objectManager->UpdateGameObjectLists();

	//PRINTING DEBUG INFO TO FILES
	if (_debugTimer < 0)
	{
		_hasMomentum = false;
		_pathfinder->NeighboursDebug((int)_playerPositionXNode, (int)_playerPositionYNode);
		_debugTimer = 10;
	}
	_debugTimer -= 1;

	_objectManager->CollectablesDebug();
	_objectManager->EnemiesDebug();

	switch (_secState)
	{
	case DebugBot::sIDLE:
		break;
	case DebugBot::EXECUTING_COMMANDS:
		if (!_actionsQ.empty())
		{
			ordersStruct orders = (_actionsQ.front())->GetOrders();

			ExecuteOrders(orders);

			if ((_actionsQ.front())->ActionDone())
			{
				delete _actionsQ.front();
				_actionsQ.pop();
				ClearOrders();
			
				//exploration breaking
				//if (_pathsQ.front().first.exploration && _breakExploration)
				//{
				//	while (!_actionsQ.empty())
				//	{
				//		delete _actionsQ.front();
				//		_actionsQ.pop();
				//	}
				//	_pathsQ.pop_front();
				//	
				//	_breakExploration = false;
				//	_secState = WAITING_FOR_PATH;
				//}
			}
		}
		else
		{
			_pathsQ.pop_front();
			_secState = WAITING_FOR_PATH;
		}
		break;
	case DebugBot::WAITING_FOR_PATH:
		if (!_pathsQ.empty())
		{
			//CreateCommands(_pathsQ.front().second);
			CreateCommands(_pathsQ.front());
			//_pathsQ.pop_front();
			_secState = EXECUTING_COMMANDS;
		}
		break;
	case DebugBot::DEBUG:
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

			//for jump from ladder with momentum testing
			//if (!_actionsQ.empty())
			//{
			//	if (_actionsQ.front()->ActionType() == WALK ||
			//		_actionsQ.front()->ActionType() == CLIMB)
			//		_runp = true;
			//}
		}
	default:
		break;
	}


#pragma region stare

	//switch (_state)
	//{
	//	case SEARCHING_FOR_EXIT:
	//		if (FindExit(exitX, exitY) && _pathfinder->TryToCalculatePath(_playerPositionXNode, _playerPositionYNode, exitX, exitY, NODE_COORDS))
	//		{
	//			CreateCommands(_pathfinder->GetPathListNode());
	//		}
	//		else
	//		{
	//			//dfs
	//			std::vector<Node> explTargets = _pathfinder->FindExplorationTargets(_playerPositionXNode, _playerPositionYNode, NODE_COORDS);

	//			if (explTargets.size() > 0)
	//			{
	//				Node target = explTargets[0];
	//				int dist = abs((int)_playerPositionXNode - target.GetX()) + abs((int)_playerPositionYNode - target.GetY());

	//				for (int i = 1; i < explTargets.size(); i++)
	//				{
	//					int distNew = abs((int)_playerPositionXNode - explTargets[i].GetX()) + abs((int)_playerPositionYNode - explTargets[i].GetY());
	//					if (distNew < dist)
	//					{
	//						target = explTargets[i];
	//						dist = distNew;
	//					}

	//				}
	//				
	//				_pathfinder->TryToCalculatePath(_playerPositionXNode, _playerPositionYNode, target.GetX(), target.GetY(), NODE_COORDS);
	//				CreateCommands(_pathfinder->GetPathListNode());

	//			}
	//			else
	//			{
	//				//no exploration target - we can search again with looser criteria, or allow usage of ropes and bombs
	//			}
	//			

	//		}

	//		_state = EXECUTING_COMMANDS;
	//		break;

	//	case EXECUTING_COMMANDS:
	//		if (!_actionsQ.empty())
	//		{
	//			ordersStruct orders = (_actionsQ.front())->GetOrders();




	//			ExecuteOrders(orders);

	//			if ((_actionsQ.front())->ActionDone())
	//			{
	//				delete _actionsQ.front();
	//				_actionsQ.pop();
	//				ClearOrders();
	//			}
	//		}
	//		else
	//		{
	//			ClearOrders();
	//			//if (_hasGoal) _lookUp = true;
	//			if (GetNodeState(_playerPositionX, _playerPositionY, PIXEL_COORDS) == spExit)
	//				_lookUp = true;
	//			else 
	//				_state = SEARCHING_FOR_EXIT;
	//		}

	//		break;

	//	case DEBUG:
	//		if (!_actionsQ.empty())
	//		{
	//			ordersStruct orders = (_actionsQ.front())->GetOrders();



	//			ExecuteOrders(orders);
	//			



	//			if ((_actionsQ.front())->ActionDone())
	//			{
	//				delete _actionsQ.front();
	//				_actionsQ.pop();
	//				ClearOrders();
	//			}

	//			//for jump from ladder with momentum testing
	//			//if (!_actionsQ.empty())
	//			//{
	//			//	if (_actionsQ.front()->ActionType() == WALK ||
	//			//		_actionsQ.front()->ActionType() == CLIMB)
	//			//		_runp = true;
	//			//}
	//		}
	//}
	
#pragma endregion
	
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