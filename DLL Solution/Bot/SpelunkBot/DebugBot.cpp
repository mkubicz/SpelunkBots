#include "stdafx.h"
#include "DebugBot.h"
#include "CentralizeAction.h"
#include "WalkAction.h"
#include "WalkUpAction.h"
#include "HangDropAction.h"
#include "JumpAboveAction.h"
#include "JumpAction.h"

DebugBot::DebugBot()
{
	_pathfinder = new Pathfinder(this);
	NewLevel();
}

void DebugBot::InitialiseHelperVariables()
{
	_state = SEARCHING_FOR_EXIT;
}

void DebugBot::NewLevel()
{
	_debugTimer = 30;

	InitialiseVariables();
	InitialiseHelperVariables();
	ClearOrders();

	while (!_actionsQ.empty())
	{
		delete _actionsQ.front();
		_actionsQ.pop();
	}

	/*
	 * TESTING AND DEBUGGING
	 */
	IMovementAction* a;
	//_state = EXECUTING_COMMANDS;

	//WALKTEST
	/*
	a = new WalkAction(this, RIGHT, true, 5, NODE_COORDS);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, true, 5, NODE_COORDS);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, true, 5, NODE_COORDS);
	_actionsQ.push(a);
	*/

	/*
	//JUMPABOVETEST
	a = new WalkAction(this, RIGHT, 5, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAboveAction(this, RIGHT);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAboveAction(this, LEFT);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAboveAction(this, RIGHT);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAboveAction(this, LEFT);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAboveAction(this, LEFT);
	_actionsQ.push(a);
	a = new JumpAboveAction(this, LEFT);
	_actionsQ.push(a);
	a = new JumpAboveAction(this, LEFT);
	_actionsQ.push(a);
	*/

	
	//HANGDROPTEST
	/*
	a = new CentralizeAction(this);
	_actionsQ.push(a);
	a = new HangDropAction(this, RIGHT, false);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, 5, NODE_COORDS);
	_actionsQ.push(a);
	a = new HangDropAction(this, RIGHT, false);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, 5, NODE_COORDS);
	_actionsQ.push(a);
	a = new HangDropAction(this, LEFT, false);
	_actionsQ.push(a);
	*/

	//WALKUPTEST
	/*
	a = new WalkAction(this, RIGHT, 5, NODE_COORDS);
	_actionsQ.push(a);
	a = new WalkUpAction(this, RIGHT);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new WalkUpAction(this, RIGHT);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new WalkUpAction(this, RIGHT);
	_actionsQ.push(a);
	a = new WalkUpAction(this, RIGHT);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, 3, NODE_COORDS);
	_actionsQ.push(a);
	a = new WalkUpAction(this, RIGHT);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new WalkUpAction(this, RIGHT);
	_actionsQ.push(a);
	a = new WalkUpAction(this, RIGHT);
	_actionsQ.push(a);
	*/

	//JUMPTEST
	/*
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 3, 0);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 5, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 7, 0);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 5, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, false, 5, 4);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 3, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, false, 2, 5);
	_actionsQ.push(a);
	a = new JumpAction(this, LEFT, false, 3, 4);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, NORUN, 3, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, LEFT, true, 3, 1);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, NORUN, 4, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, LEFT, true, 2, 2);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, NORUN, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, LEFT, true, 4, 2);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, NORUN, 2, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, LEFT, true, 6, 1);
	_actionsQ.push(a);
	a = new WalkAction(this, LEFT, NORUN, 3, NODE_COORDS);
	_actionsQ.push(a);
	*/
	//JUMPTEST2
	/*
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a); 
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a); 
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a); 
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 4, 2);
	_actionsQ.push(a);
	*/
	
	//JUMPUPTEST
	/*
	a = new JumpAction(this, RIGHT, true, 2, 1);
	_actionsQ.push(a);	
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 3, 1);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 4, 1);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 5, 1);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	*/

	//JUMPUPTEST2
	/*
	a = new JumpAction(this, RIGHT, true, 2, 2);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 3, 2);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 4, 2);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	*/

	//JUMPFLATEST
	/*
	a = new JumpAction(this, RIGHT, true, 2, 0);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 3, 0);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 4, 0);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 5, 0);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	a = new JumpAction(this, RIGHT, true, 6, 0);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);	
	a = new JumpAction(this, RIGHT, true, 7, 0);
	_actionsQ.push(a);
	a = new WalkAction(this, RIGHT, NORUN, 1, NODE_COORDS);
	_actionsQ.push(a);
	*/

}

DebugBot::~DebugBot()
{
	while (!_actionsQ.empty())
	{
		delete _actionsQ.front();
		_actionsQ.pop();
	}
	delete _pathfinder;
}


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

