#pragma once

#include "Library.h"
#include "ScriptObject.h"
#include "Game.h"

class mainMenu : public ScriptObject
{
public:
	mainMenu();
	void Start()override;
	void Update()override;
	void Render()override;
	void sendMessage(const std::string& t_messageType) override;

	Game* mainGame = nullptr;
private:
	bool waitingForGame = false;
};
