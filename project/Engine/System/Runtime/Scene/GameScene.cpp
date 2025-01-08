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

	Quaternion q = MakeAxisAngle(Normalize({ 1.0f, 0.4f, -0.2f }), 0.45f);
	Vector3f v   = { 2.1f, -0.9f, 1.3f };

	Matrix4x4 m = Matrix::MakeRotate(q);

	Vector3f rq = RotateVector(v, q);
	Vector3f rm = Matrix::Transform(v, m);

	sConsole->Log(std::format("rotation by matrix: {}", rm));
	sConsole->Log(std::format("rotation by quaterion: {}", rq));

	for (int32_t c = 4 - 1; c >= 0; --c) {

		std::string s = "";

		for (int32_t r = 0; r < 4; ++r) {
			s += std::to_string(m.m[c][r]) + " ";
		}

		sConsole->Log(s);
	}
	sConsole->Log("matrix: ");

	sConsole->Log(std::format("rotation: {}", q));

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


