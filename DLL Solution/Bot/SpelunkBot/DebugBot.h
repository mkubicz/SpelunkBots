#pragma once
#include "IBot.h"
#include "IMovementAction.h"
#include "Pathfinder.h"


class DebugBot : public IBot
{
public:
	DebugBot();
	// { }
	~DebugBot();// { }



	void ExecuteOrders(ordersStruct orders);

	void Update() override;
	void NewLevel() override;

private:
	std::queue<IMovementAction*> _actionsQ;
	Pathfinder* _pathfinder;
	
	enum STATE {SEARCHING_FOR_EXIT, CREATING_COMMANDS, EXECUTING_COMMANDS, DEBUG};
	STATE _state;
	void InitialiseHelperVariables();
	void CreateCommands(std::vector<Node> path);
	void ClearOrders();
	bool FindExit(int & x, int & y);

	int _debugTimer;
	bool _commandsCreated;

	Node current;
};