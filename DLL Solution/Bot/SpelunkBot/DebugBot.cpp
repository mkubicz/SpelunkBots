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

#pragma region Constructors and initialization

DebugBot::DebugBot()
{
	_pathfinder = new Pathfinder(this);
	_objectManager = new ObjectManager(this);
	NewLevel();
}

void DebugBot::InitialiseBotLogicState()
{	
	_botLogicState = GATHER_FROM_CC;
	_secState = WAITING_FOR_PATH;

	//for manual movementaction tests
	//_botLogicState = IDLE;
	//_secState = DEBUG;

	//manual action creation goes here
	//IMovementAction *a;
	//a = new WalkAction(this, 5);
	//_actionsQ.push(a);
	//a = new HangAction(this, xRIGHT);
	//_actionsQ.push(a);
	//a = new DropAction(this, 4);
	//_actionsQ.push(a);


}	

void DebugBot::NewLevel()
{
	//terminate botlogic thread from previous level
	_botLogicState = EXIT;
	if (_botLogicThread.joinable())
		_botLogicThread.join();

	//delete any leftover info from previous level 
	_pathsQ.clear();
	ClearOrders();
	while (!_actionsQ.empty())
	{
		delete _actionsQ.front();
		_actionsQ.pop();
	}

	//initialize variables and start new botlogic thread
	InitialiseVariables();
	_pathfinder->InitializeGrid();

	InitialiseBotLogicState();
	_botLogicThread = std::thread(&DebugBot::BotLogic, this);
}

DebugBot::~DebugBot()
{
	_botLogicState = EXIT;
	_botLogicThread.join();

	while (!_actionsQ.empty())
	{
		delete _actionsQ.front();
		_actionsQ.pop();
	}
	delete _pathfinder;
}

#pragma endregion

#pragma region pathQ methods

Node DebugBot::GetStartNodeForNextPath()
{
	Node start;
	if (_pathsQ.empty())
		start = Node((int)_playerPositionXNode, (int)_playerPositionYNode);
	else
		start = Node(_pathsQ.back().back().GetX(), _pathsQ.back().back().GetY()); //target of most recently added path is our start node

	return start;
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

#pragma endregion

#pragma region actionsQ methods

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

void DebugBot::CreateCommands(std::vector<Node> path)
{
	int currX, currY, targetX, targetY, distX, distY;
	MOVEMENTACTION prevAction = MOVEMENTACTION::IDLE;
	currX = (int)_playerPositionXNode;
	currY = (int)_playerPositionYNode;
	MVSTATE mvState;
	ACTION_TARGET actionTarget;
	MOVEMENTACTION action;

	for (int i = 0; i < path.size(); i++)
	{
		//first node in a path is start node
		if (i == 0) continue;

		targetX = path[i].GetX();
		targetY = path[i].GetY();
		distX = targetX - currX;
		distY = targetY - currY;

		action = path[i].GetActionToReach();
		actionTarget = path[i].GetActionTarget();
		mvState = path[i - 1].GetMvState();

		AddActionToActionQueue(action, actionTarget, mvState, distX, distY);

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
		//TODO: walkoffledge to ladders
		//_actionsQ.push(new WalkOffLedgeAction(this, distX, distY, jumpTarget));
		_actionsQ.push(new WalkOffLedgeAction(this, distX, distY));
		break;
	default:
		break;
	}
}

#pragma endregion

#pragma region Bot logic methods

void DebugBot::BotLogicStateDebug()
{
	std::ofstream fileStream;
	fileStream.open("botlogicstate.txt");

	fileStream << BotLogicStrings[_botLogicState] << std::endl;

	fileStream.close();
}

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
		if ((int)_playerPositionXNode == _pathfinder->GetExit().GetX() &&
			(int)_playerPositionYNode == _pathfinder->GetExit().GetY())
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
		if (_botLogicState == EXIT) return;

		BotLogicStateDebug();
		if (!_pathfinder->IsExitFound())
			_pathfinder->TryToFindExit();

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
					if (_pathfinder->TryToCalculatePath(start.GetX(), start.GetY(), c.x, c.y))
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
					if (_pathfinder->TryToCalculatePath(start.GetX(), start.GetY(), e->GetX(), e->GetY()))
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
				if (_pathfinder->TryToCalculatePath((int)_playerPositionXNode, (int)_playerPositionYNode, c.x, c.y))
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
				if (_pathfinder->TryToCalculatePath((int)_playerPositionXNode, (int)_playerPositionYNode, e.GetX(), e.GetY()))
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
			if (_pathfinder->IsExitFound())
			{
				if (_pathfinder->TryToCalculatePath((int)_playerPositionXNode, (int)_playerPositionYNode, _pathfinder->GetExit().GetX(), _pathfinder->GetExit().GetY()))
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
			if ((int)_playerPositionXNode == _pathfinder->GetExit().GetX() &&
				(int)_playerPositionYNode == _pathfinder->GetExit().GetY())
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

#pragma endregion

void DebugBot::Update()
{
	//UPDATING INFO ABOUT ENEMIES AND COLLECTABLES
	_objectManager->UpdateGameObjectLists();

	//PRINTING DEBUG INFO TO FILES
	if (_debugTimer < 0)
	{
		bool hasMomentum = false;
		_pathfinder->NeighboursDebug((int)_playerPositionXNode, (int)_playerPositionYNode, hasMomentum);
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
			CreateCommands(_pathsQ.front());
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

}
