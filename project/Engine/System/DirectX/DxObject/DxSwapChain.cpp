#include "DxSwapChain.h"
_DXOBJECT_USING

////////////////////////////////////////////////////////////////////////////////////////////
// SwapChain class methods
////////////////////////////////////////////////////////////////////////////////////////////

void SwapChain::Init(
	Device* devices, DescriptorHeaps* descriptorHeaps, CommandContext* command,
	Window* window) {

	CreateSwapChain(devices, command, window);
	CreateRTV(devices, descriptorHeaps);
}

void SwapChain::Term() {
	for (uint32_t i = 0; i < kBufferCount_; ++i) {
		descriptorsRTV_[i].Delete();
	}
}

void SwapChain::Present(UINT SyncInterval, UINT Flags) {
	swapChain_->Present(SyncInterval, Flags);
}

UINT SwapChain::GetCurrentBackBufferIndex() {
	currentBackBufferIndex_ = swapChain_->GetCurrentBackBufferIndex();
	return currentBackBufferIndex_;
}

D3D12_RESOURCE_BARRIER SwapChain::GetBackBufferTransitionBarrier(D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter) const {

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource   = resources_[currentBackBufferIndex_].Get();
	barrier.Transition.StateBefore = stateBefore;
	barrier.Transition.StateAfter  = stateAfter;

	return barrier;
}

void SwapChain::CreateSwapChain(Device* device, CommandContext* command, Window* window) {

	// スワップチェインを生成
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.Width            = window->GetSize().x;
	desc.Height           = window->GetSize().y;
	desc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.BufferUsage      = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount      = kBufferCount_;
	desc.SwapEffect       = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	auto hr = device->GetFactory()->CreateSwapChainForHwnd(
		command->GetCommandQueue(),
		window->GetHwnd(),
		&desc,
		nullptr, nullptr,
		reinterpret_cast<IDXGISwapChain1**>(swapChain_.GetAddressOf())
	);
	Assert(SUCCEEDED(hr));
	
}

void SwapChain::CreateRTV(Device* device, DescriptorHeaps* descriptorHeaps) {

	// SwapChainからResourceを引っ張ってくる
	for (uint32_t i = 0; i < kBufferCount_; ++i) {
		auto hr = swapChain_->GetBuffer(
			i, IID_PPV_ARGS(&resources_[i])
		);
		Assert(SUCCEEDED(hr));
	}

	// RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC desc = {};
	desc.Format        = kScreenFormat;
	desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	for (uint32_t i = 0; i < kBufferCount_; ++i) {
		descriptorsRTV_[i] = descriptorHeaps->GetDescriptor(DescriptorType::kDescriptor_RTV);

		device->GetDevice()->CreateRenderTargetView(
			resources_[i].Get(), &desc, descriptorsRTV_[i].GetCPUHandle()
		);
	}
}
