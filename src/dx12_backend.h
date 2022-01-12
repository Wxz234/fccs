#pragma once

#include <d3d12.h>

namespace fccs {

	void createD3D12Device(ID3D12Device** ppDevice);
	void createCommandAllocatorInPool(ID3D12Device* pDevice);
}