void DebugBot::CreateCommands(std::vector<MapSearchNode*> path)
{

	bool actionFound = false;
	int currX, currY, targetX, targetY;
	currX = (int)_playerPositionXNode;
	currY = (int)_playerPositionYNode;

	for (int i = 0; i < path.size(); i++)
	{
		targetX = path[i]->GetX();
		targetY = path[i]->GetY();

		int distX = abs(targetX - currX);
		int distY = abs(targetY - currY);

		//walk
		if (distX == 1 && currY == targetY && !actionFound)
		{
			//na razie akcje chodzenia sπ tylko 1-kratkowe, moøna dorobiÊ ich scalanie
			if (targetX > currX)
				_actionsQ.push(new WalkAction(this, RIGHT, NORUN, distX, NODE_COORDS));
			else
				_actionsQ.push(new WalkAction(this, LEFT, NORUN, distX, NODE_COORDS));
			actionFound = true;
		}

		//hangdrop
		if (distX == 1 && targetY > currY && !actionFound)
		{
			if (targetX > currX)
				_actionsQ.push(new HangDropAction(this, RIGHT, false));
			else
				_actionsQ.push(new HangDropAction(this, LEFT, false));
			actionFound = true;
		}

		//walkup
		if (distX == 1 && (distY == 1 || distY == 2) && !actionFound)
		{
			if (targetX > currX)
				_actionsQ.push(new WalkUpAction(this, RIGHT));
			else
				_actionsQ.push(new WalkUpAction(this, LEFT));
			actionFound = true;
		}

		//jumpabove
		if (currX == targetX && !actionFound)
		{
			if (IsNodePassable(currX + 1, currY + 1, NODE_COORDS))
				_actionsQ.push(new JumpAboveAction(this, RIGHT));
			else
				_actionsQ.push(new JumpAboveAction(this, LEFT));
			actionFound = true;
		}

		//jump
		if (distX > 1 && !actionFound)
		{
			if (targetX > currX)
				if (targetY > currY)
					_actionsQ.push(new JumpAction(this, RIGHT, DOWN, distX, distY));
				else
					_actionsQ.push(new JumpAction(this, RIGHT, UP, distX, distY));
			else
				if (targetY > currY)
					_actionsQ.push(new JumpAction(this, LEFT, DOWN, distX, distY));
				else
					_actionsQ.push(new JumpAction(this, LEFT, UP, distX, distY));
			actionFound = true;
		}


		currX = targetX;
		currY = targetY;
		actionFound = false;
	}
}


bool DebugBot::FindExit(int &x, int &y)
{
	for (int nodeX = 0; nodeX < X_NODES; nodeX += 1)
	{
		for (int nodeY = 0; nodeY < Y_NODES; nodeY += 1)
		{
			if (GetNodeState(nodeX, nodeY, NODE_COORDS) == spExit)
			{
				x = nodeX * PIXELS_IN_NODE;
				y = nodeY * PIXELS_IN_NODE;
				return true;
			}
		}
	}
	return false;
}

void DebugBot::Update()
{
	int exitX, exitY;

	/*
	if (_debugTimer < 0)
	{
		_pathfinder->NeighboursDebug((int)_playerPositionXNode, (int)_playerPositionYNode);
		_debugTimer = 10;
	}
	_debugTimer -= 1;
	*/

	
	switch (_state)
	{
		case SEARCHING_FOR_EXIT:
			if (FindExit(exitX, exitY) && _pathfinder->CalculatePath(_playerPositionX, _playerPositionY, exitX, exitY, PIXEL_COORDS))
			{
				CreateCommands(_pathfinder->GetPathList());
				_hasGoal = true;
				_state = EXECUTING_COMMANDS;
			}
			else 
			{
				//eksplorowanie mg≥y
				//simple solution: find fog, find standable tile next to it, try to create path -> success: go -> fail: find another fog or standable tile
				/*
				int exploreX, exploreY;
				//find node with fog
				for (int nodeX = 0; nodeX < X_NODES; nodeX += 1)
				{
					for (int nodeY = 0; nodeY < Y_NODES; nodeY += 1)
					{
						
						if (GetNodeState(nodeX, nodeY, NODE_COORDS) == -1) //-1 means fog
						{
							//find standable node within radius 5 from node with fog
							for (int iX = -5; iX <= 5; iX++)
								for (int iY = -5; iY <= 5; iY++)
								{
									if (_pathfinder->CanStandInNode(nodeX + iX, nodeY + iY))
									{
										exploreX = nodeX + iX;
										exploreY = nodeY + iY;
										//if there is a path to it create commands and go
										if (_pathfinder->CalculatePath(_playerPositionX, _playerPositionY, exploreX, exploreY, PIXEL_COORDS))
										{
											CreateCommands(_pathfinder->GetPathList());
											_state = EXECUTING_COMMANDS;
											return; //get out of this costly loop
										}

										//early exit - this loop takes too long
										//TODO instead of early exit - make random possible move and after it search again
										_state = EXECUTING_COMMANDS;
										return;
									}
								}
						}
					}
				}
				*/
			}

			break;

		case EXECUTING_COMMANDS:
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
			}
			else
			{
				ClearOrders();
				//if (_hasGoal) _lookUp = true;
				if (GetNodeState(_playerPositionX, _playerPositionY, PIXEL_COORDS) == spExit)
					_lookUp = true;
				else 
					_state = SEARCHING_FOR_EXIT;
			}

			break;
	}
	
	
}


/*
IMovementAction* a = new CentralizeAction(this);
std::queue<IMovementAction*> Q1;
Q1.push(a);

CentralizeAction a2(this);
Q1.push(&a2);
//& przed zmiennπ - pobranie adresu
//* przed wskaünikiem - wy≥uskanie wartoúci
*/