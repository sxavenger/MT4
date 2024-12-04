#include "GameScene.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/System/SxavengerSystem.h>
#include <Engine/System/Runtime/Performance/Performance.h>
#include <Engine/Console/Console.h>
#include <Engine/Content/SxavengerContent.h>
#include <Engine/Asset/SxavengerAsset.h>

//* lib
#include <Lib/Environment.h>

#include "Lib/Adapter/Json/JsonAdapter.h"


////////////////////////////////////////////////////////////////////////////////////////////
// GameScene class methods
////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Run() {

	//-----------------------------------------------------------------------------------------
	// 初期化処理
	//-----------------------------------------------------------------------------------------
	sConsole->Init();
	SystemInit();
	Init();
	SxavengerSystem::ExecuteAllAllocator();

	//-----------------------------------------------------------------------------------------
	// ゲームループ
	//-----------------------------------------------------------------------------------------
	while (SxavengerSystem::ProcessMessage()) {

		Performance::BeginFrame();
		SxavengerSystem::BeginImGuiFrame();

		//-----------------------------------------------------------------------------------------
		// 更新処理
		//-----------------------------------------------------------------------------------------

		SxavengerSystem::GetInput()->Update();
		sConsole->UpdateConsole();

		if (sConsole->IsUpdateRequired()) {
			Update();
		}

		SxavengerSystem::EndImGuiFrame();
		SxavengerSystem::TransitionAllocator();

		//-----------------------------------------------------------------------------------------
		// 描画処理
		//-----------------------------------------------------------------------------------------

		Draw();
		sConsole->Draw();
		DrawScreen();
		sConsole->DrawConsole();

		SxavengerSystem::PresentAllWindow();
		SxavengerSystem::ExecuteAllAllocator();
		//!< sub window delete時に死ぬ
		//!< shader hot reload 時に死ぬ

		Performance::EndFrame();

		if (SxavengerSystem::IsTriggerKey(KeyId::KEY_F4)) {
			break;
		}
	}

	//-----------------------------------------------------------------------------------------
	// 終了処理
	//-----------------------------------------------------------------------------------------

	Term();
	sConsole->Term();

}

void GameScene::SystemInit() {

	mainWindow_ = SxavengerSystem::CreateMainWindow(kMainWindowSize, kMainWindowTitle).lock();
	mainWindow_->SetIcon("resources/icon/SxavengerEngineIcon.ico", { 32, 32 });

	sConsole->SetWindow(mainWindow_);

	{
		std::unique_ptr<DxObject::ReflectionComputePipelineState> compute = std::make_unique<DxObject::ReflectionComputePipelineState>();
		compute->CreateBlob("common/white1x1.cs.hlsl");
		compute->ReflectionPipeline(SxavengerSystem::GetDxDevice());

		std::shared_ptr<UnorderedTexture> white1x1 = SxavengerContent::TryCreateUnorderedTexture("white1x1.png", { 1, 1 });
		white1x1->TransitionBeginUnordered(SxavengerSystem::GetMainThreadContext());
		compute->SetPipeline(SxavengerSystem::GetCommandList());


		DxObject::BindBufferDesc bind = {};
		bind.SetHandle("gOutput", white1x1->GetGPUHandleUAV());

		compute->Dispatch(SxavengerSystem::GetCommandList(), 1, 1, 1);

		white1x1->TransitionEndUnordered(SxavengerSystem::GetMainThreadContext());
		SxavengerSystem::TransitionAllocator();
	}
}

void GameScene::Init() {

	Vector3f from0 = Normalize({1.0f, 0.7f, 0.5f});
	Vector3f to0   = -from0;

	Vector3f from1 = Normalize({-0.6f, 0.9f, 0.2f});
	Vector3f to1   = Normalize({0.4f, 0.7f, -0.5f});

	Matrix4x4 mat0 = Matrix::MakeRotate(LookAt({1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}));
	Matrix4x4 mat1 = Matrix::MakeRotate(LookAt(from0, to0));
	Matrix4x4 mat2 = Matrix::MakeRotate(LookAt(from1, to1));

	
	for (int32_t i = 4 - 1; i >= 0; --i) {
		sConsole->Log(std::format("{:.3f}, {:.3f}, {:.3f}, {:.3f}", mat0.m[i][0], mat0.m[i][1], mat0.m[i][2], mat0.m[i][3]));
	}
	sConsole->Log("mat0", Console::kCommentout_);

	for (int32_t i = 4 - 1; i >= 0; --i) {
		sConsole->Log(std::format("{:.3f}, {:.3f}, {:.3f}, {:.3f}", mat1.m[i][0], mat1.m[i][1], mat1.m[i][2], mat1.m[i][3]));
	}
	sConsole->Log("mat1", Console::kCommentout_);

	for (int32_t i = 4 - 1; i >= 0; --i) {
		sConsole->Log(std::format("{:.3f}, {:.3f}, {:.3f}, {:.3f}", mat2.m[i][0], mat2.m[i][1], mat2.m[i][2], mat2.m[i][3]));
	}
	sConsole->Log("mat2", Console::kCommentout_);

}

void GameScene::Update() {
}

void GameScene::Draw() {
}

void GameScene::DrawScreen() {

	mainWindow_->BeginRendering();
	mainWindow_->ClearWindow();

	mainWindow_->EndRendering();
}

void GameScene::Term() {
	SxavengerSystem::ExecuteAllAllocator();
}


