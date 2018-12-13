#if defined(_WIN32)


#include <assert.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include "../fundamental.h"


struct rdr_ctx {
        ID3D12Device *d3d_device;
        ID3D12CommandQueue *cmd_queue;
        ID3D12CommandAllocator* cmd_alloc;
        ID3D12GraphicsCommandList* cmd_list;
        ID3D12DescriptorHeap *desc_heap;
        IDXGISwapChain3 *swap_chain;

        ID3D12Resource *backbuffer[2];
        UINT backbuffer_index;

        unsigned long long fence_value;
        ID3D12Fence* fence;
        HANDLE fence_evt;

} rdr_ctx;


int
renderer_dx12_create() {
        //ID3D12Device *dev = 0;

        //D3D_FEATURE_LEVEL fl = D3D_FEATURE_LEVEL_12_1;
        //void **pp_dev = (void**)&dev;

        //HRESULT res = D3D12CreateDevice(0, fl, &IID_ID3D12Device, pp_dev);
        //assert(!FAILED(res));

        //D3D12_COMMAND_QUEUE_DESC cmd_queue_desc;
        //cmd_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        //cmd_queue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        //cmd_queue_desc.NodeMask = 0;
        //cmd_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

        //ID3D12CommandQueue *cmd_queue = 0;
        //void **pp_cmd = (void**)&cmd_queue;
        //
        //res = ID3D12Device_CreateCommandQueue(dev, &cmd_queue_desc, &IID_ID3D12CommandQueue, pp_cmd);
       
        //assert(!FAILED(res));
        //        
        //IDXGIFactory4 *factory = 0;
        //void **pp_fac = (void**)&factory;
        //res = CreateDXGIFactory1(&IID_IDXGIFactory4, pp_fac);
        //
        //assert(!FAILED(res));

        //IDXGIAdapter *adapter = 0;

        //res = IDXGIFactory4_EnumAdapters(factory, 0, &adapter);
        //assert(!FAILED(res));

        //IDXGIOutput *adapter_output = 0;
        //IDXGIAdapter_EnumOutputs(adapter, 0, &adapter_output);
        //assert(!FAILED(res));

        //DXGI_FORMAT e_fmt = DXGI_FORMAT_R8G8B8A8_UNORM;
        //UINT flags = DXGI_ENUM_MODES_INTERLACED;
        //UINT num_modes;
        //
        //res = IDXGIOutput_GetDisplayModeList(adapter_output, e_fmt, flags, &num_modes, 0);

        //assert(!FAILED(res));

        //void *buffer = 0;
        //int bytes = 0;
        //kc_alloc_frame(KC_ALLOC_RENDERER, &buffer, &bytes);
        //KC_ASSERT(buffer && bytes && "Failed to allocate");

        //DXGI_MODE_DESC *list = 0;
        //kc_array_create_with_buffer(list, buffer, bytes);
        //kc_array_resize(list, num_modes);

        //res = IDXGIOutput_GetDisplayModeList(adapter_output, e_fmt, flags, &num_modes, list);
        //assert(!FAILED(res));

        ///* pick first atm */
        //DXGI_ADAPTER_DESC adapter_desc;
        //res = IDXGIAdapter_GetDesc(adapter, &adapter_desc);
        //assert(!FAILED(res));

        //int card_mem = (int)(adapter_desc.DedicatedVideoMemory / 1024 / 1024);

        //DXGI_MODE_DESC mode_desc;

        //int i;
        //for(i = 0; i < num_modes; ++i) {
        //        DXGI_MODE_DESC l = list[i];
        //        if (l.Height == 480) {
        //                if (l.Width == 720) {
        //                        mode_desc = l;
        //                        break;
        //                }
        //        }
        //}

        ///* swap chain */
        //HWND hwnd = GetActiveWindow();

        //DXGI_SWAP_CHAIN_DESC swap_desc = { 0 };
        //swap_desc.BufferCount = 2;
        //swap_desc.BufferDesc.Width = 720;
        //swap_desc.BufferDesc.Height = 480;
        //swap_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        //swap_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        //swap_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        //swap_desc.OutputWindow = hwnd;
        //swap_desc.Windowed = TRUE;
        //swap_desc.BufferDesc.RefreshRate.Numerator = mode_desc.RefreshRate.Numerator;
        //swap_desc.BufferDesc.RefreshRate.Denominator = mode_desc.RefreshRate.Denominator;
        //swap_desc.SampleDesc.Count = 1;
        //swap_desc.SampleDesc.Quality = 0;
        //swap_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        //swap_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        //swap_desc.Flags = 0;

        //IDXGISwapChain *swap_chain_ = 0;
        //res = IDXGIFactory4_CreateSwapChain(factory, cmd_queue, &swap_desc, &swap_chain_);
        //assert(!FAILED(res));

        //IDXGISwapChain3 *swap_chain = 0;
        //void ** pp_swap = (void**)&swap_chain;

        //res = IDXGISwapChain_QueryInterface(
        //        swap_chain_,
        //        &IID_IDXGISwapChain3,
        //        pp_swap);
        //assert(!FAILED(res));

        //rdr_ctx.swap_chain = swap_chain;

        //IDXGIFactory4_Release(factory);
        //factory = 0;

        //D3D12_DESCRIPTOR_HEAP_DESC rt_heap_desc = { 0 };
        //rt_heap_desc.NumDescriptors = 2;
        //rt_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        //rt_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

        //ID3D12DescriptorHeap *desc_heap = 0;

        //void **pp_rtvh = (void**)&desc_heap;

        //res = ID3D12Device_CreateDescriptorHeap(
        //        dev,
        //        &rt_heap_desc,
        //        &IID_ID3D12DescriptorHeap,
        //        pp_rtvh);

        //assert(!FAILED(res));

        //rdr_ctx.desc_heap = desc_heap;

        //D3D12_CPU_DESCRIPTOR_HANDLE heap_start;
        //ID3D12DescriptorHeap_GetCPUDescriptorHandleForHeapStart(
        //        desc_heap,
        //        &heap_start);
        //
        //UINT rtv_desc_size =
        //ID3D12Device_GetDescriptorHandleIncrementSize(
        //        dev,
        //        D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        //ID3D12Resource *back_buffer_rt[2] = {0};
        //void **pp_backbuffer_rt = (void**)&back_buffer_rt[0];
        //IDXGISwapChain3_GetBuffer(swap_chain, 0, &IID_ID3D12Resource, pp_backbuffer_rt);
        //assert(!FAILED(res));

        //ID3D12Device_CreateRenderTargetView(dev, back_buffer_rt[0], NULL, heap_start);

        //heap_start.ptr += rtv_desc_size;

        //pp_backbuffer_rt = (void**)&back_buffer_rt[1];
        //IDXGISwapChain3_GetBuffer(swap_chain, 0, &IID_ID3D12Resource, pp_backbuffer_rt);
        //assert(!FAILED(res));
        //
        //assert(!FAILED(res));

        //UINT index = IDXGISwapChain3_GetCurrentBackBufferIndex(swap_chain);

        //rdr_ctx.backbuffer[0] = back_buffer_rt[0];
        //rdr_ctx.backbuffer[1] = back_buffer_rt[1];
        //rdr_ctx.backbuffer_index = index;

        //ID3D12CommandAllocator* cmd_alloc = 0;
        //auto alloc_type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        //void **pp_cmd_alloc = (void**)&cmd_alloc;
        //ID3D12Device_CreateCommandAllocator(dev, alloc_type, &IID_ID3D12CommandAllocator, pp_cmd_alloc);
        //assert(!FAILED(res));

        //rdr_ctx.cmd_alloc = cmd_alloc;

        //D3D12_COMMAND_LIST_TYPE cmd_type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        //ID3D12GraphicsCommandList* cmd_list = 0;
        //void **pp_cmd_list = (void**)&cmd_list;
        //res = ID3D12Device_CreateCommandList(dev, 0, cmd_type, cmd_alloc, NULL, &IID_ID3D12GraphicsCommandList, pp_cmd_list);
        //assert(!FAILED(res));
        //ID3D12GraphicsCommandList_Close(cmd_list);

        //rdr_ctx.cmd_list = cmd_list;

        //ID3D12Fence* fence = 0;
        //auto fflag = D3D12_FENCE_FLAG_NONE;
        //void **pp_fence = (void**)&fence;
        //res = ID3D12Device_CreateFence(dev, 0, fflag, &IID_ID3D12Fence, pp_fence);
        //assert(!FAILED(res));

        //HANDLE fence_evt = 0;

        //fence_evt = CreateEventEx(NULL, FALSE, FALSE, EVENT_ALL_ACCESS);
        //assert(fence_evt);

        //rdr_ctx.fence_evt = fence_evt;
        //rdr_ctx.fence_value = 1;
        //rdr_ctx.fence = fence;

        /* http://www.rastertek.com/dx12tut03.html */

        /*
        IDXGIAdapter* adapter;
        IDXGIOutput* adapterOutput;
        unsigned int numModes, i, numerator, denominator, renderTargetViewDescriptorSize;
        unsigned long long stringLength;
        DXGI_MODE_DESC* displayModeList;
        DXGI_ADAPTER_DESC adapterDesc;
        int error;
        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        IDXGISwapChain* swapChain;
        D3D12_DESCRIPTOR_HEAP_DESC renderTargetViewHeapDesc;
        D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle;

        bool m_vsync_enabled;
        ID3D12Device* m_device;
        ID3D12CommandQueue* m_commandQueue;
        char m_videoCardDescription[128];
        IDXGISwapChain3* m_swapChain;
        ID3D12DescriptorHeap* m_renderTargetViewHeap;
        ID3D12Resource* m_backBufferRenderTarget[2];
        unsigned int m_bufferIndex;
        ID3D12CommandAllocator* m_commandAllocator;
        ID3D12GraphicsCommandList* m_commandList;
        ID3D12PipelineState* m_pipelineState;
        ID3D12Fence* m_fence;
        HANDLE m_fenceEvent;
        unsigned long long m_fenceValue;
        */

        /* save d3d items */
        //rdr_ctx.d3d_device = dev;
        //rdr_ctx.cmd_queue = cmd_queue;

        return 1;
}


