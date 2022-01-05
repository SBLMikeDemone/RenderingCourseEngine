#include <d3d12.h>
#include <windows.h>
#include <iostream>
#include <assert.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_4.h>
#include <dxcapi.h>
#include <d3dcompiler.h>
#include "stb_image.h"
#include "d3dx12.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")
#pragma comment(lib,"d3dcompiler.lib")
//#pragma comment(lib,"dxguid.lib")

bool Running;
float ClearColor[4] = { 0, 0, 1.0f, 1.0f };
const int BACKBUFFER_COUNT = 2;

struct Vertex {
	float x, y, z;	// position
	float u, v; // texcoord
};

struct MyBitmap {
	int width, height, channels;
	unsigned char* data;
};

MyBitmap MyLoadImage(const char* filepath) {
	MyBitmap bitmap;
	bitmap.data = stbi_load(filepath, &bitmap.width, &bitmap.height, &bitmap.channels, 4);
	return bitmap;
}

HRESULT CompileShader(LPCWSTR filePath, LPCSTR entryFunction, LPCSTR profile, ID3DBlob** blob) {

	// Shamelessly stolen from https://docs.microsoft.com/en-us/windows/win32/direct3d11/how-to--compile-a-shader

	UINT shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined ( _DEBUG )
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(filePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryFunction, profile, shaderFlags, 0, &shaderBlob, &errorBlob);

	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (shaderBlob)
			shaderBlob->Release();

		return hr;
	}

	*blob = shaderBlob;

	return hr;
}

LRESULT win32mainwindowcallback(HWND window, unsigned int message, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;

	switch (message) {
	case WM_CLOSE:
		Running = false;
	case WM_SIZE:
		std::cout << "Resize\n";
	case WM_KEYDOWN:
	{
		auto keycode = wParam;
		if (keycode == 'R') {
			ClearColor[0] += 0.1f;
			ClearColor[0] = fmod(ClearColor[0], 1.0f);
		}
		else if (keycode == 'G') {
			ClearColor[1] += 0.1f;
			ClearColor[1] = fmod(ClearColor[1], 1.0f);
		}
		else if (keycode == 'B') {
			ClearColor[2] += 0.1f;
			ClearColor[2] = fmod(ClearColor[2], 1.0f);
		}
	}
	default:
		result = DefWindowProc(window, message, wParam, lParam);
	}

	return result;
}

int main(int argc, char* argv[]) {
	

	HINSTANCE instance = GetModuleHandle(nullptr);
	
	auto windowName = L"RenderClassEngine";

	WNDCLASSEXW windowClass{};
	windowClass.cbSize = sizeof(WNDCLASSEXW);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = (WNDPROC)&win32mainwindowcallback;
	windowClass.hInstance = instance;
	windowClass.lpszClassName = windowName;

	assert(RegisterClassExW(&windowClass) != 0);

	int width = 800;
	int height = 600;

	HWND window = CreateWindowExW(
		0,
		windowName,
		L"WindowName",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height,
		nullptr,
		nullptr,
		instance,
		nullptr
	);
	assert(window != nullptr);

	HRESULT hr;

#if defined(_DEBUG)
	ID3D12Debug* debugInterface;
	hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface));
	assert(SUCCEEDED(hr));
	debugInterface->EnableDebugLayer();
#endif

	DXGI_SAMPLE_DESC multiSampleDesc = {};
	multiSampleDesc.Count = 1;
	
	ID3D12Device* device;
	{
		D3D_FEATURE_LEVEL minFeatureLevel = D3D_FEATURE_LEVEL_12_0;

		hr = D3D12CreateDevice(
			nullptr,
			minFeatureLevel,
			IID_PPV_ARGS(&device)
		);
		assert(SUCCEEDED(hr));
	}
	
	ID3D12CommandQueue* commandQueue;
	{
		D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
		commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		hr = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
		assert(SUCCEEDED(hr));
	}

	IDXGISwapChain1* swapChain;
	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = width;
		swapChainDesc.Height = height;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SampleDesc = multiSampleDesc;
		swapChainDesc.BufferCount = BACKBUFFER_COUNT;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		UINT createFactoryFlags = 0;
