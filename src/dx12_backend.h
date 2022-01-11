#pragma once

#include <d3d12.h>
#include <cstdint>
#include <wrl/client.h>
namespace fccs {

	void createD3D12Device(ID3D12Device** ppDevice);

	class QueueRef {
	public:
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue;
		Microsoft::WRL::ComPtr<ID3D12Fence> fence;
		uint64_t lastSubmittedInstance = 0;
		uint64_t lastCompletedInstance = 0;

		QueueRef(ID3D12Device *pDevice,ID3D12CommandQueue *pQueue) {
			queue = pQueue;
			pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
		}

		uint64_t updateLastCompletedInstance()
		{
			if (lastCompletedInstance < lastSubmittedInstance)
			{
				lastCompletedInstance = fence->GetCompletedValue();
			}
			return lastCompletedInstance;
		}
	};

}