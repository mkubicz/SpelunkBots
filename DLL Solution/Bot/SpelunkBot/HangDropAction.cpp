#include "stdafx.h"
#include "HangDropAction.h";

HangDropAction::HangDropAction(std::shared_ptr<IBot> const& bot, DIRECTIONX directionX)
	: IMovementAction(bot)
{
	_directionX = directionX;
	_actionType = HANGDROP;

	_actionInProgress = false;
	_actionDone = false;
	_hanging = false;
	_state = WALKING;

	_targetNode = Coords();
}


ordersStruct HangDropAction::GetOrders()
{
	int playerPosX = (int)_bot->GetPlayerPositionX();
	int playerPosY = (int)_bot->GetPlayerPositionY();
	ordersStruct orders;

	if (!_actionInProgress)
	{
		int nodeXnr = (int)_bot->GetPlayerPositionXNode();
		int nodeYnr = (int)_bot->GetPlayerPositionYNode();
		_targetXHang = _directionX == xRIGHT ? (nodeXnr + 1) * PIXELS_IN_NODE : (nodeXnr * PIXELS_IN_NODE) - 1;
		_targetYHang = playerPosY + PIXELS_IN_NODE;

		//hang drop is only used for drops with y = -1, when ground is right below the ledge
		//int dropX = _directionX == xRIGHT ? playerPosX + PIXELS_IN_NODE : playerPosX - PIXELS_IN_NODE;
		//for (int i = 2; i <= Y_NODES; i++) //if over 8, the bot will take damage
		//{
		//	if (!_bot->IsNodePassable(dropX, (playerPosY + i*PIXELS_IN_NODE), PIXEL_COORDS))
		//	{
		//		_targetY = playerPosY + (i-1)*PIXELS_IN_NODE;
		//		break;
		//	}
		//}
		_targetNode = Coords(_directionX == xRIGHT ? nodeXnr + 1 : nodeXnr - 1, nodeYnr + 1);

		_actionInProgress = true;
	}

	/*
	if (playerPosY == _targetY) _state = HANGING;
	if (playerPosY > _targetY) _state = FALLING;
	*/

	switch (_state)
	{
		case WALKING:
			if (_directionX == xRIGHT && _targetXHang > playerPosX) orders.goRight = true;
			if (_directionX == xLEFT && playerPosX > _targetXHang) orders.goLeft = true;
			if (closeToTarget(playerPosX, _targetXHang)) orders.duck = true;
			
			//if (playerPosY == _targetYHang) _state = HANGING;
			if (_bot->GetSpelunkerState() == spHANGING || _bot->GetSpelunkerState() == spCLIMBING) _state = HANGING;
			break;
		case HANGING:

			orders.duck = true;
			orders.jump = true;

			if (playerPosY >= _targetYHang) _state = FALLING;
			
			break;
		case FALLING:
			if (playerPosY == _targetNode.GetMidYpixel()) _actionDone = true;
			break;
	}
	
	_previousPosY = playerPosY;

	return orders;
	

}