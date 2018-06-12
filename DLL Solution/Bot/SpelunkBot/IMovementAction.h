#pragma once
//#include "stdafx.h"

//teoretycznie mo¿na zrobiæ forward declaration zamiast include
#include "IBot.h"
#include "Coords.h"

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
	//IBot* _bot;
	std::shared_ptr<IBot> const& _bot;
	//std::shared_ptr<IBot> _bot;

	MOVEMENTACTION _actionType;
	bool _actionDone;	
	bool _actionInProgress;
	DIRECTIONX _directionX;
	DIRECTIONY _directionY;
	Coords _targetNode;

	void CalculateTargetNode(int distXNode, int distYNode);
	bool ShouldTryToGrabLadder();
	bool IsNearLadderTop();

	int _centralizeMoveTimer = 0;
	int _centralizeBreakTimer = 0;
	DIRECTIONX _centralizeDir;
	void Centralize(ordersStruct *orders, int centralizingPoint);

	int _standingStillCounter = 0;
	bool IsStandingStill(int playerPosX, int playerPosY, int prevPlayerPosX, int prevPlayerPosY);

public:
	IMovementAction(std::shared_ptr<IBot> const& bot);
	~IMovementAction();

	bool ActionDone();
	MOVEMENTACTION ActionType();
	DIRECTIONX GetDirectionX();
	DIRECTIONY GetDirectionY();
	Coords GetTargetNode();
	
	virtual ordersStruct GetOrders() =0;

};
