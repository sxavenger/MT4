#include "Console.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/System/SxavengerSystem.h>

//=========================================================================================
// static variables
//=========================================================================================

const std::string Console::kConsoleName_ = "Sxavenger Engine Console";

const Color4f Console::kCommentout_ = { 0.0f, 0.55f, 0.0f, 1.0f };
const Color4f Console::kWarning_    = { 0.8f, 0.0f, 0.0f, 1.0f };
const Color4f Console::kError_      = { 0.8f, 0.8f, 0.0f, 1.0f };

////////////////////////////////////////////////////////////////////////////////////////////
// Console class methods
////////////////////////////////////////////////////////////////////////////////////////////

void Console::Init() {
	runtime_.Begin();

	//* windowFlag
	windowFlag_ = 0;
	windowFlag_ |= ImGuiWindowFlags_NoCollapse;
	windowFlag_ |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	windowFlag_ |= ImGuiWindowFlags_NoMove;
	windowFlag_ |= ImGuiWindowFlags_NoResize;

	SystemConsole::Init(this);
	RenderConsole::Init(this);
	AssetConsole::Init(this);
}

void Console::Term() {
	SystemConsole::Term();
	RenderConsole::Term();
	AssetConsole::Term();

	runtime_.End();
	EngineLog(std::format("[Console] executed time<TimeUtil::s>: {:.0f}sec", runtime_.GetDeltaTime<TimeUnit::s>().time));
}

void Console::UpdateConsole() {
	BeginDisabled(!IsForcusConsoleWindow());

	DisplayMainMenu();

	if (isDisplayConsole_) {

		//!< docking用Idを取得
		dockingId_ = ImGui::GetID(kConsoleName_.c_str());

		DisplayPerformace();

		SystemConsole::UpdateConsole();
		RenderConsole::UpdateConsole();
		AssetConsole::UpdateConsole();
	}

	EndDisabled();
}

void Console::Draw() {
	RenderConsole::Draw();
}

void Console::DrawConsole() {
	if (!window_.expired()) {
		auto window = window_.lock();

		window->BeginRendering();
		SxavengerSystem::RenderImGui();
		window->EndRendering();
	}
}

void Console::DockingConsole() const {
	ImGui::SetNextWindowDockID(dockingId_, ImGuiCond_FirstUseEver);
}

void Console::SetWindow(const std::weak_ptr<GameWindow>& window) {
	window_ = window;

	RenderConsole::CreateFrame(window.lock()->GetSize());
}

Console* Console::GetInstance() {
	static Console instance;
	return &instance;
}

void Console::DisplayMainMenu() {
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("Config")) {
		MenuDummy();

		if (ImGui::BeginMenu("Console")) {
			MenuDummy();

			//* console config
			ImGui::SeparatorText("console");
			ImGui::Checkbox("display console", &isDisplayConsole_);

			// windowflag
			ImGuiWindowFlags lockWindow = 0;
			lockWindow |= ImGuiWindowFlags_NoMove;
			lockWindow |= ImGuiWindowFlags_NoResize;
			ImGui::CheckboxFlags("lock console", &windowFlag_, lockWindow);

			// .ini layout
			if (ImGui::Button("output layout ini")) {
				SxavengerSystem::GetImGuiController()->OutputLayout();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Layout")) {
			ImGui::Dummy({ 200.0f, 0.0f });
			//!< ImGui公式のstyleに関するeditor
			ImGui::ShowStyleEditor();
			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Window")) {
		MenuDummy();
		ImGui::Checkbox("display system console", &(SystemConsole::isDisplaySystemConsole_));
		ImGui::Checkbox("display render console", &(RenderConsole::isDisplayRenderConsole_));
		ImGui::Checkbox("display asset console", &(AssetConsole::isDisplayAssetConsole_));
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("System")) {
		MenuDummy();
		SystemConsole::ShowSystemMenu();
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Render")) {
		MenuDummy();
		RenderConsole::ShowRenderConsoleMenu();
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Graphics")) {
		MenuDummy();
		RenderConsole::ShowGraphicsMenu();
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}

void Console::DisplayPerformace() {
	// systemの状態によってstyleを変える
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4 defaultWindowColor = style.Colors[ImGuiCol_WindowBg]; //!< 後で元に戻すので保存

	if (SystemConsole::IsUpdateRequired()) {
		// windowの色を変更
		style.Colors[ImGuiCol_WindowBg] = ImGuiController::ToImVec4({ 45, 5, 8, 255 });
	}

	DockingConsole();
	ImGui::Begin("Performance ## System Console", nullptr, windowFlag_ | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

	DeltaTimePoint<TimeUnit::s> framesPerSec = Performance::GetDeltaTime<TimeUnit::s>();

	ImGui::Text("exec speed / frame: %.6f", framesPerSec.time);
	ImGui::SameLine();
	ImGui::Text("FPS: %.0f", 1.0f / framesPerSec.time);
	

	ImGui::End();

	style.Colors[ImGuiCol_WindowBg] = defaultWindowColor;
}

bool Console::IsForcusConsoleWindow() {
	if (!window_.expired()) {
		auto window = window_.lock();
		return window.get() == SxavengerSystem::GetForcusWindow();
	}

	return false;
}

void Console::BeginDisabled(bool isDisable) {
	// 無効化状態のアルファ値を調整
	float alpha = 1.0f;
	ImGui::PushStyleVar(ImGuiStyleVar_DisabledAlpha, alpha);
	ImGui::BeginDisabled(isDisable);
}

void Console::EndDisabled() {
	ImGui::EndDisabled();
	ImGui::PopStyleVar(); // スタイルを元に戻す
}

void Console::MenuDummy() {
	static const ImVec2 size = { 200.0f, 0.0f };
	ImGui::Dummy(size);
}
