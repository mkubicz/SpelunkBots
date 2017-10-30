#pragma once
#include "IBot.h"

class DebugBot : public IBot
{
public:
	DebugBot() { }
	~DebugBot() { }

	void Update() override;
};