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
	if (gameStarted) return;

	if (waitingForGame) 
	{ 
		mainGame->initGame();
		waitingForGame = false;
		gameStarted = true;
	}
}

void mainMenu::Render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::Begin("Crescent Rise", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

	if (!gameStarted && !waitingForGame)
	{
		float buttonWidth = 150.0f;
		float buttonHeight = 50.0f;
		float windowWidth = ImGui::GetWindowWidth();
		float windowHeight = ImGui::GetWindowHeight();
		float centerX = (windowWidth - buttonWidth) * 0.5f;
		float centerY = (windowHeight - buttonHeight) * 0.5f;


		ImGui::SetWindowFontScale(4.0f);

		ImVec2 textSize = ImGui::CalcTextSize("Crescent Rise");
		ImGui::SetCursorPos(ImVec2((windowWidth - textSize.x) * 0.5f, centerY - 100.0f));

		ImGui::TextColored(ImVec4(0.8f, 0.0f, 1.0f, 1.0f), "Crescent Rise");
		ImGui::SetWindowFontScale(1.0f);

		ImGui::SetCursorPos(ImVec2(centerX, centerY));

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.0f, 1.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.4f, 1.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.8f, 1.0f, 1.0f));

		if (ImGui::Button("PLAY GAME", ImVec2(150, 50)))
		{
			waitingForGame = true;
			gameStarted = true;
		}

		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));

		ImGui::SetCursorPos(ImVec2(centerX, centerY + 70.f));

		if (ImGui::Button("Quit Game", ImVec2(150, 50)))
		{
			Window::Get().closeGame();
		}
		ImGui::PopStyleColor(3);

		ImGui::PopStyleVar();
	}
	else
	{
		ImGui::SetWindowFontScale(4.0f);
		ImVec2 textSize = ImGui::CalcTextSize("Game is Loading . . .");
		float windowWidth = ImGui::GetWindowWidth();
		float windowHeight = ImGui::GetWindowHeight();
		float centerX = (windowWidth - textSize.x) * 0.5f;
		float centerY = (windowHeight - textSize.y) * 0.5f;
		ImGui::SetCursorPos(ImVec2(centerX, centerY));
		
		ImGui::Text("Game is Loading . . .");

		ImGui::SetWindowFontScale(1.0f);
		ImVec2 textSize2 = ImGui::CalcTextSize("this may take a while");

		ImGui::SetCursorPos(ImVec2((windowWidth - textSize2.x) * 0.5f, centerY + 200.0f));
		ImGui::Text("this may take a while");

		gameStarted = false;
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
