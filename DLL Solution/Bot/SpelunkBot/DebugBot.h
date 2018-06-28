#pragma once
#include "IBot.h"
#include "IMovementAction.h"
#include "Pathfinder.h"
#include "ObjectManager.h"
#include "PathScheduler.h"
#include "MovementSimulator.h"
#include <fstream>
#include <ostream>
#include <thread>
#include <chrono>

class DebugBot : public IBot
{
public:
	DebugBot();
	~DebugBot();

	void ExecuteOrders(ordersStruct orders);

	void Update() override;
	void NewLevel() override;

	void CreateHelpers() override;

private:
	//std::queue<IMovementAction*> _actionsQ;
	//std::shared_ptr<IBot> _bot;
	std::shared_ptr<Pathfinder> _pathfinder;
	std::shared_ptr<ObjectManager> _objectManager;
	std::shared_ptr<PathScheduler> _pathScheduler;
	std::shared_ptr<MovementSimulator> _movementSimulator;
	std::thread _botLogicThread;
	
	//std::vector<collectableObject> _collectablesList;
	//std::deque<std::vector<MapNode*>> _pathsQ;

	//enum SECONDARY_STATE { sIDLE, NEW_TARGET, UNREACHABLE_TARGET, EXECUTING_COMMANDS,
	//					   CREATING_COMMANDS, FINISHED, WAITING_FOR_PATH, DEBUG };
	enum BOTLOGIC_STATE { IDLE, DEBUG, START, EXIT, WAITING, GATHER_FROM_CC, EXPLORE_CC, /*PICK_TARGET_IN_NEXT_CC,*/
						  MOVE_TO_NEXT_CC, SEARCH_FOR_EXIT, GO_TO_EXIT, EXIT_REACHED, UNREACHABLE_EXIT,
						  NO_EXIT_EXPLORE, NO_EXIT_ERROR, /*EXHAUST_QUEUE,*/ /*DISARM_ARROWTRAP,*/ DISARM_ARROWTRAP, DISARM_AT_NOITEM, DISARM_AT_HASITEM };
	BOTLOGIC_STATE _botLogicState;
	BOTLOGIC_STATE _prevBotLogicState;
	BOTLOGIC_STATE _nextBotLogicState;
	//SECONDARY_STATE _secState;
	bool _botLogicWaiting;
	bool _botLogicExhaustQueue;
	int _waitTimer;
	MapNode *_beforeAT, *_afterAT;
	Coords _targetAfterDisarm;
	Coords _arrowTrapToDisarm;

	//void CreateCommands(std::vector<MapNode*> path);
	//void AddActionToActionQueue(MOVEMENTACTION action, ACTION_TARGET jumpTarget, MVSTATE mvState, int distX, int distY);
	void ClearOrders();

	//botlogic methods
	void InitialiseBotLogicState();
	void BotLogic();
	void BotLogicWaiting();
	void BotLogicExhaustQueue();

	//pathQ methods
	//bool IsPathToTargetSheduled(int x, int y);
	//bool PathNearlyCompleted();

	//debug
	int _debugTimer;
	void BotLogicStateDebug();

};