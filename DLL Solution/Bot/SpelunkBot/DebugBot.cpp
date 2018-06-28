#include "stdafx.h"
#include "DebugBot.h"


#pragma region Constructors and initialization

DebugBot::DebugBot()
{
}

void DebugBot::CreateHelpers()
{
	_movementSimulator = std::make_shared<MovementSimulator>(_bot);
	_pathfinder = std::make_shared<Pathfinder>(_bot, _movementSimulator);
	_objectManager = std::make_shared<ObjectManager>(_bot, _pathfinder);
	_pathScheduler = std::make_shared<PathScheduler>(_bot, _pathfinder);
}

void DebugBot::InitialiseBotLogicState()
{	
	_botLogicState = START;
	//_botLogicState = DEBUG;

	_nextBotLogicState = IDLE;
	_botLogicWaiting = false;
	_botLogicExhaustQueue = false;
	_waitTimer = 0;
	_targetAfterDisarm = Coords(-1, -1);
	_arrowTrapToDisarm = Coords(-1, -1);
}	

void DebugBot::NewLevel()
{
	//terminate botlogic thread from previous level
	_botLogicState = EXIT;
	if (_botLogicThread.joinable())
		_botLogicThread.join();

	//initialize variables and start new botlogic thread
	//InitialiseVariables();
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
}

#pragma endregion


#pragma region orders

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

	_rightPressed = orders.rightPressed;
	_leftPressed = orders.leftPressed;
	_rightReleased = orders.rightReleased;
	_leftReleased = orders.leftReleased;
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

	_rightPressed = false;
	_leftPressed = false;
	_rightReleased = false;
	_leftReleased = false;
}

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
	//case DebugBot::EXHAUST_QUEUE:
	//	_waitTimer--;
	//	if (_waitTimer == 0 || _pathScheduler->NoSheduledPaths()) _botLogicWaiting = false;
	//	break;
	case DebugBot::GATHER_FROM_CC:
		_waitTimer--;
		if (_waitTimer == 0 || _pathScheduler->NoSheduledPaths()) _botLogicWaiting = false;
		break;
	case DebugBot::EXPLORE_CC:
		_waitTimer--;
		if (_waitTimer == 0 || _pathScheduler->NoSheduledPaths()) _botLogicWaiting = false;
		break;
	//case DebugBot::PICK_TARGET_IN_NEXT_CC:
	//	break;
	case DebugBot::MOVE_TO_NEXT_CC:
		//na razie bez timera, ale w sumie nie zaszkodzi³oby jakby by³
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

