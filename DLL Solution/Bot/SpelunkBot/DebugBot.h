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

	std::deque<std::pair<moveTarget, std::vector<Node>>> _pathsQ;
	bool IsPathToTargetSheduled(moveTarget target);
	//bool TryToCalculatePathDist(int targetX, int targetY, std::vector<std::pair<std::vector<Node>, int>> *pathsDists);
	std::vector< std::pair<moveTarget, std::vector<Node>> > CalculatePathsToReachableTargets(std::vector<moveTarget> targets);
	std::pair<moveTarget, std::vector<Node>> SelectShortestPath(std::vector<std::pair<moveTarget, std::vector<Node>>> paths);

	//enum PRIMARY_STATE {pIDLE, EXPLORING_GATHERING, GATHERING, GOING_TO_EXIT, SEARCHING_FOR_EXIT};
	enum SECONDARY_STATE {sIDLE, NEW_TARGET, UNREACHABLE_TARGET, EXECUTING_COMMANDS, CREATING_COMMANDS, FINISHED, WAITING_FOR_PATH, DEBUG};
	//PRIMARY_STATE _primState;
	SECONDARY_STATE _secState;

	enum BOTLOGIC_STATE { EXIT, GATHERING, EXPLORING, GOING_TO_EXIT, blIDLE };
	BOTLOGIC_STATE _botLogicState;

	//enum STATE {SEARCHING_FOR_EXIT, CREATING_COMMANDS, EXECUTING_COMMANDS, DEBUG};
	//STATE _state;


	int _sleepTimer = 0;

	void InitialiseHelperVariables();
	void CreateCommands(std::vector<Node> path);
	void AddActionToActionQueue(MOVEMENTACTION action, ACTION_TARGET jumpTarget, MVSTATE mvState, int distX, int distY);
	void ClearOrders();
	void TryToFindExit();
	void BotLogic();
	void BotLogic2();

	bool _botLogicInProgress = false;
	std::thread _botLogicThread;

	int _debugTimer;
	int _attackTimer = 0;
	bool _commandsCreated;


	//Node current;
	Node* _target;
};