#pragma once
#include "IBot.h"
#include "IMovementAction.h"
#include "Pathfinder.h"
#include "ObjectManager.h"

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

private:
	std::queue<IMovementAction*> _actionsQ;
	Pathfinder* _pathfinder;
	ObjectManager* _objectManager;
	std::thread _botLogicThread;
	
	//std::vector<collectableObject> _collectablesList;
	std::deque<std::vector<MapNode*>> _pathsQ;

	enum SECONDARY_STATE { sIDLE, NEW_TARGET, UNREACHABLE_TARGET, EXECUTING_COMMANDS,
						   CREATING_COMMANDS, FINISHED, WAITING_FOR_PATH, DEBUG };
	enum BOTLOGIC_STATE { IDLE, START, EXIT, WAITING, GATHER_FROM_CC, EXPLORE_CC, PICK_TARGET_IN_NEXT_CC,
						  MOVE_TO_NEXT_CC, SEARCH_FOR_EXIT, GO_TO_EXIT, EXIT_REACHED, UNREACHABLE_EXIT,
						  NO_EXIT_EXPLORE, NO_EXIT_ERROR };
	BOTLOGIC_STATE _botLogicState;
	BOTLOGIC_STATE _prevState;
	SECONDARY_STATE _secState;
	bool _botLogicWaiting = false;
	int _waitTimer = 0;

	void CreateCommands(std::vector<MapNode*> path);
	void AddActionToActionQueue(MOVEMENTACTION action, ACTION_TARGET jumpTarget, MVSTATE mvState, int distX, int distY);
	void ClearOrders();

	//botlogic methods
	void InitialiseBotLogicState();
	void BotLogic();
	void BotLogicWaiting();

	//pathQ methods
	bool IsPathToTargetSheduled(int x, int y);
	MapNode GetStartNodeForNextPath();
	bool PathNearlyCompleted();

	//debug
	int _debugTimer;
	void BotLogicStateDebug();

};