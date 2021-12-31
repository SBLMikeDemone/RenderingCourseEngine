#include <d3d12.h>
#include <windows.h>
#include <iostream>
#include <assert.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_4.h>
#include <dxcapi.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")
//#pragma comment(lib,"dxil.lib")

bool Running;
float ClearColor[4] = { 0, 0, 1.0f, 1.0f };
const int BACKBUFFER_COUNT = 2;

struct Vertex {
	float x, y, z;		// position
	float u, v;			// texcoords
	float nx, ny, nz;	// normals
};

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

	int width = 500;
	int height = 600;

	HWND window = CreateWindowExW(
		0,
		windowName,
		L"WindowName",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800, 600,
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
		commandQueueDesc.Priority = D3D12_COMMAND_LIST_TYPE_DIRECT;

		hr = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
		assert(SUCCEEDED(hr));
	}

	IDXGISwapChain1* swapChain;
	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
		swapChainDesc.Width = width;
		swapChainDesc.Height = height;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SampleDesc.Count = 1;
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
	
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	{
		// Create sphere buffer
		constexpr int vertCount = 24;
		Vertex cubeVerts[vertCount] = {
			// Front Face
			{-1.0, -1.0, -1.0, 0.0, 1.0,-1.0, -1.0, -1.0},
			{-1.0,  1.0, -1.0, 0.0, 0.0,-1.0,  1.0, -1.0},
			{ 1.0,  1.0, -1.0, 1.0, 0.0, 1.0,  1.0, -1.0},
			{ 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0, -1.0},

			// Back Face
			{-1.0, -1.0, 1.0, 1.0, 1.0,-1.0, -1.0, 1.0},
			{ 1.0, -1.0, 1.0, 0.0, 1.0, 1.0, -1.0, 1.0},
			{ 1.0,  1.0, 1.0, 0.0, 0.0, 1.0,  1.0, 1.0},
			{-1.0,  1.0, 1.0, 1.0, 0.0,-1.0,  1.0, 1.0},

			// Top Face
			{-1.0, 1.0, -1.0, 0.0, 1.0,-1.0, 1.0, -1.0},
			{-1.0, 1.0,  1.0, 0.0, 0.0,-1.0, 1.0,  1.0},
			{ 1.0, 1.0,  1.0, 1.0, 0.0, 1.0, 1.0,  1.0},
			{ 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0},

			// Bottom Face
			{-1.0, -1.0, -1.0, 1.0, 1.0,-1.0, -1.0, -1.0},
			{ 1.0, -1.0, -1.0, 0.0, 1.0, 1.0, -1.0, -1.0},
			{ 1.0, -1.0,  1.0, 0.0, 0.0, 1.0, -1.0,  1.0},
			{-1.0, -1.0,  1.0, 1.0, 0.0,-1.0, -1.0,  1.0},

			// Left Face
			{-1.0, -1.0,  1.0, 0.0, 1.0,-1.0, -1.0,  1.0},
			{-1.0,  1.0,  1.0, 0.0, 0.0,-1.0,  1.0,  1.0},
			{-1.0,  1.0, -1.0, 1.0, 0.0,-1.0,  1.0, -1.0},
			{-1.0, -1.0, -1.0, 1.0, 1.0,-1.0, -1.0, -1.0},

			// Right Face
			{ 1.0, -1.0, -1.0, 0.0, 1.0, 1.0, -1.0, -1.0},
			{ 1.0,  1.0, -1.0, 0.0, 0.0, 1.0,  1.0, -1.0},
			{ 1.0,  1.0,  1.0, 1.0, 0.0, 1.0,  1.0,  1.0},
			{ 1.0, -1.0,  1.0, 1.0, 1.0, 1.0, -1.0,  1.0},
		};

		D3D12_RESOURCE_DESC vertexBuffDesc = {};
		vertexBuffDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vertexBuffDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
		vertexBuffDesc.Width = vertCount * sizeof(Vertex);
		vertexBuffDesc.Height = 1;
		vertexBuffDesc.DepthOrArraySize = 1;
		vertexBuffDesc.MipLevels = 1;
		vertexBuffDesc.Format = DXGI_FORMAT_UNKNOWN;
		vertexBuffDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		vertexBuffDesc.SampleDesc.Count = 1;
		vertexBuffDesc.SampleDesc.Quality = 0;

		ID3D12Resource* vertexBuffResource;

		D3D12_HEAP_PROPERTIES committedHeapProp = {};
		committedHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

		hr = device->CreateCommittedResource(
			&committedHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&vertexBuffDesc,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
			nullptr,
			IID_PPV_ARGS(&vertexBuffResource));
		assert(SUCCEEDED(hr));

		vbView.BufferLocation = vertexBuffResource->GetGPUVirtualAddress();
		vbView.SizeInBytes = sizeof(Vertex) * vertCount;
		vbView.StrideInBytes = sizeof(Vertex);
	}

	D3D12_INDEX_BUFFER_VIEW ibView = {};
	{
		constexpr int indicesCount = 36;
		int indices[indicesCount] = {
			// front face
			0, 1, 2,
			0, 2, 3,

			// Back Face
			4, 5, 6,
			4, 6, 7,

			// Top Face
			8, 9, 10,
			8, 10, 11,

			// Bottom Face
			12, 13, 14,
			12, 14, 15,

			// Left Face
			16, 17, 18,
			16, 18, 19,

			// Right Face
			20, 21, 22,
			20, 22, 23
		};

		D3D12_RESOURCE_DESC indexBuffDesc = {};
		indexBuffDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		indexBuffDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
		indexBuffDesc.Width = indicesCount * sizeof(int);
		indexBuffDesc.Height = 1;
		indexBuffDesc.DepthOrArraySize = 1;
		indexBuffDesc.MipLevels = 1;
		indexBuffDesc.Format = DXGI_FORMAT_UNKNOWN;
		indexBuffDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		indexBuffDesc.SampleDesc.Count = 1;
		indexBuffDesc.SampleDesc.Quality = 0;

		ID3D12Resource* indexBuffResource;

		D3D12_HEAP_PROPERTIES committedHeapProp = {};
		committedHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

		hr = device->CreateCommittedResource(
			&committedHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&indexBuffDesc,
			D3D12_RESOURCE_STATE_INDEX_BUFFER,
			nullptr,
			IID_PPV_ARGS(&indexBuffResource));
		assert(SUCCEEDED(hr));

		ibView.BufferLocation = indexBuffResource->GetGPUVirtualAddress();
		ibView.SizeInBytes = sizeof(int) * indicesCount;
		ibView.Format = DXGI_FORMAT_R32_UINT;
	}

	ID3D12RootSignature* rootSignature;
	{
		// Creat root signature
		D3D12_ROOT_SIGNATURE_DESC1 rootSignatureDescription = {};
		rootSignatureDescription.NumParameters = 0;
		rootSignatureDescription.pParameters = nullptr;
		rootSignatureDescription.NumStaticSamplers = 0; // Create a static sampler for earth texture?
		rootSignatureDescription.pStaticSamplers = nullptr;
		rootSignatureDescription.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

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
	{
		psoDescription.pRootSignature = rootSignature;

		psoDescription.VS = D3D12_SHADER_BYTECODE{ nullptr, 0 }; // Vertex shader
		psoDescription.PS = D3D12_SHADER_BYTECODE{ nullptr, 0 }; // Pixel shader
		psoDescription.DS = D3D12_SHADER_BYTECODE{ nullptr, 0 }; // Domain shader
		psoDescription.HS = D3D12_SHADER_BYTECODE{ nullptr, 0 }; // Hull shader
		psoDescription.GS = D3D12_SHADER_BYTECODE{ nullptr, 0 }; // Geometry shader
		// TODO: Load some shaders
		//LoadShadersFromDisk();

		psoDescription.StreamOutput = {};
		psoDescription.BlendState = {};
		psoDescription.SampleMask = 0xFFFFFFFF;

		psoDescription.RasterizerState = {};
		psoDescription.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		psoDescription.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		psoDescription.RasterizerState.DepthClipEnable = true;

		psoDescription.DepthStencilState = {};

		D3D12_INPUT_ELEMENT_DESC shaderInputs[3];
		shaderInputs[0] = {};
		shaderInputs[0].SemanticName = "Position";
		shaderInputs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		shaderInputs[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		//shaderInputs[0].SemanticIndex = 0;
		//shaderInputs[0].InputSlot = 0;
		//shaderInputs[0].AlignedByteOffset = 0;
		//shaderInputs[0].InstanceDataStepRate = 0;

		shaderInputs[1] = {};
		shaderInputs[1].SemanticName = "Texcoord";
		shaderInputs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		shaderInputs[1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;

		shaderInputs[2] = {};
		shaderInputs[2].SemanticName = "Normal";
		shaderInputs[2].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		shaderInputs[2].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;

		psoDescription.InputLayout.NumElements = 3;
		psoDescription.InputLayout.pInputElementDescs = shaderInputs;

		//psoDescription.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		psoDescription.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		psoDescription.NumRenderTargets = 1;
		psoDescription.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		psoDescription.SampleDesc = {};
	}

	ID3D12CommandAllocator* commandAllocator;
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	assert(SUCCEEDED(hr));

	ID3D12GraphicsCommandList* commandList;
	{
		hr = device->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			commandAllocator,
			nullptr,
			IID_PPV_ARGS(&commandList)
		);
		assert(SUCCEEDED(hr));

		hr = commandList->Close();
		assert(SUCCEEDED(hr));
	}

	ID3D12Fence* fence;
	hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));

	
	uint64_t lastExecutedFenceValue = 0;
	MSG message;
	Running = true;
	while (Running) {
		while (PeekMessageW(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessageW(&message);
		}

		uint64_t lastCompletedFenceValue = fence->GetCompletedValue();
		if (lastExecutedFenceValue - lastCompletedFenceValue > 0) {
			Sleep(1);
			continue;
		}

		// Render
		{
			int frame = ((IDXGISwapChain3*)swapChain)->GetCurrentBackBufferIndex();

			hr = commandAllocator->Reset();
			assert(SUCCEEDED(hr));

			hr = commandList->Reset(commandAllocator, nullptr); // TODO: Pipeline state?
			assert(SUCCEEDED(hr));

			commandList->IASetVertexBuffers(0, 1, &vbView);
			commandList->IASetIndexBuffer(&ibView);
			
			D3D12_RESOURCE_TRANSITION_BARRIER transitionToWrite = {};
			transitionToWrite.pResource = renderTargets[frame];
			transitionToWrite.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
			transitionToWrite.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
			transitionToWrite.Subresource = 0;
			
			D3D12_RESOURCE_BARRIER transitionToWriteBarrier = {};
			transitionToWriteBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			transitionToWriteBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			transitionToWriteBarrier.Transition = transitionToWrite;

			commandList->ResourceBarrier(1, &transitionToWriteBarrier);
			

			auto renderViewDescriptor = rtvDescriptorStart;
			renderViewDescriptor.ptr += (size_t) rtvDescriptorSize * frame; // rtvDescriptorStart points to beginning of heap, then there's the two back buffers at the start
			commandList->ClearRenderTargetView(renderViewDescriptor, ClearColor, 0, nullptr);
			
			D3D12_RESOURCE_TRANSITION_BARRIER transitionToPresent = {};
			transitionToPresent.pResource = renderTargets[frame];
			transitionToPresent.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			transitionToPresent.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
			transitionToPresent.Subresource = 0;

			D3D12_RESOURCE_BARRIER transitionToPresentBarrier = {};
			transitionToPresentBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			transitionToPresentBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			transitionToPresentBarrier.Transition = transitionToPresent;

			commandList->ResourceBarrier(1, &transitionToPresentBarrier);
			
			hr = commandList->Close();
			assert(SUCCEEDED(hr));
		}
		
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
}

//std::cout << "\nError code: " << GetLastError() << "\n";
