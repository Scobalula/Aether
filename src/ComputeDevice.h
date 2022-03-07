#pragma once

namespace Aether
{
	// A class to hold a device for use in compute.
	class ComputeDevice
	{
	public:
		// The actual device.
		ID3D11Device* Device;
		// The device context.
		ID3D11DeviceContext* Context;
		// The adapter we are using.
		IDXGIAdapter* Adapter;

		// Creates a new blank compute device.
		ComputeDevice() :
			Device(nullptr),
			Context(nullptr),
			Adapter(nullptr) {}

		// Gets the name of the adapter.
		const std::wstring GetAdapterName() const;

		~ComputeDevice();
	};
}

