#pragma once
//#include "stdafx.h"

//teoretycznie mo�na zrobi� forward declaration zamiast include
#include "IBot.h"
#include "Node.h"

struct ordersStruct
{
	bool goRight = false;
	bool goLeft = false;
	bool lookUp = false;
	bool duck = false;
	bool jump = false;
	bool attack = false;
	bool ropep = false;
	bool bombp = false;
	bool run = false;
	bool pay = false;
	bool itemp = false;
};

class IMovementAction
{
protected:
	IBot* _bot;
	//DebugBot bot;

	MOVEMENTACTION _actionType;
	bool _actionDone;	
	bool _actionInProgress;
	//bool _goingRight;
	//bool _goingUp;
	DIRECTIONX _directionX;
	DIRECTIONY _directionY;
	//bool _running;
	int _targetX;
	int _targetY;
	bool ShouldTryToGrabLadder(Node target);
	bool IsNearLadderTop(int playerPosX, int playerPosY);

	int _centralizeMoveTimer = 0;
	int _centralizeBreakTimer = 0;
	DIRECTIONX _centralizeDir;
	void Centralize(ordersStruct *orders, int centralizingPoint);

	int _standingStillCounter;
	bool IsStandingStill(int playerPosX, int playerPosY, int prevPlayerPosX, int prevPlayerPosY);

public:
	IMovementAction(IBot* bot);
	~IMovementAction();

	MOVEMENTACTION ActionType();
	bool ActionDone();

	//bool IsRunning();
	DIRECTIONX GetDirectionX();
	DIRECTIONY GetDirectionY();
	
	virtual ordersStruct GetOrders() =0;

};
