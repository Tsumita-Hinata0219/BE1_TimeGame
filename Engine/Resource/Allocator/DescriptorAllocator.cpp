#include "DescriptorAllocator.h"
#include "Core/DirectXManager/DirectXManager.h"


void DescriptorAllocator::Init(UINT numDescriptors)
{
    device_ = DirectXManager::GetInstance()->GetDevice();
    assert(device_);

    descriptorCountPerHeap_ = numDescriptors;

    // CBV Heap
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.NumDescriptors = numDescriptors;
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        HRESULT hr = device_->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heapCBV_));
        assert(SUCCEEDED(hr));
        descriptorSizeCBV_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }

    // SRV Heap
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.NumDescriptors = numDescriptors;
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        HRESULT hr = device_->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heapSRV_));
        assert(SUCCEEDED(hr));
        descriptorSizeSRV_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }

    // UAV Heap
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.NumDescriptors = numDescriptors;
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        HRESULT hr = device_->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heapUAV_));
        assert(SUCCEEDED(hr));
        descriptorSizeUAV_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }

    // RTV Heap
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.NumDescriptors = numDescriptors;
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        HRESULT hr = device_->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heapRTV_));
        assert(SUCCEEDED(hr));
        descriptorSizeRTV_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    // DSV Heap
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.NumDescriptors = numDescriptors;
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        HRESULT hr = device_->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heapDSV_));
        assert(SUCCEEDED(hr));
        descriptorSizeDSV_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    }
}


void DescriptorAllocator::Finalize()
{
    heapCBV_.Reset();
    heapSRV_.Reset();
    heapUAV_.Reset();
    heapRTV_.Reset();
    heapDSV_.Reset();

    usedCBV_ = usedSRV_ = usedUAV_ = usedRTV_ = usedDSV_ = 0;

    while (!freeCBVSlots_.empty()) freeCBVSlots_.pop();
    while (!freeSRVSlots_.empty()) freeSRVSlots_.pop();
    while (!freeUAVSlots_.empty()) freeUAVSlots_.pop();
    while (!freeRTVSlots_.empty()) freeRTVSlots_.pop();
    while (!freeDSVSlots_.empty()) freeDSVSlots_.pop();
}


void DescriptorAllocator::BeginFrame()
{
    usedCBV_ = 0;
    usedSRV_ = 0;
    usedUAV_ = 0;
    usedRTV_ = 0;
    usedDSV_ = 0;
}


