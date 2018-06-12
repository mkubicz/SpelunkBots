#include "stdafx.h"
#include "WalkUpAction.h"

WalkUpAction::WalkUpAction(std::unique_ptr<IBot> const& bot, DIRECTIONX directionX)
	: IMovementAction(bot)
{
	_actionType = WALKUP;
	_directionX = directionX;

	_actionInProgress = false;
	_againstTheWall = false;

	_finished = false;
	_finishedTimer = 6;
}


/*
* Makes the bot walk 1 tile up or 2 tiles up and 1 tile left/right (like on the stairs)
*		 _
*	_	|
* _|   _|
*
*/

ordersStruct WalkUpAction::GetOrders()
{
	ordersStruct orders;

	if (!_finished)
	{
		//first - get targets
		if (!_actionInProgress)
		{
			//int nodenr = (int)_bot->GetPlayerPositionXNode() + 1;
			int nodenr = _directionX == xRIGHT ? (int)_bot->GetPlayerPositionXNode() + 1 : (int)_bot->GetPlayerPositionXNode() - 1;
			_targetX = (nodenr * PIXELS_IN_NODE) + (PIXELS_IN_NODE / 2);

			//check if need to go 1 node up or 2 nodes up
			if (_bot->IsNodePassable(nodenr, _bot->GetPlayerPositionYNode() - 1, NODE_COORDS))
				_targetY = _bot->GetPlayerPositionY() - PIXELS_IN_NODE;
			else
				_targetY = _bot->GetPlayerPositionY() - (PIXELS_IN_NODE * 2);


			_actionInProgress = true;
		}

		//second - hug the wall
		if (!_againstTheWall)
		{
			_directionX == xRIGHT ? orders.goRight = true : orders.goLeft = true;
			if (IsBotAgainstTheWall(_bot->GetPlayerPositionX(), _directionX == xRIGHT)) _againstTheWall = true;
		}
		else
		{
			//third - jump up
			if (_bot->GetPlayerPositionY() > _targetY)
			{
				orders.jump = true;
			}
			if (!closeToTarget(_bot->GetPlayerPositionX(), _targetX))
				_directionX == xRIGHT ? orders.goRight = true : orders.goLeft = true;


			if (_bot->GetPlayerPositionY() <= _targetY && closeToTarget(_bot->GetPlayerPositionX(), _targetX) && _previousPosX != _bot->GetPlayerPositionX())
				_finished = true;

		}

		_previousPosX = _bot->GetPlayerPositionX();
	}
	else
	{
		_finishedTimer -= 1;
		if (_finishedTimer == 0)
			_actionDone = true;
	}

	return orders;
}


//ordersStruct WalkUpAction::GetOrders()
//{
//	ordersStruct orders;
//
//	//first - get targets
//	if (!_actionInProgress)
//	{
//		//int nodenr = (int)_bot->GetPlayerPositionXNode() + 1;
//		int nodenr = _goingRight ? (int)_bot->GetPlayerPositionXNode() + 1 : (int)_bot->GetPlayerPositionXNode() - 1;
//		_targetX = (nodenr * PIXELS_IN_NODE) + (PIXELS_IN_NODE/2);
//		
//		//check if need to go 1 node up or 2 nodes up
//		if (_bot->IsNodePassable(nodenr, _bot->GetPlayerPositionYNode() - 1, NODE_COORDS))
//			_targetY = _bot->GetPlayerPositionY() - PIXELS_IN_NODE;
//		else
//			_targetY = _bot->GetPlayerPositionY() - (PIXELS_IN_NODE * 2);
//		
//
//		_actionInProgress = true;
//	}
//
//	//second - hug the wall
//	if (!_againstTheWall)
//	{
//		_goingRight ? orders.goRight = true : orders.goLeft = true;
//		if (IsBotAgainstTheWall(_bot->GetPlayerPositionX(), _goingRight)) _againstTheWall = true;
//	}
//	else
//	{
//		//third - jump up
//		if (_bot->GetPlayerPositionY() > _targetY)
//		{
//			orders.jump = true;
//		}
//		if (!closeToTarget(_bot->GetPlayerPositionX(), _targetX))
//			_goingRight ? orders.goRight = true : orders.goLeft = true;
//		
//
//		if (_bot->GetPlayerPositionY() <= _targetY && closeToTarget(_bot->GetPlayerPositionX(), _targetX) && _previousPosX != _bot->GetPlayerPositionX())
//			_actionDone = true;
//
//	}
//
//	_previousPosX = _bot->GetPlayerPositionX();
//
//	return orders;
//}





/*
ordersStruct WalkUpAction::GetOrders()
{
	if (!_actionInProgress)
	{
		//trzeba jednak ustalaæ targetX
		//niech specyfikacja walkUpLeft bêdzie taka, ¿e bot przeskakuje na kratkê o 1 do góry i o 1 w lewo/prawo
		//i moja w tym g³owa, nie jego, ¿eby nie dosta³ tej komendy nie stoj¹c obok kratki na któr¹ mo¿na wejœæ
		//jak jednak dostanie tak¹ komendê nie stoj¹c obok kratki na któr¹ mo¿na wejœæ to mo¿e np. od razu zrobiæ action done
		//_targetX = _bot->GetPlayerPositionXNode();
		//_actionType == WALKUPLEFT ? _targetX -= 1 : _targetX += 1;
		_targetY = _bot->GetPlayerPositionY() - 1;

		_actionInProgress = true;
	}

	ordersStruct orders;

	double intpartX, fractpartX, intpartY, fractpartY;
	GetIntPartFractPart(_bot->GetPlayerPositionXNode(), intpartX, fractpartX);
	GetIntPartFractPart(_bot->GetPlayerPositionYNode(), intpartY, fractpartY);

	if (_bot->GetPlayerPositionYNode() <= _targetY && fractpartX > 0.6875 && fractpartY == 0.5) //fractpartY == 0.5 -> bot is not in air
	{
		_actionDone = true;
	}
	else
	{
		if (_actionType == WALKUPLEFT)
		{
			orders.goLeft = true;
			if (fractpartX == 0.31) orders.jump = true; //i jest ziemia kratkê po lewej
		}
		else
		{
			orders.goRight = true;
			if (fractpartX == 0.6875) orders.jump = true; //i jest ziemia kratkê po prawej
		}
	}



	
	return orders;
}
*/