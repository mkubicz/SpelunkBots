#pragma once
#include "IBot.h"


class DebugBot : public IBot
{
public:
	DebugBot();// { }
	~DebugBot() { }

	enum ACTION 
	{IDLE, CENTRALIZE, WALKRIGHT, WALKLEFT};

	std::queue<ACTION> _actionsQ;
	ACTION _currentAction = IDLE;
	bool _actionDone = false;

	bool CloseToZero(double x);

	bool Centralize();

	bool _walkLeftInProgress = false;
	double _targetPositionX;
	bool WalkLeft(double pixels);


	void Update() override;
};