DescriptorHandle DescriptorAllocator::AllocateCBV()
{
    uint32_t index;
    if (!freeCBVSlots_.empty()) {
        index = freeCBVSlots_.front();
        freeCBVSlots_.pop();
    }
    else {
        assert(usedCBV_ < descriptorCountPerHeap_);
        index = usedCBV_++;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = heapCBV_->GetCPUDescriptorHandleForHeapStart();
    cpuHandle.ptr += index * descriptorSizeCBV_;

    D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = heapCBV_->GetGPUDescriptorHandleForHeapStart();
    gpuHandle.ptr += index * descriptorSizeCBV_;

    return { cpuHandle, gpuHandle, index };
}
DescriptorHandle DescriptorAllocator::AllocateSRV()
{
    uint32_t index;
    if (!freeSRVSlots_.empty()) {
        index = freeSRVSlots_.front();
        freeSRVSlots_.pop();
    }
    else {
        assert(usedSRV_ < descriptorCountPerHeap_);
        index = usedSRV_++;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = heapSRV_->GetCPUDescriptorHandleForHeapStart();
    cpuHandle.ptr += index * descriptorSizeSRV_;

    D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = heapSRV_->GetGPUDescriptorHandleForHeapStart();
    gpuHandle.ptr += index * descriptorSizeSRV_;

    return { cpuHandle, gpuHandle, index };
}
DescriptorHandle DescriptorAllocator::AllocateUAV()
{
    uint32_t index;
    if (!freeUAVSlots_.empty()) {
        index = freeUAVSlots_.front();
        freeUAVSlots_.pop();
    }
    else {
        assert(usedUAV_ < descriptorCountPerHeap_);
        index = usedUAV_++;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = heapUAV_->GetCPUDescriptorHandleForHeapStart();
    cpuHandle.ptr += index * descriptorSizeUAV_;

    D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = heapUAV_->GetGPUDescriptorHandleForHeapStart();
    gpuHandle.ptr += index * descriptorSizeUAV_;

    return { cpuHandle, gpuHandle, index };
}
DescriptorHandle DescriptorAllocator::AllocateRTV()
{
    uint32_t index;
    if (!freeRTVSlots_.empty()) {
        index = freeRTVSlots_.front();
        freeRTVSlots_.pop();
    }
    else {
        assert(usedRTV_ < descriptorCountPerHeap_);
        index = usedRTV_++;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = heapRTV_->GetCPUDescriptorHandleForHeapStart();
    cpuHandle.ptr += index * descriptorSizeRTV_;

    D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = heapRTV_->GetGPUDescriptorHandleForHeapStart();
    gpuHandle.ptr += index * descriptorSizeRTV_;

    return { cpuHandle, gpuHandle, index };
}
DescriptorHandle DescriptorAllocator::AllocateDSV()
{
    uint32_t index;
    if (!freeDSVSlots_.empty()) {
        index = freeDSVSlots_.front();
        freeDSVSlots_.pop();
    }
    else {
        assert(usedDSV_ < descriptorCountPerHeap_);
        index = usedDSV_++;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = heapDSV_->GetCPUDescriptorHandleForHeapStart();
    cpuHandle.ptr += index * descriptorSizeDSV_;

    D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = heapDSV_->GetGPUDescriptorHandleForHeapStart();
    gpuHandle.ptr += index * descriptorSizeDSV_;

    return { cpuHandle, gpuHandle, index };
}


void DescriptorAllocator::FreeCBV(uint32_t index)
{
    assert(index < descriptorCountPerHeap_);
    freeCBVSlots_.push(index);
}
void DescriptorAllocator::FreeSRV(uint32_t index)
{
    assert(index < descriptorCountPerHeap_);
    freeSRVSlots_.push(index);
}
void DescriptorAllocator::FreeUAV(uint32_t index)
{
    assert(index < descriptorCountPerHeap_);
    freeUAVSlots_.push(index);
}
void DescriptorAllocator::FreeRTV(uint32_t index)
{
    assert(index < descriptorCountPerHeap_);
    freeRTVSlots_.push(index);
}
void DescriptorAllocator::FreeDSV(uint32_t index)
{
    assert(index < descriptorCountPerHeap_);
    freeDSVSlots_.push(index);
}


void DescriptorAllocator::DrawImGui()
{
    if (ImGui::TreeNode("DescriptorAllocator"))
    {
        ImGui::Text("CBV: %u / %u", usedCBV_, descriptorCountPerHeap_);
        ImGui::ProgressBar(float(usedCBV_) / descriptorCountPerHeap_, ImVec2(0.0f, 0.0f));

        ImGui::Text("SRV: %u / %u", usedSRV_, descriptorCountPerHeap_);
        ImGui::ProgressBar(float(usedSRV_) / descriptorCountPerHeap_, ImVec2(0.0f, 0.0f));

        ImGui::Text("UAV: %u / %u", usedUAV_, descriptorCountPerHeap_);
        ImGui::ProgressBar(float(usedUAV_) / descriptorCountPerHeap_, ImVec2(0.0f, 0.0f));

        ImGui::Text("RTV: %u / %u", usedRTV_, descriptorCountPerHeap_);
        ImGui::ProgressBar(float(usedRTV_) / descriptorCountPerHeap_, ImVec2(0.0f, 0.0f));

        ImGui::Text("DSV: %u / %u", usedDSV_, descriptorCountPerHeap_);
        ImGui::ProgressBar(float(usedDSV_) / descriptorCountPerHeap_, ImVec2(0.0f, 0.0f));

        ImGui::TreePop();
    }
}
