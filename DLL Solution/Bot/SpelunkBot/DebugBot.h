#pragma once
#include "IBot.h"
#include "IMovementAction.h"
#include "Pathfinder.h"
#include "ObjectManager.h"


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
	
	std::queue<collectableObject> _collectablesQ;
	//std::vector<collectableObject> _unreachableCollectables;
	std::map<int, bool> _unreachableColl;



	enum PRIMARY_STATE {pIDLE, EXPLORING_GATHERING, GOING_TO_EXIT, SEARCHING_FOR_EXIT};
	enum SECONDARY_STATE {sIDLE, NEW_TARGET, UNREACHABLE_TARGET, EXECUTING_COMMANDS, FINISHED, DEBUG};
	PRIMARY_STATE _primState;
	SECONDARY_STATE _secState;

	//enum STATE {SEARCHING_FOR_EXIT, CREATING_COMMANDS, EXECUTING_COMMANDS, DEBUG};
	//STATE _state;

	void InitialiseHelperVariables();
	void CreateCommands(std::vector<Node> path);
	void AddActionToActionQueue(MOVEMENTACTION action, ACTION_TARGET jumpTarget, MVSTATE mvState, int distX, int distY);
	void ClearOrders();
	bool FindExit(int & x, int & y);
	Node* TryToFindExit();

	int _debugTimer;
	int _attackTimer = 0;
	bool _commandsCreated;

	//Node current;
	Node* _target;
};