#include "pch.h"
#include "Utility.h"
#include "ComputeDevice.h"

const std::wstring Aether::ComputeDevice::GetAdapterName() const
{
	return Utility::GetAdapterName(Adapter);
}

Aether::ComputeDevice::~ComputeDevice()
{
	if (Device != nullptr)
	{
		Context->ClearState();
		Context->Flush();
		Context->Release();
	}

	if (Device != nullptr)
	{
		Device->Release();
	}
}
