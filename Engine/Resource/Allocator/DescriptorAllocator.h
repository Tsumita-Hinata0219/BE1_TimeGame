#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include <queue>
#include <cassert>


struct DescriptorHandle {
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	uint32_t index;
};


/* Descriptorを管理するクラス */
class DescriptorAllocator {

private: // シングルトン

	DescriptorAllocator() = default;
	~DescriptorAllocator() = default;
	DescriptorAllocator(const DescriptorAllocator&) = delete;
	const DescriptorAllocator& operator=(const DescriptorAllocator&) = delete;

public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static DescriptorAllocator* GetInstance() {
		static DescriptorAllocator instance;
		return &instance;
	}

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(UINT numDescriptors = 65536);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// フレーム初め
	/// </summary>
	void BeginFrame();

	/// <summary>
	/// 各種 Descriptor 確保メソッド
	/// </summary>
	DescriptorHandle AllocateCBV(); // CBV
	DescriptorHandle AllocateSRV(); // SRV
	DescriptorHandle AllocateUAV(); // UAV
	DescriptorHandle AllocateRTV(); // RTV
	DescriptorHandle AllocateDSV(); // DSV

	/// <summary>
	/// 再利用
	/// </summary>
	void FreeCBV(uint32_t index);
	void FreeSRV(uint32_t index);
	void FreeUAV(uint32_t index);
	void FreeRTV(uint32_t index);
	void FreeDSV(uint32_t index);

	/// <summary>
	/// ImGui で使用状況表示
	/// </summary>
	void DrawImGui();

#pragma region

	// ヒープ
	ID3D12DescriptorHeap* GetDescriptorHeapCBV() { return heapCBV_.Get(); }
	ID3D12DescriptorHeap* GetDescriptorHeapSRV() { return heapSRV_.Get(); }
	ID3D12DescriptorHeap* GetDescriptorHeapUAV() { return heapUAV_.Get(); }
	ID3D12DescriptorHeap* GetDescriptorHeapRTV() { return heapRTV_.Get(); }
	ID3D12DescriptorHeap* GetDescriptorHeapDSV() { return heapDSV_.Get(); }

	// 使用数
	uint32_t GetUsageCBV() const { return usedCBV_; }
	uint32_t GetUsageSRV() const { return usedSRV_; }
	uint32_t GetUsageUAV() const { return usedUAV_; }
	uint32_t GetUsageRTV() const { return usedRTV_; }
	uint32_t GetUsageDSV() const { return usedDSV_; }

	// 総数
	uint32_t GetTotalCBV() const { return descriptorCountPerHeap_; }
	uint32_t GetTotalSRV() const { return descriptorCountPerHeap_; }
	uint32_t GetTotalUAV() const { return descriptorCountPerHeap_; }
	uint32_t GetTotalRTV() const { return descriptorCountPerHeap_; }
	uint32_t GetTotalDSV() const { return descriptorCountPerHeap_; }

#pragma endregion


private:

	// Device
	Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;

	// Heap
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapCBV_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapSRV_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapUAV_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapRTV_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapDSV_ = nullptr;

	// 使用数
	uint32_t usedCBV_ = 0;
	uint32_t usedSRV_ = 0;
	uint32_t usedUAV_ = 0;
	uint32_t usedRTV_ = 0;
	uint32_t usedDSV_ = 0;

	// 各Heapの1つあたりのサイズ
	uint32_t descriptorSizeCBV_ = 0;
	uint32_t descriptorSizeSRV_ = 0;
	uint32_t descriptorSizeUAV_ = 0;
	uint32_t descriptorSizeRTV_ = 0;
	uint32_t descriptorSizeDSV_ = 0;

	// 空きスロット管理用キュー（フリーリスト）
	std::queue<uint32_t> freeCBVSlots_;
	std::queue<uint32_t> freeSRVSlots_;
	std::queue<uint32_t> freeUAVSlots_;
	std::queue<uint32_t> freeRTVSlots_;
	std::queue<uint32_t> freeDSVSlots_;

	// Heapあたりの総Descriptor数
	uint32_t descriptorCountPerHeap_ = 0;

};