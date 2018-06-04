#include "stdafx.h"
#include "DebugBot.h"


#pragma region Constructors and initialization

DebugBot::DebugBot()
{
	_pathfinder = new Pathfinder(this);
	_objectManager = new ObjectManager(this, _pathfinder);
	_pathScheduler = new PathScheduler(this, _pathfinder);
	NewLevel();
}

void DebugBot::InitialiseBotLogicState()
{	
	_botLogicState = START;
	_secState = WAITING_FOR_PATH;
	_botLogicWaiting = false;
	_waitTimer = 0;
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
	//_pathsQ.clear();
	//ClearOrders();
	//while (!_actionsQ.empty())
	//{
	//	delete _actionsQ.front();
	//	_actionsQ.pop();
	//}

	//initialize variables and start new botlogic thread
	InitialiseVariables();
	_pathfinder->NewLevel();
	_objectManager->NewLevel();
	_pathScheduler->NewLevel();

	InitialiseBotLogicState();
	_botLogicThread = std::thread(&DebugBot::BotLogic, this);
}

DebugBot::~DebugBot()
{
	_botLogicState = EXIT;
	_botLogicThread.join();

	//while (!_actionsQ.empty())
	//{
	//	delete _actionsQ.front();
	//	_actionsQ.pop();
	//}
	delete _pathfinder;
	delete _objectManager;
	delete _pathScheduler;
}

#pragma endregion

#pragma region pathQ 


//
//bool DebugBot::IsPathToTargetSheduled(int x, int y)
//{
//	bool ok = false;
//
//	for (int j = 0; j < _pathsQ.size(); j++)
//	{
//		if (_pathsQ[j].back()->GetX() == x &&
//			_pathsQ[j].back()->GetY() == y)
//		{
//			ok = true;
//			break;
//		}
//	}
//
//	return ok;
//}

#pragma endregion

#pragma region actionsQ

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
//
//void DebugBot::CreateCommands(std::vector<MapNode*> path)
//{
//	int currX, currY, targetX, targetY, distX, distY;
//	MOVEMENTACTION prevAction = MOVEMENTACTION::IDLE;
//	currX = (int)_playerPositionXNode;
//	currY = (int)_playerPositionYNode;
//	MVSTATE mvState;
//	ACTION_TARGET actionTarget;
//	MOVEMENTACTION action;
//
//	for (int i = 0; i < path.size(); i++)
//	{
//		//first node in a path is start node
//		if (i == 0) continue;
//
//		targetX = path[i]->GetX();
//		targetY = path[i]->GetY();
//		distX = targetX - currX;
//		distY = targetY - currY;
//
//		action = path[i]->GetActionToReach();
//		actionTarget = path[i]->GetActionTarget();
//		mvState = path[i - 1]->GetMvState();
//
//		AddActionToActionQueue(action, actionTarget, mvState, distX, distY);
//
//		currX = targetX;
//		currY = targetY;
//		prevAction = action;
//	}
//}
//
//void DebugBot::AddActionToActionQueue(MOVEMENTACTION action, ACTION_TARGET jumpTarget, MVSTATE mvState, int distX, int distY)
//{
//	DIRECTIONX directionX;
//	DIRECTIONY directionY;
//
//	if (distX > 0) directionX = xRIGHT;
//	else if (distX == 0) directionX = xNONE;
//	else directionX = xLEFT;
//
//	if (distY > 0) directionY = yDOWN;
//	else if (distY == 0) directionY = yNONE;
//	else directionY = yUP;
//
//	switch (action)
//	{
//	case IDLE:
//		break;
//	case CENTRALIZE:
//		_actionsQ.push(new CentralizeAction(this));
//		break;
//	case WALK:
//		//if (distX == 2), then bot is walking over a hole which should be runned
//		if (abs(distX) == 2)
//		{
//			_actionsQ.push(new WalkAction(this, distX, RUN));
//			break;
//		}
//
//		if (!_actionsQ.empty())
//		{
//			if (_actionsQ.back()->ActionType() == WALK &&
//				((distX > 0 && _actionsQ.back()->GetDirectionX() == xRIGHT) ||
//				(distX < 0 && _actionsQ.back()->GetDirectionX() == xLEFT)))
//			{
//				//previous action is also a WalkAction, and its going in the same direction,
//				//so we increase its distance instead of creating a new command (ActionBatching�)
//				dynamic_cast<WalkAction*>(_actionsQ.back())->AddDistance(distX);
//				break;
//			}
//		}
//
//		_actionsQ.push(new WalkAction(this, distX));
//
//		break;
//	case HANG:
//		_actionsQ.push(new HangAction(this, directionX));
//		break;
//	case DROP:
//		_actionsQ.push(new DropAction(this, jumpTarget, distY));
//		break;
//	case CLIMBFROMHANG:
//		_actionsQ.push(new ClimbFromHangAction(this, directionX));
//		break;
//	case CLIMB:
//		if (!_actionsQ.empty())
//		{
//			if (_actionsQ.back()->ActionType() == CLIMB &&
//				((distY > 0 && _actionsQ.back()->GetDirectionY() == yDOWN) ||
//				(distY < 0 && _actionsQ.back()->GetDirectionY() == yUP)))
//			{
//				//previous action is also a ClimbAction, and its going in the same direction,
//				//so we increase its distance instead of creating a new command (ActionBatching�)
//				dynamic_cast<ClimbAction*>(_actionsQ.back())->AddDistance(distY);
//				break;
//			}
//		}
//
//		_actionsQ.push(new ClimbAction(this, distY));
//		break;
//	case JUMPABOVERIGHT:
//		_actionsQ.push(new JumpAboveAction(this, xRIGHT));
//		break;
//	case JUMPABOVELEFT:
//		_actionsQ.push(new JumpAboveAction(this, xLEFT));
//		break;
//	case JUMP:
//		_actionsQ.push(new JumpAction(this, jumpTarget, distX, distY));
//		break;
//	case JUMPFROMLADDER:
//		if (mvState == mvCLIMBING_WITH_MOMENTUM)
//			_actionsQ.push(new JumpFromLadderAction(this, jumpTarget, true, distX, distY));
//		else if (mvState == mvCLIMBING)
//			_actionsQ.push(new JumpFromLadderAction(this, jumpTarget, false, distX, distY));
//		else
//			std::cout << "Error: MVSTATE is not CLIMBING when creating JUMPFROMLADDER" << std::endl;
//		break;
//	case WALKOFFLEDGE:
//		//TODO: walkoffledge to ladders
//		//_actionsQ.push(new WalkOffLedgeAction(this, distX, distY, jumpTarget));
//		_actionsQ.push(new WalkOffLedgeAction(this, distX, distY));
//		break;
//	default:
//		break;
//	}
//}

