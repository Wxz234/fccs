#pragma once

#include <fccs/fccs.h>
#include <dxgi1_6.h>

namespace fccs {

	void createD3D12Device(ID3D12Device** ppDevice);
	void createDXGIFactory(IDXGIFactory** ppFactory);
	ID3D12CommandAllocator* getCommandAllocatorInPool(ID3D12Device* pDevice, rhi::CommandQueueType type);
	void updateBusyAllocator(ID3D12CommandAllocator* pAllocator, ID3D12Fence* pFence, uint64_t submitValue);

	namespace rhi {
		D3D12_RESOURCE_STATES convertResourceStates(ResourceStates stateBits);
	}
}