int
renderer_dx12_render() {
        //HRESULT ok;
        //D3D12_RESOURCE_BARRIER barrier;
        //D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle;
        //UINT rtv_desc_size;
        //float color[4];
        //ID3D12CommandList* pp_cmd_list[1];
        //UINT64 fence_wait_for;
        //ID3D12PipelineState* pipeline = 0;
        //
        //ok = ID3D12CommandAllocator_Reset(rdr_ctx.cmd_alloc);
        //assert(SUCCEEDED(ok));

        //ok = ID3D12GraphicsCommandList_Reset(rdr_ctx.cmd_list, rdr_ctx.cmd_alloc, pipeline);
        //assert(SUCCEEDED(ok));

        //barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        //UINT index = rdr_ctx.backbuffer_index;
        //barrier.Transition.pResource = rdr_ctx.backbuffer[index];

        //barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        //barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        //barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        //barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        //ID3D12GraphicsCommandList_ResourceBarrier(rdr_ctx.cmd_list, 1, &barrier);

        //D3D12_CPU_DESCRIPTOR_HANDLE heap_handle = ID3D12DescriptorHeap_GetCPUDescriptorHandleForHeapStart(rdr_ctx.desc_heap);

        //rtv_desc_size = ID3D12Device_GetDescriptorHandleIncrementSize(
        //        rdr_ctx.d3d_device,
        //        D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        //if (rdr_ctx.backbuffer_index == 1) {
        //        heap_handle.ptr += rtv_desc_size;
        //}

        //ID3D12GraphicsCommandList_OMSetRenderTargets(
        //        rdr_ctx.cmd_list,
        //        1,
        //        &heap_handle,
        //        FALSE,
        //        NULL);

        //color[0] = (float)(rand() % 255) / 255.0f;
        //color[1] = (float)(rand() % 255) / 255.0f;
        //color[2] = (float)(rand() % 255) / 255.0f;
        //color[3] = 1.0f;

        //ID3D12GraphicsCommandList_ClearRenderTargetView(
        //        rdr_ctx.cmd_list,
        //        heap_handle,
        //        color,
        //        0,
        //        NULL);

        //barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        //barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        //
        //ID3D12GraphicsCommandList_ResourceBarrier(
        //        rdr_ctx.cmd_list,
        //        1,
        //        &barrier);

        //ok = ID3D12GraphicsCommandList_Close(
        //        rdr_ctx.cmd_list);

        //assert(SUCCEEDED(ok));

        //pp_cmd_list[0] = rdr_ctx.cmd_list;

        //ID3D12CommandQueue_ExecuteCommandLists(
        //        rdr_ctx.cmd_queue,
        //        1,
        //        pp_cmd_list);

        //fence_wait_for = rdr_ctx.fence_value;

        //ok = ID3D12CommandQueue_Signal(
        //        rdr_ctx.cmd_queue,
        //        rdr_ctx.fence,
        //        fence_wait_for);

        //assert(SUCCEEDED(ok));
        //rdr_ctx.fence_value += 1;

        //UINT64 comp_value = ID3D12Fence_GetCompletedValue(
        //        rdr_ctx.fence);

        ////if (comp_value < fence_wait_for) {
        //ok = ID3D12Fence_SetEventOnCompletion(
        //        rdr_ctx.fence,
        //        fence_wait_for,
        //        rdr_ctx.fence_evt);

        //        assert(SUCCEEDED(ok));
        ////}
        //WaitForSingleObject(rdr_ctx.fence_evt, INFINITE);

        //
        //if(rdr_ctx.backbuffer_index == 0) {
        //        rdr_ctx.backbuffer_index = 1;
        //} else {
        //        rdr_ctx.backbuffer_index = 0;
        //}
        //
        //IDXGISwapChain_Present(
        //        rdr_ctx.swap_chain,
        //        0,
        //        0);

        ///* http://www.rastertek.com/dx12tut03.html */
        return 1;
}


int
renderer_dx12_destroy() {
        return 0;
}


#endif
