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
	// { }
	~DebugBot();// { }



	void ExecuteOrders(ordersStruct orders);

	void Update() override;
	void NewLevel() override;

	ObjectManager* GetObjectManager();
	
	//SpState SpelunkerState();

private:
	std::queue<IMovementAction*> _actionsQ;
	Pathfinder* _pathfinder;
	ObjectManager* _objectManager;

	Node _exit = Node(0, 0);
	bool _exitFound = false;

	bool _breakExploration = false;
	
	//std::queue<collectableObject> _collectablesQ;
	std::vector<collectableObject> _collectablesList;
	//std::vector<collectableObject> _unreachableCollectables;
	std::map<int, bool> _unreachableCollectables;
	std::queue<Node> _targetQ;

	//std::deque<std::pair<moveTarget, std::vector<Node>>> _pathsQ;
	std::deque<std::vector<Node>> _pathsQ;

	bool IsPathToTargetSheduled(int x, int y);
	//bool TryToCalculatePathDist(int targetX, int targetY, std::vector<std::pair<std::vector<Node>, int>> *pathsDists);
	std::vector< std::pair<moveTarget, std::vector<Node>> > CalculatePathsToReachableTargets(std::vector<moveTarget> targets);
	std::pair<moveTarget, std::vector<Node>> SelectShortestPath(std::vector<std::pair<moveTarget, std::vector<Node>>> paths);
	Node GetStartNodeForNextPath();

	//enum PRIMARY_STATE {pIDLE, EXPLORING_GATHERING, GATHERING, GOING_TO_EXIT, SEARCHING_FOR_EXIT};
	enum SECONDARY_STATE {sIDLE, NEW_TARGET, UNREACHABLE_TARGET, EXECUTING_COMMANDS, CREATING_COMMANDS, FINISHED, WAITING_FOR_PATH, DEBUG};
	//PRIMARY_STATE _primState;
	SECONDARY_STATE _secState;

	//enum BOTLOGIC_STATE { EXIT, GATHERING, EXPLORING, GOING_TO_EXIT, blIDLE };
	//BOTLOGIC_STATE _botLogicState;

	enum BOTLOGIC_STATE { IDLE, EXIT, WAITING, GATHER_FROM_CC, EXPLORE_CC, PICK_TARGET_IN_NEXT_CC, MOVE_TO_NEXT_CC, SEARCH_FOR_EXIT, GO_TO_EXIT, EXIT_REACHED, UNREACHABLE_EXIT, NO_EXIT_EXPLORE, NO_EXIT_ERROR };
	BOTLOGIC_STATE _botLogicState;
	BOTLOGIC_STATE _prevState;
	bool _botLogicWaiting = false;
	void BotLogicStateDebug();

	//enum STATE {SEARCHING_FOR_EXIT, CREATING_COMMANDS, EXECUTING_COMMANDS, DEBUG};
	//STATE _state;


	int _waitTimer = 0;

	void InitialiseHelperVariables();
	void CreateCommands(std::vector<Node> path);
	void AddActionToActionQueue(MOVEMENTACTION action, ACTION_TARGET jumpTarget, MVSTATE mvState, int distX, int distY);
	void ClearOrders();
	void TryToFindExit();
	void BotLogic();
	void BotLogicWaiting();


	bool _botLogicInProgress = false;
	std::thread _botLogicThread;

	int _debugTimer;
	int _attackTimer = 0;
	bool _commandsCreated;


	//Node current;
	Node* _target;
};