void DebugBot::BotLogicExhaustQueue()
{
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(100ms);

	switch (_botLogicState)
	{
	case DISARM_AT_HASITEM:
		if (_pathScheduler->GetCurrentActionType() != WAIT &&
			_pathScheduler->GetCurrentActionType() != THROWITEM && 
			!IsHoldingItem())
		{
			if (!_pathScheduler->NoSheduledPaths())
				_pathScheduler->ClearSheduledPaths();

			//wait until current action is done
			if (!_pathScheduler->ActionInProgress())
			{
				_botLogicState = DISARM_AT_NOITEM;
				_botLogicExhaustQueue = false;
			}

			break;
		}
		if (_pathScheduler->NoSheduledPaths())
		{
			_botLogicState = _nextBotLogicState;
			_botLogicExhaustQueue = false;
			break;
		}
		break;
	default:
		if (_pathScheduler->NoSheduledPaths())
		{
			_botLogicState = _nextBotLogicState;
			_botLogicExhaustQueue = false;
		}
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
		if (_botLogicState == DEBUG)
		{
			std::this_thread::sleep_for(100ms);
			continue;
		}

		playerC = Coords(_playerPositionXNode, _playerPositionYNode, NODE);

		if (!_pathfinder->IsExitFound())
			_pathfinder->TryToFindExit();

		//czy to potrzebne? mo¿e jednak mo¿na liczyæ za ka¿dym razem?
		//if (_botLogicState == GATHER_FROM_CC || _botLogicState == EXPLORE_CC)
		//{
			BotLogicStateDebug();
			_pathfinder->CalculateConnectedComponents(ALLOW_ARROWTRAPS);
			_pathfinder->SCCDebug();
			currentCCnr = _pathfinder->GetCCnr(playerC);

			_objectManager->UpdateGameObjectLists();
			_objectManager->ItemsDebug();
			_objectManager->EnemiesDebug();

			_pathfinder->Dijkstra(playerC, ALLOW_ARROWTRAPS);
			_pathfinder->DijkstraDebug(true);
		//}

		if (_botLogicWaiting)
		{
			BotLogicWaiting();
			continue;
		}

		if (_botLogicExhaustQueue)
		{
			BotLogicExhaustQueue();
			continue;
		}
		//GetHeldItemID();
		

		switch (_botLogicState)
		{
		case IDLE:
			std::this_thread::sleep_for(100ms);
			break;
		case START:
			//a little delay so map updates itself before bot starts to search for stuff
			std::this_thread::sleep_for(100ms);
			_botLogicState = GATHER_FROM_CC;
			break;
		case GATHER_FROM_CC:
		{
			std::vector<Item> itemsList = _objectManager->GetItems(kndTreasure, currentCCnr);
			Item* closest = NULL;

			//znajdujemy najbli¿szy item który nie jest ju¿ scheduled, jak takiego nie ma to czekamy,
			// jak nie ma ju¿ nic scheduled to przechodzimy do daszego stanu.

			Coords start = _pathScheduler->GetStartNodeForNextPath();
			_pathfinder->Dijkstra(start, ALLOW_ARROWTRAPS);

			for (int i = 0; i < itemsList.size(); i++)
			{
				if (_pathScheduler->IsScheduled(itemsList[i].GetCoords()))
					continue;

				if (closest == NULL || _pathfinder->GetDijDist(itemsList[i].GetCoords()) < _pathfinder->GetDijDist(closest->GetCoords()))
					closest = &itemsList[i];
			}

			if (closest != NULL)
			{
				if (_pathfinder->GetNodeFromGrid(closest->GetCoords())->IsArrowTrapOnWay())
				{
					_botLogicExhaustQueue = true;
					_nextBotLogicState = DISARM_ARROWTRAP;
					_targetAfterDisarm = closest->GetCoords();
				}
				else
					_pathScheduler->TryToSchedulePath(closest->GetCoords(), STOP_ON_ARROWTRAPS);
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
			break;
		}
		case EXPLORE_CC:
		{
			std::vector<Item> itemsList = _objectManager->GetItems(kndTreasure, currentCCnr);

			if (!itemsList.empty())
			{
				//mo¿na tu dodaæ przerywanie eksploracji
				//na razie wydaje sie to problematyczne, a korzyœci daje znikome
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
					if (_pathfinder->GetNodeFromGrid(e->GetCoords())->IsArrowTrapOnWay())
					{
						_botLogicExhaustQueue = true;
						_nextBotLogicState = DISARM_ARROWTRAP;
						_targetAfterDisarm = e->GetCoords();
					}
					else
						_pathScheduler->TryToSchedulePath(e->GetCoords(), STOP_ON_ARROWTRAPS);
				}
				else
				{
					_botLogicState = MOVE_TO_NEXT_CC;
				}
			}
			else
			{
				_waitTimer = 10;
				_botLogicWaiting = true;
			}


			break;
		}
		case MOVE_TO_NEXT_CC:
		{
			//a mo¿e po prostu DFS-em znajdŸ pierwszy cel w innej CC?
			Coords e;
			if (_pathfinder->TryToFindExplorationTarget(playerC, ALLOW_ARROWTRAPS))
			{
				e = _pathfinder->GetExplorationTarget();

				if (_pathfinder->GetNodeFromGrid(e)->IsArrowTrapOnWay())
				{
					_botLogicExhaustQueue = true;
					_nextBotLogicState = DISARM_ARROWTRAP;
					_targetAfterDisarm = e;
					break;
				}
				else if (_pathScheduler->TryToSchedulePath(e, STOP_ON_ARROWTRAPS))
				{
					_botLogicExhaustQueue = true;
					_nextBotLogicState = GATHER_FROM_CC;
					break;
				}
			}

			_botLogicState = SEARCH_FOR_EXIT;

			break;
		}
		//case MOVE_TO_NEXT_CC:
		//	if (_pathScheduler->NoSheduledPaths())
		//		_botLogicState = GATHER_FROM_CC;
		//	else
		//	{
		//		_botLogicWaiting = true;
		//	}
		//	break;
		case SEARCH_FOR_EXIT:
			if (_pathfinder->IsExitFound())
			{
				Coords exit = _pathfinder->GetExit();
				if (_pathScheduler->TryToSchedulePath(exit, STOP_ON_ARROWTRAPS))
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
		case DISARM_ARROWTRAP:
		{
			//_arrowTrapToDisarm = _pathfinder->GetNodeFromGrid(_targetAfterDisarm)->GetArrowTrapCoords();

			//_pathfinder->Dijkstra(playerC, ALLOW_SELECTED, std::vector<Coords>{_arrowTrapToDisarm});
			_pathfinder->Dijkstra(playerC, ALLOW_ARROWTRAPS);
			auto path = _pathfinder->GetDijPath(_targetAfterDisarm);

			//calculating FIRST arrowtrap on the way to _targetAfterDisarm. 
			//If there is more, they will be handled in next runs of DISARM_ARROWTRAP.
			for (auto node : path)
			{
				if (node->IsArrowTrapOnWay())
				{
					_afterAT = node;
					_arrowTrapToDisarm = node->GetArrowTrapCoords();
					break;
				}
				else _beforeAT = node;
			}

			bool canDisarmByMovement = false;
			if (_afterAT->GetActionToReach() == WALK)
				canDisarmByMovement = true;
			else if (_afterAT->GetActionToReach() == HANG ||
				_afterAT->GetActionToReach() == HANGDROP)
			{
				int dir = _pathfinder->ArrowTrapL(_arrowTrapToDisarm) ? -1 : 1;
				
				if (_pathfinder->Passable(_arrowTrapToDisarm.OffsetX(1 * dir)) &&
					_pathfinder->Passable(_arrowTrapToDisarm.OffsetX(2 * dir)) &&
					_pathfinder->Passable(_arrowTrapToDisarm.OffsetX(3 * dir)))
					canDisarmByMovement = true;
			}


			if (canDisarmByMovement)
			{
				_pathScheduler->TryToSchedulePath(_afterAT->GetCoords(), ALLOW_SELECTED, std::vector<Coords>{_arrowTrapToDisarm});
				_botLogicExhaustQueue = true;
				_nextBotLogicState = GATHER_FROM_CC;
			}
			else
				_botLogicState = DISARM_AT_NOITEM;

			break;
		}
		case DISARM_AT_NOITEM:
		{
			Item* heldItem = _objectManager->GetItemByID(GetHeldItemID());
			if (heldItem != NULL)
				_botLogicState = DISARM_AT_HASITEM;
			else
			{

				////break if that arrowtrap is already disarmed for some reason
				if (IsArrowTrapDisarmed(_arrowTrapToDisarm.GetX(), _arrowTrapToDisarm.GetY()))
				{
					_botLogicState = GATHER_FROM_CC;
					break;
				}

				std::vector<Item> junkHeavy = _objectManager->GetItems(kndJunkHeavy, currentCCnr);
				std::vector<Item> itemsList = _objectManager->GetItems(kndJunkLight, currentCCnr);
				itemsList.insert(end(itemsList), begin(junkHeavy), end(junkHeavy));

				//std::unique_ptr<Item> itemPtr = FindClosest(itemsList);

				_pathfinder->Dijkstra(playerC, STOP_ON_ARROWTRAPS);
				_pathfinder->DijkstraDebug(false);

				Item* closest = NULL;
				for (int i = 0; i < itemsList.size(); i++)
				{
					if (_pathfinder->GetDijDist(itemsList[i].GetCoords()) == INT_MAX) continue;

					if (closest == NULL || _pathfinder->GetDijDist(itemsList[i].GetCoords()) < _pathfinder->GetDijDist(closest->GetCoords()))
						closest = &itemsList[i];
				}

				if (closest != NULL)
				{
					_pathScheduler->TryToSchedulePath(closest->GetCoords(), STOP_ON_ARROWTRAPS);
					_pathScheduler->ScheduleAction(std::make_shared<PickUpItemAction>(_bot, closest->GetID()));
					_botLogicExhaustQueue = true;
					_nextBotLogicState = DISARM_AT_HASITEM;
				}
				else
				{
					//mo¿esz zrobiæ exploit albo odpuœciæ przechodzenie przez tê pu³apkê
				}
			}

			break;
		}
		case DISARM_AT_HASITEM:
		{
			Item* heldItem = _objectManager->GetItemByID(GetHeldItemID());
			if (heldItem == NULL)
				_botLogicState = DISARM_AT_NOITEM;
			else
			{
				////break if that arrowtrap is already disarmed for some reason
				if (IsArrowTrapDisarmed(_arrowTrapToDisarm.GetX(), _arrowTrapToDisarm.GetY()))
				{
					_botLogicState = GATHER_FROM_CC;
					break;
				}

				//_pathfinder->Dijkstra(playerC, ALLOW_SELECTED, std::vector<Coords>{_arrowTrapToDisarm});
				//_pathfinder->DijkstraDebug(false);

				//change throwing place if its in danger of getting hit by the arrow from disarmed trap
				_beforeAT = &_pathfinder->GetSafeNodeToDisarmAT(*_beforeAT, _arrowTrapToDisarm);

				//set direction of the throw
				directions dirToAT, dirToAfterAT, throwDir;
				dirToAT = _pathfinder->CalculateDirections(_beforeAT->GetCoords(), _arrowTrapToDisarm);
				dirToAfterAT = _pathfinder->CalculateDirections(_beforeAT->GetCoords(), _afterAT->GetCoords());
				throwDir = dirToAT.x == dirToAfterAT.x ? dirToAT : dirToAfterAT;

				//throwing down laddertops
				if (_beforeAT->GetActionTarget() == LADDERTOP && throwDir.y == yDOWN)
				{
					MapNode *beforeATold = _beforeAT;

					if (_beforeAT->GetActionToReach() == WALK)
						_beforeAT = _beforeAT->GetDijParent();
					else
					{
						if (_pathfinder->CanStandInNode(Coords(_beforeAT->GetX() - 1, _beforeAT->GetY())))
							_beforeAT = _pathfinder->GetNodeFromGrid(Coords(_beforeAT->GetX() - 1, _beforeAT->GetY()));
						else if (_pathfinder->CanStandInNode(Coords(_beforeAT->GetX() + 1, _beforeAT->GetY())))
							_beforeAT = _pathfinder->GetNodeFromGrid(Coords(_beforeAT->GetX() + 1, _beforeAT->GetY()));
						else
						{
							//ultra rare case - can stay as-is for now
						}
					}

					if (beforeATold->GetX() > _beforeAT->GetX()) throwDir.x = xRIGHT;
					if (beforeATold->GetX() < _beforeAT->GetX()) throwDir.x = xLEFT;
				}

				_pathScheduler->TryToSchedulePath(_beforeAT->GetCoords(), STOP_ON_ARROWTRAPS);
				_pathScheduler->ScheduleAction(std::make_shared<CentralizeAction>(_bot));
				_pathScheduler->ScheduleAction(std::make_shared<ThrowItemAction>(_bot, throwDir));
				auto waitAction = std::make_shared<WaitAction>(_bot);
				waitAction->SetCondition(IsArrowTrapDisarmed, _arrowTrapToDisarm.GetX(), _arrowTrapToDisarm.GetY());
				waitAction->SetTimeCondition(3000);
				_pathScheduler->ScheduleAction(waitAction);

				_botLogicExhaustQueue = true;
				_nextBotLogicState = GATHER_FROM_CC;
			}
			break;
		}
		case EXIT:
			return;
		}
	}
}


//teraz niech arrowtrapy dziel¹ na cc
//void DebugBot::BotLogic()
//{
//	using namespace std::chrono_literals;
//	int currentCCnr;
//	Coords playerC;
//
//	while (true)
//	{
//		if (_botLogicState == EXIT) return;
//		if (_botLogicState == DEBUG)
//		{
//			std::this_thread::sleep_for(100ms);
//			continue;
//		}
//
//		playerC = Coords(_playerPositionXNode, _playerPositionYNode, NODE);
//
//		if (!_pathfinder->IsExitFound())
//			_pathfinder->TryToFindExit();
//
//		//czy to potrzebne? mo¿e jednak mo¿na liczyæ za ka¿dym razem?
//		//if (_botLogicState == GATHER_FROM_CC || _botLogicState == EXPLORE_CC)
//		//{
//		BotLogicStateDebug();
//		_pathfinder->CalculateConnectedComponents();
//		_pathfinder->SCCDebug();
//		currentCCnr = _pathfinder->GetCCnr(playerC);
//
//		_objectManager->UpdateGameObjectLists();
//		_objectManager->ItemsDebug();
//		_objectManager->EnemiesDebug();
//
//		_pathfinder->Dijkstra(playerC);
//		_pathfinder->DijkstraDebug(true);
//		//}
//
//		if (_botLogicWaiting)
//		{
//			BotLogicWaiting();
//			continue;
//		}
//
//		if (_botLogicExhaustQueue)
//		{
//			BotLogicExhaustQueue();
//			continue;
//		}
//
//
//		switch (_botLogicState)
//		{
//		case IDLE:
//			std::this_thread::sleep_for(100ms);
//			break;
//		case START:
//			//a little delay so map updates itself before bot starts to search for stuff
//			std::this_thread::sleep_for(100ms);
//			_botLogicState = GATHER_FROM_CC;
//			break;
//		case GATHER_FROM_CC:
//		{
//			std::vector<Item> itemsList = _objectManager->GetItems(kndTreasure, currentCCnr);
//			Item* closest = NULL;
//
//			//znajdujemy najbli¿szy item który nie jest ju¿ scheduled, jak takiego nie ma to czekamy,
//			// jak nie ma ju¿ nic scheduled to przechodzimy do daszego stanu.
//
//			Coords start = _pathScheduler->GetStartNodeForNextPath();
//			_pathfinder->Dijkstra(start);
//
//			for (int i = 0; i < itemsList.size(); i++)
//			{
//				if (_pathScheduler->IsScheduled(itemsList[i].GetCoords()))
//					continue;
//
//				if (closest == NULL || _pathfinder->GetDijDist(itemsList[i].GetCoords()) < _pathfinder->GetDijDist(closest->GetCoords()))
//					closest = &itemsList[i];
//			}
//
//			if (closest != NULL)
//			{
//				_pathScheduler->TryToSchedulePath(closest->GetCoords());
//			}
//			else if (_pathScheduler->NoSheduledPaths())
//			{
//				_botLogicState = EXPLORE_CC;
//			}
//			else
//			{
//				_waitTimer = 10;
//				_botLogicWaiting = true;
//			}
//
//			break;
//		}
//		case EXPLORE_CC:
//		{
//			std::vector<Item> itemsList = _objectManager->GetItems(kndTreasure, currentCCnr);
//
//			if (!itemsList.empty())
//			{
//				//mo¿na tu dodaæ przerywanie eksploracji
//				//na razie wydaje sie to problematyczne, a korzyœci daje znikome
//				//_breakExploration = true;
//				_botLogicState = GATHER_FROM_CC;
//				break;
//			}
//
//			if (_pathScheduler->NoSheduledPaths())
//			{
//				std::vector<MapNode*> nodes = _pathfinder->GetAllNodesFromCC(currentCCnr);
//				MapNode * e;
//				bool exploratonTargetFound = false;
//				for (int i = 0; i < nodes.size(); i++)
//				{
//					if (_pathfinder->isCloseToFog(nodes[i]->GetCoords(), 5))
//					{
//						e = nodes[i];
//						exploratonTargetFound = true;
//						break;
//					}
//				}
//
//				if (exploratonTargetFound)
//				{
//					_pathScheduler->TryToSchedulePath(e->GetCoords());
//				}
//				else
//				{
//					_botLogicState = MOVE_TO_NEXT_CC;
//				}
//			}
//			else
//			{
//				_waitTimer = 10;
//				_botLogicWaiting = true;
//			}
//
//
//			break;
//		}
//		case MOVE_TO_NEXT_CC:
//		{
//			//a mo¿e po prostu DFS-em znajdŸ pierwszy cel w innej CC?
//			Coords e;
//			if (_pathfinder->TryToFindTargetInNextCC(playerC, false))
//			{
//				e = _pathfinder->GetTargetInNextCC();
//
//				if (_pathfinder->GetNodeFromGrid(e)->IsArrowTrapOnWay())
//				{
//					_botLogicExhaustQueue = true;
//					_nextBotLogicState = DISARM_AT_NOITEM;
//					_targetAfterDisarm = e;
//					break;
//				}
//				else if (_pathScheduler->TryToSchedulePath(e, false))
//				{
//					_botLogicExhaustQueue = true;
//					_nextBotLogicState = GATHER_FROM_CC;
//					break;
//				}
//			}
//
//			_botLogicState = SEARCH_FOR_EXIT;
//
//			break;
//		}
//		//case MOVE_TO_NEXT_CC:
//		//	if (_pathScheduler->NoSheduledPaths())
//		//		_botLogicState = GATHER_FROM_CC;
//		//	else
//		//	{
//		//		_botLogicWaiting = true;
//		//	}
//		//	break;
//		case SEARCH_FOR_EXIT:
//			if (_pathfinder->IsExitFound())
//			{
//				Coords exit = _pathfinder->GetExit();
//				if (_pathScheduler->TryToSchedulePath(exit))
//				{
//					_botLogicState = GO_TO_EXIT;
//				}
//				else
//					_botLogicState = UNREACHABLE_EXIT;
//
//			}
//			else
//			{
//				if (_pathfinder->IsFogOnMap())
//					_botLogicState = NO_EXIT_EXPLORE;
//				else
//					_botLogicState = NO_EXIT_ERROR;
//			}
//
//			break;
//		case GO_TO_EXIT:
//			if (playerC == _pathfinder->GetExit())
//				_botLogicState = EXIT_REACHED;
//			else
//			{
//				_botLogicWaiting = true;
//			}
//			break;
//		case EXIT_REACHED:
//			//mo¿na wcisn¹æ strza³kê w górê albo coœ
//			std::this_thread::sleep_for(100ms);
//			break;
//		case UNREACHABLE_EXIT:
//			std::this_thread::sleep_for(100ms);
//			break;
//		case NO_EXIT_EXPLORE:
//			std::this_thread::sleep_for(100ms);
//			break;
//		case NO_EXIT_ERROR:
//			std::this_thread::sleep_for(100ms);
//			break;
//		case DISARM_AT_NOITEM:
//		{
//			Item* heldItem = _objectManager->GetItemByID(GetHeldItemID());
//			if (heldItem != NULL)
//				_botLogicState = DISARM_AT_HASITEM;
//			else
//			{
//				std::vector<Item> junkHeavy = _objectManager->GetItems(kndJunkHeavy, currentCCnr);
//				std::vector<Item> itemsList = _objectManager->GetItems(kndJunkLight, currentCCnr);
//				itemsList.insert(end(itemsList), begin(junkHeavy), end(junkHeavy));
//
//				//std::unique_ptr<Item> itemPtr = FindClosest(itemsList);
//
//				//_pathfinder->Dijkstra(playerC, true);
//				//_pathfinder->DijkstraDebug(true);
//
//				Item* closest = NULL;
//				for (int i = 0; i < itemsList.size(); i++)
//				{
//					if (_pathfinder->GetDijDist(itemsList[i].GetCoords()) == INT_MAX) continue;
//
//					if (closest == NULL || _pathfinder->GetDijDist(itemsList[i].GetCoords()) < _pathfinder->GetDijDist(closest->GetCoords()))
//						closest = &itemsList[i];
//				}
//
//				if (closest != NULL)
//				{
//					_pathScheduler->TryToSchedulePath(closest->GetCoords());
//					_pathScheduler->ScheduleAction(std::make_shared<PickUpItemAction>(_bot, closest->GetID()));
//					_botLogicExhaustQueue = true;
//					_nextBotLogicState = DISARM_AT_HASITEM;
//				}
//				else
//				{
//					//mo¿esz zrobiæ exploit albo odpuœciæ przechodzenie przez tê pu³apkê
//				}
//			}
//
//			break;
//		}
//		case DISARM_AT_HASITEM:
//		{
//			Item* heldItem = _objectManager->GetItemByID(GetHeldItemID());
//			if (heldItem == NULL)
//				_botLogicState = DISARM_AT_NOITEM;
//			else
//			{
//				_pathfinder->Dijkstra(playerC, false);
//				auto path = _pathfinder->GetDijPath(_targetAfterDisarm);
//
//				MapNode *beforeAT, *beforeATold, *afterAT;
//				for (auto node : path)
//				{
//					if (node->IsArrowTrapOnWay())
//					{
//						afterAT = node;
//						break;
//					}
//					else beforeAT = node;
//				}
//
//				Coords arrowTrap = afterAT->GetArrowTrapCoords();
//
//				directions dirToAT, dirToAfterAT, throwDir;
//				dirToAT = _pathfinder->CalculateDirections(beforeAT->GetCoords(), arrowTrap);
//				dirToAfterAT = _pathfinder->CalculateDirections(beforeAT->GetCoords(), afterAT->GetCoords());
//
//				throwDir = dirToAT.x == dirToAfterAT.x ? dirToAT : dirToAfterAT;
//
//				if (beforeAT->GetActionTarget() == LADDERTOP && throwDir.y == yDOWN)
//				{
//					beforeATold = beforeAT;
//
//					if (beforeAT->GetActionToReach() == WALK)
//						beforeAT = beforeAT->GetDijParent();
//					else
//					{
//						if (_pathfinder->CanStandInNode(Coords(beforeAT->GetX() - 1, beforeAT->GetY())))
//							beforeAT = _pathfinder->GetNodeFromGrid(Coords(beforeAT->GetX() - 1, beforeAT->GetY()));
//						else if (_pathfinder->CanStandInNode(Coords(beforeAT->GetX() + 1, beforeAT->GetY())))
//							beforeAT = _pathfinder->GetNodeFromGrid(Coords(beforeAT->GetX() + 1, beforeAT->GetY()));
//						else
//						{
//							//ultra rare case - can stay as-is for now
//						}
//					}
//
//					if (beforeATold->GetX() > beforeAT->GetX()) throwDir.x = xRIGHT;
//					if (beforeATold->GetX() < beforeAT->GetX()) throwDir.x = xLEFT;
//				}
//
//				_pathScheduler->TryToSchedulePath(beforeAT->GetCoords());
//				_pathScheduler->ScheduleAction(std::make_shared<CentralizeAction>(_bot));
//				_pathScheduler->ScheduleAction(std::make_shared<ThrowItemAction>(_bot, throwDir));
//				auto waitAction = std::make_shared<WaitAction>(_bot);
//				waitAction->SetCondition(IsArrowTrapDisarmed, arrowTrap.GetX(), arrowTrap.GetY());
//				waitAction->SetTimeCondition(3000);
//				_pathScheduler->ScheduleAction(waitAction);
//
//				_botLogicExhaustQueue = true;
//				_nextBotLogicState = GATHER_FROM_CC;
//			}
//			break;
//		}
//		case EXIT:
//			return;
//		}
//	}
//}

#pragma endregion
//int timer = -1;
//std::ofstream fileStream;

void DebugBot::Update()
{
	Coords playerc = Coords((int)_playerPositionXNode, (int)_playerPositionYNode);
	
	//PRINTING DEBUG INFO TO FILES
	if (_debugTimer < 0)
	{
		bool hasMomentum = false;
		_pathfinder->NeighboursDebug(playerc, hasMomentum);
		_debugTimer = 10;

		//_movementSimulator->MakeSimulation(129, 72, 8, 0, directions{ xRIGHT, yDOWN });
	}
	_debugTimer -= 1;



	//_objectManager->UpdateGameObjectLists();
	//auto items = _objectManager->GetItems(spArrow);

	//if (!items.empty())
	//{
	//	if (timer == -1)
	//	{
	//		fileStream.open(".\\MovementSimulationArrowPix.txt");
	//		timer = 300;
	//	}


	//	fileStream << items[0].GetCoords().GetXpixel() << ", " << items[0].GetCoords().GetYpixel() << std::endl;
	//	
	//	if (timer > 0) timer--;
	//	if (timer == 0) fileStream.close();
	//}



	ordersStruct orders = _pathScheduler->GetOrdersFromCurrentAction();
	ExecuteOrders(orders);

}
