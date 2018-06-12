#include "stdafx.h"
#include "DebugBot.h"


#pragma region Constructors and initialization

DebugBot::DebugBot()
{
	//_bot = std::unique_ptr<DebugBot>(this);
	//_bot = std::make_unique<DebugBot>(this);

	//_bot = std::make_shared<DebugBot>();
	//_bot = std::shared_ptr<DebugBot>(this);
	//_pathfinder = std::make_shared<Pathfinder>(_bot);
	//_objectManager = std::make_shared<ObjectManager>(_bot, _pathfinder);
	//_pathScheduler = std::make_shared<PathScheduler>(_bot, _pathfinder);

	//_pathfinder = new Pathfinder(this);
	//_objectManager = new ObjectManager(this, _pathfinder);
	//_pathScheduler = new PathScheduler(this, _pathfinder);
	//NewLevel();
}

void DebugBot::CreateHelpers()
{
	_pathfinder = std::make_shared<Pathfinder>(_bot);
	_objectManager = std::make_shared<ObjectManager>(_bot, _pathfinder);
	_pathScheduler = std::make_shared<PathScheduler>(_bot, _pathfinder);
}

void DebugBot::InitialiseBotLogicState()
{	
	_botLogicState = START;
	_nextBotLogicState = IDLE;
	_secState = WAITING_FOR_PATH;
	_botLogicWaiting = false;
	_waitTimer = 0;
	_targetAfterDisarm = Coords(-1,-1);
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
	//if (!_pathfinder) _pathfinder = std::make_shared<Pathfinder>(_bot);
	//if (!_objectManager) _objectManager = std::make_shared<ObjectManager>(_bot, _pathfinder);
	//if (!_pathScheduler) _pathScheduler = std::make_shared<PathScheduler>(_bot, _pathfinder);


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

	//while (!_actionsQ.empty())
	//{
	//	delete _actionsQ.front();
	//	_actionsQ.pop();
	//}
	//delete _pathfinder;
	//delete _objectManager;
	//delete _pathScheduler;
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
	case DebugBot::EXHAUST_QUEUE:
		_waitTimer--;
		if (_waitTimer == 0 || _pathScheduler->NoSheduledPaths()) _botLogicWaiting = false;
		break;
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

		//czy to potrzebne? mo¿e jednak mo¿na liczyæ za ka¿dym razem?
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
			break;
		case EXHAUST_QUEUE:
			if (_pathScheduler->NoSheduledPaths())
			{
				_botLogicState = _nextBotLogicState;
				_nextBotLogicState = IDLE;
			}
			else
			{
				_waitTimer = 10;
				_botLogicWaiting = true;
			}
			break;
		case GATHER_FROM_CC:
		{
			std::vector<Item> itemsList = _objectManager->GetItems(kndTreasure, currentCCnr);
			Item* closest = NULL;

			//znajdujemy najbli¿szy item który nie jest ju¿ scheduled, jak takiego nie ma to czekamy,
			// jak nie ma ju¿ nic scheduled to przechodzimy do daszego stanu.

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
				if (_pathfinder->GetNodeFromGrid(closest->GetCoords())->IsArrowTrapOnWay())
				{
					_botLogicState = EXHAUST_QUEUE;
					_nextBotLogicState = DISARM_ARROWTRAP;
					_targetAfterDisarm = closest->GetCoords();
				}
				else
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

			//single path
			//if (_pathScheduler->NoSheduledPaths())
			//{
			//	//tak bym chcia³:
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
			//a mo¿e po prostu DFS-em znajdŸ pierwszy cel w innej CC?
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
			//we guaranteed empty action queue earlier
			/*
			1. find something to disarm the trap with
			2. go to it and pick it up
			3. go right before the dangerous node in the path and throw the stone
			4. ???
			5. profit
			*/
			//sprawdŸ te¿ czy nie masz czegoœ w ³apie (w sumie na razie nie muszê bo na razie tylko w tym stanie podnoszê przedmioty)

			Item* heldItem = _objectManager->GetItemByID(GetHeldItemID());
			if (heldItem == NULL)
			{
				std::vector<Item> junkHeavy = _objectManager->GetItems(kndJunkHeavy, currentCCnr);
				std::vector<Item> itemsList = _objectManager->GetItems(kndJunkLight, currentCCnr);
				itemsList.insert(end(itemsList), begin(junkHeavy), end(junkHeavy));

				//std::unique_ptr<Item> itemPtr = FindClosest(itemsList);

				Item* closest = NULL;
				for (int i = 0; i < itemsList.size(); i++)
				{
					if (closest == NULL || _pathfinder->GetDijDist(itemsList[i].GetCoords()) < _pathfinder->GetDijDist(closest->GetCoords()))
						closest = &itemsList[i];
				}

				if (closest != NULL)
				{
					_pathScheduler->TryToSchedulePath(closest->GetCoords());
					_pathScheduler->ScheduleAction(std::make_shared<PickUpItemAction>(_bot, closest->GetID()));
					_botLogicState = EXHAUST_QUEUE;
					_nextBotLogicState = DISARM_ARROWTRAP;
				}
				else
				{
					//i have the item
				}
			}
			else
			{
				std::cout << "hehe patrz przemek mam przedmiot";
			}

			break;
		}
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