#if defined(_DEBUG)
		createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

		IDXGIFactory2* factory;
		hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&factory));
		assert(SUCCEEDED(hr));

		hr = factory->CreateSwapChainForHwnd(
			commandQueue,
			window,
			&swapChainDesc,
			nullptr,
			nullptr,
			&swapChain);
		assert(SUCCEEDED(hr));
	}

	// Types of descriptors: 
	// Render target view (RTV), 
	// Depth Stencil view (DSV), 
	// Shader Resource View (SRV), 
	// Unordered access views (UAVs)
	// Constant Buffer Views (CBV), 
	// samplers
	ID3D12DescriptorHeap* renderviewDescriptorHeap;
	{
		D3D12_DESCRIPTOR_HEAP_DESC renderViewHeapDesc{};
		renderViewHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		renderViewHeapDesc.NumDescriptors = BACKBUFFER_COUNT;

		hr = device->CreateDescriptorHeap(&renderViewHeapDesc, IID_PPV_ARGS(&renderviewDescriptorHeap));
		assert(SUCCEEDED(hr));
	}

	auto rtvDescriptorStart = renderviewDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	int rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	ID3D12Resource* renderTargets[BACKBUFFER_COUNT];
	{
		auto descriptor = rtvDescriptorStart;

		for (int i = 0; i < BACKBUFFER_COUNT; i++) {
			hr = swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i]));
			assert(SUCCEEDED(hr));

			device->CreateRenderTargetView(renderTargets[i], nullptr, descriptor);
			descriptor.ptr += rtvDescriptorSize;
		}
	}
	
	ID3D12CommandAllocator* commandAllocator[BACKBUFFER_COUNT];
	{
		for (int i = 0; i < BACKBUFFER_COUNT; i++) {
			hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator[i]));
			assert(SUCCEEDED(hr));
		}
	}

	ID3D12GraphicsCommandList* commandList;
	{
		hr = device->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			commandAllocator[0],
			nullptr,
			IID_PPV_ARGS(&commandList)
		);
		assert(SUCCEEDED(hr));
	}

	D3D12_HEAP_PROPERTIES defaultHeap = {};
	defaultHeap.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_HEAP_PROPERTIES uploadHeap = {};
	uploadHeap.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	{
		constexpr int vertCount = 4;
		Vertex cubeVerts[vertCount] = {

			{0.1f, 0.1f, 0.5f, 0, 0},
			{0.9f, 0.1f, 0.5f, 1, 0},
			{0.1f, 0.9f, 0.5f, 0, 1},
			{0.9f, 0.9f, 0.5f, 1, 1},
		};

		int vertSize = sizeof(cubeVerts);

		D3D12_RESOURCE_DESC vertexUploadBuffDesc = {};
		vertexUploadBuffDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vertexUploadBuffDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
		vertexUploadBuffDesc.Width = vertSize;
		vertexUploadBuffDesc.Height = 1;
		vertexUploadBuffDesc.DepthOrArraySize = 1;
		vertexUploadBuffDesc.MipLevels = 1;
		vertexUploadBuffDesc.Format = DXGI_FORMAT_UNKNOWN;
		vertexUploadBuffDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		vertexUploadBuffDesc.SampleDesc = multiSampleDesc;

		ID3D12Resource* vertexUploadBuff;
		hr = device->CreateCommittedResource(
			&uploadHeap,
			D3D12_HEAP_FLAG_NONE,
			&vertexUploadBuffDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertexUploadBuff));
		assert(SUCCEEDED(hr));

		void* gpuData;
		D3D12_RANGE range = {};
		//
		// No CPU reads will be done from the resource.
		//
		vertexUploadBuff->Map(0, &range, &gpuData);
		memcpy(gpuData, cubeVerts, vertSize);
		vertexUploadBuff->Unmap(0, nullptr);

		vbView.BufferLocation = vertexUploadBuff->GetGPUVirtualAddress();
		vbView.SizeInBytes = vertSize;
		vbView.StrideInBytes = sizeof(Vertex);
	}

	D3D12_INDEX_BUFFER_VIEW ibView = {};
	{
		constexpr int indexCount = 6;
		unsigned int indices[indexCount] = {
			0, 1, 2,
			1, 3, 2
		};

		int idxSize = sizeof(indices);

		D3D12_RESOURCE_DESC indexUploadBuffDesc = {};
		indexUploadBuffDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		indexUploadBuffDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
		indexUploadBuffDesc.Width = idxSize;
		indexUploadBuffDesc.Height = 1;
		indexUploadBuffDesc.DepthOrArraySize = 1;
		indexUploadBuffDesc.MipLevels = 1;
		indexUploadBuffDesc.Format = DXGI_FORMAT_UNKNOWN;
		indexUploadBuffDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		indexUploadBuffDesc.SampleDesc = multiSampleDesc;

		ID3D12Resource* indexUploadBuff;
		hr = device->CreateCommittedResource(
			&uploadHeap,
			D3D12_HEAP_FLAG_NONE,
			&indexUploadBuffDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexUploadBuff));
		assert(SUCCEEDED(hr));

		void* gpuData;
		D3D12_RANGE range = {};
		//
		// No CPU reads will be done from the resource.
		//
		indexUploadBuff->Map(0, &range, &gpuData);
		memcpy(gpuData, indices, idxSize);
		indexUploadBuff->Unmap(0, nullptr);

		ibView.BufferLocation = indexUploadBuff->GetGPUVirtualAddress();
		ibView.SizeInBytes = idxSize;
		ibView.Format = DXGI_FORMAT_R32_UINT;
	}

	ID3D12RootSignature* rootSignature;
	{
		D3D12_DESCRIPTOR_RANGE1 descRange[1] = {};

		descRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descRange[0].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC;
		descRange[0].NumDescriptors = 1;
		descRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//descRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
		//descRange[1].NumDescriptors = 1;
		//descRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		// TODO: Constant Buffer View
		D3D12_ROOT_PARAMETER1 rootParams[1] = {};
		rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParams[0].DescriptorTable.NumDescriptorRanges = 1;
		rootParams[0].DescriptorTable.pDescriptorRanges = &descRange[0];
		rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		//rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;

		D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		staticSamplers[0].MaxAnisotropy = 0;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		staticSamplers[0].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;


		// Creat root signature
		D3D12_ROOT_SIGNATURE_DESC1 rootSignatureDescription = {};
		rootSignatureDescription.NumParameters = 1;
		rootSignatureDescription.pParameters = rootParams;
		rootSignatureDescription.NumStaticSamplers = 1; // Create a static sampler for earth texture?
		rootSignatureDescription.pStaticSamplers = staticSamplers;
		rootSignatureDescription.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		D3D12_VERSIONED_ROOT_SIGNATURE_DESC rootSigWrapper = {};
		rootSigWrapper.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
		rootSigWrapper.Desc_1_1 = rootSignatureDescription;

		ID3DBlob* serializedRootSig;
		hr = D3D12SerializeVersionedRootSignature(&rootSigWrapper, &serializedRootSig, nullptr);
		assert(SUCCEEDED(hr));

		// Assign root signature
		hr = device->CreateRootSignature(0,
			serializedRootSig->GetBufferPointer(),
			serializedRootSig->GetBufferSize(),
			IID_PPV_ARGS(&rootSignature));
		assert(SUCCEEDED(hr));
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDescription = {};
	ID3D12PipelineState* pipelineStateObject;
	{
		D3D12_SHADER_BYTECODE vertexShaderByteCode = {};
		{
			ID3DBlob* vertexShader; // TODO: Free this
			hr = CompileShader(L"SimpleShader.vs", "main", "vs_5_0", &vertexShader);
			assert(SUCCEEDED(hr));
			vertexShaderByteCode.pShaderBytecode = vertexShader->GetBufferPointer();
			vertexShaderByteCode.BytecodeLength = vertexShader->GetBufferSize();
		}

		D3D12_SHADER_BYTECODE pixelShaderByteCode = {};
		{
			ID3DBlob* pixelShader; // TODO: Free this
			hr = CompileShader(L"SimpleShader.ps", "main", "ps_5_0", &pixelShader);
			assert(SUCCEEDED(hr));
			pixelShaderByteCode.pShaderBytecode = pixelShader->GetBufferPointer();
			pixelShaderByteCode.BytecodeLength = pixelShader->GetBufferSize();
		}

		D3D12_INPUT_LAYOUT_DESC inputLayout;
		{
			D3D12_INPUT_ELEMENT_DESC shaderInputs[2] = {};
			shaderInputs[0] = {};
			shaderInputs[0].SemanticName = "Position";
			shaderInputs[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			shaderInputs[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;

			shaderInputs[1] = {};
			shaderInputs[1].SemanticName = "TEXCOORD";
			shaderInputs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
			shaderInputs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
			shaderInputs[1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;

			/*
			shaderInputs[2] = {};
			shaderInputs[2].SemanticName = "Normal";
			shaderInputs[2].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			shaderInputs[2].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;*/

			inputLayout.NumElements = 2;
			inputLayout.pInputElementDescs = shaderInputs;
		}

		psoDescription.pRootSignature = rootSignature;

		psoDescription.VS = vertexShaderByteCode;
		psoDescription.PS = pixelShaderByteCode;

		psoDescription.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		psoDescription.SampleMask = 0xffffffff;

		psoDescription.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		psoDescription.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		psoDescription.RasterizerState.FrontCounterClockwise = true;
		psoDescription.RasterizerState.DepthClipEnable = true;
		psoDescription.RasterizerState.MultisampleEnable = false;

		psoDescription.InputLayout = inputLayout;

		psoDescription.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDescription.NumRenderTargets = 1;
		psoDescription.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		psoDescription.SampleDesc = multiSampleDesc;

		hr = device->CreateGraphicsPipelineState(&psoDescription, IID_PPV_ARGS(&pipelineStateObject));
		assert(SUCCEEDED(hr));
	}

	// Fill out the Viewport
	D3D12_VIEWPORT viewport; 
	{
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float) width;
		viewport.Height = (float) height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
	}

	// Fill out a scissor rect
	D3D12_RECT scissorRect;
	{
		scissorRect.left = 0;
		scissorRect.top = 0;
		scissorRect.right = width;
		scissorRect.bottom = height;
	}

	ID3D12DescriptorHeap* srvHeap;
	{
		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = 1;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	}

	// Load textures
	{
		MyBitmap bm = MyLoadImage("Assets/earthmap1k.jpg");

		D3D12_RESOURCE_DESC textureBuffDesc = {};
		textureBuffDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureBuffDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
		textureBuffDesc.Width = bm.width;
		textureBuffDesc.Height = bm.height;
		textureBuffDesc.DepthOrArraySize = 1;
		textureBuffDesc.MipLevels = 1;
		textureBuffDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		textureBuffDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		textureBuffDesc.SampleDesc = multiSampleDesc;

		ID3D12Resource* textureBuffer;
		hr = device->CreateCommittedResource(
			&defaultHeap,
			D3D12_HEAP_FLAG_NONE,
			&textureBuffDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&textureBuffer));
		assert(SUCCEEDED(hr));
		
		
		D3D12_RESOURCE_DESC uploadBuffDesc = CD3DX12_RESOURCE_DESC::Buffer((size_t)bm.width * (size_t)bm.height);
		
		ID3D12Resource* textureUploadBuffer;
		hr = device->CreateCommittedResource(
			&uploadHeap,
			D3D12_HEAP_FLAG_NONE,
			&uploadBuffDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&textureUploadBuffer));
		assert(SUCCEEDED(hr));

		void* gpuData;
		D3D12_RANGE range = {};
		//
		// No CPU reads will be done from the resource.
		//
		hr = textureUploadBuffer->Map(0, &range, &gpuData);
		assert(SUCCEEDED(hr));
		memcpy(gpuData, bm.data, (size_t)bm.width * (size_t)bm.height);
		textureUploadBuffer->Unmap(0, nullptr);

		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(textureBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		commandList->ResourceBarrier(1, &barrier);

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		device->CreateShaderResourceView(textureBuffer, &srvDesc, srvHeap->GetCPUDescriptorHandleForHeapStart());
	}

	uint64_t lastExecutedFenceValue = 0;
	ID3D12Fence* fence;
	{
		hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
		assert(SUCCEEDED(hr));
	}
	

	D3D12_RESOURCE_BARRIER transitionToWriteBarrier[BACKBUFFER_COUNT] = {};
	{
		for (int i = 0; i < BACKBUFFER_COUNT; i++) {

			D3D12_RESOURCE_TRANSITION_BARRIER transitionToWrite;
			transitionToWrite.pResource = renderTargets[i];
			transitionToWrite.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
			transitionToWrite.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
			transitionToWrite.Subresource = 0;

			transitionToWriteBarrier[i].Type = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			transitionToWriteBarrier[i].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			transitionToWriteBarrier[i].Transition = transitionToWrite;
		}
	}
	
	D3D12_RESOURCE_BARRIER transitionToPresentBarrier[BACKBUFFER_COUNT] = {};
	{
		for (int i = 0; i < BACKBUFFER_COUNT; i++) {
			D3D12_RESOURCE_TRANSITION_BARRIER transitionToPresent = {};
			transitionToPresent.pResource = renderTargets[i];
			transitionToPresent.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			transitionToPresent.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
			transitionToPresent.Subresource = 0;

			transitionToPresentBarrier[i].Type = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			transitionToPresentBarrier[i].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			transitionToPresentBarrier[i].Transition = transitionToPresent;
		}
	}


	hr = commandList->Close();
	assert(SUCCEEDED(hr));

	commandQueue->ExecuteCommandLists(1, (ID3D12CommandList**)&commandList);
	lastExecutedFenceValue++;
	hr = commandQueue->Signal(fence, lastExecutedFenceValue);
	assert(SUCCEEDED(hr));


	MSG message;
	Running = true;
	while (Running) {
		while (PeekMessageW(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessageW(&message);
		}

		uint64_t lastCompletedFenceValue = fence->GetCompletedValue();
		if (lastExecutedFenceValue - lastCompletedFenceValue > BACKBUFFER_COUNT-1) {
			Sleep(1);
			continue;
		}

		int frame = ((IDXGISwapChain3*)swapChain)->GetCurrentBackBufferIndex();

		hr = commandAllocator[frame]->Reset();
		assert(SUCCEEDED(hr));

		hr = commandList->Reset(commandAllocator[frame], pipelineStateObject);
		assert(SUCCEEDED(hr));

		commandList->ResourceBarrier(1, &transitionToWriteBarrier[frame]);

		auto renderViewDescriptor = rtvDescriptorStart;
		renderViewDescriptor.ptr += (size_t)rtvDescriptorSize * frame; // rtvDescriptorStart points to beginning of heap, then there's the two back buffers at the start
		commandList->ClearRenderTargetView(renderViewDescriptor, ClearColor, 0, nullptr);
		
		commandList->OMSetRenderTargets(1, &renderViewDescriptor, FALSE, nullptr);
		commandList->RSSetViewports(1, &viewport);
		commandList->RSSetScissorRects(1, &scissorRect);

		commandList->SetGraphicsRootSignature(rootSignature);
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // Shouldn't this be done with pipeline state object?
		commandList->SetPipelineState(pipelineStateObject);
		commandList->IASetVertexBuffers(0, 1, &vbView);
		commandList->IASetIndexBuffer(&ibView);
		commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
		
		commandList->ResourceBarrier(1, &transitionToPresentBarrier[frame]);
			
		hr = commandList->Close();
		assert(SUCCEEDED(hr));
		
		commandQueue->ExecuteCommandLists(1, (ID3D12CommandList**) &commandList);

		lastExecutedFenceValue++;
		hr = commandQueue->Signal(fence, lastExecutedFenceValue);
		assert(SUCCEEDED(hr));

		// ... What do here?

		hr = swapChain->Present(1, 0);
		assert(SUCCEEDED(hr));

		// ... What do here?
	}

	return (0);
} 

/*
void compile(LPCWSTR filename) {
	HRESULT hr;

	IDxcLibrary* compilerLib;
	hr = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&compilerLib));
	assert(SUCCEEDED(hr));

	IDxcCompiler* compiler;
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
	assert(SUCCEEDED(hr));

	IDxcIncludeHandler* includeHandler;
	hr = compilerLib->CreateIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr));

	IDxcBlobEncoding* blob;
	hr = compilerLib->CreateBlobFromFile(filename, nullptr, &blob);
	assert(SUCCEEDED(hr));

	LPCWSTR params[] =
	{
		L"-Zpr",
		L"-WX",
#ifdef _DEBUG
		L"-Zi",
		L"-Qembed_debug",
		L"-Od",
#else
		L"-O3"
#endif // DEBUG
	};

	IDxcOperationResult* res;
	hr = compiler->Compile(
		blob,
		filename,
		L"main",
		L"vs_6_0",
		params, sizeof(params) / sizeof(LPCWSTR),
		nullptr, 0,
		includeHandler,
		&res);
	assert(SUCCEEDED(hr));
}*/