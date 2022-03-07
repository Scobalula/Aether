#include "pch.h"
#include "ComputeDeviceFactory.h"

void Aether::ComputeDeviceFactory::Initialize()
{
	IDXGIFactory* dxgiFactory = nullptr;
	IDXGIAdapter* dxgiAdapter = nullptr;

	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&dxgiFactory)))
	{
		// TODO: Logging
		throw std::exception("Failed to create DXGI Factory");
	}

	UINT i = 0;

	while (dxgiFactory->EnumAdapters(i++, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND)
		Adapters.push_back(dxgiAdapter);

	dxgiFactory->Release();
}

IDXGIAdapter* Aether::ComputeDeviceFactory::FindAdapter(const wchar_t* name)
{
	if (name != nullptr)
	{
		for (auto adapter : Adapters)
		{
			DXGI_ADAPTER_DESC desc{};

			if (FAILED(adapter->GetDesc(&desc)))
			{
				// TODO: Logging
				continue;
			}

			if (lstrcmp(desc.Description, name))
			{
				return adapter;
			}
		}
	}

	return nullptr;
}

void Aether::ComputeDeviceFactory::InitializeDevice(Aether::ComputeDevice& device, const char* name)
{

	int sizeOf = MultiByteToWideChar(CP_UTF8, 0, name, -1, NULL, 0);
	auto wideBuf = std::make_unique<wchar_t[]>(sizeOf);
	MultiByteToWideChar(CP_UTF8, 0, name, -1, wideBuf.get(), sizeOf);

	InitializeDevice(device, wideBuf.get());
}

void Aether::ComputeDeviceFactory::InitializeDevice(Aether::ComputeDevice& device, const wchar_t* name)
{
	if (Adapters.size() == 0)
		throw std::exception("No available adapters or initialize wasn't called.");

	auto adapter = FindAdapter(name);

	// Either couldn't find the adapter or were not provided
	// an adapter name, take first one from list.
	if (adapter == nullptr)
		adapter = Adapters[0];

	// We don't require anything beyond DX11
	D3D_FEATURE_LEVEL returnedFeatureLevel;

	auto result = D3D11CreateDevice(
		adapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
#if _DEBUG
		D3D11_CREATE_DEVICE_DEBUG,
#else
		0,
#endif
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&device.Device,
		&returnedFeatureLevel,
		&device.Context);

	if (FAILED(result))
	{
		// TODO: Logging
		throw std::exception("Failed to create DirectX device.");
	}

	device.Adapter = adapter;

	// We require at least DirectX 11
	if (returnedFeatureLevel < D3D_FEATURE_LEVEL_11_0)
	{
		// TODO: Logging
		throw std::exception("Invalid feature level returned for device.");
	}
}

Aether::ComputeDeviceFactory::~ComputeDeviceFactory()
{
	for (auto adapter : Adapters)
	{
		adapter->Release();
	}

	Adapters.clear();
}