#pragma endregion

#pragma region Bot logic

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
		if (_waitTimer == 0 || _pathScheduler->NoSheduledPaths()) _botLogicWaiting = false;
		break;
	case DebugBot::EXPLORE_CC:
		_waitTimer--;
		if (_waitTimer == 0 || _pathScheduler->NoSheduledPaths()) _botLogicWaiting = false;
		break;
	case DebugBot::PICK_TARGET_IN_NEXT_CC:
		break;
	case DebugBot::MOVE_TO_NEXT_CC:
		//na razie bez timera, ale w sumie nie zaszkodzi�oby jakby by�
		if (_pathScheduler->NoSheduledPaths()) _botLogicWaiting = false;
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
	Coords playerC;

	while (true)
	{
		if (_botLogicState == EXIT) return;

		playerC = Coords(_playerPositionXNode, _playerPositionYNode, NODE);

		if (!_pathfinder->IsExitFound())
			_pathfinder->TryToFindExit();

		//czy to potrzebne? mo�e jednak mo�na liczy� za ka�dym razem?
		//if (_botLogicState == GATHER_FROM_CC || _botLogicState == EXPLORE_CC)
		//{
			BotLogicStateDebug();
			_pathfinder->CalculateConnectedComponents();
			_pathfinder->SCCDebug();
			currentCCnr = _pathfinder->GetCCnr(playerC);

			_objectManager->UpdateGameObjectLists();
			_objectManager->ItemsDebug();
			_objectManager->EnemiesDebug();

			_pathfinder->Dijkstra(playerC);
			_pathfinder->DijkstraDebug(true);
		//}

		if (_botLogicWaiting)
		{
			BotLogicWaiting();
			continue;
		}

		switch (_botLogicState)
		{
		case IDLE:
			std::this_thread::sleep_for(100ms);
			break;
		case START:
			//a little delay so map updates itself before bot starts to search for stuff
			std::this_thread::sleep_for(100ms);
			_botLogicState = GATHER_FROM_CC;
		case GATHER_FROM_CC:
		{
			std::vector<Item> itemsList = _objectManager->GetItems(spTreasure, currentCCnr);
			Item* closest = NULL;

			//znajdujemy najbli�szy item kt�ry nie jest ju� scheduled, jak takiego nie ma to czekamy,
			// jak nie ma ju� nic scheduled to przechodzimy do daszego stanu.

			Coords start = _pathScheduler->GetStartNodeForNextPath();
			_pathfinder->Dijkstra(start);

			for (int i = 0; i < itemsList.size(); i++)
			{
				if (_pathScheduler->IsScheduled(itemsList[i].GetCoords()))
					continue;

				if (closest == NULL || _pathfinder->GetDijDist(itemsList[i].GetCoords()) < _pathfinder->GetDijDist(closest->GetCoords()))
					closest = &itemsList[i];
			}

			if (closest != NULL)
			{
				_pathScheduler->TryToSchedulePath(closest->GetCoords());
			}
			else if (_pathScheduler->NoSheduledPaths())
			{
				_botLogicState = EXPLORE_CC;
			}
			else
			{
				_waitTimer = 10;
				_botLogicWaiting = true;
			}


			//if (_pathScheduler->NoSheduledPaths())
			//{
			//	//tak bym chcia�:
			//	//closest = FindClosestItem(itemsList);

			//	for (int i = 0; i < itemsList.size(); i++)
			//	{
			//		if (closest == NULL)
			//			closest = &itemsList[i];
			//		else if (_pathfinder->GetDijDist(itemsList[i].GetCoords()) < _pathfinder->GetDijDist(closest->GetCoords()))
			//			closest = &itemsList[i];
			//	}


			//	if (closest == NULL || _pathfinder->GetDijDist(closest->GetCoords()) == INT_MAX)
			//		_botLogicState = EXPLORE_CC;
			//	else
			//	{
			//		_pathScheduler->TryToSchedulePath(closest->GetCoords());
			//	}
			//}
			//else
			//{
			//	_waitTimer = 10;
			//	_botLogicWaiting = true;
			//}

			break;
		}
		case EXPLORE_CC:
		{
			std::vector<Item> itemsList = _objectManager->GetItems(spTreasure, currentCCnr);

			if (!itemsList.empty())
			{
				//mo�na tu doda� przerywanie eksploracji
				//na razie wydaje sie to problematyczne, a korzy�ci daje znikome
				//_breakExploration = true;
				_botLogicState = GATHER_FROM_CC;
				break;
			}

			if (_pathScheduler->NoSheduledPaths())
			{
				std::vector<MapNode*> nodes = _pathfinder->GetAllNodesFromCC(currentCCnr);
				MapNode * e;
				bool exploratonTargetFound = false;
				for (int i = 0; i < nodes.size(); i++)
				{
					if (_pathfinder->isCloseToFog(nodes[i]->GetCoords(), 5))
					{
						e = nodes[i];
						exploratonTargetFound = true;
						break;
					}
				}

				if (exploratonTargetFound)
				{
					_pathScheduler->TryToSchedulePath(e->GetCoords());
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
			//a mo�e po prostu DFS-em znajd� pierwszy cel w innej CC?
			Coords e;
			if (_pathfinder->TryToFindExplorationTarget(playerC))
			{
				e = _pathfinder->GetExplorationTarget();
				if (_pathScheduler->TryToSchedulePath(e))
				{
					_botLogicState = MOVE_TO_NEXT_CC;
					break;
				}
			}

			_botLogicState = SEARCH_FOR_EXIT;

			break;
		}
		case MOVE_TO_NEXT_CC:
			if (_pathScheduler->NoSheduledPaths())
				_botLogicState = GATHER_FROM_CC;
			else
			{
				_botLogicWaiting = true;
			}
			break;
		case SEARCH_FOR_EXIT:
			if (_pathfinder->IsExitFound())
			{
				Coords exit = _pathfinder->GetExit();
				if (_pathScheduler->TryToSchedulePath(exit))
				{
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
			if (playerC == _pathfinder->GetExit())
				_botLogicState = EXIT_REACHED;
			else
			{
				_botLogicWaiting = true;
			}
			break;
		case EXIT_REACHED:
			//mo�na wcisn�� strza�k� w g�r� albo co�
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
	Coords playerc = Coords((int)_playerPositionXNode, (int)_playerPositionYNode);

	//PRINTING DEBUG INFO TO FILES
	if (_debugTimer < 0)
	{
		bool hasMomentum = false;
		_pathfinder->NeighboursDebug(playerc, hasMomentum);
		_debugTimer = 10;
	}
	_debugTimer -= 1;


	ordersStruct orders = _pathScheduler->GetOrdersFromCurrentAction();
	ExecuteOrders(orders);


}
