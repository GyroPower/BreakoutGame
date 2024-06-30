#include"GUI.h"
#include<string>

GUI::GUI(unsigned int w_width, unsigned int w_height) :
	startGame(false),w_width(w_width),w_height(w_height) {}

GUI::~GUI(){ }

void GUI::mainMenuWindow(GameState& state, unsigned int& level) {

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration 
		| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);

	bool b_state = state == GAME_MENU;

	if (ImGui::Begin("menu", &b_state, flags)) {

		std::string title = "BREAKOUT with ImGui";
		ImGui::SetWindowFontScale(2.5);
		ImVec2 text_size = ImGui::CalcTextSize(title.c_str());
		ImGui::SetCursorPos(ImVec2(this->w_width/2 - text_size.x/2,this->w_height/3));
		ImGui::TextColored(ImVec4(1.0, 0.5, 0, 1.0),"BREAKOUT with ImGui");
		
		text_size = ImGui::CalcTextSize("Start");
		ImGui::SetCursorPos(ImVec2(this->w_width / 2 - text_size.x / 2,
			this->w_height / 4 + this->w_height / 4));
		ImGui::SetWindowFontScale(2);
		ImGui::PushID(1);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0, 0.5, 0, 1.0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0, 0.6, 0, 1.0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0, 0.7, 0, 1.0));
		
		if (ImGui::Button("Start")) 
			state = GAME_ACTIVE;
			
		
		
		
		std::string level_in = "Level: " + std::to_string(level);

		text_size = ImGui::CalcTextSize(level_in.c_str());

		ImGui::SetCursorPos(ImVec2(this->w_width / 2 - text_size.x,
			this->w_height / 4 + this->w_height / 4 + this->w_height / 8));
		if (ImGui::ArrowButton("#left", ImGuiDir_Left))
			if (level == 1)
				level = 4;
			else
				level -= 1;

		ImGui::SetCursorPos(ImVec2(this->w_width / 2 + text_size.x - (text_size.x / 4 + text_size.x / 10),
			this->w_height / 4 + this->w_height / 4 + this->w_height / 8));
		if (ImGui::ArrowButton("#right", ImGuiDir_Right))
			if (level >= 4)
				level = 1;
			else
				level += 1;

		ImGui::PopStyleColor(3);
		ImGui::PopID();

		ImGui::SetCursorPos(ImVec2(this->w_width / 2 - text_size.x / 2,
			this->w_height / 4 + this->w_height / 4 + this->w_height / 8));
		ImGui::TextColored(ImVec4(1.0, 0.5, 0, 1.0), level_in.c_str());


	}
	ImGui::End();

}

void GUI::showKeysPressed(bool& show_keys_pressed) {


	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration
		| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x/6,viewport->Size.y/8));

	if (ImGui::Begin("show_keys", &show_keys_pressed, flags)) {

		struct funcs {
			static bool IsLegacyNativeDupe(ImGuiKey key) 
			{
				return key >= 0 && key < 512 && ImGui::GetIO().KeyMap[key] != -1;
			}
		};
		
		ImGui::Text("keys down: "); for (ImGuiKey key = ImGuiKey_KeysData_OFFSET; key < ImGuiKey_COUNT; key = (ImGuiKey)(key + 1)) { if (funcs::IsLegacyNativeDupe(key) || !ImGui::IsKeyDown(key)) continue; ImGui::SameLine(); ImGui::Text(ImGui::IsNamedKey(key) ? "\"%s\"" : "\"%s\" %d", ImGui::GetKeyName(key), key); ImGui::SameLine(); ImGui::Text("(%.02f)", ImGui::GetKeyData(key)->DownDuration); }
	}
	ImGui::End();

}