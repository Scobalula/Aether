#pragma once
#include "ComputeDevice.h"

namespace Aether
{
	class ComputeDeviceFactory
	{
	public:
		// Underlying list of adapters.
		std::vector<IDXGIAdapter*> Adapters;

		// Creates the compute device factory.
		ComputeDeviceFactory(bool init)
		{
			if (init)
			{
				Initialize();
			}
		}

		// Initializes the factory and populates the lists.
		void Initialize();
		// Finds an adapter with the provided name.
		IDXGIAdapter* FindAdapter(const wchar_t* name);
		// Initializes the provided device.
		void InitializeDevice(Aether::ComputeDevice& device, const char* name);
		// Initializes the provided device.
		void InitializeDevice(Aether::ComputeDevice& device, const wchar_t* name);

		// Destroys the factory and frees all resources.
		~ComputeDeviceFactory();
	};
}

