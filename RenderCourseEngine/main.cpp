#include <d3d12.h>
#include <windows.h>
#include <iostream>
#include <assert.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_4.h>
#include <dxcapi.h>
#include <d3dcompiler.h>
#pragma warning(push)
#pragma warning(disable: 26451)
#pragma warning(disable: 6262)
#include "stb_image.h"
#pragma warning(pop)
#include "d3dx12.h"

#define _USE_MATH_DEFINES
#include <math.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")
#pragma comment(lib,"d3dcompiler.lib")

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
	bitmap.channels = 4;
	return bitmap;
}

struct SphereDefinition {
	Vertex* verts;
	int* indices;
	int vertCount;
	int indexCount;
};

HRESULT CreateSphereMesh(int vertsPerLoop, int loopsPerHeight, SphereDefinition* out) {

	long vertBufferSize = (vertsPerLoop + 1L) * (loopsPerHeight + 1L);
	Vertex* verts = new Vertex[vertBufferSize];//(Vertex*) malloc(sizeof(Vertex) * vertBufferSize);
	if (verts == NULL) {
		return -1;
	}

	long indexBufferSize = vertBufferSize * 2L * 3L; // every vert has 2 triangles, and every triangle has 3 verts
	int* indices = new int[indexBufferSize]; //(int*) malloc(sizeof(int) * indexBufferSize);
	if (indices == NULL) {
		return -1;
	}

	for (int y = 0; y <= loopsPerHeight; y++) {
		float angleFromYAxis = (float) (y * M_PI / loopsPerHeight);
		float yPos = cos(angleFromYAxis);
		float radius = sin(angleFromYAxis);
		float v = (float)(loopsPerHeight - y) / loopsPerHeight;

		for (int x = 0; x < vertsPerLoop; x++) {

			float angleAroundXAxis = (float) (x * 2.0 * M_PI / vertsPerLoop);
			float xPos = radius * cos(angleAroundXAxis);
			float zPos = radius * sin(angleAroundXAxis);

			float u = (float)(x) / vertsPerLoop;

			Vertex vert = {(float)xPos, yPos, zPos, u, v};
			verts[y * vertsPerLoop + x] = vert;
		}
	}

	int index = 0;
	for (int y = 0; y < loopsPerHeight; y++) {
		for (int x = 0; x < vertsPerLoop; x++) {

			/*
			   TL   A
				*---*
				 \  | \
                  \ |  \ 
				    *---*
					I    R

				I = index
				R = right
				TL = top left
				A = across
			*/

			int vertIndex = y * vertsPerLoop + x;
			int vertAcross = vertIndex + vertsPerLoop;
			int vertTopLeft = vertIndex + vertsPerLoop - 1;
			if (vertIndex == 0) {
				vertTopLeft = vertIndex + vertsPerLoop * 2;
			}
			int vertRight = vertIndex + 1;

			indices[index++] = vertIndex;
			indices[index++] = vertAcross;
			indices[index++] = vertTopLeft;

			indices[index++] = vertIndex;
			indices[index++] = vertRight;
			indices[index++] = vertAcross;
		}
	}

	out->verts = verts;
	out->indices = indices;
	out->indexCount = indexBufferSize;
	out->vertCount = vertBufferSize;

	return 1;
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

	SphereDefinition sphere;
	hr = CreateSphereMesh(40, 40, &sphere);
	assert(SUCCEEDED(hr));

	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	{
		int vertSize = sizeof(Vertex) * sphere.vertCount;

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
		memcpy(gpuData, sphere.verts, vertSize);
		vertexUploadBuff->Unmap(0, nullptr);

		vbView.BufferLocation = vertexUploadBuff->GetGPUVirtualAddress();
		vbView.SizeInBytes = vertSize;
		vbView.StrideInBytes = sizeof(Vertex);
	}

	D3D12_INDEX_BUFFER_VIEW ibView = {};
	{
		int idxSize = sizeof(int) * sphere.indexCount;

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
		memcpy(gpuData, sphere.indices, idxSize);
		indexUploadBuff->Unmap(0, nullptr);

		ibView.BufferLocation = indexUploadBuff->GetGPUVirtualAddress();
		ibView.SizeInBytes = idxSize;
		ibView.Format = DXGI_FORMAT_R32_UINT;
	}

	ID3D12RootSignature* rootSignature;
	{
		D3D12_DESCRIPTOR_RANGE1 descRange[1] = {};

		descRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
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
		staticSamplers[0].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;


		// Creat root signature
		D3D12_ROOT_SIGNATURE_DESC1 rootSignatureDescription = {};
		rootSignatureDescription.NumParameters = 1;
		rootSignatureDescription.pParameters = rootParams;
		rootSignatureDescription.NumStaticSamplers = 1;
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
		hr = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
		assert(SUCCEEDED(hr));
	}

	// Load textures
	MyBitmap bm = MyLoadImage("Assets/earthmap1k.jpg"); // TODO: Free
	{

		// Create buffer for the image to be placed into
		D3D12_RESOURCE_DESC textureBuffDesc = {};
		ID3D12Resource* textureBuffer;
		{
			textureBuffDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			textureBuffDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			textureBuffDesc.Width = bm.width;
			textureBuffDesc.Height = bm.height;
			textureBuffDesc.DepthOrArraySize = 1;
			textureBuffDesc.MipLevels = 1;
			textureBuffDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			textureBuffDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			textureBuffDesc.SampleDesc = multiSampleDesc;

			hr = device->CreateCommittedResource(
				&defaultHeap,
				D3D12_HEAP_FLAG_NONE,
				&textureBuffDesc,
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(&textureBuffer));
			assert(SUCCEEDED(hr));
		}

		// Deals with alignment issues and creates a footprint that was use to copy upload buffer to texture buffer
		UINT64 textureUploadBufferSize;
		D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
		device->GetCopyableFootprints(&textureBuffDesc, 0, 1, 0, &footprint, nullptr, nullptr, &textureUploadBufferSize);

		// Create upload buffer and copy resource to upload buffer
		ID3D12Resource* textureUploadBuffer;
		{
			D3D12_RESOURCE_DESC uploadBuffDesc = CD3DX12_RESOURCE_DESC::Buffer(textureUploadBufferSize);

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
			hr = textureUploadBuffer->Map(0, &range, &gpuData);
			assert(SUCCEEDED(hr));

			const uint64_t k_channelCountDst = 4;
			uint64_t rowPitch = footprint.Footprint.RowPitch;
			uint64_t width = bm.width;
			for (uint64_t h = 0; h < bm.height; ++h)
			{
				for (uint64_t w = 0; w < bm.width; ++w)
				{
					unsigned char* dst = ((unsigned char*)gpuData) + (h * rowPitch + k_channelCountDst * w);

					unsigned char* src = bm.data + (k_channelCountDst * (h * width + w));

					memcpy(dst, src, k_channelCountDst * sizeof(char));
				}
			}

			textureUploadBuffer->Unmap(0, nullptr);
		}

		// Copy texture from upload heap to texture heap
		{
			D3D12_TEXTURE_COPY_LOCATION src = {};
			src.pResource = textureUploadBuffer;
			src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
			src.PlacedFootprint = footprint;

			D3D12_TEXTURE_COPY_LOCATION dest = {};
			dest.pResource = textureBuffer;
			dest.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;

			commandList->CopyTextureRegion(&dest, 0, 0, 0, &src, nullptr);

			auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(textureBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
			commandList->ResourceBarrier(1, &barrier);
		}


		// Create actual shader resource view
		{
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.Format = textureBuffDesc.Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;

			device->CreateShaderResourceView(textureBuffer, &srvDesc, srvHeap->GetCPUDescriptorHandleForHeapStart());
		}
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

		ID3D12DescriptorHeap* descriptorHeaps[] = { srvHeap };
		commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
		commandList->SetGraphicsRootDescriptorTable(0, srvHeap->GetGPUDescriptorHandleForHeapStart());

		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // Shouldn't this be done with pipeline state object?
		commandList->SetPipelineState(pipelineStateObject);
		commandList->IASetVertexBuffers(0, 1, &vbView);
		commandList->IASetIndexBuffer(&ibView);
		commandList->DrawIndexedInstanced(sphere.indexCount, 1, 0, 0, 0);
		
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