#include "mainMenu.h"
#include "Window.h"

mainMenu::mainMenu()
{
}

void mainMenu::Start()
{
	Window::Get().initGui();
}

void mainMenu::Update()
{
	if (waitingForGame) 
	{ 
		mainGame->initGame();
		waitingForGame = false;
	}
}

void mainMenu::Render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


	ImGui::Begin("mainWindow");

	if (ImGui::Button("Play Game"))
	{
		waitingForGame = true;
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void mainMenu::sendMessage(const std::string& t_messageType)
{
	if (t_messageType == "DestroyContext")
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		mainGame = nullptr;
	}
}
